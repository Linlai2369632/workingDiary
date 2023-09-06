#ifndef SEARCHPROJECT_H
#define SEARCHPROJECT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QFileInfo>
#include <QSqlQuery>
#include <QMessageBox>
#include <QCompleter>
#include <QPalette>
#include <QShortcut>
#include <QCloseEvent>
#include "projecttable.h"

// 前置宣告 - 避免因為互相引用產生錯誤
class ProjectTable;

namespace Ui {
class searchProject;
}

class searchProject : public QWidget
{
    Q_OBJECT

public:
    explicit searchProject(QWidget *parent = nullptr);
    ~searchProject();

private slots:
    void on_pbCreate_clicked();

    void on_pbSearch_clicked();

    void on_pbUpdate_clicked();

    void on_pbDelete_clicked();

    void on_pbShowAllProjectName_clicked();

private:
    QString dbName = "workingDiary.db";

    QStringList projectNameList;

    QString projectName;

private:
    Ui::searchProject *ui;

    ProjectTable *proTable;

    bool initDbTable();

    void handleTextChanged();

    void setLabelBlack();

    bool hasUnsaveChanged();

    bool importProjectNameFromDb();

    void setAutoFilled();

    void setUI2SearchMode();

    void setUI2ResearchMode();

    bool InsertDb();

    void deleteTable();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // SEARCHPROJECT_H
