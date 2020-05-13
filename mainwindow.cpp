#include "mainwindow.hpp"
#include "ui_mainwindow.h"

bool MainWindow::verifyGridValue(const size_t new_width, const size_t new_height, const size_t numb_walls)
{
    if(new_width < 2 || new_height < 2)
    {
        QMessageBox::warning(this, "Warning", "Grid width or height must be greater than 1.");
        return false;
    }
    if(numb_walls > new_width * new_height)
    {
        QMessageBox::warning(this, "Warning", "The number of walls cannot exceed the number of cells.");
        return false;
    }
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    grid = new Grid;
    ui->graphicsView->setScene(grid);
    ui->graphicsView->setSceneRect(QRectF(0, 0, 0, 0));
    grid->addWidget(ui->buttonGeneration);
    connect(ui->buttonGeneration, SIGNAL(clicked()), this, SLOT(buttonGenerationHandle()));
    loadSetting();
}

MainWindow::~MainWindow()
{
    saveSetting();
    delete grid;
    delete ui;
}

void MainWindow::buttonGenerationHandle()
{
    const int new_width = ui->widthSpinBox->value();
    const int new_height = ui->heightSpinBox->value();
    const int number_walls = ui->wallsSpinBox->value();

    if (verifyGridValue(new_width, new_height, number_walls))
    {
        if(new_width == grid->width() && new_height == grid->height())
            grid->update(number_walls);
        else
            grid->build(new_width, new_height, number_walls);
    }
}

void MainWindow::saveSetting()
{
    QSettings settings(OrganizationName, AppName);
    settings.beginGroup(GroupName);
    settings.setValue(Parameter, this->geometry());
    settings.endGroup();
}

void MainWindow::loadSetting()
{
    QSettings settings(OrganizationName, AppName);
    settings.beginGroup(GroupName);
    const QRect rect = settings.value(Parameter).toRect();
    this->setGeometry(rect);
    settings.endGroup();
}
