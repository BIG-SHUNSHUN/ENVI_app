#include "displaypanel.h"
#include "ui_displaypanel.h"

DisplayPanel::DisplayPanel(QWidget *parent) :
    QWidget(parent , Qt::Window),
    ui(new Ui::DisplayPanel)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
}

DisplayPanel::~DisplayPanel()
{
    delete ui;
}

QLabel *DisplayPanel::GetDisplayLabel()
{
    return ui->labelDisplay;
}

QTableView *DisplayPanel::GetTableView()
{
    return ui->tableView;
}
