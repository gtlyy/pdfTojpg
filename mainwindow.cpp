#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myfun.hpp"

#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <QSysInfo>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_Save->setText(QDir::homePath());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open pdf", QDir::homePath(), "files (*.pdf)");
    ui->lineEdit->setText(fileName);
}


void MainWindow::on_pushButton_3_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, "保存目录", QDir::homePath());
    ui->lineEdit_Save->setText(dirName);
}


void MainWindow::on_pushButton_2_clicked()
{
    QProcess p(this);
    QString cmd, sFile, pStart, pEnd, spEnd;
    QString prefix, quality, saveDir;
    QStringList arg;
    char *chStr;
    QString msgs;

    cmd = cmd + "gs ";
    if (ui->lineEdit->text() != NULL)
    {
        sFile += "\"" + ui->lineEdit->text() + "\"";
    } else {
        QMessageBox::warning(this, "提示", "请先选择pdf文件！");
        return;
    }
    pStart = ui->spinBox_start->text() == "0" ? "1" : ui->spinBox_start->text();
    if (ui->spinBox_end->text() != "0") {
        spEnd = spEnd + "-dLastPage=" + ui->spinBox_end->text();
    }
    prefix = ui->lineEdit_2->text();
    quality = ui->comboBox_quality->currentText();
    saveDir = ui->lineEdit_Save->text();

    cmd = cmd + "-dNOSAFER " + "-r" + quality + " -dBATCH " + "-sDEVICE=jpeg " + "-dNOPAUSE " + "-dEPSCrop " + \
            "-dFirstPage=" + pStart + " " + spEnd + " -sOutputFile=" + prefix + "%d.jpg " + sFile;

    std::cout << cmd.toStdString() << std::endl;

//    chStr = QString2char(cmd);

    // pdf to jpg
    QString errMsg = MyExec(cmd, saveDir);
    std::cout << errMsg.toStdString() << std::endl;

//#if defined (Q_OS_LINUX)
//    p.setProgram("bash");
//    arg << "-c" << chStr ;
//#elif defined (Q_OS_WIN32)
//    p.setProgram("cmd");
//    arg << "/c" << chStr ;
//#endif

//    p.setWorkingDirectory(saveDir);
//    p.setArguments(arg);
//    p.start();
//    p.waitForStarted();
//    p.waitForFinished();

//    QString errMsg = QString::fromUtf8(p.readAllStandardError());
    if (errMsg == "")
    {
        msgs = "Pdf to jpgs ok.\n";
//         QMessageBox::information(this, "pdfToJpg", "成功！");
    }
    else
    {
        QMessageBox::warning(this, "Error", errMsg);
        return;
    }

    // jpg to all-in-one.jpg
    if (ui->checkBox->isChecked()) {
        QString inJpg = prefix + "*.jpg";
        QString append;
        if (ui->comboBox_append->currentText() == "竖向")
        {
            append = " -append ";
        }
        else
        {
            append = " +append ";
        }
        cmd = "convert " + inJpg + append + ui->lineEdit_allinone->text();
        std::cout << cmd.toStdString() << std::endl;
        QString errMsg2 = MyExec(cmd, saveDir);
        std::cout << errMsg2.toStdString() << std::endl;
        if (errMsg2 == "")
        {
//            QMessageBox::information(this, "pdfToJpg all-in-one", "成功！");
            msgs = msgs + "To all-in-one.jpg ok.";
        }
        else
        {
            QMessageBox::warning(this, "Error", errMsg2);
            return;
        }
    }
    QMessageBox::information(this, "pdfToJpg", "成功！");

    // del out-*.jpg
    if (ui->comboBox_delornot->currentText() == "是")
    {
        MyExec("rm -f " + saveDir + "/" + ui->lineEdit_2->text() + "*.jpg", saveDir);
    }

}
