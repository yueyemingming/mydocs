# EOS源码备忘-Block Produce机制

这里的代码基于1.2.5版本

## 1.总体流程

nodeos启动时会加载producer_plugin，在这个插件的start函数中会调用schedule_production_loop()开始循环：

```c++
void producer_plugin::plugin_startup(){
   ilog("starting...");

   my->schedule_production_loop();

   //...
    
       ilog("producer plugin:  plugin_startup() end");
       } catch( ... ) {
          // always call plugin_shutdown, even on exception
          plugin_shutdown();
          throw;
       }
    } FC_CAPTURE_AND_RETHROW()
}
```



schedule_production_loop()中会调用producer_plugin_impl::start_block(bool &last_block)尝试出块，根据出块尝试结果，会执行maybe_produce_block()或者等待。

start_block()会先判断节点是否应该出块，如果节点轮到出块，则会调用 chain.start_block(block_time, blocks_to_confirm)，这里最终调用chain/controller.cpp中的start_block函数出块，注意代码中有很多个start_block函数。

chain/controller.cpp中的start_block函数主要是设置好_pending_block_state，之后执行on_block transaction。on_block是一个很基础的合约，在eosio.system下的 system_contract::onblock，主要是添加出块奖励和重选出块节点，注意这里eosforce实现了不同的onblock合约。

准备好出块之后，在producer_plugin_impl::start_block中会执行需要执行的trx。

计算好区块之后，逻辑回到schedule_production_loop，根据start_block的结果，如果成功，会调用maybe_produce_block，这里最终调用void producer_plugin_impl::produce_block()，为待出的块计算签名，广播，最后调用commit_block出发新区块的处理逻辑。

下面详细分析一下以上的流程。

2.几个问题

在分析代码之前，先要看一下nodeos的几个问题。

EOS开发者在实现出块逻辑时，代码里有大量的关于_pending_block_mode的if判断，我们这里先梳理一下_pending_block_mode不同值在不同时刻的意义，以便后面不会产生混乱。

2.1 _pending_block_mode

_pending_block_mode 在 https://github.com/EOSIO/eos/issues/3161 中加入， 完整的提交在 https://github.com/EOSIO/eos/pull/3170 。

这个修改改动不小，改变了之前的出块代码结构。

提交是为了解决下面这个bug：

Upon receiving a transaction that throws a soft-fail excepion, which indicates the transaction may be good but the block is full, the producer plugin assumes it is the next active producer and attempts to sign and send off a block, expects success and then reapplies the transaction.

If any of the following assumptions fail this results in a relatively tight loop of creating a block which is exhausted, attempting to play a transaction which soft fails, dropping that block on the floor and trying again until the transaction hard fails due to its own expiration.

This is obviously a bug

在之前的版本中有一个当执行transaction时抛出了一个soft-fail异常时，整个出块插件会不停循环调用loop的bug，在原来的实现当某个transaction soft失败之后，出块节点会认为区块中有个空位置，此时会重新跑一边loop来尝试将未被apply的transaction加入区块，但是，如果由于区块中transaction已满而造成错误时，系统会不断运行loop，因为第二次运行依然是满的，直到有些交易因为超时而被抛弃的时候，此时才能正常出块。

在修正这个bug的同时，EOS团队重构了部分代码，将之前的一些分散处理的错误情况改为记录_pending_block_mode状态，原本block_production_loop()有这些返回：

namespaceblock_production_condition {enumblock_production_condition_enum { produced =0, not_synced =1, not_my_turn =2, not_time_yet =3, no_private_key =4, low_participation =5, lag =6, exception_producing_block =7, fork_below_watermark =8, };}

通过这些返回来判断分别执行现在用_pending_block_mode判断的逻辑。很多原本在之前的maybe_produce_block（后来改名现在是produce_block）判定的逻辑挪到在start_block中判断当前状态：

