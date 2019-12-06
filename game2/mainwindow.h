#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "register.h"
#include "gamelobby.h"
#include "fightui.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_signupButtion_clicked();

    void on_loginButton_clicked();

private:
    Ui::MainWindow *ui;
    Register *register_win;
    GameLobby *GameLobbyWin;
};

#endif // MAINWINDOW_H
