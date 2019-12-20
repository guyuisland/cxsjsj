#ifndef MYLABEL_H
#define MYLABEL_H

#include<QLabel>
#include<QEvent>
#include<QSize>
#include<QMovie>
#include<QDebug>
#include<QPropertyAnimation>

class QPropertyAnimation;

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
