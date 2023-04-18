#include "headers/ImageManager.h"
#include <QFileDialog>
#include "headers/ImageViewManager.h"
#include "mainwindow.h"
#include "headers/Image.h"

ImageManager::ImageManager()
{

}

ImageManager::~ImageManager()
{
    for(int i = 0; i < m_imageList.size(); i++)
    {
        delete m_imageList[i];
    }
}

void ImageManager::AddImage(Image *image)
{
    m_imageList.push_back(image);
    Notify();
}

void ImageManager::DeleteImage(int index)
{
    if(index < 0 || index >= m_imageList.size())
        return;
    ImageViewManager::GetInstance()->CloseViewWhenImageDeleted(m_imageList[index]->GetId());
    delete m_imageList[index];    //delete内存空间
    m_imageList.removeAt(index);
    Notify();
}

void ImageManager::DeleteAllImage()
{
    ImageViewManager::GetInstance()->CloseAllView();
    for(int i =m_imageList.size()-1; i>=0; i--)
    {
        delete m_imageList[i];
        m_imageList.removeAt(i);
    }
    Notify();
}

void ImageManager::SaveImage(int index)
{
    if(index < 0 ||index>=m_imageList.size())
        return ;
    QFileDialog* dialog = new QFileDialog(nullptr, "保存影像", "G:\\");
    //设置为保存模式
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    if(dialog->exec())
    {
        QString fileName = dialog->selectedFiles()[0];
        m_imageList[index]->Save(fileName.toLocal8Bit().data());
    }
    delete dialog;
}

void ImageManager::RegisterObserver(MainWindow *mainWindow)
{
    this->m_observer = mainWindow;
}

int ImageManager::Size()
{
    return m_imageList.size();
}

const Image *ImageManager::GetImage(int index)
{
    return m_imageList[index];
}

ImageManager *ImageManager::GetInstance()
{
    return m_instance;
}

void ImageManager::ReleaseImageManager()
{
    delete m_instance;
}

void ImageManager::Notify()
{
    m_observer->UpdateWhenImageChanged();
}

ImageManager* ImageManager::m_instance = new ImageManager();
