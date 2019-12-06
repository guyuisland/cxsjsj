#ifndef MYLABEL_H
#define MYLABEL_H

#include<QLabel>
#include<QEvent>

class MyLabel :public QLabel
{
Q_OBJECT;
public:
    MyLabel(QWidget *parent = 0);
    ~MyLabel();
public:
    void enterEvent(QEvent *e);//鼠标进入事件
    void leaveEvent(QEvent *e);//鼠标离开事件
};

#endif // MYLABEL_H
