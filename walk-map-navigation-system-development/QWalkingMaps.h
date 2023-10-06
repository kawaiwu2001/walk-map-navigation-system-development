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
    void SelectFromLocationSignal(int);   //�Á�̎���x�����c��̖�Ĳ�
    void SelectToLocationSignal(int);     //�Á�̎���x��K�c��̖�Ĳ�
    void ShowError(int);                    //�Á�̎���e�`��Ϣ�Kݔ���Ĳ�
    void ShowSameFromOrganizations(const QString&); //�Á��@ʾ��ͬ���c���Q�Ĳ�
    void ShowSameToOrganizations(const QString&);       //�Á��@ʾ��ͬ�K�c���Q�Ĳ�
    void EnterFromOrganizations();      //�Á�̎��ݔ�����c���Q�Ĳ�
    void EnterToOrganizations();    //�Á�̎��ݔ��K�c���Q�Ĳ�

private:
    Ui::QWalkingMapsClass ui;
    QGraphicsScene scene;   // ����һ������
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
