#ifndef NATIVESOCKET_H
#define NATIVESOCKET_H

#include "socket.h"

class QUdpSocket;

class NativeServerSocket: public ServerSocket{
    Q_OBJECT

public:
    NativeServerSocket();

    virtual bool listen();
    virtual void daemonize();

private slots:
    void processNewConnection();
    void processNewDatagram();

private:
    QTcpServer *server;
    QUdpSocket *daemon;
};


class NativeClientSocket: public ClientSocket{
    Q_OBJECT

public:
    NativeClientSocket();
    NativeClientSocket(QTcpSocket *socket);

    virtual void connectToHost();
    void connectToNode(QString addr, int port);
    virtual void disconnectFromHost();
    virtual void send(const QString &message);
    virtual bool isConnected() const;
    virtual QString peerName() const;
    virtual QString peerAddress() const;

private slots:
    void getMessage();
    void raiseError(QAbstractSocket::SocketError socket_error);

private:
    QTcpSocket * const socket;

    void init();
};

#endif // NATIVESOCKET_H
