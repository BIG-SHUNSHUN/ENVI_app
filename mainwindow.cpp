#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTabWidget>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_treeViewWrapper = new TreeViewWrapper(ui->treeViewImage);
    ImageViewManager::GetInstance()->SetComboBox(ui->cmBoxView);
    ImageViewManager::GetInstance()->SetMode(IMAGEMODE::GERY);

    m_RGBLabel[0] = ui->labelR;
    m_RGBLabel[1] = ui->labelG;
    m_RGBLabel[2] = ui->labelB;

    m_RGBRadioBtn[0] = ui->radioButtonR;
    m_RGBRadioBtn[1] = ui->radioButtonG;
    m_RGBRadioBtn[2] = ui->radioButtonB;

    //注册观察者
    ImageManager::GetInstance()->RegisterObserver(this);

    ui->tabWidget->setCurrentIndex(0);

    //连接信号槽
    connect(ui->actionOpen,&QAction::triggered,this,&MainWindow::OpenFile);
    connect(ui->actionCloseSeleted, &QAction::triggered,this, &MainWindow::CloseFile);
    connect(ui->actionCloseAll, &QAction::triggered, this, &MainWindow::CloseAllFile);
    connect(ui->actionSave, &QAction::triggered,this, &MainWindow::SaveFile);
    connect(ui->treeViewImage,&QTreeView::clicked, this, &MainWindow::SelectBandToShow);
    connect(ui->tabWidget,&QTabWidget::currentChanged, this, &MainWindow::TabChanged);
    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::LoadImageView);
}

MainWindow::~MainWindow()
{
    delete ui;
    ImageManager::ReleaseImageManager();
    delete m_treeViewWrapper;
    ImageViewManager::ReleaseInstance();
}

void MainWindow::UpdateWhenImageChanged()
{
    m_treeViewWrapper->UpdateTreeView();    //更新treeView信息
    UpdateDimLabel(ImageManager::GetInstance()->Size() - 1);    //更新Dim信息
    ClearTab();
}

void MainWindow::OpenFile()
{
    QFileDialog dialog(this, "打开影像文件", "G:/2 研究生阶段/Stage_1/10 面向对象程序设计",
                       "Image Files(*.jpeg *.jpg *.png *.bmp);;All Files(*.*)");
    if(dialog.exec())
    {
        QString fileData =  dialog.selectedFiles()[0];
        QFileInfo fileInfo(fileData);
        if(fileInfo.suffix() == "jpeg" || fileInfo.suffix() == "jpg" || fileInfo.suffix() == "bmp" || fileInfo.suffix() == "png")
        {
            Mat img = imread(fileData.toLocal8Bit().toStdString());
            Image* image = new Image(img);
            image->SetFileName(fileInfo.absolutePath().toLatin1().toStdString());
            ImageManager::GetInstance()->AddImage(image);
        }
        else
        {
            //创建影像
            Image* image =ImageFactory::CreateImage(fileData.toLocal8Bit().data());
            //添加影像
            ImageManager::GetInstance()->AddImage(image);
        }
    }
}

void MainWindow::CloseFile()
{
    //获得当前选中QModelIndex
    QModelIndex index = ui->treeViewImage->currentIndex();
    if(!index.isValid())
        return;
    //获得顶端结点的row
    int row = m_treeViewWrapper->GetTopParentRow(&index);
    //删除影像
    ImageManager::GetInstance()->DeleteImage(row);
}

void MainWindow::CloseAllFile()
{
    ImageManager::GetInstance()->DeleteAllImage();
}

void MainWindow::SaveFile()
{
    //获得当前选中QModelIndex
    QModelIndex index = ui->treeViewImage->currentIndex();
    if(!index.isValid())
        return;
    //获得顶端结点的row
    int row = m_treeViewWrapper->GetTopParentRow(&index);
    //保存影像
    ImageManager::GetInstance()->SaveImage(row);
}

void MainWindow::LoadImageView()
{
    if(ImageViewManager::GetInstance()->GetMode() == IMAGEMODE::GERY)
    {
        if(ui->labelGrey->text().size() == 0)
            return;
    }
    if(ImageViewManager::GetInstance()->GetMode() == IMAGEMODE::RGB)
    {
        for(int i = 0; i < 3; i++)
        {
            if(m_RGBLabel[i]->text().size() == 0)
                return;
        }
    }
    ImageViewManager::GetInstance()->LoadImageView(this);
}

void MainWindow::TabChanged(int i)
{
    if(i == 0)
        ImageViewManager::GetInstance()->SetMode(IMAGEMODE::GERY);
    else if(i == 1)
        ImageViewManager::GetInstance()->SetMode(IMAGEMODE::RGB);
}

void MainWindow::SelectBandToShow(const QModelIndex& index)
{
    if(!index.isValid())
        return;
    if(index.parent().isValid() == false)
        return;
    //获得顶端结点的row
    int row = m_treeViewWrapper->GetTopParentRow(&index);
    UpdateDimLabel(row);    //更新Dim信息
    SelectWhichBandToShow(index);
}

void MainWindow::UpdateDimLabel(int index)
{
    ImageManager* imageManager = ImageManager::GetInstance();
    //刷新Dims信息
    if(imageManager->Size()>0)
    {
        const HeadInfo& headerInfo = imageManager->GetImage(index)->GetHeaderInfo();    //影像头文件信息
        ui->labelDims->setText(QString(tr("%0 × %1")).arg(headerInfo.m_lines).arg(headerInfo.m_samples));
    }
    else
        ui->labelDims->setText("");
}

void MainWindow::SelectWhichBandToShow(const QModelIndex &index)
{
    static int order = 0;
    if(ImageViewManager::GetInstance()->GetMode() == IMAGEMODE::GERY)
    {
        ui->labelGrey->setText(index.data().toString());
        ImageViewManager::GetInstance()->SetGreyInfo(m_treeViewWrapper->GetTopParentRow(&index), index.row());
    }
    if(ImageViewManager::GetInstance()->GetMode() == IMAGEMODE::RGB)
    {
        m_RGBLabel[order]->setText(index.data().toString());
        ImageViewManager::GetInstance()->SetRGBInfo(order, m_treeViewWrapper->GetTopParentRow(&index), index.row());
        order = (order + 1) % 3;
        m_RGBRadioBtn[order]->setChecked(true);
    }
}

void MainWindow::ClearTab()
{
    ui->labelGrey->setText(QString(""));
    ui->labelR->setText(QString(""));
    ui->labelG->setText(QString(""));
    ui->labelB->setText(QString(""));
}

