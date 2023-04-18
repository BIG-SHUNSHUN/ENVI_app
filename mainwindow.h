#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QLabel>
#include <QRadioButton>

class TreeViewWrapper;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void UpdateWhenImageChanged();    //当Image变化时更新

private:
    Ui::MainWindow *ui;
    TreeViewWrapper* m_treeViewWrapper;
    QLabel* m_RGBLabel[3];
    QRadioButton* m_RGBRadioBtn[3];

    void OpenFile();
    void CloseFile();
    void CloseAllFile();
    void SaveFile();
    void LoadImageView();
    void TabChanged(int i);    //tabWidget的index变化时发生
    void SelectBandToShow(const QModelIndex& index);    //选择要显示的波段

    void UpdateDimLabel(int index);
    void SelectWhichBandToShow(const QModelIndex& index);
    void ClearTab();
};
#endif // MAINWINDOW_H
