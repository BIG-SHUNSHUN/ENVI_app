#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include "ImageViewManager.h"
#include "Image.h"

class ImageView : public QWidget
{
    Q_OBJECT
    friend class ImageViewManager;
public:
    explicit ImageView(QWidget *parent = nullptr, int number = 0);
    void ShowRGBImage(const Image* image, int r, int g, int b);    //将Image的r、g、b三波段显示到窗口上
    int GetImageId();

private:
    int m_number;    //窗口序号
    const Image* m_image;    //要显示到窗口的Image
    int m_RGBBand[3];    //需要合成的三波段

    void paintEvent(QPaintEvent *event);    //绘图事件
    void closeEvent(QCloseEvent *event);    //窗口关闭事件
};

#endif // IMAGEVIEW_H
