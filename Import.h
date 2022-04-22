#ifndef IMPORT_H
#define IMPORT_H
#define _CRT_SECURE_NO_WARNINGS

#pragma execution_character_set("utf-8")

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QPainter>

class Import : public QObject
{
    Q_OBJECT
public:
    Import(){};

public:
    int cW,cH; //图片原始尺寸
    QImage* image0 = NULL;
    QImage* mlabelimage = NULL;  //人工label，真的
    QImage* ylabelimage = NULL;  //预测label，推测的
};

#endif // IMPORT_H
