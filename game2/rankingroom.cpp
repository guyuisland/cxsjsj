#include "rankingroom.h"
#include "ui_rankingroom.h"
#include<QBitmap>
#include<QPainter>
#include<QMouseEvent>
#include<QDebug>

RankingRoom::RankingRoom(ClientSocket *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RankingRoom),
    _client(client)
{
    ui->setupUi(this);
    QPixmap img1;
    QFont ft("Microsoft YaHei", 30);
    //ft.setPointSize(11);
    ui->TOP->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#ff4757"));

    img1.load(":/Image/rankroom.jpg");
    QPixmap pixMap= img1.scaled(1280,720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->background->setPixmap(pixMap);


    ui->TOP->setPalette(pa);

    //刷新用户名、waiting列表
    QString str = "排行榜";
    ui->TOP->setText(str);

    //QPixmap icon1(tr(":/Image/rankroom.jpg"));
    //ui->pushButton->setIcon(icon1);
    //ui->pushButton->setFixedSize(icon1.size());
    QLabel *p;
    ui->back->installEventFilter(this);
    //100,100为QLabel的宽高
    img1.load(":/Image/back.png");
    pixMap= img1.scaled(50,50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //50为圆形的半径
    pixMap =  PixmapToRound(pixMap, 25);
    ui->back->setPixmap(pixMap);
    CreateView();

}
bool RankingRoom::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->back)//指定某个QLabel
     {
         if (event->type() == QEvent::MouseButtonPress) //鼠标点击
         {
             QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换
             if(mouseEvent->button() == Qt::LeftButton)
             {
                 emit back();
             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }
     }
}
QPixmap RankingRoom::PixmapToRound(const QPixmap &src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }
    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}
void RankingRoom::CreateView()
{
    mainLayout = new QVBoxLayout;       //垂直布局
    mainLayout->setSpacing(100);         //设置控件间距
    mainLayout->setMargin(100);          //设置边缘间距
    //testBtn = new QPushButton("Test");
    //添加QTableView代码
    tableView = new QTableView;
    standItemModel = new QStandardItemModel();
    //添加表头
    standItemModel->setColumnCount(4);
    standItemModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("Rank"));   //设置表头内容
    standItemModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("UserName"));
    standItemModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("Level"));
    standItemModel->setHeaderData(3,Qt::Horizontal,QStringLiteral("WinRate"));
    //向表格添加100行内容
//    for(int i=0;i<100;i++)
//    {
//        QStandardItem *standItem1 = new QStandardItem(tr("%1").arg(i+1));
//        QStandardItem *standItem2 = new QStandardItem(tr("第%1行").arg(i+1));
//        QStandardItem *standItem3 = new QStandardItem("123");
//        QStandardItem *standItem4 = new QStandardItem("456");
//        standItemModel->setItem(i,0,standItem1);                                //表格第i行，第0列添加一项内容
//       // standItemModel->item(i,0)->setForeground(QBrush(QColor(255,0,0)));      //设置字符颜色
//        standItemModel->item(i,0)->setTextAlignment(Qt::AlignCenter);           //设置表格内容居中
//        standItemModel->setItem(i,1,standItem2);                               //表格第i行，第1列添加一项内容
//        standItemModel->setItem(i,2,standItem3);
//        standItemModel->setItem(i,3,standItem4);
//    }
    tableView->setModel(standItemModel);    //挂载表格模型
    //设置表格属性
    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);        //表头信息显示居中
    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);  //设定表头列宽不可变
    tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
//    tableView->setColumnWidth(0,100);       //设定表格第0列宽度
//    tableView->setColumnWidth(1,100);
//    tableView->setColumnWidth(2,100);
//    tableView->setColumnWidth(3,100);
    tableView->verticalHeader()->hide();    //隐藏默认显示的行头
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选中时整行选中
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置表格属性只读，不能编辑
//    tableView->setContextMenuPolicy(Qt::CustomContextMenu);         //需要在表格使用右键菜单，需要启动该属性
//    tableView->sortByColumn(0,Qt::AscendingOrder);                 //表格第0列，按降序排列
    //mainLayout->addWidget(testBtn);
    mainLayout->addWidget(tableView);    //添加控件
    this->setLayout(mainLayout);        //显示垂直布局
}


RankingRoom::~RankingRoom()
{
    delete ui;
}

void RankingRoom::on_pushButton_clicked()
{
    emit back();
}

void RankingRoom::on_update_ranking()
{
    json sendInfo;
    sendInfo["define"] = GET_RANKING;

    std::string (ClientSocket::*p)(std::string) = &ClientSocket::Send_Recv;
    strWatcherPtr = new QFutureWatcher<std::string>;
    QFuture<std::string> ret = QtConcurrent::run(_client, p, sendInfo.dump());
    connect(strWatcherPtr, SIGNAL(finished()), this, SLOT(update_ranking()));
    strWatcherPtr->setFuture(ret);
}

void RankingRoom::update_ranking()
{
    json recvInfo = json::parse(strWatcherPtr->result());
    disconnect(strWatcherPtr, SIGNAL(finished()), this, SLOT(update_ranking()));
    delete strWatcherPtr;
    std::vector<string> name;
    std::vector<int> lv;
    std::vector<float> rate;
    if(recvInfo["define"] == QUERY_SUCCESS)
    {
        name = recvInfo["name"].get<std::vector<std::string>>();
        lv = recvInfo["lv"].get<std::vector<int>>();
        rate = recvInfo["rate"].get<std::vector<float>>();
    }
    QStandardItem *standItemPtr;
    QString t;
    for(int i=0; i< name.size(); i++)
    {
        t = QString("%1").arg(i + 1);
        standItemPtr = new QStandardItem(t);
        standItemModel->setItem(i,0,standItemPtr);
        standItemPtr = new QStandardItem(QString::fromStdString(name[i]));
        standItemModel->setItem(i,1,standItemPtr);
        t = QString("%1").arg(lv[i]);
        standItemPtr = new QStandardItem(t);
        standItemModel->setItem(i,2,standItemPtr);
        t = QString("%1").arg(rate[i]);
        standItemPtr = new QStandardItem(t);
        standItemModel->setItem(i,3,standItemPtr);
    }
}

