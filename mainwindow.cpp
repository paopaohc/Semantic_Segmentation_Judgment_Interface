#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    import0 = new Import(); //！！！！！！困了我两个小时的sb问题

    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget_2->horizontalHeader()->setVisible(false);
    ui->tableWidget_2->verticalHeader()->setVisible(false);

    connect(ui->action_4, SIGNAL(triggered()), ui->label_3, SLOT(OnZoomInImage()));   //上一张图片
    connect(ui->action_5, SIGNAL(triggered()), ui->label_3, SLOT(OnZoomInImage()));   //下一张图片

    connect(ui->action_6, SIGNAL(triggered()), ui->label_3, SLOT(OnZoomInImage()));   //图片放大
    connect(ui->action_6, SIGNAL(triggered()), ui->label_4, SLOT(OnZoomInImage()));
    connect(ui->action_6, SIGNAL(triggered()), ui->label_6, SLOT(OnZoomInImage()));

    connect(ui->action_7, SIGNAL(triggered()), ui->label_3, SLOT(OnZoomOutImage()));   //图片缩小
    connect(ui->action_7, SIGNAL(triggered()), ui->label_4, SLOT(OnZoomOutImage()));
    connect(ui->action_7, SIGNAL(triggered()), ui->label_6, SLOT(OnZoomOutImage()));

    connect(ui->action_8, SIGNAL(triggered()), ui->label_3, SLOT(OnPresetImage()));   //图片还原
    connect(ui->action_8, SIGNAL(triggered()), ui->label_4, SLOT(OnPresetImage()));
    connect(ui->action_8, SIGNAL(triggered()), ui->label_6, SLOT(OnPresetImage()));

    connect(ui->label_3, SIGNAL(magnification(double)), this, SLOT(updatemagnification(double))); //修正放大率
    connect(ui->label_4, SIGNAL(magnification(double)), this, SLOT(updatemagnification(double))); //this指mainwindow
    connect(ui->label_6, SIGNAL(magnification(double)), this, SLOT(updatemagnification(double)));

    connect(ui->label_4, SIGNAL(maskimagepaint(QImage*)), ui->label_4, SLOT(updatamaskimage(QImage*))); //输出掩膜图像
    connect(ui->label_6, SIGNAL(maskimagepaint(QImage*)), ui->label_6, SLOT(updatamaskimage(QImage*)));

    connect(ui->action_9, SIGNAL(triggered()), this, SLOT(updataform())); //刷新显示图像
    connect(ui->action_10, SIGNAL(triggered()), this, SLOT(autocategory())); //自动生成类别目录
}

MainWindow::~MainWindow()
{
    delete ui;
}

//导入图片
void MainWindow::on_action_triggered(){
    QString dlgTitle = "选择图片";   //对话框标题
    QString curPath = "D:/datasets/xiongyiming/road_dataset/JPEGImages";
    QString filter = "图片文件(*.jpg *.bmp *.png)"; //文件过滤器
    imageName_list = QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);

    image0 = new QImage(imageName_list[mnow]);
    import0->image0 = image0;

    ui->label_3->image = image0;
    ui->label_3->update();
    ui->label_3->getmask = false;

    ui->label_3->temporiginimage = import0->image0;
    ui->label_4->temporiginimage = import0->image0;
    ui->label_6->temporiginimage = import0->image0;

    import0->cW = image0->width();
    import0->cH = image0->height();
    QString imagesize(QString::number(import0->cW));
    imagesize = imagesize + "×" + QString::number(import0->cH);
    ui->picturename->setText(imageName_list[mnow]);
    ui->picturesize->setText(imagesize);
    ui->magnification->setText(QString::number(1.0 * 100, 'f', 1) + "%");
    //qDebug()<<ui->tableWidget->item(0,0)->text();
}

//导入人工标签
void MainWindow::on_action_2_triggered()
{
    QString dlgTitle = "选择人工label"; //对话框标题
    QString curPath = "D:/datasets/xiongyiming/road_dataset/SegmentationClassPNG";
    QString filter = "标签文件(*.jpg *.bmp *.png)"; //文件过滤器
    QString aFileName = QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);

    image1 = new QImage(aFileName);
    import0->mlabelimage = image1;
    ui->label_4->image = image1;

    ui->label_4->templabelimage = import0->mlabelimage;

    ui->label_4->update();
}

//导入预测标签
void MainWindow::on_action_3_triggered()
{
    QString dlgTitle = "选择预测label"; //对话框标题
    QString curPath = "D:/datasets/xiongyiming/road_dataset/SegmentationClassPNG";
    QString filter = "标签文件(*.jpg *.bmp *.png)"; //文件过滤器
    QString aFileName = QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);

    image2 = new QImage(aFileName);
    import0->ylabelimage = image2;
    ui->label_6->image = image2;

    ui->label_6->templabelimage = import0->ylabelimage;

    ui->label_6->update();
}

void MainWindow::updatemagnification(double m)
{
    ui->magnification->setText(QString::number(m * 100, 'f', 1) + "%");
}

