#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("任務進度管理");

    ui->teOnGoing->setEnabled(false);
    ui->teProgress->setEnabled(false);
    ui->teToDo->setEnabled(false);
    ui->pbSaveAndUpdate->setEnabled(false);
    ui->pbDelete->setEnabled(false);
    ui->pbDelete->setEnabled(false);
    ui->teProgress->setAcceptRichText(false);
    ui->teOnGoing->setAcceptRichText(false);
    ui->teToDo->setAcceptRichText(false);
    ui->leDate->setPlaceholderText("yyyy-MM-dd");
    searchSeq = nullptr;
    searchPro = nullptr;

    // 設置快捷鍵 ctrl + S
    QShortcut *shortCut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(shortCut, &QShortcut::activated, ui->pbSaveAndUpdate, &QPushButton::click);

    // 檢查資料庫是否已存在
    if(!QFileInfo::exists(dbName)) {
        qDebug() << "database not exist";
        QMessageBox messageBox;
        if(QMessageBox::Yes == messageBox.question(this, "詢問", "未檢測到資料庫\n是否建立新的資料庫和表單?", QMessageBox::Yes, QMessageBox::No)) {
            if(!createDatabase()) {
                return;
            }
        }
        else {
            exit(0);
        }
    }
    else {
        // 連線到資料庫
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);

        if (!db.open()) {
            qDebug() << "Database error:" << db.lastError().text();
        }
        qDebug() << "Database connected.";
        db.close();
    }

    QDate currentDate = QDate::currentDate();
    Date = currentDate.toString("yyyy-MM-dd");
    ui->leDate->setText(Date);

    // 設置日期欄位自動補滿
    setAutoFilled();

    // 設置當文字變動時，標簽文字轉為紅色提示
    handleTextChanged();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 檢測退出時是否有尚未儲存的變更
    if(hasUnsaveChanged()) {
        QMessageBox messageBox;
        if(QMessageBox::Yes == messageBox.warning(this, "警告", "檢測到未儲存變更\n是否繼續退出?", QMessageBox::Yes , QMessageBox::No)) {
            // 用戶選擇繼續退出
            event->accept();
        }
        else {
            QMessageBox messageBox;
            messageBox.information(this, "溫馨提醒", "請記得備份 workingDiary.db 喔!\n~^w^~\n\t~^w^~\n\t\t~^w^~");
            event->ignore();
        }
    }
    else {
        QMessageBox messageBox;
        messageBox.information(this, "溫馨提醒", "請記得備份 workingDiary.db 喔!\n~^w^~\n\t~^w^~\n\t\t~^w^~");
        event->accept();
    }
}

void MainWindow::setUI2SearchMode()
{
    ui->pbSearch->setText("查詢");
    ui->leDate->setEnabled(true);
    ui->pbSaveAndUpdate->setEnabled(false);
    ui->pbDelete->setEnabled(false);
    ui->teOnGoing->setEnabled(false);
    ui->teProgress->setEnabled(false);
    ui->teToDo->setEnabled(false);
    ui->teOnGoing->clear();
    ui->teProgress->clear();
    ui->teToDo->clear();
    setLabelBlack();
}

void MainWindow::setUI2ResearchMode()
{
    ui->pbSearch->setText("重新查詢");
    ui->leDate->setEnabled(false);
    ui->pbSaveAndUpdate->setEnabled(true);
    ui->pbDelete->setEnabled(true);
    ui->teOnGoing->setEnabled(true);
    ui->teProgress->setEnabled(true);
    ui->teToDo->setEnabled(true);
    setLabelBlack();
}

bool MainWindow::hasUnsaveChanged()
{
    QPalette palette1 = ui->label->palette();
    QPalette palette2 = ui->label_2->palette();
    QPalette palette3 = ui->label_3->palette();
    QColor currentColor1 = palette1.color(QPalette::WindowText);
    QColor currentColor2 = palette2.color(QPalette::WindowText);
    QColor currentColor3 = palette3.color(QPalette::WindowText);
    if(currentColor1 == Qt::red || currentColor2 == Qt::red || currentColor3 == Qt::red) {
        return true;
    }
    return false;
}

