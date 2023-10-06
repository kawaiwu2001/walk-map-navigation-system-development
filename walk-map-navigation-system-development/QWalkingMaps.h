#pragma once

#include <QtWidgets/QMainWindow>
#include "MyView.h"
#include "MyMap.h"
#include "ui_QWalkingMaps.h"
#include<QDebug>
#include<QCompleter>
#include<QMessageBox>

class QWalkingMaps : public QMainWindow
{
    Q_OBJECT
public slots:        
    void SelectFromLocationSignal(int);   //用來處理選擇起點信號的槽
    void SelectToLocationSignal(int);     //用來處理選擇終點信號的槽
    void ShowError(int);                    //用來處理錯誤信息並輸出的槽
    void ShowSameFromOrganizations(const QString&); //用來顯示相同起點名稱的槽
    void ShowSameToOrganizations(const QString&);       //用來顯示相同終點名稱的槽
    void EnterFromOrganizations();      //用來處理輸入起點名稱的槽
    void EnterToOrganizations();    //用來處理輸入終點名稱的槽

private:
    Ui::QWalkingMapsClass ui;
    QGraphicsScene scene;   // 定义一个场景
    QLabel* FromLabel;
    QLabel* ToLabel;
    map<wstring, int> SameFromOrganizations;
    map<wstring, int> SameToOrganizations;
    int LastSelectFromLocationIndex = -1;
    int LastSelectToLocationIndex = -1;
public:
    QWalkingMaps(QWidget* parent = Q_NULLPTR);
    MyMap *map = new MyMap(scene);
    ~QWalkingMaps();
};
