#include "client.h"
#include "ui_client.h"
#include <QHostAddress>

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    tcpSocket = nullptr;
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,&QTcpSocket::connected,[=](){//主要部分为connected，它与服务器的newConnection是对应的。
                                                   //newConnection为接收方获取请求后发送的信号，本质是被动的
                                                   //connected为确认连接请求发送方发送连接后是否连接成功，成功就会发送这个信号
        ui->textEditRead->setText("连接服务器成功");//连接成功后向客户端中的textEditRead控件发送一个可视化的文本通知用户两端已经连接成功了

    });
    connect(tcpSocket,&QTcpSocket::readyRead,[=](){//看服务端讲的，跟那里差不多
        //获取对方发送的内容
        QByteArray array = tcpSocket->readAll();
        //客户端显示服务端发送的内容
        ui->textEditRead->append(array);
    });
}

Client::~Client()
{
    delete ui;
}

void Client::on_buttonConnect_clicked()
{
    //获取服务器ip和端口
    qint16 port = ui->lineEditPort->text().toInt();
    QString ip = ui->lineEditIP->text();

    //发送连接请求
    tcpSocket->connectToHost(QHostAddress(ip),port);

}


void Client::on_buttonSend_clicked()
{   if(nullptr == tcpSocket){
        return;
    }
    //获取编辑的文本
    QString str = ui->textEditWrite->toPlainText();
    //发送数据
    tcpSocket->write(str.toUtf8().data());
}


void Client::on_buttonClose_clicked()//关闭套接字
{
    if(nullptr == tcpSocket){
        return;

    }
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    tcpSocket = nullptr;
}

