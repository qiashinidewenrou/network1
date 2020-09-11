#include "widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->createMenu();
}

Widget::~Widget()
{

}

void Widget::createMenu()
{
    m_menu1 = new QMenu();
    m_menu1->addAction("1_1",this,SLOT(actionsSlot()));
    m_menu1->addAction("1_2",this,SLOT(actionsSlot()));
    m_menu1->addAction("1_3",this,SLOT(actionsSlot()));

    QMenu* menu2 = m_menu1->addMenu("2_0");
    menu2->addAction("2_1",this,SLOT(actionsSlot()));

    QMenu* menu3 = menu2->addMenu("3_0");
    menu2->addSeparator();
    menu3->addAction("3_1",this,SLOT(actionsSlot()));
    menu3->addAction("3_2",this,SLOT(actionsSlot()));
    menu3->addAction("3_3",this,SLOT(actionsSlot()));

    menu2->addAction("2_2",this,SLOT(actionsSlot()));
    menu2->addAction("2_3",this,SLOT(actionsSlot()));

    m_menu1->addSeparator();

    m_menu1->addAction("1_4",this,SLOT(actionsSlot()));
}

void Widget::actionsSlot()
{
    QAction *action = (QAction*)sender();
    QString actionText = action->text();

    qDebug()<<actionText<<"triggerred!"<<endl;

    if("1_1" == actionText) {
        //...
    } else if("1_2" == actionText) {
        //...
    }
        //...
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    m_menu1->exec(QCursor::pos());
    event->accept();
}
