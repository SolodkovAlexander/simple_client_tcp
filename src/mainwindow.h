#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void onConnect();
    void onConnected();
    void onDisconnected();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onSendMessage();
    void onReceiveMessage();

private:
    void log(const QString &message);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
};

#endif // MAINWINDOW_H
