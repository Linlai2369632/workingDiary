#include "searchproject.h"
#include "ui_searchproject.h"

searchProject::searchProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchProject)
{
    ui->setupUi(this);
    setWindowTitle("專案進度管理");
    proTable = nullptr;

    ui->pbUpdate->setEnabled(false);
    ui->pbDelete->setEnabled(false);
    ui->teDevelopeRecord->setAcceptRichText(false);
    ui->teParticipateMember->setAcceptRichText(false);
    ui->teProjectInformation->setAcceptRichText(false);
    ui->teToDo->setAcceptRichText(false);
    ui->leProjectName->setPlaceholderText("前後不留白，區分大小寫");

    if(!initDbTable()) {
        return;
    }

    if(!importProjectNameFromDb()) {
        return;
    }

    if(projectNameList.empty() == false) {
        setAutoFilled();
    }

    // 設置 CTRL + S 快捷鍵
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(shortcut, &QShortcut::activated, ui->pbUpdate, &QPushButton::click);

    // 設置提醒使用者有尚未儲存的變動
    handleTextChanged();
}

searchProject::~searchProject()
{
    delete ui;
}

void searchProject::closeEvent(QCloseEvent *event)
{
    // 檢測退出時是否有尚未儲存的變更
    if(hasUnsaveChanged()) {
        QMessageBox messageBox;
        if(QMessageBox::Yes == messageBox.warning(this, "警告", "檢測到未儲存變更\n是否繼續退出?", QMessageBox::Yes , QMessageBox::No)) {
            // 用戶選擇繼續退出
            event->accept();
        }
        else {
            event->ignore();
        }
    }
    else {
        event->accept();
    }
}