…// 默认是producing，即正常出块_pending_block_mode = pending_block_mode::producing;// If the next block production opportunity is in the present or future, we’re synced.if( !_production_enabled ) {// 对应not_synced，当前没有同步完成_pending_block_mode = pending_block_mode::speculating; }// Not our turnconstauto& scheduled_producer = hbs->get_scheduled_producer(block_time);if( _producers.find(scheduled_producer.producer_name) != _producers.end()) {// 对应not_my_turn，不是轮到当前节点出块_pending_block_mode = pending_block_mode::speculating; }autoprivate_key_itr = _private_keys.find( scheduled_producer.block_signing_key );if( private_key_itr == _private_keys.end() ) { ilog(“Not producing block because I don’t have the private key for KaTeX parse error: Expected 'EOF', got '}' at position 159: …:speculating; }̲// determine if…{producer}” signed a BFT confirmation OR block at a higher block number (watermark)thanthecurrentfork′shead({watermark}) than the current fork&#x27;s head (watermark)thanthecurrentfork 
′
 shead({head_block_num})", (“producer”, scheduled_producer.producer_name) (“watermark”,currrent_watermark_itr->second) (“head_block_num”,hbs->block_num));// 对应fork_below_watermark 被watermark排除出块_pending_block_mode = pending_block_mode::speculating; } }…

3.块计算逻辑 producer_plugin_impl::start_block()

首先来看下start_block的返回值：

