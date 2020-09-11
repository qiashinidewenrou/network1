#include "tremotectrlpc.h"
#include <QHostAddress>
#include <QUdpSocket>
#include <QProcess>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>

TRemoteCtrlPC::TRemoteCtrlPC(QObject *parent)
{
    Q_UNUSED(parent)
}

void TRemoteCtrlPC::wakeOnLAN(QString strMac)
{
    /*将字符串MAC转换成十六进制 存入数组*/
    char cstrMacAddr[6];
    bool isOK;
    for(int j = 0;j < 6; j++){
        if(j < 5){
            cstrMacAddr[j] = strMac.mid(strMac.indexOf(":",j*3) - 2,2).toInt(&isOK,16);
            if(false == isOK)
                return ;
        }else if(j == 5){
            cstrMacAddr[j] = strMac.mid(strMac.indexOf(":",j*3 - 1) + 1,2).toInt(&isOK,16);
            if(false == isOK)
                return ;
        }
    }

    /*构建一个魔术唤醒包  6字节0xFF 和  16次的 Mac地址*/
    QByteArray magicPacket;

    /*6个 0xFF*/
    for(int i = 0;i < 6; i++){
        magicPacket[i] = 0xFF;
    }

    /*16个 MAC*/
    for(int k = 0; k < 16; k++)
    {
        for(int l = 0; l < 6; l++)
            magicPacket[(k+1)*6 + l] = cstrMacAddr[l];
    }

    QHostAddress FakeAddress;
    FakeAddress.setAddress ("192.168.2.255");

    QUdpSocket udpSocket;
    udpSocket.writeDatagram(magicPacket, 102, FakeAddress, 9);
}

void TRemoteCtrlPC::remoteShutDown(QString strIp)
{
        QJsonObject json;
        json.insert("type_code", "run_off");
        QJsonDocument jsonDoc(json);
        QByteArray array = jsonDoc.toJson(QJsonDocument::Indented);

        QTimer timer;
        timer.setInterval(3000); // 设置超时时间 3 秒
        timer.setSingleShot(true); // 单次触发

        QString url = "http://"+strIp+":8083/mmccs/api/exe_control";

        QUrl surl;
        surl.setUrl(url);
        QNetworkRequest request(surl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=UTF-8"));

        QNetworkAccessManager ma;
        QNetworkReply *reply = ma.post(request, array);;
        QEventLoop eventloop;
        QObject::connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
        connect(&timer, &QTimer::timeout, &eventloop, &QEventLoop::quit);
        timer.start();
        eventloop.exec();

        QByteArray backarray;
        ///////////////////////////////
        if (timer.isActive())
        {
            // 处理响应
            timer.stop();
            if (reply->error() != QNetworkReply::NoError)
            {
                // 错误处理
                qDebug() << "error:" << reply->errorString();
                reply->abort();
                reply->deleteLater();
                reply = nullptr;
                return;
            }
            else
            {
                backarray = QString::fromLocal8Bit(reply->readAll()).toUtf8();
                reply->abort();
                reply->deleteLater();
                reply = nullptr;
                return;
            }
        }
        else
        {
            // 处理超时
            disconnect(reply, &QNetworkReply::finished, &eventloop, &QEventLoop::quit);
            reply->abort();
            reply->deleteLater();
            qDebug() << "Timeout";
            return;
        }
        //////////////////////////////


    #if 0
    m_strIp = strIp;
    qDebug()<<"m_strIp:"<<m_strIp;
    /*命令的执行过程会阻塞 放入线程执行*/
    this->start();
    #endif
}

void TRemoteCtrlPC::run()
{
    QProcess cmd;
    QString strResult;
    QByteArray arrayOut;
    QString strCmd = "shutdown -s -t 5 -m \\\\"; //四个斜杠  转义
    strCmd += m_strIp;


    qDebug()<<"关闭电脑:"<<strCmd;

    cmd.start(strCmd);
    cmd.waitForFinished();       // !!!阻塞操作 放在主线程会导致窗口卡死  直至执行完成

    arrayOut = cmd.readAllStandardOutput();
    strResult = QString::fromLocal8Bit(arrayOut);

    emit signalCmdFinished(strResult); //命令执行完成   返回命令执行结果
}

