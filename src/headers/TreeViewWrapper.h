#ifndef TREEVIEWWRAPPER_H
#define TREEVIEWWRAPPER_H

#include <QTreeView>

class TreeViewWrapper
{
public:
    TreeViewWrapper(QTreeView* treeView);
    virtual ~TreeViewWrapper();
    int GetTopParentRow(const QModelIndex* index);    //获得顶层item
    void UpdateTreeView();
private:
    QTreeView* m_treeView;
};

#endif // TREEVIEWWRAPPER_H
