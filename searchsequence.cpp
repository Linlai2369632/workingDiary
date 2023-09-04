#include "searchsequence.h"
#include "ui_searchsequence.h"

searchsequence::searchsequence(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchsequence)
{
    ui->setupUi(this);

    mainwindow = nullptr;

    ui->leDateStart->setPlaceholderText("yyyy-MM-dd");
    ui->leDateEnd->setPlaceholderText("yyyy-MM-dd");

    setWindowTitle("區間查詢");
    // 取得資料庫裡所有的 Date
    if(!importDateFromDb()) {
        return;
    }

    if(dateList.empty() == false) {
        setAutoFilled();
    }

    // 設定表格欄位名稱
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->insertColumn(2);
    ui->tableWidget->insertColumn(3);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("日期"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("本日進度"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("正在進行"));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("待辦事項"));

    // 設定表格的列寬以填滿視窗
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}

searchsequence::~searchsequence()
{
    delete ui;
}

void searchsequence::init()
{
    ui->tableWidget->setRowCount(0);

    ui->tbFinish->clear();
    ui->tbProcessing->clear();
    ui->tbToDo->clear();

    sequenceDateList.clear();
}

bool searchsequence::importDateFromDb()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
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
        return false;
    }

    foreach (QString Date, dateList) {
        qDebug() << Date;
    }
    db.close();
    return true;
}

void searchsequence::setAutoFilled()
{
    QCompleter *completer = new QCompleter(dateList, this);
    ui->leDateStart->setCompleter(completer);
    ui->leDateEnd->setCompleter(completer);
}

bool searchsequence::isDateValid(QString date)
{
    // 檢查日期輸入格式是否正確
    if(!(QDate::fromString(date, "yyyy-MM-dd").isValid())) {
        QMessageBox messageBox;
        messageBox.warning(this, "警告", "請輸入合法日期");
        return false;
    }
    return true;
}

bool searchsequence::splitDate()
{
    foreach (QString Date, dateList) {
        QStringList parts = Date.split("-");
        if(parts.size() == 3) {
            yearList.append(parts[0]);
            monthList.append(parts[1]);
            dayList.append(parts[2]);
        }
        else {
            qDebug() << "日期格式錯誤";
            return false;
        }
    }
    return true;
}

void searchsequence::findClosestStartDate()
{
    QString closestDate = startDate;
    int minDifference = INT_MIN;

    foreach (const QString &date, dateList) {
        // 尋找下一筆離 startDate 最近的日期
        int difference = QDate::fromString(date, "yyyy-MM-dd").daysTo(QDate::fromString(startDate, "yyyy-MM-dd"));
        if(difference >= 0) {
            continue;
        }
        else {
            if (difference > minDifference) {
                minDifference = difference;
                closestDate = date;
            }
        }
    }
    startDate = closestDate;
}

void searchsequence::findClosestEndDate()
{
    QString closestDate = endDate;
    int minDifference = INT_MAX;

    foreach (const QString &date, dateList) {
        int difference = QDate::fromString(date, "yyyy-MM-dd").daysTo(QDate::fromString(endDate, "yyyy-MM-dd"));
        if(difference <= 0) {
            continue;
        }
        else {
            if (difference < minDifference) {
                minDifference = difference;
                closestDate = date;
            }
        }
    }
    endDate = closestDate;
}

bool checkDateSequence(QString start, QString finish)
{
    if(start == finish) {
        return true;
    }

    int difference = QDate::fromString(start, "yyyy-MM-dd").daysTo(QDate::fromString(finish, "yyyy-MM-dd"));
//    qDebug() << difference;
    if(difference <= 0) {
        return false;
    }
    return true;
}

void searchsequence::getSequenceDate()
{
    if(startDate == endDate) {
        sequenceDateList.append(startDate);
        return;
    }

    bool flag = false;
    foreach(QString date, dateList) {
        if(date == startDate) {
            sequenceDateList.append(startDate);
            flag = true;
            continue;
        }
        if(flag == true) {
            if(date == endDate) {
                sequenceDateList.append(endDate);
                return;
            }
            sequenceDateList.append(date);
        }
    }
}

void searchsequence::setTableFromDb()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isValid()) {
        qDebug() << "Database connection is not valid.";
        return;
    }

    foreach (QString date, sequenceDateList) {
        QSqlQuery query;
        query.prepare("SELECT * FROM diary WHERE date = :date");
        query.bindValue(":date", date);

        if(!query.exec()) {
            qDebug() << "Query error:" << query.lastError().text();
             return;
        }

        if(query.next()) {
            QString progress = query.value("progress").toString();
            QString ongoing = query.value("onGoing").toString();
            QString todos = query.value("ToDo").toString();
            ui->tbFinish->append(date);
            ui->tbFinish->append(progress + "\n");
            ui->tbProcessing->append(date);
            ui->tbProcessing->append(ongoing + "\n");
            ui->tbToDo->append(date);
            ui->tbToDo->append(todos + "\n");

            // 取得目前表格的行數
            int row = ui->tableWidget->rowCount();
            // 插入一新行
            ui->tableWidget->insertRow(row);
            // 將日期插入到第一列
            QPushButton *dateButton = new QPushButton(date);
            dateButton->setStyleSheet("background-color: #a6cdff;");
            dateButton->setFont(QFont("Microsoft JhengHei", 10, QFont::Bold));
            connect(dateButton, &QPushButton::clicked, [=](){
                QMessageBox messageBox;
                if(QMessageBox::Yes == messageBox.question(this, "詢問", "是否開啟編輯視窗?", QMessageBox::Yes, QMessageBox::No)) {
                    mainwindow = new MainWindow;
                    mainwindow->show();
                    emit setSearchDate(date);
                }
            });
            ui->tableWidget->setCellWidget(row, 0, dateButton);
//            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(date));

            // 將本日進度插入到第二列
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(progress));

            // 將正在進行插入到第三列
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(ongoing));

            // 將待辦事項插入到第四列
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(todos));
        }
    }
}

void searchsequence::on_pbSearch_clicked()
{
    init();

    startDate = ui->leDateStart->text();
    endDate = ui->leDateEnd->text();

    // 檢查日期格式
    if(!isDateValid(startDate) || !isDateValid(endDate)) {
        return;
    }

    // 檢查開始日期和結束日期的區間是否正確
    // Ex:start:2023-09-25，end:2023-08-30 is not valid
    if(!checkDateSequence(startDate, endDate)) {
        qDebug() << "Wrong Date Sequence";
        QMessageBox messageBox;
        messageBox.warning(this, "警告", "請確認開始日期和結束日期填寫順序正確");
        return;
    }

    // 檢查是否存在 startDate 資料，或離 startDate 最近的資料
    if(dateList.contains(startDate) == false) {
        findClosestStartDate();
    }
    if(!dateList.contains(endDate)) {
        findClosestEndDate();
    }

    qDebug() << "startDate:" << startDate;
    qDebug() << "endDate:" << endDate;

    // 取得開始和結束區間內的所有日期
    getSequenceDate();
    qDebug() << "sequenceDate:" << sequenceDateList;
    if(sequenceDateList.isEmpty()) {
        QMessageBox MessageBox;
        MessageBox.information(this, "提示", "查無資料");
        return;
    }

    // 從資料庫獲取資料
    setTableFromDb();
}
