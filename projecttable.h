#ifndef PROJECTTABLE_H
#define PROJECTTABLE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
#include "searchproject.h"

// 前置宣告 - 避免因為互相引用產生錯誤
class searchProject;

namespace Ui {
class ProjectTable;
}

class ProjectTable : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectTable(QWidget *parent = nullptr);
    ~ProjectTable();

    searchProject *searchPro;

private:
    Ui::ProjectTable *ui;

    void importDataFromDb();

signals:
    void setProjectName(QString projectName);
private slots:
    void on_pbRefresh_clicked();
};

#endif // PROJECTTABLE_H
