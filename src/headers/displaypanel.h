#ifndef DISPLAYPANEL_H
#define DISPLAYPANEL_H

#include <QWidget>
#include <QLabel>
#include <QTableView>

namespace Ui {
class DisplayPanel;
}

class DisplayPanel : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayPanel(QWidget *parent = nullptr);
    ~DisplayPanel();
    QLabel* GetDisplayLabel();
    QTableView* GetTableView();

private:
    Ui::DisplayPanel *ui;
};

#endif // DISPLAYPANEL_H
