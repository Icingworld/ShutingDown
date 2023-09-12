#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("关机小助手");
    setWindowIcon(QIcon(":/Icon.png"));
    ui->stackedWidget->setCurrentIndex(0);
    on_mode_1_clicked(true);
    ui->time_2->setDateTime(QDateTime::currentDateTime());
    ui->special->setCheckState(Qt::Unchecked);

    // 创建一个系统托盘图标
    trayIcon = new QSystemTrayIcon(QIcon(":/Icon.png"), this);
    trayIcon->setToolTip("关机小助手");
    // 创建一个菜单
    QMenu * trayMenu = new QMenu(this);
    statAction = trayMenu->addAction("无计划");
    statAction->setEnabled(false);
    statAction->setIcon(QIcon());
    statAction->setIconVisibleInMenu(false);
    cancelAction = trayMenu->addAction("-");
    cancelAction->setEnabled(false);
    cancelAction->setIcon(QIcon());
    cancelAction->setIconVisibleInMenu(false);
    trayMenu->addSeparator();
    QAction * quitAction = trayMenu->addAction("退出小助手", this, &QCoreApplication::quit);
    quitAction->setIcon(QIcon());
    quitAction->setIconVisibleInMenu(false);
    // 将菜单设置到托盘图标上
    trayIcon->setContextMenu(trayMenu);
    // 显示托盘图标
    trayIcon->show();

    connect(cancelAction, &QAction::triggered, this, &MainWindow::on_cancelButton_clicked);
    connect(&timer, &QTimer::timeout, this, &MainWindow::count);
    timer.setInterval(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_mode_1_clicked(bool checked)
{
    ui->mode_1->setCheckState(Qt::Checked);
    if (checked)
    {
        ui->mode_2->setCheckState(Qt::Unchecked);
        ui->time_1->setEnabled(true);
        ui->measure->setEnabled(true);
        ui->time_2->setEnabled(false);
    }
}

void MainWindow::on_mode_2_clicked(bool checked)
{
    ui->mode_2->setCheckState(Qt::Checked);
    if (checked)
    {
        ui->mode_1->setCheckState(Qt::Unchecked);
        ui->time_1->setEnabled(false);
        ui->measure->setEnabled(false);
        ui->time_2->setEnabled(true);
    }
}

void MainWindow::on_createButton_clicked()
{
    if (special)
    {
        hide();
        trayIcon->setToolTip("小助手");
    }

    QString time = "";
    cancelAction->setEnabled(true);
    cancelAction->setText("取消任务");
    if (ui->mode_1->checkState())
    {
        mode = "定时";
        int measure = ui->measure->currentIndex();
        if (measure == 0)
        {
            time = ui->time_1->text();
        } else if (measure == 1) {
            time = QString::number(ui->time_1->text().toInt() * 60);
        } else if (measure == 2) {
            time = QString::number(ui->time_1->text().toInt() * 60 * 60);
        } else {
            time = QString::number(ui->time_1->text().toInt() * 60 * 60 * 24);
        }
        fixed = "共计时：" + ui->time_1->text() + ui->measure->currentText();
    } else {
        mode = "定点";
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QTime currentTime = currentDateTime.time();
        QTime destTime = ui->time_2->time();

        int secondsDiff = currentTime.secsTo(destTime);
        if (secondsDiff <= 0)
        {
            secondsDiff += 24 * 60 * 60;
        }
        time = QString::number(secondsDiff);
        fixed = "目标：" + destTime.toString("hh:mm:ss");;
    }
    timeCount = time.toInt();
    ui->stackedWidget->setCurrentIndex(1);
    ui->statusLabel->setText("开始计时");
    timer.start();
}

void MainWindow::on_cancelButton_clicked()
{
    timer.stop();
    ui->statusLabel->setText("停止计时");
    ui->stackedWidget->setCurrentIndex(0);
    statAction->setText("无任务");
    cancelAction->setText("-");
    cancelAction->setEnabled(false);

    if (special)
    {
        show();
        trayIcon->setToolTip("关机小助手");
    }
}

void MainWindow::on_special_clicked(bool checked)
{
    special = checked;
}

void MainWindow::count()
{
    timeCount--;
    if (timeCount == 0)
    {
        QProcess process;
        process.start("shutdown /s /t 0");
        process.waitForFinished();

    } else if (timeCount <= 3) {
        ui->statusLabel->setText("即将关机，再见！");
        if (special)
        {
            trayIcon->showMessage("小助手提示", "即将关机，再见！", QSystemTrayIcon::Information, 3000);
        }
        ui->cancelButton->setVisible(false);
    } else {
        statAction->setText("倒计时：" + QString::number(timeCount));
        ui->statusLabel->setText("模式：" + mode + "<br><br>" + fixed + "<br><br>倒计时：" + QString::number(timeCount));
    }
}
