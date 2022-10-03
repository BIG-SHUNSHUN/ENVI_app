#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QVector>
#include "Image.h"
#include "mainwindow.h"

class MainWindow;

//Image管理类
class ImageManager
{
public:
    virtual ~ImageManager();
    void AddImage(Image* image);
    void DeleteImage(int index);
    void DeleteAllImage();
    void SaveImage(int index);
    void RegisterObserver(MainWindow* mainWindow);    //注册观察者
    int Size();
    const Image* GetImage(int index);

    static ImageManager* GetInstance();
    static void ReleaseImageManager();

private:
    ImageManager();
    void Notify();

    QVector<Image*> m_imageList;
    MainWindow* m_observer;

    static ImageManager* m_instance;
};

#endif // IMAGEMANAGER_H
