#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QFileInfo>
#include <QSqlQuery>
#include <QDate>
#include <QMessageBox>
#include <QPalette>
#include <QCloseEvent>
#include <QShortcut>
#include "searchsequence.h"
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

// 前置宣告 - 避免因為互相引用產生錯誤
class searchsequence;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void setSearchDate(QString dateToSearch);

private slots:
    void on_pbSaveAndUpdate_clicked();

    void on_pbSearch_clicked();

    void on_pbSearchSequence_clicked();

private:
    QString dbName = "workingDiary.db";

    QString Date;

    QString progress;

    QString onGoing;

    QString ToDo;

    searchsequence *searchSeq;

private:
    Ui::MainWindow *ui;

    bool createDatabase();

    void setLabelRed();

    void setLabelBlack();

    bool hasUnsaveChanged();

    bool CheckIfExist();

    void InsertDb();

    void updateDb();

    void uploadDatebase();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
