#ifndef FIGHTUI_H
#define FIGHTUI_H

#include <QDialog>

#include <QLabel>
#include <QMouseEvent>
#include "MyLabel.h"
namespace Ui {
class FightUI;
}

class FightUI : public QDialog
{
    Q_OBJECT

public:
    explicit FightUI(QWidget *parent = nullptr);
    ~FightUI();
    bool eventFilter(QObject *obj, QEvent *event);	// 添加时间过滤器声明
signals:
    void sendsignal();

private slots:

private:
    Ui::FightUI *ui;
    void InitUi();
    void click_monster1();
    MyLabel *myLabel;
};

#endif // FIGHTUI_H
