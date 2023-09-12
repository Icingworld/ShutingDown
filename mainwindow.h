#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDateTime>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer timer;
    QString mode;
    QString fixed;
    int timeCount;
    bool special{false};
    QSystemTrayIcon * trayIcon;
    QAction * statAction;
    QAction * cancelAction;

private slots:
    void on_mode_1_clicked(bool checked);
    void on_mode_2_clicked(bool checked);
    void on_createButton_clicked();
    void on_cancelButton_clicked();
    void on_special_clicked(bool checked);
    void count();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