void MainWindow::handleTextChanged()
{
    connect(ui->teProgress, &QTextEdit::textChanged, [&](){
        QPalette palette = ui->label->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->label->setPalette(palette);
    });
    connect(ui->teOnGoing, &QTextEdit::textChanged, [this](){
        QPalette palette = ui->label_2->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->label_2->setPalette(palette);
    });  
    connect(ui->teToDo, &QTextEdit::textChanged, [this](){
        QPalette palette = ui->label_3->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->label_3->setPalette(palette);
    });
}

void MainWindow::setLabelBlack()
{
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::black);
    ui->label->setPalette(palette);
    ui->label_2->setPalette(palette);
    ui->label_3->setPalette(palette);
}

void MainWindow::setAutoFilled()
{
    dateList.clear();
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    if(query.exec("SELECT Date FROM diary")) {
        while(query.next()) {
            QString date = query.value(0).toString();
            dateList.append(date);
        }
    }
    else {
        qDebug() << "Query failed:" << query.lastError().text();
        db.close();
        return;
    }

    QCompleter *completer = new QCompleter(dateList, this);
    ui->leDate->setCompleter(completer);
    db.close();
}

bool MainWindow::createDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }

    // 在資料庫中建立表格
    QSqlQuery query;
    if(!query.exec("CREATE TABLE diary ("
                       "date TEXT PRIMARY KEY, "
                       "progress TEXT, "
                       "onGoing TEXT, "
                       "ToDo TEXT)")) {
        qDebug() << "Table creation error:" << query.lastError().text();
        qDebug() << "建立表格失敗";
        return false;
    }
    qDebug() << "database create success";
    db.close();

    return true;
}

void MainWindow::InsertDb()
{
    Date = ui->leDate->text();
    progress = ui->teProgress->toPlainText();
    onGoing = ui->teOnGoing->toPlainText();
    ToDo = ui->teToDo->toPlainText();

    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return;
    }

    QSqlQuery query;

    query.prepare("INSERT INTO diary (date, progress, onGoing, ToDo) VALUES (:date, :progress, :onGoing, :ToDo)");
    query.bindValue(":date", Date);
    query.bindValue(":progress", progress);
    query.bindValue(":onGoing", onGoing);
    query.bindValue(":ToDo", ToDo);

    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
    } else {
        qDebug() << "Data inserted.";
    }
    db.close();
    QMessageBox messageBox;
    messageBox.information(this, "题示", "建立成功!");
}

void MainWindow::updateDb()
{
   Date = ui->leDate->text();
   progress = ui->teProgress->toPlainText();
   onGoing = ui->teOnGoing->toPlainText();
   ToDo = ui->teToDo->toPlainText();

   QSqlDatabase db = QSqlDatabase::database();
   if(!db.open()) {
       qDebug() << "Database error:" << db.lastError().text();
       return;
   }

   QSqlQuery query;
   query.prepare("UPDATE diary SET progress = :progress, onGoing = :onGoing, ToDo = :ToDo WHERE date = :date");
   query.bindValue(":progress", progress);
   query.bindValue(":onGoing", onGoing);
   query.bindValue(":ToDo", ToDo);
   query.bindValue(":date", Date);

   if (!query.exec()) {
       qDebug() << "Query error:" << query.lastError().text();
   } else {
       qDebug() << "Data updated.";
   }
   db.close();

   QMessageBox messageBox;
   messageBox.information(this, "提示", "資料儲存成功");

   setLabelBlack();
}

bool MainWindow::CheckIfExist()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM diary WHERE date = :date");
    query.bindValue(":date", Date);

    if(!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        db.close();
        return false;
    }

    if(query.next()) {
        qDebug() << "Data found for date:" << Date;
        db.close();
        return true;
    } else {
        qDebug() << "Data not found for date:" << Date;
        db.close();
        return false;
    }
}

