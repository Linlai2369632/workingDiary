#ifndef SEARCHSEQUENCE_H
#define SEARCHSEQUENCE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QFileInfo>
#include <QSqlQuery>
#include <QMessageBox>
#include <QCompleter>
#include <QDate>
#include <QPalette>
#include <QPushButton>
#include "mainwindow.h"

// 前置宣告 - 避免因為互相引用產生錯誤
class MainWindow;

namespace Ui {
class searchsequence;
}

class searchsequence : public QWidget
{
    Q_OBJECT

public:
    explicit searchsequence(QWidget *parent = nullptr);
    ~searchsequence();

    MainWindow *mainwindow;

private slots:
    void on_pbSearch_clicked();

private:
    QString dbName = "workingDiary.db";

    QStringList dateList;

    QStringList yearList, monthList, dayList;

    QString startDate, endDate;

    QStringList sequenceDateList;

private:
    Ui::searchsequence *ui;

//    MainWindow *mainwindow;

    void init();

    bool connectToDatabase();

    bool importDateFromDb();

    void setAutoFilled();

    bool splitDate();

    bool isDateValid(QString date);

    void findClosestStartDate();

    void findClosestEndDate();

    void getSequenceDate();

    void setTableFromDb();

signals:
    void setSearchDate(QString dateToSearch);

};

#endif // SEARCHSEQUENCE_H
