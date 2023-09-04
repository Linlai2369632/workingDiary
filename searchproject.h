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
//#include <QDate>
#include <QPalette>

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

private:
    QString dbName = "workingDiary.db";

    QStringList projectNameList;

    QString projectName;

private:
    Ui::searchProject *ui;

    bool initDbTable();

    bool importProjectNameFromDb();

    void setAutoFilled();

    bool InsertDb();
};

#endif // SEARCHPROJECT_H
