#ifndef IMAGEVIEWMANAGER_H
#define IMAGEVIEWMANAGER_H

#include <QVector>
#include <QComboBox>

class ImageView;

enum IMAGEMODE
{
    GERY,
    RGB
};

//显示窗口管理类
class ImageViewManager
{
public:
    virtual ~ImageViewManager();
    void SetMode(IMAGEMODE mode);
    IMAGEMODE GetMode();
    void SetComboBox(QComboBox* comboBox);
    void SetGreyInfo(int imgIndex, int band);
    void SetRGBInfo(int order, int imgIndex, int band);
    void LoadImageView(QWidget* parent);
    void RemoveImageView(ImageView* view);
    void CloseViewWhenImageDeleted(int id);
    void CloseAllView();

    static ImageViewManager* GetInstance();
    static void ReleaseInstance();

private:
    ImageViewManager();
    bool IsUnique();    //判断所选的三波段是否来自同一幅影像
    void UpdateWindowInfo();

    QVector<ImageView*> m_imageViewList;    //显示窗口list
    IMAGEMODE m_mode;    //当前显示模式
    int m_grey[2];    //保存波段索引，用于灰度图像绘图
    int m_RGB[3][2];    //保存波段索引，用于彩色图像绘图
    QComboBox* m_comboBoxWindow;    //影像窗口列表

    static ImageViewManager* m_instance;
};

#endif // IMAGEVIEWMANAGER_H
