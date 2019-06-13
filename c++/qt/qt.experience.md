# Qt经验总结

1. 创建图形工程，尽可能带上ui模块，目的是为了，防止手动创建ui组件，要手动析构的麻烦和不必要的错误。
2. 经典的后台控制前台ui模型：**多线程+信号槽** 方式。

    **mythread.h  线程头文件**

    ```c++
    #ifndef MYTHREAD_H
    #define MYTHREAD_H

    #include <QThread>
    #include "widget.h"
    class Widget ;              //主窗口声明

    class MyThread : public QThread
    {
        Q_OBJECT                //信号槽的关键语句

    public:
        explicit MyThread(Widget *pWin = nullptr);
        ~MyThread();
        Widget* pWin;           //窗口句柄
        void run();

    signals:
        void sigUI(const int);  //信号
    };

    #endif // MYTHREAD_H
    ```

    **myThread.cpp    线程实体**

    ```cpp
    #include "mythread.h"

    MyThread::MyThread(Widget* p) : pWin(p) {
        //连接主窗口的信号
        connect( this, SIGNAL(sigUI(const int)), pWin, SLOT(uiSlot(const int)) );
    }

    MyThread::~MyThread() {}

    void MyThread::run(){
        emit sigShow(100);              //调用信号槽
    }
    ```

    **widget.h    主窗口头文件**

    ```cpp
    #include "mythread.h"

    class MyThread;                     //声明子线程

    class Widget : public QWidget
    {
        Q_OBJECT

        MyThread* myThread;             //添加子线程

    public slots:
        void uiSlot(const int i=0);     //声明槽
    ```

    **widget.cpp  主窗口实现**

    ```cpp
    Widget::Widget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Widget),
        myThread(new MyThread(this))
    {
        ui->setupUi(this);

        myThread->start();              //子线程启动
        ...
    }

    void Widget::uiSlot(const int i){   //槽实现
        qDebug() << i ;
    }

    ...
    ```

3. 信号槽的设计原因：为了保证信号在一个线程中，槽在另外一个线程中，实现线程中通信的目的。比如窗口线程的槽就在窗口线程中运行，不应当后台线程直接控制窗口上的ui组件，而是通过信号槽机制来控制窗口ui。
4. 定时器也能实现后台流程，但能用线程的，尽可能用线程，原因有2：
    * 定时器cpu占用率远比线程高。
    * 定时器本身并非以单独线程身份在后台定时运行，而是在原线程中，定时器中有sleep操作，原线程也会同时sleep暂停运行。
