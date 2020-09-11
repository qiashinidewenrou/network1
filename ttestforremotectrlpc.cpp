#include "ttestforremotectrlpc.h"
#include "ui_ttestforremotectrlpc.h"

TTestForRemoteCtrlPC::TTestForRemoteCtrlPC(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TTestForRemoteCtrlPC)
{
    ui->setupUi(this);
    initUI();
}

TTestForRemoteCtrlPC::~TTestForRemoteCtrlPC()
{
    delete ui;
}

void TTestForRemoteCtrlPC::initUI()
{
    resize(200,200);

    m_pRemoteCtrlPC = new TRemoteCtrlPC(this);

    #if 0
    m_pComboxWake = new QComboBox(this);
    m_pComboxWake->addItem("00:2B:6C:68:09:3B");
    m_pComboxWake->setEditable(true);
    m_pComboxWake->setFixedHeight(30);
#endif

    lineditwake = new QLineEdit(this);
    lineditwake->setFixedHeight(30);

    QPushButton *buttonWake = new QPushButton("Wake",this);
    buttonWake->setFixedHeight(30);
    connect(buttonWake,SIGNAL(clicked()),this,SLOT(slotWake()));

#if 0
    m_pComboxShutdown  = new QComboBox(this);
    m_pComboxShutdown->addItem("192.168.2.193");
    m_pComboxShutdown->setEditable(true);
    m_pComboxShutdown->setFixedHeight(30);
#endif
    lineditShutdown = new QLineEdit(this);
    lineditShutdown->setFixedHeight(30);

    QPushButton *buttonShutdown = new QPushButton("ShutDown",this);
    buttonShutdown->setFixedHeight(30);
    connect(buttonShutdown,SIGNAL(clicked()),this,SLOT(slotShutdown()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addSpacing(20);
    layout->addWidget(lineditwake);
    layout->addSpacing(20);
    layout->addWidget(buttonWake);
    layout->addSpacing(20);
    layout->addWidget(lineditShutdown);
    layout->addSpacing(20);
    layout->addWidget(buttonShutdown);

    this->setLayout(layout);
}

void TTestForRemoteCtrlPC::slotWake()
{
    qDebug("唤醒电脑 ");

    QString strMac = "D0-DF-9A-D7-4B-FD";
  //  m_pRemoteCtrlPC->wakeOnLAN(strMac);
  //  m_pRemoteCtrlPC->wakeOnLAN();
    m_pRemoteCtrlPC->wakeOnLAN(lineditwake->text());
}

void TTestForRemoteCtrlPC::slotShutdown()
{
    qDebug("关闭电脑 ");
    m_pRemoteCtrlPC->remoteShutDown(lineditShutdown->text());
}