enumclassstart_block_result{succeeded,// 出块成功failed,// 出块失败waiting,// 需要等待exhausted// 出块繁忙};

在schedule_production_loop中会进行错误处理。

进入start_block之后， 会先判定当前_pending_block_mode， 在上文中已经阐述了，

首先，因为考虑到在出块时有可能还在进行同步区块，所以这里首先有个判定，如果当前最高区块的时间和当前时间差距太大（超过5秒），就会认为还在同步区块，这时会等一段时间再出，返回 start_block_result::waiting

if(_pending_block_mode == pending_block_mode::speculating) {autohead_block_age = now - chain.head_block_time();if(head_block_age > fc::seconds(5))returnstart_block_result::waiting; }

在这之后是调用chain.start_block来做要出的区块的构造逻辑：

try{uint16_tblocks_to_confirm =0;if(_pending_block_mode == pending_block_mode::producing) {// determine how many blocks this producer can confirm// 1) if it is not a producer from this node, assume no confirmations (we will discard this block anyway)// 2) if it is a producer on this node that has never produced, the conservative approach is to assume no// confirmations to make sure we don’t double sign after a crashTODO:make these watermarks durable?// 3) if it is a producer on this node where this node knows the last block it produced, safely set it -UNLESS-// 4) the producer on this node’s last watermark is higher (meaning on a different fork)if(currrent_watermark_itr != _producer_watermarks.end()) {autowatermark = currrent_watermark_itr->second;if(watermark < hbs->block_num) { blocks_to_confirm =std::min(std::numeric_limits::max(), (uint16_t)(hbs->block_num - watermark)); } } } chain.abort_block(); chain.start_block(block_time, blocks_to_confirm); } FC_LOG_AND_DROP();

之后是执行交易的过程，每个出块节点会维护一个待打包进块的交易的集合，

首先看下 persisted transactions:

// 这里处理持久化交易, 所有过期的交易会被忽略.// remove all persisted transactions that have now expiredauto& persisted_by_id = _persistent_transactions.get();auto& persisted_by_expiry = _persistent_transactions.get();while(!persisted_by_expiry.empty() && persisted_by_expiry.begin()->expiry <= pbs->header.timestamp.to_time_point()) { persisted_by_expiry.erase(persisted_by_expiry.begin()); }

这里的持久化交易是对于所谓的RPC Node来讲的,RPC Node作为直接面向某些用户,负责接收用户的请求, 对于这类节点,发送给它的交易都应该执行,具体的内容可以参考 https://github.com/EOSIO/eos/issues/3295 中的描述.

下面是处理所有没有被执行的交易.

如果一个节点既不是出块节点,也不是RPC节点的话,那它是不需要执行任何交易的:

if(_producers.empty() && persisted_by_id.empty()) {// if this node can never produce and has no persisted transactions,// there is no need for unapplied transactions they can be droppedchain.drop_all_unapplied_transactions(); }else{ … }

之后筛选出所有要被执行的交易:

std::vector apply_trxs; {// derive appliable transactions from unapplied_transactions and drop droppable transactionsautounapplied_trxs = chain.get_unapplied_transactions(); apply_trxs.reserve(unapplied_trxs.size());// 这个函数判断要执行trx的类型autocalculate_transaction_category = [&](consttransaction_metadata_ptr& trx) {if(trx->packed_trx.expiration() < pbs->header.timestamp.to_time_point()) {// 已经超时,会被忽略returntx_category::EXPIRED; }elseif(persisted_by_id.find(trx->id) != persisted_by_id.end()) {// 持久化交易,会执行returntx_category::PERSISTED; }else{// 常规交易returntx_category::UNEXPIRED_UNPERSISTED; } };for(auto& trx: unapplied_trxs) {autocategory = calculate_transaction_category(trx);// 一个交易被执行条件:// 1. 没有过期// 2. 当前节点出块 或者 当前节点不出块,但交易是持久交易if(category == tx_category::EXPIRED || (category == tx_category::UNEXPIRED_UNPERSISTED && _producers.empty())) { chain.drop_unapplied_transaction(trx); }elseif(category == tx_category::PERSISTED || (category == tx_category::UNEXPIRED_UNPERSISTED && _pending_block_mode == pending_block_mode::producing)) { apply_trxs.emplace_back(std::move(trx)); } } }

这里会把所有要执行的交易存入apply_trxs.下面会逐个执行交易:

for(constauto& trx: apply_trxs) {// 判断是否超出了最大的执行时间,注意在EOS中每个块的cpu执行时间是有上限的.if(block_time <= fc::time_point::now()) exhausted =true;if(exhausted) {break; }try{// 刷新时间记录autodeadline = fc::time_point::now() + fc::milliseconds(_max_transaction_time_ms);booldeadline_is_subjective =false;if(_max_transaction_time_ms <0|| (_pending_block_mode == pending_block_mode::producing && block_time < deadline)) { deadline_is_subjective =true; deadline = block_time; }// 执行交易,这里的过程我们在其他文档中具体分析autotrace = chain.push_transaction(trx, deadline);if(trace->except) {if(failure_is_subjective(*trace->except, deadline_is_subjective)) {// 根据trx执行来判断是否超出了最大的执行时间exhausted =true; }else{// this failed our configured maximum transaction time, we don’t want to replay itchain.drop_unapplied_transaction(trx); } } }catch(constguard_exception& e ) { app().get_plugin().handle_guard_exception(e);returnstart_block_result::failed; } FC_LOG_AND_DROP(); }

这里注意guard_exception. TODO

以上代码执行了所有的交易,下面是对延迟交易的处理:

注意这里所谓的黑名单交易,这里和节点的黑白名单功能不同,这里是用来排除所有执行失败的延迟交易.

if(_pending_block_mode == pending_block_mode::producing) {// 整理_blacklisted_transactions, 如果已经超时,则删去交易(此时肯定是执行不了了)auto& blacklist_by_id = _blacklisted_transactions.get();auto& blacklist_by_expiry = _blacklisted_transactions.get();autonow = fc::time_point::now();while(!blacklist_by_expiry.empty() && blacklist_by_expiry.begin()->expiry <= now) { blacklist_by_expiry.erase(blacklist_by_expiry.begin()); }// 执行延迟交易autoscheduled_trxs = chain.get_scheduled_transactions();for(constauto& trx : scheduled_trxs) {// 同样的逻辑,没有抽象if(block_time <= fc::time_point::now()) exhausted =true;if(exhausted) {break; }// configurable ratio of incoming txns vs deferred txns// 这是一个新近交易和延迟交易执行的比例,// 注意当trx及其多时,需要平衡两种交易的执行时间while(_incoming_trx_weight >=1.0&& orig_pending_txn_size && _pending_incoming_transactions.size()) {autoe = _pending_incoming_transactions.front(); _pending_incoming_transactions.pop_front(); --orig_pending_txn_size; _incoming_trx_weight -=1.0; on_incoming_transaction_async(std::get<0>(e),std::get<1>(e),std::get<2>(e)); }if(block_time <= fc::time_point::now()) { exhausted =true;break; }// 如果之前执行出错过,就不执行了if(blacklist_by_id.find(trx) != blacklist_by_id.end()) {continue; }// 以下的逻辑差不多try{autodeadline = fc::time_point::now() + fc::milliseconds(_max_transaction_time_ms);booldeadline_is_subjective =false;if(_max_transaction_time_ms <0|| (_pending_block_mode == pending_block_mode::producing && block_time < deadline)) { deadline_is_subjective =true; deadline = block_time; }autotrace = chain.push_scheduled_transaction(trx, deadline);if(trace->except) {if(failure_is_subjective(*trace->except, deadline_is_subjective)) { exhausted =true; }else{autoexpiration = fc::time_point::now() + fc::seconds(chain.get_global_properties().configuration.deferred_trx_expiration_window);// this failed our configured maximum transaction time, we don’t want to replay it add it to a blacklist_blacklisted_transactions.insert(transaction_id_with_expiry{trx, expiration}); } } }catch(constguard_exception& e ) { app().get_plugin().handle_guard_exception(e);returnstart_block_result::failed; } FC_LOG_AND_DROP(); _incoming_trx_weight += _incoming_defer_ratio;if(!orig_pending_txn_size) _incoming_trx_weight =0.0; } }

最后会再检查下执行时间,同时发起对incoming_transaction的处理,如果没问题的话,会返回成功:

if(exhausted || block_time <= fc::time_point::now()) {returnstart_block_result::exhausted; }else{// attempt to apply any pending incoming transactions_incoming_trx_weight =0.0;while(orig_pending_txn_size && _pending_incoming_transactions.size()) {autoe = _pending_incoming_transactions.front(); _pending_incoming_transactions.pop_front(); --orig_pending_txn_size; on_incoming_transaction_async(std::get<0>(e),std::get<1>(e),std::get<2>(e));if(block_time <= fc::time_point::now())returnstart_block_result::exhausted; }returnstart_block_result::succeeded; }

以上就是producer_plugin_impl::start_block的流程,这个过程成功之后,如果是出块节点,此时的pending_block已经计算好了,下一步就是调用maybe_produce_block完成出块流程.

4.出块逻辑 producer_plugin_impl::maybe_produce_block()

上述start_block执行完毕之后,如果出块正常,则会调用maybe_produce_block:

_timer.async_wait([&chain,weak_this,cid=++_timer_corelation_id](constboost::system::error_code& ec) {autoself = weak_this.lock();if(self && ec != boost::asio::error::operation_aborted && cid == self->_timer_corelation_id) {// pending_block_state expected, but can’t assert inside async_waitautoblock_num = chain.pending_block_state() ? chain.pending_block_state()->block_num :0;autores = self->maybe_produce_block(); fc_dlog(_log,“Producing Block #${num} returned: ${res}”, (“num”, block_num)(“res”, res)); } });

让我们来看下maybe_produce_block实现:

boolproducer_plugin_impl::maybe_produce_block() {autoreschedule = fc::make_scoped_exit([this]{// 完成之后会回调schedule_production_loop 来继续出下一个块schedule_production_loop(); });try{// 完成出块并广播produce_block();returntrue; }catch(constguard_exception& e ) { app().get_plugin().handle_guard_exception(e);returnfalse; }catch( boost::interprocess::bad_alloc& ) { raise(SIGUSR1);returnfalse; } FC_LOG_AND_DROP(); fc_dlog(_log,“Aborting block due to produce_block error”);// 清理缓存块的状态chain::controller& chain = app().get_plugin().chain(); chain.abort_block();returnfalse;}

以下是produce_block:

voidproducer_plugin_impl::produce_block() {//ilog(“produce_block ${t}”, (“t”, fc::time_point::now())); // for testing _produce_time_offset_us// 确定一定是出块状态才能调用到这里EOS_ASSERT(_pending_block_mode == pending_block_mode::producing, producer_exception,“called produce_block while not actually producing”);//检查chain::controller& chain = app().get_plugin().chain();constauto& pbs = chain.pending_block_state();constauto& hbs = chain.head_block_state(); EOS_ASSERT(pbs, missing_pending_block_state,“pending_block_state does not exist but it should, another plugin may have corrupted it”);autosignature_provider_itr = _signature_providers.find( pbs->block_signing_key ); EOS_ASSERT(signature_provider_itr != _signature_providers.end(), producer_priv_key_not_found,“Attempting to produce a block for which we don’t have the private key”);//idump( (fc::time_point::now() - chain.pending_block_time()) );// 完成出块chain.finalize_block();// 签名区块chain.sign_block( [&](constdigest_type& d ) {autodebug_logger = maybe_make_debug_time_logger();returnsignature_provider_itr->second(d); } );// 把完成的区块广播出去,并调用其他系统的handle函数chain.commit_block();autohbt = chain.head_block_time();//idump((fc::time_point::now() - hbt));block_state_ptr new_bs = chain.head_block_state(); _producer_watermarks[new_bs->header.producer] = chain.head_block_num(); ilog(“Produced block KaTeX parse error: Expected 'EOF', got '#' at position 9: {id}... #̲{n} @ ${t} signed by ${p} [trxs: ${count}, lib: ${lib}, confirmed: ${confs}]”, (“p”,new_bs->header.producer)(“id”,fc::variant(new_bs->id).as_string().substr(0,16)) (“n”,new_bs->block_num)(“t”,new_bs->header.timestamp) (“count”,new_bs->block->transactions.size())(“lib”,chain.last_irreversible_block_num())(“confs”, new_bs->header.confirmed));}

在finalize_block中会首先更新cpu和net的资源:

// Update resource limits:resource_limits.process_account_limit_updates();constauto& chain_config = self.get_global_properties().configuration;uint32_tmax_virtual_mult =1000;uint64_tCPU_TARGET = EOS_PERCENT(chain_config.max_block_cpu_usage, chain_config.target_block_cpu_usage_pct); resource_limits.set_block_parameters( { CPU_TARGET, chain_config.max_block_cpu_usage, config::block_cpu_usage_average_window_ms / config::block_interval_ms, max_virtual_mult, {99,100}, {1000,999}}, {EOS_PERCENT(chain_config.max_block_net_usage, chain_config.target_block_net_usage_pct), chain_config.max_block_net_usage, config::block_size_average_window_ms / config::block_interval_ms, max_virtual_mult, {99,100}, {1000,999}} ); resource_limits.process_block_usage(pending->_pending_block_state->block_num);

之后计算hash, 首先是所有action的

voidset_action_merkle(){vector action_digests; action_digests.reserve( pending->_actions.size() );for(constauto& a : pending->_actions ) action_digests.emplace_back( a.digest() );// 计算merkle hash值pending->_pending_block_state->header.action_mroot = merkle( move(action_digests) ); }

之后是所有交易的:

voidset_trx_merkle(){vector trx_digests;constauto& trxs = pending->_pending_block_state->block->transactions; trx_digests.reserve( trxs.size() );for(constauto& a : trxs ) trx_digests.emplace_back( a.digest() ); pending->_pending_block_state->header.transaction_mroot = merkle( move(trx_digests) ); }

最后生产区块摘要信息:

voidcreate_block_summary(constblock_id_type& id){autoblock_num = block_header::num_from_id(id);autosid = block_num &0xffff; db.modify( db.get(sid), [&](block_summary_object& bso ) { bso.block_id = id; }); }

至此所有区块的数据已经完备.下一步是为区块签名:

void sign_block( const std::function<signature_type( const digest_type& )>& signer_callback ) {

  auto p = pending->_pending_block_state;

  p->sign( signer_callback );

  static_cast<signed_block_header&>(*p->block) = p->header;
1
2
3
4
5
} /// sign_block

签名成功之后就是调用commit_block

voidcommit_block(booladd_to_fork_db ){// 这一步是最后一步了,执行完成之后会清理待发送的区块信息autoreset_pending_on_exit = fc::make_scoped_exit([this]{ pending.reset(); });try{// 如果是自己出的块,会默认接受if(add_to_fork_db) { pending->_pending_block_state->validated =true;autonew_bsp = fork_db.add(pending->_pending_block_state); emit(self.accepted_block_header, pending->_pending_block_state); head = fork_db.head(); EOS_ASSERT(new_bsp == head, fork_database_exception,“committed block did not become the new head in fork database”); }// 如果是回放 则直接写入reversible_blocks中if( !replaying ) { reversible_blocks.create( [&](auto& ubo ) { ubo.blocknum = pending->_pending_block_state->block_num; ubo.set_block( pending->_pending_block_state->block ); }); }// 触发accepted_block信号, 调用其它模块的回调emit( self.accepted_block, pending->_pending_block_state ); }catch(…) {// dont bother resetting pending, instead abort the blockreset_pending_on_exit.cancel(); abort_block();throw; }// push the state for pending.pending->push(); }

至此以上就是整个出块流程, accepted_block信号会触发net_plugin_impl中的回调,最终会调用net_plugin中的void dispatch_manager::bcast_block (const signed_block &bsum)函数,将新出的块广播给所有对端.

5.需要留意的问题
————————————————
版权声明：本文为CSDN博主「EOS原力」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_43458313/java/article/details/84029555