#ifndef RANKINGROOM_H
#define RANKINGROOM_H

#include <QWidget>
#include "clientsocket.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>
namespace Ui {
class RankingRoom;
}

class RankingRoom : public QWidget
{
    Q_OBJECT

public:
    explicit RankingRoom(ClientSocket *client, QWidget *parent = nullptr);
    ~RankingRoom();

signals:
    void back();

private slots:
    void on_pushButton_clicked();

private:
    Ui::RankingRoom *ui;
    void Init();
    void CreateView();
    QPushButton *testBtn;
    QVBoxLayout *mainLayout;
    //    QTreeView *treeView;
    QStandardItemModel *standItemModel;
    QTableView *tableView;
};

#endif // RANKINGROOM_H
