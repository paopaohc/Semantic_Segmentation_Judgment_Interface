#ifndef MLABEL_H
#define MLABEL_H
#define _CRT_SECURE_NO_WARNINGS

#pragma execution_character_set("utf-8")

#include <QLabel>
#include <QObject>

class MLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MLabel(QWidget *parent = 0);
    QImage *image = NULL;
    QImage *temporiginimage = NULL;
    QImage *templabelimage = NULL;

    int imagewidth = 0;     //显示的图片的宽
    int imageheight = 0;    //显示的图片的高
    int XCenter = 0;        //Mlabel坐标原点的X轴偏移量
    int YCenter = 0;        //Mlabel坐标原点的Y轴偏移量
    int XPtInterval = 0;    //平移X轴的值
    int YPtInterval = 0;    //平移Y轴的值
    qreal ZoomValue = 1.0;  //鼠标缩放值
    QPoint OldPos;          //旧的鼠标位置
    bool Pressed = false;   //鼠标是否被摁压
    bool getmask = true;    //能否鼠标双击获取掩膜
    bool returnimage = false; //标签图像成掩膜图像后鼠标双击后是否能返回原标签图像

public:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event) override;               //鼠标滚轮滚动
    void mousePressEvent(QMouseEvent *event) override;          //鼠标摁下(右键）
    void mouseMoveEvent(QMouseEvent *event) override;           //鼠标移动
    void mouseReleaseEvent(QMouseEvent *event) override;        //鼠标松开(右键)
    void mouseDoubleClickEvent(QMouseEvent *event)override;     //鼠标双击(右键)
    void emmagnification();

private slots:
    void OnZoomInImage();       //图片放大
    void OnZoomOutImage();      //图片缩小
    void OnPresetImage();       //图片还原
    void updatamaskimage(QImage* maskimage);

signals:
    void magnification(double m);
    void maskimagepaint(QImage* maskimage);
};

#endif // MLABEL_H
