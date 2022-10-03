#include "ImageView.h"
#include <QPainter>

ImageView::ImageView(QWidget *parent, int number) : QWidget(parent, Qt::Window)
{
    m_number = number;
    this->setWindowTitle(QString("窗口%0").arg(number));
    this->setAttribute(Qt::WA_DeleteOnClose);    //关闭窗口时释放窗口资源
}

void ImageView::ShowRGBImage(const Image *image, int r, int g, int b)
{
    m_image = image;
    m_RGBBand[0] = r; m_RGBBand[1] = g; m_RGBBand[2] = b;
    this->setFixedSize(image->GetHeaderInfo().m_samples, image->GetHeaderInfo().m_lines);
}

int ImageView::GetImageId()
{
    return m_image->GetId();
}

void ImageView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //绘图
    m_image->ShowImage(&painter, m_RGBBand);
}

void ImageView::closeEvent(QCloseEvent *event)
{
    //删除影像
    ImageViewManager::GetInstance()->RemoveImageView(this);
}
