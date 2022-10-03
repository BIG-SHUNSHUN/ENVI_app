#include "TreeViewWrapper.h"
#include <QStandardItemModel>
#include <QHeaderView>

TreeViewWrapper::TreeViewWrapper(QTreeView* treeView)
{
    m_treeView = treeView;
    m_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

TreeViewWrapper::~TreeViewWrapper()
{

}

int TreeViewWrapper::GetTopParentRow(const QModelIndex *index)
{
    //由QModelIndex得到QStandardItem
    QStandardItemModel* model = (QStandardItemModel*)(m_treeView->model());
    QStandardItem* item = model->itemFromIndex(*index);
    while (item->parent()!=nullptr)
    {
        item = item->parent();
    }
    return item->row();
}

void TreeViewWrapper::UpdateTreeView()
{
    //创建Model
    QStandardItemModel* model = new QStandardItemModel(m_treeView);
    for(int i = 0; i< ImageManager::GetInstance()->Size(); i++)
    {
        const Image* image = ImageManager::GetInstance()->GetImage(i);    //影像
        QString fileName = QString::fromLocal8Bit(image->GetFileName().c_str());    //文件名
        QStandardItem* item = new QStandardItem(fileName);
        model->appendRow(item);  //添加文件名信息
        const HeadInfo& headerInfo = image->GetHeaderInfo();    //影像头文件信息
        //添加波段名信息
        if(headerInfo.m_bandNames.size()!=0)
        {
            for(int band = 0; band < headerInfo.m_bands; band++)
            {
                QStandardItem* subItem = new QStandardItem(QString::fromLocal8Bit(headerInfo.m_bandNames[band].c_str()));
                item->appendRow(subItem);
            }
        }
        else
        {
            for(int band = 0; band < headerInfo.m_bands; band++)
            {
                QStandardItem* subItem = new QStandardItem(QString("Band %0").arg(band+1));
                item->appendRow(subItem);
            }
        }
    }
    QAbstractItemModel* modelPre = m_treeView->model();
    if(modelPre !=nullptr)
        delete modelPre;
    m_treeView->setModel(model);
    m_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_treeView->expandAll();
    m_treeView->header()->hide();
}
