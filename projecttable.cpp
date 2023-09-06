#include "projecttable.h"
#include "ui_projecttable.h"

ProjectTable::ProjectTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectTable)
{
    ui->setupUi(this);
    setWindowTitle("檢視所有專案");
    searchPro = nullptr;

    // 設定表格欄位名稱
    ui->twProjectInformation->insertColumn(0);
    ui->twProjectInformation->insertColumn(1);
    ui->twProjectInformation->insertColumn(2);
    ui->twProjectInformation->insertColumn(3);
    ui->twProjectInformation->insertColumn(4);
    ui->twProjectInformation->setHorizontalHeaderItem(0, new QTableWidgetItem("專案"));
    ui->twProjectInformation->setHorizontalHeaderItem(1, new QTableWidgetItem("專案訊息"));
    ui->twProjectInformation->setHorizontalHeaderItem(2, new QTableWidgetItem("參與人員"));
    ui->twProjectInformation->setHorizontalHeaderItem(3, new QTableWidgetItem("開發/維護 記錄"));
    ui->twProjectInformation->setHorizontalHeaderItem(4, new QTableWidgetItem("待辦事項"));


    // 設定表格的列寬以填滿視窗
    ui->twProjectInformation->horizontalHeader()->setStretchLastSection(true);
    ui->twProjectInformation->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    importDataFromDb();
}

ProjectTable::~ProjectTable()
{
    delete ui;
}

void ProjectTable::importDataFromDb()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    if(!query.exec("SELECT * FROM projectDiary")) {
        qDebug() << "Query failed:" << query.lastError().text();
        db.close();
        return;
    }
    qDebug() << "Query success";
    while(query.next()) {
        QString projectName = query.value("projectName").toString();
        QString projectInformation = query.value("projectInformation").toString();
        QString participateMember = query.value("participateMember").toString();
        QString developeRecord = query.value("developeRecord").toString();
        QString todo = query.value("todo").toString();
        qDebug() << projectName;

        // 取得目前表格的行數
        int row = ui->twProjectInformation->rowCount();
        // 插入一新行
        ui->twProjectInformation->insertRow(row);
        // 將專案名稱插入到第一列
        QPushButton *projectNameButton = new QPushButton(projectName);
        projectNameButton->setStyleSheet("background-color: #a6cdff;");
        projectNameButton->setFont(QFont("Microsoft JhengHei", 12, QFont::Bold));
        connect(projectNameButton, &QPushButton::clicked, [=](){
            QMessageBox messageBox;
            if(QMessageBox::Yes == messageBox.question(this, "詢問", "是否開啟編輯視窗?", QMessageBox::Yes, QMessageBox::No)) {
                searchPro = new searchProject;
                searchPro->show();
                emit setProjectName(projectName);
            }
        });
        ui->twProjectInformation->setCellWidget(row, 0, projectNameButton);

        // 將"專案信息"插入到第二列
        ui->twProjectInformation->setItem(row, 1, new QTableWidgetItem(projectInformation));

        // 將"參與人員"插入到第三列
        ui->twProjectInformation->setItem(row, 2, new QTableWidgetItem(participateMember));

        // 將"開發/維護 記錄"插入到第四列
        ui->twProjectInformation->setItem(row, 3, new QTableWidgetItem(developeRecord));

        // 將"待辦事項"插入到第五列
        ui->twProjectInformation->setItem(row, 4, new QTableWidgetItem(todo));
    }

    db.close();
}

void ProjectTable::on_pbRefresh_clicked()
{
    ui->twProjectInformation->setRowCount(0);
    importDataFromDb();
}
