
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <time.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerExpire()));

    serialPort = new QSerialPort(this);
    min_time = 0;
    inc_rate = 0;;
    off_count = 0;;
    curr_count = 0;

    ui->serialPortComboBox->clear();

    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    foreach (QSerialPortInfo serInfo, infoList) {
        ui->serialPortComboBox->insertItem(-1, serInfo.portName());
    }
}

MainWindow::~MainWindow() {
    if(serialPort->isOpen()) {
        serialPort->close();
        delete serialPort;
    }
    delete ui;
}

void MainWindow::onTimerExpire() {
    curr_count++;
    ui->lcdNumber->display(curr_count);

    if(curr_count >= off_count) {
        serialPort->setRequestToSend(true);
        timer->stop();
        ui->startPushButton->setEnabled(true);
    }
}

void MainWindow::on_startPushButton_clicked() {
    if(!serialPort->isOpen()) {
        QMessageBox::critical(this, "Nauto Random Power Loss Tester", "Open Serial Port first...", QMessageBox::Ok);
        return;
    }
    serialPort->setRequestToSend(false);

    min_time = ui->minTimeLineEdit->text().toInt();
    inc_rate = ui->incrRateLineEdit->text().toInt();

    srand(time(NULL));
    off_count =  ((min_time + (rand() % 5)) * inc_rate);
    curr_count = 0;

    qDebug() << "Current Count = " << curr_count << " Off Count = " << off_count;
    timer->setInterval((int)(1000.00/(float)inc_rate));
    timer->start();
    ui->startPushButton->setDisabled(true);
}

void MainWindow::on_serialOpenPushButton_clicked() {
    serialPort = new QSerialPort(ui->serialPortComboBox->currentText(), this);
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::SoftwareControl);
    if(serialPort->open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, "Nauto Random Power Loss Tester", "Serial port opened...", QMessageBox::Ok);
    } else {
        QMessageBox::critical(this, "Nauto Random Power Loss Tester", "Serial port could not be opened...", QMessageBox::Ok);
        delete serialPort;
    }
}

void MainWindow::on_serialClosePushButton_clicked() {
    if(serialPort->isOpen()) {
        serialPort->close();
        QMessageBox::information(this, "Nauto Random Power Loss Tester", "Serial port closed...", QMessageBox::Ok);
        delete serialPort;
    }
}


void MainWindow::on_pushButton_clicked() {
    bool val = serialPort->isRequestToSend();
    qDebug() << "Setting RTS = " << val;
    serialPort->setRequestToSend(!val);
}
