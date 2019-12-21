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
#include <QFutureWatcher>
#include <QtConcurrent>

namespace Ui {
class RankingRoom;
}

class RankingRoom : public QWidget
{
    Q_OBJECT

public:
    explicit RankingRoom(ClientSocket *client, QWidget *parent = nullptr);
    bool eventFilter(QObject *obj, QEvent *event);	// 添加时间过滤器声明
    ~RankingRoom();
public slots:
    void on_update_ranking();
signals:
    void back();

private slots:
    void on_pushButton_clicked();

    void update_ranking();

private:

    Ui::RankingRoom *ui;
    void Init();
    void CreateView();
    QPushButton *testBtn;
    QVBoxLayout *mainLayout;
    //    QTreeView *treeView;
    QStandardItemModel *standItemModel;
    QTableView *tableView;
    ClientSocket *_client;//与服务器连接的socket
    QPixmap PixmapToRound(const QPixmap &src, int radius);
    QFutureWatcher<std::string> *strWatcherPtr;
};

#endif // RANKINGROOM_H
