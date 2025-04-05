#ifndef MYFUN_HPP
#define MYFUN_HPP
#include <iostream>
#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include <QDir>
#include <QString>

char* QString2char(QString s);
QString MyExec(QString cmd, QString execDir);

#endif // MYFUN_HPP
