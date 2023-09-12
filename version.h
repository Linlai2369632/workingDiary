#ifndef VERSION_H
#define VERSION_H

/*

資料庫屬性:
類型:SQLITE
#table :diary
屬性 :
1.date TEXT primary key
2.progress TEXT
3.onGoing TEXT
4.ToDo TEXT

#table :projectDiary
屬性:
1.projectName TEXT primary key
2.projectInformation TEXT
3.participateMember TEXT
4.developeRecord TEXT
5.todo TEXT

2023-08-29:
  1.介面設計。
  2.建立空的資料庫和連接到資料庫。
  3.資料庫查詢。(Primary key 為日期)
  4.資料庫儲存
2023-08-30:
  1.資料庫更新。
  2.優化查詢和儲存前的檢查機制。
  3.檢測 textEdit 內文字變動，提醒用戶有尚未儲存的變更。
  4.在關閉程序前檢查是否有尚未儲存的變更，並詢問用戶使繼續退出。
2023-08-31:
  1.ctrl + s 快捷鍵設計。
  2.使用者輸入時自動填滿。
  3.根據使用者輸入的"開始日期"和"結束日期"，檢測距離最近的日期。
  4.檢查開始和結束日期順序是否填寫正確。
2023-09-01
  1.mainwindow 在查詢前，要再檢查是否有尚未儲存的變更。
  2.若搜尋區間內開始日期和結束日期相同，會回報錯誤的情形。
  3.取得開始和結束區間內的所有日期。
  4.完成 searchsequence.ui 的設計。(保留一個tableView欄位)
  5.將資料填入 TextBrowser。
  6.將資料填入tableWidget。
2023-09-02:
  1.區間查詢:按下搜尋時，清空已經顯示的資料。
  2.區間查詢:修正當日其為同一天時，判斷日期區間錯誤的情形。
  3.檢查日期輸入格式。
  4.searchsequence.cpp。搜尋結果包括:查無資料。
  5.日期欄位LineEdit提示日期格式。
  6.searchsequence.cpp。tableWidget 日期欄位，雙擊開啟 mainwindow.ui，並自動填入日期進行搜尋。
2023-09-03:
  1.在建構函式將未檢測到 db 時自動建立新的資料庫，改為詢問後建立。若不建立，則程序關閉。
2023-09-04:
  1.mainwindow 日期設置自動填滿。
  2.修正: table 建立新資料後，應重新呼叫setAutoFilled()。
  3.searchproject 進度:
    (1)構造函數。
    (2)完成在 table 建立新資料。(button:建立)
2023-09-05:
  1.searchProject: 更新按鈕默認不可選，只有在專案名稱存在 projectNameList 後才可選。
    >建立成功後，更新按鈕設為可選。
  2.db.open 的地方補加上 db.close。
  3.searchProject: 完成查詢功能。
  4.修正錯誤: searchProject: workindDiary table 缺少 projectInformation 欄位。
  5.searchProject: 完成更新功能。
  6.searchProject: 修正建立專案時去掉頭尾空白之後的一些問題:
    >>QMessaheBox 的訊息格式。
    >>QLineEdit 的訊息格式。
  7.新增 searchProject 的 QLineEdit 题示訊息。
  8.searchProject: 快捷鍵 CTRL + S 設計。
  9.searchProject: 完成查詢功能。"檢查是否有尚未儲存的變更"。
  10.searchProject:內容變動時，Label 顏色設為紅色。
  11.maindow:將"本日進度"，"正在進行"，"待辦事項"的方框改為直立式。
  12.mainwindow:在已經查詢的情況下，將查詢 button text 改為:"重新查詢"，並更改 pbSaved enabled 的設定。
2023-09-06:
  1.searchProject: 設計 setUI2SearchMode();和 setUI2ResearchMode();來幫助使用者流暢的使用各項功能。
  2.searchProject: 取消使用 "更新" button ，偵測專案是否存來來 enabled 的方式。
  3.searchproject: "建立" 成功後，UI 介面應等同於"查詢"被按下。
  4.searchProject: 修正錯誤，pbCreate()中， messageBox("再次確認是否建立專案\n\"%1\"?")，選擇"No"後會直接切到查詢後的 UI 模式。
  5.searchProject: 設計 closeEvent();
  6.mainwindow: 調整 "搜尋" 後的介面操作，使其更加合理。
  7.workingDiary: 增加刪除功能。
  8.searchProject: 新增一個 "查看" button，和 projecttable.ui。使其顯示資料庫中所有已存在的專案資訊。並可點擊 "專案名稱" 前往該專案之編輯介面。
2023-09-12:
  1.修正錯誤:複製不同的字體格式時會保留複製時的字體。
  2.workingDiary:點擊重新搜尋後，清空介面。



正在進行:


待辦:


失敗:
-加密 db 或 table。(失敗:Qt 不支援 database 加密)。
-將 .db 上傳到雲端。需再想想有什麼辦法使存取 .db 更方便。
    (Google-Drive, FTP)

*/

#endif // VERSION_H
