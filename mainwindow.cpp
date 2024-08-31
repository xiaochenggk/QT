#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tcpServer = nullptr;
    tcpSocket = nullptr;

    //创建监听对象
    tcpServer = new QTcpServer(this);//第一步中的创建TcpServer对象
    //监听端口和IP
    tcpServer->listen(QHostAddress::Any,8080);//第二步中使用QTcpServer::listen函数监听服务器所在的ip和端口

    connect(tcpServer,&QTcpServer::newConnection,this,[=](){//讲过的newConnection信号，判断客户端发送来一个新的连接请求后，QTcpServer对象
                                                            //会发送一个newConnection信号，不算到socket编程步骤，这是QT中对于网络编程一个
                                                            //特有的机制，方便编程
        //取出建立好的套接字
        tcpSocket = tcpServer->nextPendingConnection();
        //获取对方的IP和端口
        QString ip = tcpSocket->peerAddress().toString();
        qint16 port = tcpSocket->peerPort();
        QString temp = QString("[%1:%2]:成功连接").arg(ip).arg(port);//获取IP地址没啥好讲的，记住peerAddress和peerPort这两个函数就行了。
                                                                    //peerAddress负责获取服务器监控ip，peerPort获取端口号。但是这里不
                                                                    //属于网络编程的步骤，相当于优化一下功能而已
        ui->textEditRead->setText(temp);//再将数据获取的IP和端口发送给textEditRead控件

        connect(tcpSocket,&QTcpSocket::readyRead,[=](){//当套接字接收到数据以后会发送一个readyRead信号，又是一个优质的信号与槽机制
            //去除全部文本
            QByteArray array = tcpSocket->readAll();//触发信号后我们将接收到的信号读出来，readyRead信号和readAll函数的结合操作
                                                    //这就是我们的热血的组合技
            ui->textEditRead->append(array);//这里不使用setText而是使用append是因为append为追加，如果使用setText每次都会被覆盖
                                            //也就代表着如果有多批文本传送过来，前一个文本被后一个文本覆盖无法显示所有内容。那么使用
                                            //append函数的话就会在第一个文本后追加内容而不会覆盖了。
        });

    });


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_buttonSend_clicked()//ui中的send按钮
{
    if(nullptr == tcpSocket){
        return;
    }
    //获取编辑区中我输入的文本
    QString str = ui->textEdit_2->toPlainText();
    //把上面获取的文本数据发送给客户端
    tcpSocket->write(str.toUtf8().data());
}


void MainWindow::on_pushButton_2_clicked()//关闭套接字
{
    if(nullptr == tcpSocket){
        return;
    }
    tcpSocket->disconnectFromHost();//记住这个函数就行，用于关闭套接字
    tcpSocket->close();
    tcpSocket = nullptr;
}

