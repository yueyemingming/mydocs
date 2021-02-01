# QT——延时\定时器问题的汇总

QT延时/等待/定时器怎么写？大概分为两类，一个是非阻塞延时，一个是阻塞延时，这里做一个小结，第三种耗时方法用的较少

------

**一、阻塞型延时**
Qt一般使用QThread::sleep()来延时，但是这样会阻塞住线程，体验太差，这样只可以用在子线程中。

```c
QThread::msleep(50);//阻塞延时50ms
```

或者使用定时器：

```c
void Delay_MSec_Suspend(unsigned int msec)
{    
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < _Timer );
}
```

**二、非阻塞型延时**
在等待中，不断强制进入当前线程的事件循环，这样可以把堵塞的事件都处理掉，从而避免程序卡死
可以处理本线程的事件循环

```c
void Widget::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime ) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        //这条语句能够使程序在while等待期间，去处理一下本线程的事件循环，处理事件循环最多100ms必须返回本语句，
        //如果提前处理完毕，则立即返回这条语句。这也就导致了该Delay_MSec函数的定时误差可能高达100ms。
    }
}
```

或者使用子事件循环，在子事件循环中，父事件循环仍然是可以执行的

```c
void Delay_MSec(unsigned int msec)
{
    QEventLoop loop;	//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));	//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();	//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}
```

**三、耗时代码**
例如后台需要载入图片，GUI响应太卡，可以使用这种方法

```c
for(i=0; i < 1000000; i++)
{
    //QCoreApplication::processEvents(QEventLoop::AllEvents);    
    //去处理本线程的事件循环，避免本线程被堵塞
    QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
    //如果不够频繁，可以增加第二参数来缓解卡顿
 
    for(j=0; j < 1000000; j++)
    {
        //QCoreApplication::processEvents(QEventLoop::AllEvents);
        //处理事件循环，不建议放在这里，可能过于频繁
        doSomeThing();
    }
}

123456789101112131415
```

一般来说，processEvents()不宜被调用的过于频繁，也不宜被调用的不够频繁。过于频繁的话，一方面会使线程的响应更好，但另一方面会导致原本就耗时的任务变得更加耗时；不够频繁的话，显然可能会使GUI线程的响应变差，例如每500ms才被调用一次，那么GUI的事件循环就只能500ms才被处理一次，当然，这个问题可以通过设定processEvents()的第二个形参略微得到缓解，更好的做法是，保证被调的周期<200ms（再小一些更好，看程序需求），这样不至于肉眼可见的卡顿。

副作用：（特别注意！）

1、在点击按钮之后，这个20s的耗时任务开始执行，尚未执行完毕时，我们点击了GUI的关闭按钮，那么GUI会立即消失，但是这个耗时任务仍然会在后台执行，直到执行完毕，进程才会退出。解决办法：重写关闭事件，在关闭事件的函数中直接结束进程。
2、在点击按钮之后，这个20s的耗时任务开始执行，执行到第5秒时，我们再次点击了这个按钮，那么QT又会执行一个新的20s任务，这个新任务完成后，又会接着把第一个20s任务从上次被打断的第5秒继续执行。如果这个任务是可重入的，后果仅仅是被执行了两遍，如果任务不可重入，那情况就彻底糟糕了。解决办法：点击按钮后把这个按钮disable掉，执行完再enable