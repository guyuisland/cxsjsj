#include "mylabel.h"

MyLabel::MyLabel(QWidget* parent) :QLabel(parent)
{

}
MyLabel::~MyLabel()
{

}
void MyLabel::enterEvent(QEvent *e)
{
    setText("Ok, mouse is on");
}
void MyLabel::leaveEvent(QEvent *e)
{
    setText(" ");
}

