#pragma once
#include "cell.hpp"
#include "grid.hpp"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

static const QString OrganizationName("Organization");
static const QString AppName("App");
static const QString GroupName("MainWindow");
static const QString Parameter("Position");

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void buttonGenerationHandle();

protected:
    bool verifyGridValue(const size_t new_width, const size_t new_heightm, const size_t numb_walls);
    void saveSetting();
    void loadSetting();

private:
    Ui::MainWindow* ui;
    Grid* grid;
};
