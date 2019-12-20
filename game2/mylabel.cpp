#include "mylabel.h"
#include<QMovie>
#include<math.h>

MyLabel::MyLabel(QWidget* parent) :QLabel(parent)
{


}
MyLabel::~MyLabel()
{

}

void MyLabel::enterEvent(QEvent *event)
{
    this->setCursor(Qt::PointingHandCursor);
    move(x() -25,y() -25);
    resize(width()+50,height()+50);
}

void MyLabel::leaveEvent(QEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    move(x()+25,y()+25);
    resize(width()-50,height()-50);

}






