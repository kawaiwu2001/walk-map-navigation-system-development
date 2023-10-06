#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <codecvt>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "LocationItem.h"

using namespace std;

typedef struct Point {
	int x = 0;
	int y = 0;
}Point;

typedef struct Organization {
	int organization_index = -1;
	struct Organization* next = NULL;
}Organization;

typedef struct Location {
	Point point;
	LocationItem* item = NULL;
	Organization* organization = NULL;
}Location;

std::wstring s2ws(const std::string& str);

std::string ws2s(const std::wstring& wstr);

class MyMap:public QObject
{
	Q_OBJECT
signals:
	void SelectFromLocationSignal(int x);
	void SelectToLocationSignal(int x);
	void ShowError(int x);
public slots:
	void DisplayLocationInfo(int x, QPointF point);
	void HideLocationInfo();
	void DisplayPath();
	void FlashingPath();
public:
	MyMap(QGraphicsScene& GS);
	~MyMap();
	int GetLocationCount();
	int GetOrganizationCount();
	Location operator[](int index);
	wstring GetOrganizationName(int index);
	int GetOrganizationIndex(int index);
	vector<int> MyMap::GetOrganizationIndexs(wstring str);
	void DijkstraPath(int* dist, int* path, int v0); //��Ӌ����̆�Դ·�� dist(��i���c��̾��x�Ĕ��M ÿ���c����̾��x) path(��̾��x) v0(���c����))
	void ShowPath(int* path, int vEnd, int vStart);
	double scale = 1;
	double radius = 6;
private:
	Location* locationList = NULL;
	string* organizationList = NULL;
	int* organizationIndexList = NULL;
	QGraphicsScene& gs;
	int** AdjMatrix = NULL;
	int locationCount = 0;
	int organizationCount = 0;
	int edgeCount = 0;
	int InitLocation(const char* filename);
	int InitOrganization(const char* filename);
	int InitLocorg(const char* filename);    //��ʼ�����c�ͽM��֮�g���P�S
	int InitEdge(const char* filename);
	QGraphicsSimpleTextItem* gst = NULL;//5s�Ҹ�����
	stack<QGraphicsLineItem*> PathLines;//�S�S��·�� �Á�Ū�W
	QTimer* timer = NULL;//�Á��W
	bool FlashingFlag = false; //�Ƿ��W
};