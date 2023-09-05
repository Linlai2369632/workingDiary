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
#include <QShortcut>

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

private:
    QString dbName = "workingDiary.db";

    QStringList projectNameList;

    QString projectName;

private:
    Ui::searchProject *ui;

    bool initDbTable();

    void handleTextChanged();

    void setLabelBlack();

    bool hasUnsaveChanged();

    bool importProjectNameFromDb();

    void setAutoFilled();

    bool InsertDb();

    void triggerUpdateButton();

    void deleteTable();
};

#endif // SEARCHPROJECT_H
