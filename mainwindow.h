#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define _CRT_SECURE_NO_WARNINGS

#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <QMenu>
#include <QDir>
#include <QFileDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include <QHash>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <list>

#include "Import.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Import *import0;
    QImage *image0;
    QImage *image1;
    QImage *image2;
    std::vector<QColor> classes;

    QStringList imageName_list;  //图片列表
    int mnow = 0;   //当前显示图片

private slots:
    void updatemagnification(double m);
    void updataform();
    void autocategory();
    void on_action_triggered();
    void on_action_2_triggered();
    void on_action_3_triggered();
    void on_tableWidget_cellChanged(int row, int column);
};

#endif // MAINWINDOW_H