void MainWindow::on_pbSaveAndUpdate_clicked()
{
    // 檢查 Date 欄位是否存在該筆資料
    // 資料庫不存在該筆資料 - INSERT, - UPDATE
    if(!CheckIfExist()) {
        QMessageBox messageBox;
        if(QMessageBox::Yes == messageBox.warning(this, "詢問", "當前資料庫無該日期資料，\n是否繼續進行?", QMessageBox::Yes, QMessageBox::No)) {
            InsertDb();
            updateDb();
            return;
        }
        else {
            return;
        }
    }
    // 資料庫存在該筆資料 - UPDATE
    QMessageBox messageBox;
    if(QMessageBox::No == messageBox.question(this, "是否更新", "請再次確認是否更新資料", QMessageBox::Yes, QMessageBox::No)) {
        qDebug() << "update cancel";
        return;
    }
    else {
       updateDb();
    }
}

void MainWindow::on_pbSearch_clicked()
{
    // 檢查是否有尚未儲存的變更
    if(hasUnsaveChanged()) {
        QMessageBox messageBox;
        if(QMessageBox::No == messageBox.warning(this, "警告", "有尚未儲存的變更\n是否繼續?", QMessageBox::Yes, QMessageBox::No)){
            return;
        }
    }

    if(ui->pbSearch->text() == "重新查詢") {
        setUI2SearchMode();
        return;
    }

    // 檢查日期輸入格式是否正確
    Date = ui->leDate->text();
    if(!QDate::fromString(Date, "yyyy-MM-dd").isValid()) {
        QMessageBox messageBox;
        messageBox.warning(this, "警告", "請輸入合法日期");
        return;
    }
    // 從資料庫檢索是否存在該日期資料
    if(!CheckIfExist()) {
        QMessageBox messageBox;
        if(QMessageBox::Yes == messageBox.warning(this, "詢問", "當前資料庫無該日期資料，\n是否建立?", QMessageBox::Yes, QMessageBox::No)) {
            // 在資料庫建立該日期資料
            InsertDb();
            setAutoFilled();
        }
        else {
            return;
        }
    }
    // 從資料庫讀取資料
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM diary WHERE date = :date");
    query.bindValue(":date", Date);

    if(!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        db.close();
        return;
    }

    if(query.next()) {
        QString progress = query.value("progress").toString();
        QString ongoing = query.value("onGoing").toString();
        QString todos = query.value("ToDo").toString();
        ui->teProgress->setPlainText(progress);
        ui->teOnGoing->setPlainText(ongoing);
        ui->teToDo->setPlainText(todos);
    }
    db.close();

    setUI2ResearchMode();
}

void MainWindow::on_pbSearchSequence_clicked()
{
    searchSeq = new searchsequence;
    searchSeq->show();
    connect(searchSeq, &searchsequence::setSearchDate, [this](QString dateToSearch){
       qDebug() << "dateToSearch" << dateToSearch;
       searchSeq->mainwindow->ui->leDate->setText(dateToSearch);
       searchSeq->mainwindow->ui->pbSearch->click();
    });
}

void MainWindow::on_pbProjectDiary_clicked()
{
    searchPro = new searchProject;
    searchPro->show();
}

void MainWindow::on_pbDelete_clicked()
{
    QMessageBox messageBox;
    if(QMessageBox::No == messageBox.warning(this, "詢問", "請再次確認是否刪除該日期所有資料?", QMessageBox::Yes, QMessageBox::No)) {
        return;
    }

    Date = ui->leDate->text();
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return;
    }
    QSqlQuery query;
    query.prepare("DELETE FROM diary WHERE date = :date");
    query.bindValue(":date", Date);
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
    ui->teProgress->clear();
    ui->teOnGoing->clear();
    ui->teToDo->clear();
    setUI2SearchMode();
}
