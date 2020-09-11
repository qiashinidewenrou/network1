#ifndef TTESTFORREMOTECTRLPC_H
#define TTESTFORREMOTECTRLPC_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "tremotectrlpc.h"
#include <QLineEdit>

namespace Ui {
class TTestForRemoteCtrlPC;
}

class TTestForRemoteCtrlPC : public QWidget
{
    Q_OBJECT

public:
    explicit TTestForRemoteCtrlPC(QWidget *parent = 0);
    ~TTestForRemoteCtrlPC();

private:
    Ui::TTestForRemoteCtrlPC *ui;

    QLineEdit *lineditwake;

    QLineEdit *lineditShutdown;

public slots:
    void slotWake();
    void slotShutdown();

private:
    void initUI(void);
    QComboBox *m_pComboxWake;
    QComboBox *m_pComboxShutdown;
    TRemoteCtrlPC *m_pRemoteCtrlPC;

};

#endif // TTESTFORREMOTECTRLPC_H
