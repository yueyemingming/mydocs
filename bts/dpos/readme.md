# DPOS共识算法 -- 缺失的白皮书

- [DPOS算法概要](#dpos算法概要)
  - [情况1 正常操作](#情况1-正常操作)
  - [情况2 少数分叉](#情况2-少数分叉)
  - [情况3 离线少数的多重生产](#情况3-离线少数的多重生产)
  - [情况4 网络碎片化](#情况4-网络碎片化)
  - [情况5 在线少数的多重生产](#情况5-在线少数的多重生产)
  - [情况6 最后不可逆块](#情况6-最后不可逆块)
  - [情况7 生产者法定人数不足](#情况7-生产者法定人数不足)
  - [情况8 多数生产者舞弊](#情况8-多数生产者舞弊)
- [交易作为权益证明（TaPoS）](#交易作为权益证明tapos)
- [确定性生产者洗牌](#确定性生产者洗牌)
- [结论](#结论)

这篇“缺失的白皮书”是对委托权益证明（DPOS）的分析，目的是为DPOS的工作原理及其鲁棒性根源提供一个分析。

DPOS的早期描述可以在bitshares.org上找到；不过，那个描述还包含了许多不属于实际共识过程的内容。

所有区块链本质上都是一种由交易驱动的确定性状态机。共识是商定确定性交易顺序和过滤无效交易的过程。有许多不同的共识算法都可以产生等效的交易排序，但DPOS已经通过在多个区块链上经年累月的可靠运行证明自身是健壮、安全和有效的。

像所有共识算法一样，块生产者可能导致的最大损害是审查。所有块的有效性必须基于确定性的开源状态机逻辑。

## DPOS算法概要

DPOS算法分为两部分：选择一组块生产者和调度生产。选举过程确保利益相关方最终得到控制，因为当网络不能顺利运行时，利益相关方的损失最大。选举方法对实际运行中如何达成共识几乎没有影响，因此，本文将重点介绍如何在块生产者被选择之后达成共识。

为了帮助解释这个算法，我想假设3个块生产者A，B和C。因为共识（的达成）需要2/3+1多数来解决所有情况，这个简化的模型将假设生产者C是打破僵局的那个人。在现实世界中，将有21个或更多的块生产者。像工作量证明一样，一般规则是最长链胜出。任何时候当一个诚实的对等节点看到一个有效的更长链，它都会从当前分叉切换到更长的这条链。

我将举例说明在大多数想得到的网络条件下DPOS如何运行。这些例子应该可以帮助您理解为什么DPOS稳健且难以破坏。

### 情况1 正常操作

在正常操作模式下，块生产者每3秒钟轮流生成一个块。假设没有人错过自己的轮次，那么这将产生最长链。块生产者在被调度轮次之外的任何时间段出块都是无效的。

![001.png](001.png)

### 情况2 少数分叉

不超过节点总数三分之一的恶意或故障节点可能创建少数分叉。在这种情况下，少数分叉每9秒只能产生一个块，而多数分叉每9秒可以产生两个块。这样，诚实的2/3多数将永远比少数（的链）更长。

![002.png](002.png)

### 情况3 离线少数的多重生产

（离线的）少数人可以试图产生无限数量的分叉，但是他们的所有分叉都将比多数人的那条链短，因为少数人在出块速度上注定比多数人来的更慢。

![003.png](003.png)

### 情况4 网络碎片化

网络完全有可能碎片化，导致没有任何分叉拥有多数块生成者。在这种情况下，最长的链将倒向最大的那个少数群体。当网络连通性恢复时，较小的少数群体会自然切换到最长的那条链，明确的共识将恢复。

![004.png](004.png)

有可能存在这样三个分叉，其中两个最长的分叉长度相同。在这种情况下，第3个（较小）分叉的块生产者重新加入网络时会打破平局。块生产者总数为奇数，因此不可能长时间保持平局。稍后我们还会讲到生产者“洗牌”，它使得出块顺序随机化，从而确保即使是生产者数目相同的两个分叉也会以不同的步长增长，最终导致一个分叉超过另一个。

### 情况5 在线少数的多重生产

在这种场景下，少数节点B在其时间段内产生了两个或更多可供选择的块。下一个计划生产者（C）可以选择基于B产生的任何一种方案继续构建链条。一旦如此，这个选择就成为最长的链，而所有选择B1的节点都将切换分叉。少数不良生产者企图广播再多的替代块也无关紧要，它们作为最长链的一部分永远不会超过一轮。

![005.png](005.png)

### 情况6 最后不可逆块

在网络碎片化的情况下，多个分叉都有可能持续不断增长相当长的时间。长远来看最长的链终将获胜，但观察者需要一种确切的手段来判定一个块是否绝对处于增长最快的那条链。这可以通过观察来自2/3+1多数块生产者的确认来决定。

在下图中，块B已被C和A所确认，这代表了2/3+1多数确认，由此我们可以推断没有其它链会比这个更长 – 如果2/3的生产者是诚实的。

![006.png](006.png)

请注意，这个“规则”类似于比特币的6块确认“规则”。一些聪明人也许可以谋划一系列事件使得两个节点（应该是“交易”？）出现在不同的最后不可逆块上。这种边缘案例要求攻击者能完全控制通信延迟，并且在几分钟内两次--而不是一次--使用该控制。即便这真的发生了，那么最长链（胜出）的长期规则仍然适用。我们估计这种攻击的可能性足够接近0，且经济后果无关紧要，因此不足为虑。

### 情况7 生产者法定人数不足

在缺乏明晰的生产者法定人数这种不太可能的情况下，少数人还是可以继续出块。利益相关方可以在这些块里包括更改投票的交易。这些投票可以选出一组新的生产者，并将出块参与率恢复到100％。一旦如此，少数链将最终超过所有其他以低于100％参与率运行的链。

在此过程中，所有观察者都会知道，在一条参与率超过67%的链形成之前，网络状态是不定的。那些选择在此条件下进行交易的人所冒的风险与选择接受不到6个确认的人相似。他们知道存在这样一些小的可能性，即：共识也许最终在一个不同的分叉上建立起来。在实践中，这种情况比接受少于3个比特币交易确认的块要安全多了。

### 情况8 多数生产者舞弊

如果多数生产者变得腐败，那么他们可以产生无限数量的分叉，每个分叉都看起来以2/3多数确认向前走。这种情况下，最后不可逆块算法蜕变为最长链算法。最长链就是为最大多数所批准的那条链，而这将由少数剩下的诚实节点决定。这种行为不会持续很长时间，因为利益相关方最终会投票替换生产者。

![007.png](007.png)

## 交易作为权益证明（TaPoS）

当用户为一个交易签名时，他们是在对区块链状态的一定假设下这样做的。这个假设是基于他们对最近几个块的看法。如果最长链的共识发生改变，则潜在会使签名者之前的假设失效。

就TaPoS而言，所有交易都包含最近一个块的散列，如果该块在链历史中不存在则这些交易被认为是无效的。任何在孤儿分叉上给交易签名的人，都会发现该交易无效且无法迁移到主分叉。

该过程的一个附带作用是可以抵御试图产生替代链的长期攻击。每个利益相关方在每次交易时都直接对区块链做出确认。随着时间推移，所有的块都是由所有利益相关方确认过的，这在一条伪造链里是无法复制的。

## 确定性生产者洗牌

在上面所有例子中，我们展示的都是块生产者按循环调度出块。实际上，每出N个块（N是生产者数量），块生产者集合都会洗牌一次。这种随机性确保块生成者B不会总是忽略块生成者A，每当形成多个拥有相同数量生产者的分叉时，平局最终都会被打破。

## 结论

在每一个我们能想到的自然网络分裂的情况下，委托权益证明都是强健的，甚至在面对相当数量生产者舞弊的情形时也是安全的。不像其它共识算法，当大多数生产者不合格时，DPOS还是可以继续工作。在此过程中，社区可以投票替换掉不合格的生产者，直到恢复100％参与率。我还不知道有任何其它算法可以在如此高强度和变化多端的失败条件下依然保持强健。

说到底，DPOS引人注目的安全性来自于其选择块生产者和验证节点质量的算法。运用赞成投票的过程可以确保一个人即使拥有50％的有效投票权也不能独自挑选哪怕一个生产者。DPOS旨在优化拥有强壮网络连接的诚实节点100％参与（共识过程）的名义条件。这使得DPOS有能力在平均只有1.5秒的时间内以99.9％的确定性确认交易，同时以优雅和可检测的方式降级 – 从降级中恢复正常也不过是小事一桩。

其它共识算法以网络条件差的不诚实节点为名义条件展开设计，这样设计的最终结果就是性能更差、延迟更高、通信开销高的网络，而且这个网络在33％节点失效的情况下会完全停摆。

在BitShares成功运行三年以及在Steem运行一年期间，我们经历了各种各样的网络条件和软件错误。DPOS成功穿行于其间，在处理了比任何其它区块链更多交易的同时持续达成共识，展现了非凡的能力。