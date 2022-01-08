#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    //make connections
    connect(ui->buttonConnect, &QPushButton::clicked, this, &MainWindow::onConnect);
    connect(ui->buttonSendMessage, &QPushButton::clicked, this, &MainWindow::onSendMessage);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReceiveMessage);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, &MainWindow::onSocketError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onConnect()
{
    //client disconnect
    if (socket->state() != QAbstractSocket::UnconnectedState)
    {
        log(QString("Client (%1:%2) started disconnect from server (%3:%4).")
            .arg(socket->localAddress().toString(),
                 QString::number(socket->localPort()),
                 socket->peerAddress().toString(),
                 QString::number(socket->peerPort())));
        socket->disconnectFromHost();
    }

    //client connect
    QString addressStr = ui->editAddress->text();
    quint16 port = ui->editPort->text().toUShort();
    log(QString("Client started connect to server (%1:%2).")
        .arg(addressStr,
             QString::number(port)));

    /*socket->bind();
     * don't use this, because address will shown as (::ffff:127.0.0.1:35693)*/
    socket->connectToHost(QHostAddress(addressStr), port);
}

void MainWindow::onConnected()
{
    log(QString("Client (%1:%2) finished connect to server (%3:%4).")
        .arg(socket->localAddress().toString(),
             QString::number(socket->localPort()),
             socket->peerAddress().toString(),
             QString::number(socket->peerPort())));
}

void MainWindow::onDisconnected()
{
    log(QString("Client (%1:%2) finished disconnect from server (%3:%4).")
        .arg(socket->localAddress().toString(),
             QString::number(socket->localPort()),
             socket->peerAddress().toString(),
             QString::number(socket->peerPort())));
}

void MainWindow::onSocketError(QAbstractSocket::SocketError socketError)
{
    log(QString("Error (%1): %2.")
        .arg(QString::number(socketError),
             socket->errorString()));
}

void MainWindow::onSendMessage()
{
    QString message = ui->editMessage->toPlainText();

    log(QString("Client (%1:%2) started send message (\"%3\") [%4 bytes] to server (%5:%6).")
        .arg(socket->localAddress().toString(),
             QString::number(socket->localPort()),
             message,
             QString::number(message.toUtf8().count()),
             socket->peerAddress().toString(),
             QString::number(socket->peerPort())));

    qint64 bytesWritten = socket->write(message.toUtf8());
    log(QString("Client finished send message (%1 bytes).")
        .arg(bytesWritten));
}

void MainWindow::onReceiveMessage()
{
    QByteArray messageBytes = socket->readAll();
    log(QString("Message (\"%1\") [%2 bytes] received from server (%3:%4).")
        .arg(QString::fromUtf8(messageBytes),
             QString::number(messageBytes.count()),
             socket->peerAddress().toString(),
             QString::number(socket->peerPort())));
}

void MainWindow::log(const QString &message)
{
    ui->editLog->appendPlainText(message);
}