//自动在最后一行添加行，tableWidget上发生改变触发on_tableWidget_cellChanged函数
void MainWindow::on_tableWidget_cellChanged(int row, int column){
    ui->tableWidget->blockSignals(true);   //阻塞信号防止在执行此槽函数时因为对tableWidget造成修改从而再次触发on_tableWidget_cellChanged函数，导致死循环
    //ui->tableWidget->setItem(row, column, new QTableWidgetItem(*(ui->tableWidget->item(row, column))));//QTableWidgetItem::Type
    int crow = ui->tableWidget->rowCount();//获取现有表格控件中的行数
    if (ui->tableWidget->item(crow - 2, 0) != NULL){
        ui->tableWidget->insertRow(crow-1);
    }
    ui->tableWidget->blockSignals(false);
}

void MainWindow::updataform()
{
    if(import0->mlabelimage == NULL || import0->ylabelimage== NULL){
        QMessageBox::warning(this, tr("warning"),tr("缺失人工或预测标签"), QMessageBox::Yes);
        return;
    }
    //qDebug()<<import0->mlabelimage;
    int correctsum = 0;
    int wrongsum = 0;

    QString correct, wrong,categorynum;
    double corr, wro,cate;
    int allpixel = import0->mlabelimage->width()*import0->mlabelimage->height();

    QColor mcolor;
    QColor ycolor;
    std::unordered_map<std::string,int> correctmap;      //正确检测数量统计  //std::unordered_map<QColor,int>编译不通过，std::unordered_map<int,int>能通过
    std::unordered_map<std::string,int> wrongmap;        //误检统计        // 即便使用QHash/QMap通过QHash/QMap<QColor,int>的定义,也无法使用correctmap[mcolor] ++; 会报错QColor无法用作键值

    for(int i=0;i<import0->mlabelimage->width();++i){
        for(int j=0;j<import0->mlabelimage->height();++j){
            mcolor = import0->mlabelimage->pixel(i,j);
            ycolor = import0->ylabelimage->pixel(i,j);
            if(mcolor == ycolor){
                correctmap[mcolor.name().toStdString()] ++;
            }
            else{
                wrongmap[mcolor.name().toStdString()] ++;
            }
        }
    }
    for(int i = 0; i < classes.size(); i++){
        int co = correctmap[classes[i].name().toStdString()];
        int wr = wrongmap[classes[i].name().toStdString()];
        int ca = co + wr;

        correctsum += co;
        wrongsum += wr;

        //correct = QString::number(co);
        //wrong = QString::number(wr);
        //categorynum = QString::number(ca);

        if (co != 0)
        {
            corr = (double)co / ca * 100;
            correct = QString::number(corr, 'f', 1) + "%";
        }
        if (wr != 0)
        {
            wro = (double)wr / ca * 100;
            wrong = QString::number(wro, 'f', 1) + "%";
        }
        if (categorynum != 0)
        {
            cate = (double)ca / allpixel * 100;
            categorynum = categorynum + "(" + QString::number(cate, 'f', 1) + "%" + ")";
        }

        ui->tableWidget->setItem(i+1,2,new QTableWidgetItem(categorynum));
        ui->tableWidget->setItem(i+1,3,new QTableWidgetItem(correct));
        ui->tableWidget->setItem(i+1,4,new QTableWidgetItem(wrong));

    }

    //总统计
    //correct = QString::number(correctsum);
    //wrong = QString::number(wrongsum);
    if (correctsum != 0)
    {
        corr = (double)correctsum / allpixel * 100;
        correct = QString::number(corr, 'f', 1) + "%" ;
    }
    if (wrongsum != 0)
    {
        wro = (double)wrongsum / allpixel * 100;
        wrong =  QString::number(wro, 'f', 1) + "%" ;
    }

    int crow = ui->tableWidget->rowCount();
    ui->tableWidget->setItem(crow - 1, 2, new QTableWidgetItem(QString::number(allpixel)));
    ui->tableWidget->setItem(crow - 1, 3, new QTableWidgetItem(correct));
    ui->tableWidget->setItem(crow - 1, 4, new QTableWidgetItem(wrong));
}


void MainWindow::autocategory(){
    if(import0->mlabelimage == NULL){
        QMessageBox::warning(this, tr("warning"),tr("缺失人工标签"), QMessageBox::Yes);
        return;
    }
    QColor color;
    int crow = ui->tableWidget->rowCount();
    for(int i=0;i<import0->mlabelimage->width();++i){
        for(int j=0;j<import0->mlabelimage->height();++j){
            color = import0->mlabelimage->pixel(i,j);
            if(std::find(classes.begin(),classes.end(),color) == classes.end()){
                classes.push_back(color);
                QTableWidgetItem *cellt = new QTableWidgetItem(QString::number(crow-4));
                cellt->setBackgroundColor(color);
                ui->tableWidget->setItem(crow-3, 0, cellt);
                ++crow;
            }
        }
    }
}