void searchProject::deleteTable()
{
    //      delete table, use it carefully!!! Confirm that you have already backed up the database.
    //      delete table, use it carefully!!! Confirm that you have already backed up the database.
    //      delete table, use it carefully!!! Confirm that you have already backed up the database.
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);
        if (!db.open()) {
            qWarning() << "無法打開數據庫";
            return;
        }
        QSqlQuery query;
        QString tableName = "projectDiary";
        QString deleteTableQuery = "DROP TABLE " + tableName;
        if (!query.exec(deleteTableQuery)) {
            qWarning() << "刪除表格失敗: " << query.lastError().text();
        } else {
            qDebug() << "成功刪除表格: " << tableName;
        }
        db.close();
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
        db.close();
        return false;
    }

    if (!query.next()) {
        if(!query.exec("CREATE TABLE projectDiary ("
                       "projectName TEXT PRIMARY KEY, "
                       "projectInformation TEXT, "
                       "participateMember TEXT, "
                       "developeRecord TEXT, "
                       "todo TEXT)")) {
            qDebug() << "Table creation error:" << query.lastError().text();
            qDebug() << "建立表格失敗";
            db.close();
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
        db.close();
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

void searchProject::handleTextChanged()
{
    connect(ui->teProjectInformation, &QTextEdit::textChanged, [this](){
        QPalette palette = ui->label->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->label->setPalette(palette);
    });
    connect(ui->teParticipateMember, &QTextEdit::textChanged, [this](){
        QPalette palette = ui->label_3->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->label_3->setPalette(palette);
    });
    connect(ui->teDevelopeRecord, &QTextEdit::textChanged, [this](){
        QPalette palette = ui->label_4->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->label_4->setPalette(palette);
    });
    connect(ui->teToDo, &QTextEdit::textChanged, [this](){
        QPalette palette = ui->label_5->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->label_5->setPalette(palette);
    });
}

void searchProject::setLabelBlack()
{
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::black);
    ui->label->setPalette(palette);
    ui->label_3->setPalette(palette);
    ui->label_4->setPalette(palette);
    ui->label_5->setPalette(palette);
}

bool searchProject::hasUnsaveChanged()
{
    QPalette palette1 = ui->label->palette();
    QPalette palette2 = ui->label_3->palette();
    QPalette palette3 = ui->label_4->palette();
    QPalette palette4 = ui->label_5->palette();
    QColor color1 = palette1.color(QPalette::WindowText);
    QColor color2 = palette2.color(QPalette::WindowText);
    QColor color3 = palette3.color(QPalette::WindowText);
    QColor color4 = palette4.color(QPalette::WindowText);
    if(color1 == Qt::red || color2 == Qt::red || color3 == Qt::red || color4 == Qt::red) {
        return true;
    }
    return false;
}

void searchProject::setUI2SearchMode()
{
    ui->pbSearch->setText("查詢");
    ui->leProjectName->setEnabled(true);
    ui->pbCreate->setEnabled(true);
    ui->pbUpdate->setEnabled(false);
    ui->pbDelete->setEnabled(false);
    ui->teDevelopeRecord->clear();
    ui->teParticipateMember->clear();
    ui->teProjectInformation->clear();
    ui->teToDo->clear();
    setLabelBlack();
}

void searchProject::setUI2ResearchMode()
{
    ui->pbSearch->setText("重新查詢");
    ui->leProjectName->setEnabled(false);
    ui->pbCreate->setEnabled(false);
    ui->pbUpdate->setEnabled(true);
    ui->pbDelete->setEnabled(true);
    setLabelBlack();
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
    query.prepare("INSERT INTO projectDiary (projectName, projectInformation, participateMember, developeRecord, todo) VALUES (:projectName, :projectInformation, :participateMember, :developeRecord, :todo)");
    query.bindValue(":projectName", projectName);
    query.bindValue(":projectInformation", projectInformation);
    query.bindValue(":participateMember", participateMember);
    query.bindValue(":developeRecord", developeRecord);
    query.bindValue(":todo", todo);

    if(!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        db.close();
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

    // 再次詢問是否建立專案
    QMessageBox messageBox;
    QString mas = QString("再次確認是否建立專案\n\"%1\"?").arg(projectName);
    if(QMessageBox::No == messageBox.question(this, "詢問", mas, QMessageBox::Yes, QMessageBox::No)) {
        if(ui->pbSearch->text() == "重新查詢") {
            return;
        }
        return;
    }

    // 檢查 project 是否已經存在
    if(projectNameList.contains(projectName) == true) {
        QMessageBox messageBox;
        messageBox.information(this, "提示", "該專案已存在!\n請使用查尋~");
        return;
    }
    // 在 database 建立
    if(InsertDb()) {
        QMessageBox messageBox;
        messageBox.information(this, "提示", "專案建立成功!");
    }
    // 更新自動填滿清單
    importProjectNameFromDb();
    setAutoFilled();
    ui->leProjectName->setText(projectName);
    // set UI control
    setUI2ResearchMode();
}

void searchProject::on_pbSearch_clicked()
{
    // 檢查是否有尚未儲存的變更
    if(hasUnsaveChanged() == true) {
        QMessageBox messageBox;
        if(QMessageBox::No == messageBox.warning(this, "警告", "有尚未儲存的變更\n是否繼續?", QMessageBox::Yes, QMessageBox::No)) {
            return;
        }
    }

    // 將 UI 設為 "重新查詢"
    if(ui->pbSearch->text() == "重新查詢") {
        setUI2SearchMode();
        return;
    }

    // 檢查專案是否存在，若不存在則詢問是否建立
    projectName = ui->leProjectName->text();
    projectName = projectName.trimmed();
    ui->leProjectName->setText(projectName);
    if(!projectNameList.contains(projectName)) {
        QMessageBox messageBox;
        if(QMessageBox::Yes == messageBox.question(this, "詢問", "未搜尋到該專案\n是否建立?", QMessageBox::Yes, QMessageBox::No)) {
            emit ui->pbCreate->click();
            return;
        }
        else {
            return;
        }
    }

    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM projectDiary WHERE projectName = :projectName");
    query.bindValue(":projectName", projectName);

    if(!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        db.close();
        return;
    }

    if(query.next()) {
        QString projectInformation = query.value("projectInformation").toString();
        QString participateMember = query.value("participateMember").toString();
        QString developeRecord = query.value("developeRecord").toString();
        QString todo = query.value("todo").toString();
        ui->teProjectInformation->setPlainText(projectInformation);
        ui->teParticipateMember->setPlainText(participateMember);
        ui->teDevelopeRecord->setPlainText(developeRecord);
        ui->teToDo->setPlainText(todo);
    }
    db.close();

    // 將 UI 設為 "重新查詢"
    setUI2ResearchMode();
}

void searchProject::on_pbUpdate_clicked()
{
    QMessageBox messageBox;
    if(QMessageBox::No == messageBox.warning(this, "提醒", "更新後將覆蓋原有資料\n是否繼續?", QMessageBox::Yes, QMessageBox::No)) {
        return;
    }
    projectName = ui->leProjectName->text();
    QString projectInformation = ui->teProjectInformation->toPlainText();
    QString participateMember = ui->teParticipateMember->toPlainText();
    QString developeRecord = ui->teDevelopeRecord->toPlainText();
    QString todo = ui->teToDo->toPlainText();

    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE projectDiary SET projectInformation = :projectInformation, participateMember = :participateMember, "
                  "developeRecord = :developeRecord, todo = :todo WHERE projectName = :projectName");
    query.bindValue(":projectInformation", projectInformation);
    query.bindValue(":projectName", projectName);
    query.bindValue(":participateMember", participateMember);
    query.bindValue(":developeRecord", developeRecord);
    query.bindValue(":todo", todo);

    if(!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        QMessageBox messageBox;
        messageBox.warning(this, "失敗", "資料更新失敗");
        db.close();
        return;
    }
    else {
        QMessageBox messageBox;
        messageBox.information(this, "成功", "資料更新成功");
    }
    db.close();
    setLabelBlack();
}

void searchProject::on_pbDelete_clicked()
{
    QMessageBox messageBox;
    if(QMessageBox::No == messageBox.warning(this, "詢問", "請再次確認是否刪除該專案資料?", QMessageBox::Yes, QMessageBox::No)) {
        return;
    }

    projectName = ui->leProjectName->text();
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return;
    }
    QSqlQuery query;
    query.prepare("DELETE FROM projectDiary WHERE projectName = :projectName");
    query.bindValue(":projectName", projectName);
    if(!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        db.close();
        return;
    }
    else {
        QMessageBox messageBox;
        messageBox.information(this, "提示", "資料刪除成功!");
        qDebug() << "delete Success!";
    }
    ui->teDevelopeRecord->clear();
    ui->teParticipateMember->clear();
    ui->teProjectInformation->clear();
    ui->teToDo->clear();
    setUI2SearchMode();
}

void searchProject::on_pbShowAllProjectName_clicked()
{
    proTable = new ProjectTable;
    proTable->show();
    connect(proTable, &ProjectTable::setProjectName, [this](QString projectName){
        proTable->searchPro->ui->leProjectName->setText(projectName);
        proTable->searchPro->ui->pbSearch->click();
    });
}
