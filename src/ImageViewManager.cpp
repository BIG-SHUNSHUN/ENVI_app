#include "headers/ImageViewManager.h"
#include <QMessageBox>

#include "headers/ImageManager.h"
#include "headers/ImageView.h"

ImageViewManager::ImageViewManager()
{

}

bool ImageViewManager::IsUnique()
{
    if(m_RGB[0][0] != m_RGB[1][0] || m_RGB[1][0] != m_RGB[2][0] || m_RGB[2][0] != m_RGB[0][0])
        return false;
    else
        return true;
}

void ImageViewManager::UpdateWindowInfo()
{
    m_comboBoxWindow->clear();
    for(int i = 0; i < m_imageViewList.size(); i++)
    {
        m_comboBoxWindow->addItem(m_imageViewList[i]->windowTitle());
    }
}

void ImageViewManager::CloseViewWhenImageDeleted(int id)
{
    for(int i = m_imageViewList.size() - 1; i >= 0; i--)
    {
        if(m_imageViewList[i]->GetImageId() == id)
        {
            delete m_imageViewList[i];
            m_imageViewList.remove(i);
        }
    }
    UpdateWindowInfo();
}

void ImageViewManager::CloseAllView()
{
    for(int i = m_imageViewList.size() - 1; i >= 0; i--)
    {
        delete m_imageViewList[i];
        m_imageViewList.remove(i);
        UpdateWindowInfo();
    }
}

ImageViewManager::~ImageViewManager()
{
    for(int i = 0; i< m_imageViewList.size(); i++)
    {
        delete m_imageViewList[i];
    }
}

void ImageViewManager::SetMode(IMAGEMODE mode)
{
    m_mode = mode;
}

IMAGEMODE ImageViewManager::GetMode()
{
    return m_mode;
}

void ImageViewManager::SetComboBox(QComboBox *comboBox)
{
    m_comboBoxWindow = comboBox;
}

void ImageViewManager::SetGreyInfo(int imgIndex, int band)
{
    m_grey[0] = imgIndex;
    m_grey[1] = band;
}

void ImageViewManager::SetRGBInfo(int order, int imgIndex, int band)
{
    m_RGB[order][0] = imgIndex;
    m_RGB[order][1] = band;
}

void ImageViewManager::LoadImageView(QWidget* parent)
{
    static int number = 0;
    if(m_mode == IMAGEMODE::RGB)
    {
        if(IsUnique() == false)
        {
            QMessageBox::information(nullptr, "错误", "选择了多个影像的波段数据");
            return;
        }
    }
    ImageView* view = new ImageView(parent, number + 1);
    if(m_mode == IMAGEMODE::GERY)
        view->ShowRGBImage(ImageManager::GetInstance()->GetImage(m_grey[0]), m_grey[1], m_grey[1], m_grey[1]);
    if(m_mode == IMAGEMODE::RGB)
        view->ShowRGBImage(ImageManager::GetInstance()->GetImage(m_RGB[0][0]), m_RGB[0][1], m_RGB[1][1], m_RGB[2][1]);
    m_imageViewList.append(view);
    view->show();    //显示窗口
    number++;

    UpdateWindowInfo();
}

void ImageViewManager::RemoveImageView(ImageView* view)
{    
    for(int i = 0; i < m_imageViewList.size(); i++)
    {
        if(m_imageViewList[i]->m_number == view->m_number)
        {
            m_imageViewList.remove(i);
            break;
        }
    }
    UpdateWindowInfo();
}

ImageViewManager *ImageViewManager::GetInstance()
{
    return m_instance;
}

void ImageViewManager::ReleaseInstance()
{
    delete m_instance;
}

ImageViewManager*  ImageViewManager::m_instance = new ImageViewManager();
