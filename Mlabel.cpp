#include "Mlabel.h"

#include <QPainter>
#include <QDebug>
#include <QWheelEvent>
#include <QMenu>
#include <QFileDialog>

MLabel::MLabel(QWidget *parent):QLabel(parent){}

//QPainter画图
void MLabel::paintEvent(QPaintEvent *event)
{
    if (image == NULL)
        return QWidget::paintEvent(event);

    QPainter painter(this);  //this指的是这个框
    painter.setRenderHint(QPainter::SmoothPixmapTransform); //消锯齿

    // 根据窗口计算应该显示的图片的大小
    imagewidth = qMin(image->width(), this->width());
    imageheight = imagewidth * 1.0 / (image->width() * 1.0 / image->height());
    imageheight = qMin(imageheight, this->height());
    imagewidth = imageheight * 1.0 * (image->width() * 1.0 / image->height());

    // 平移  这个函数简单来说就是实现对坐标原点的重新定义，参数为重新定义的x坐标，y坐标
    XCenter = this->width() / 2 + XPtInterval;
    YCenter = this->height() / 2 + YPtInterval;
    painter.translate(XCenter, YCenter);

    // 缩放
    painter.scale(ZoomValue, ZoomValue);

    // 绘制图像
    QRect picRect(-imagewidth / 2, -imageheight / 2, imagewidth, imageheight);
    painter.drawPixmap(picRect, QPixmap::fromImage(*image));
}

//鼠标滚轮滚动
void MLabel::wheelEvent(QWheelEvent *event)
{
    int value = event->delta();  //QWheelEvent的delta()函数获得滚轮滚动的距离值，通过此值判断滚轮滚动的方向。若delta大于0，则表示滚轮向前（远离用户的方向），小于零则表明向后，（靠近用户的方向）
    if (value > 0)  //放大
        OnZoomInImage();
    else            //缩小
        OnZoomOutImage();

    update(); //定时调用 update（）调用绘图事件来实现画面更新，它会通过立即调用paintEvent()来直接重新绘制窗口部件
}

//鼠标摁下
void MLabel::mousePressEvent(QMouseEvent *event)
{
    if (image != NULL)
    {
        if (event->button() == Qt::RightButton)
        {
            OldPos = event->pos();
            Pressed = true;
        }
    }
}
//鼠标移动
void MLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (image != NULL)
    {
        if (!Pressed)
            return QWidget::mouseMoveEvent(event);

        this->setCursor(Qt::ClosedHandCursor);  //让你的鼠标箭头在特定的位置显示某种形状
        QPoint pos = event->pos();
        int xPtInterval = pos.x() - OldPos.x(); //pos.x()是鼠标相较于label_3左边缘的距离,可为负数
        int yPtInterval = pos.y() - OldPos.y();

        XPtInterval += xPtInterval;
        YPtInterval += yPtInterval;

        OldPos = pos;
        update();
    }
}
//鼠标松开
void MLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (image != NULL)
    {
        if (event->button() == Qt::RightButton)
        {
            Pressed = false;
            setCursor(Qt::ArrowCursor);
        }
    }
}

//鼠标双击
void MLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(getmask){
        if (image != NULL && !returnimage)
        {
            if (event->button() == Qt::RightButton){
                QPoint pos = event->pos();
                int x2image = pos.x() - XCenter + imagewidth*ZoomValue /2;
                int y2image = pos.y() - YCenter + imageheight*ZoomValue/2;

                //image->setPixel(QPoint(x2image,y2image),qRgb(0,255,0));
                double proportion = image->width()/(imagewidth*ZoomValue);
                QColor color = image->pixel(x2image*proportion, y2image*proportion);
                //QString colorStr = color.name();
                //std::string a = colorStr.toStdString();
                int red = color.red();
                int green = color.green();
                int blue = color.blue();
                QImage* maskimage = new QImage(*temporiginimage);

                for(int i=0;i<image->width();++i){
                    for(int j=0;j<image->height();++j){
                        color = image->pixel(i,j);
                        if(color.red() == red && color.green() == green && color.blue() == blue){
                            maskimage->setPixel(QPoint(i,j),qRgb(0,255,0));
                        }
                    }
                }
            returnimage = true;
            emit maskimagepaint(maskimage);
            }
        }
        else{
            returnimage = false;
            image = templabelimage;
            update();
        }
    }
}

//图片放大
void MLabel::OnZoomInImage()
{
    if (image != NULL)
    {
        //ZoomValue += 0.05;
        ZoomValue += ZoomValue * 0.1;
        emit magnification(ZoomValue);
        update();
    }
}

//图片缩小
void MLabel::OnZoomOutImage()
{
    if (image != NULL)
    {
        //ZoomValue -= 0.05;
        ZoomValue -= ZoomValue * 0.1;
        if (ZoomValue < 0.15)
        {
            ZoomValue = 0.1;
        }
        emit magnification(ZoomValue);
        update();
    }
}
//图片还原
void MLabel::OnPresetImage()
{
    if (image != NULL)
    {
        ZoomValue = 1.0;
        XPtInterval = 0;
        YPtInterval = 0;
        emit magnification(ZoomValue);
        update();
    }
}

//更新掩膜图像
void MLabel::updatamaskimage(QImage* maskimage)
{
    image = maskimage;
    update();
}

void MLabel::emmagnification()
{
    emit magnification(ZoomValue);
}
