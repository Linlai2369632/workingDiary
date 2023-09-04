#include "searchproject.h"
#include "ui_searchproject.h"

searchProject::searchProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchProject)
{
    ui->setupUi(this);

    setWindowTitle("專案進度管理");

    if(!initDbTable()) {
        return;
    }

    if(!importProjectNameFromDb()) {
        return;
    }

    if(projectNameList.empty() == false) {
        setAutoFilled();
    }

}

searchProject::~searchProject()
{
    delete ui;
}

bool searchProject::initDbTable()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.setDatabaseName(dbName);

    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }
    // 檢查 database 中是否存在表格 projectDiary，若不存在，則建立。
    QSqlQuery query;
    if (!query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='projectDiary'")) {
        qDebug() << "查詢表格是否存在时出错：" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        if(!query.exec("CREATE TABLE projectDiary ("
                       "projectName TEXT PRIMARY KEY, "
                       "participateMember TEXT, "
                       "developeRecord TEXT, "
                       "todo TEXT)")) {
            qDebug() << "Table creation error:" << query.lastError().text();
            qDebug() << "建立表格失敗";
            return false;
        }
        qDebug() << "database create table workingDiary success";
    }
    else{
        qDebug() << "table workingDiary has exist";
    }

    db.close();
    return true;
}

bool searchProject::importProjectNameFromDb()
{
    projectNameList.clear();
    QSqlDatabase db = QSqlDatabase::database();

    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    if(query.exec("SELECT projectName FROM projectDiary")) {
        while(query.next()) {
            QString projectName = query.value(0).toString();
            projectNameList.append(projectName);
        }
    }
    else {
        qDebug() << "Query failed:" << query.lastError().text();
        return false;
    }

    foreach (QString project, projectNameList) {
        qDebug() << project;
    }

    db.close();
    return true;
}

void searchProject::setAutoFilled()
{
    QCompleter *completer = new QCompleter(projectNameList);
    ui->leProjectName->setCompleter(completer);
}

bool searchProject::InsertDb()
{
    QString projectInformation = ui->teProjectInformation->toPlainText();
    QString participateMember = ui->teParticipateMember->toPlainText();
    QString developeRecord = ui->teDevelopeRecord->toPlainText();
    QString todo = ui->teToDo->toPlainText();

    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO projectDiary (projectName, participateMember, developeRecord, todo) VALUES (:projectName, :participateMember, :developeRecord, :todo)");
    query.bindValue(":projectName", projectName);
    query.bindValue(":participateMember", participateMember);
    query.bindValue(":developeRecord", developeRecord);
    query.bindValue(":todo", todo);

    if(!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Data inserted.";
    }

    db.close();
    return true;
}

void searchProject::on_pbCreate_clicked()
{
    if(ui->leProjectName->text().isEmpty()) {
        QMessageBox messageBox;
        messageBox.warning(this, "警告", "請輸入專案名稱");
        return;
    }

    projectName = ui->leProjectName->text();

    // 使用 QRegExp 來確認文本不僅僅是空格和換行。 2.移除前後方的空白。
    projectName = projectName.trimmed();
    if (projectName.isEmpty()) {
        QMessageBox messageBox;
        messageBox.warning(this, "格式錯誤", "專案名稱不能只包含空格和換行");
        return;
    }

    // 檢查 project 是否已經存在
    if(projectNameList.contains(projectName) == true) {
        QMessageBox messageBox;
        messageBox.information(this, "提示", "該專案已存在!\n請使用搜尋~");
        return;
    }
    // 在 database 建立
    if(InsertDb()) {
        importProjectNameFromDb();
        setAutoFilled();
        QMessageBox messageBox;
        messageBox.information(this, "提示", "建立表格成功!");
    }
}
