#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class QTimer;
class QSerialPort;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startPushButton_clicked();
    void onTimerExpire();
    void on_serialOpenPushButton_clicked();
    void on_serialClosePushButton_clicked();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    int min_time;
    int inc_rate;
    int off_count;
    int curr_count;

    /* Serial Port Interface */
    QSerialPort *serialPort;
};

#endif // MAINWINDOW_H
