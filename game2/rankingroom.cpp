#include "rankingroom.h"
#include "ui_rankingroom.h"


RankingRoom::RankingRoom(ClientSocket *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RankingRoom)
{
    ui->setupUi(this);
    QPixmap img1;
//    img1.load(":/Image/user.png");
//    QPixmap pixMap= img1.scaled(1280,720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    ui->label->setPixmap(pixMap);
    CreateView();
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
    for(int i=0;i<100;i++)
    {
        QStandardItem *standItem1 = new QStandardItem(tr("%1").arg(i+1));
        QStandardItem *standItem2 = new QStandardItem(tr("第%1行").arg(i+1));
        QStandardItem *standItem3 = new QStandardItem("123");
        QStandardItem *standItem4 = new QStandardItem("456");
        standItemModel->setItem(i,0,standItem1);                                //表格第i行，第0列添加一项内容
        standItemModel->item(i,0)->setForeground(QBrush(QColor(255,0,0)));      //设置字符颜色
        standItemModel->item(i,0)->setTextAlignment(Qt::AlignCenter);           //设置表格内容居中
        standItemModel->setItem(i,1,standItem2);                               //表格第i行，第1列添加一项内容
        standItemModel->setItem(i,2,standItem3);
        standItemModel->setItem(i,3,standItem4);
    }
    tableView->setModel(standItemModel);    //挂载表格模型
    //设置表格属性
    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);        //表头信息显示居中
    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);  //设定表头列宽不可变
    tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    tableView->setColumnWidth(0,100);       //设定表格第0列宽度
    tableView->setColumnWidth(1,100);
    tableView->setColumnWidth(2,100);
    tableView->setColumnWidth(3,100);
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
