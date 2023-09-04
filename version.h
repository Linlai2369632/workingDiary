#ifndef VERSION_H
#define VERSION_H

/*

資料庫屬性:
類型:SQLITE
table :diary
屬性 :
1.date TEXT primary key
2.progress TEXT
3.onGoing TEXT
4.ToDo TEXT

table :projectDiary
屬性:
1.projectName TEXT primary key
2.participateMember TEXT
3.developeRecord TEXT
4.todo TEXT

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
    (2)在 table 建立新資料。

正在進行:


待辦:
-新增一個 PprojectDiary 板塊，用來記錄不同 project 的進程和待辦事項。

失敗:
-加密 db 或 table。(失敗:Qt 不支援 database 加密)。
-將 .db 上傳到雲端。

*/

#endif // VERSION_H
