#include "MyMap.h"

void MyMap::DijkstraPath(int* dist, int* path, int v0) {
	int i, j, k;
	bool* visited = new bool[locationCount];
	for (i = 0; i < locationCount; i++)     //初始化 
	{
		if (AdjMatrix[v0][i] > 0 && i != v0)
		{
			dist[i] = AdjMatrix[v0][i];
			path[i] = v0;     //path记录最短路径上从v0到i的前一个顶点 
		}
		else
		{
			dist[i] = INT_MAX;    //若i不与v0直接相邻，则权值置为无穷大 
			path[i] = -1;
		}
		visited[i] = false;
		path[v0] = v0;
		dist[v0] = 0;
	}
	visited[v0] = true;
	for (i = 1; i < locationCount; i++)     //循环扩展n-1次 
	{
		int min = INT_MAX;
		int u = 0;
		for (j = 0; j < locationCount; j++)    //寻找未被扩展的权值最小的顶点 
		{
			if (visited[j] == false && dist[j] < min)
			{
				min = dist[j];
				u = j;
			}
		}
		visited[u] = true;
		for (k = 0; k < locationCount; k++)   //更新dist数组的值和路径的值 
		{
			if (visited[k] == false && AdjMatrix[u][k] > 0 && min + AdjMatrix[u][k] < dist[k])
			{
				dist[k] = min + AdjMatrix[u][k];
				path[k] = u;
			}
		}
	}
	delete[] visited;
}

void MyMap::ShowPath(int* path, int vEnd, int vStart)
{
	if (timer) timer->stop();
	while (!PathLines.empty()) {
		gs.removeItem(PathLines.top());
		PathLines.pop();
	}
	stack<int> s;
	int u = vEnd;
	while (vEnd != vStart)
	{
		s.push(vEnd);
		vEnd = path[vEnd];
	}
	s.push(vEnd);
	int backp = -1;
	while (!s.empty())
	{
		qDebug() << this->GetOrganizationName(this->operator[](s.top()).organization->organization_index);
		if (backp != -1) {
			QGraphicsLineItem* gl = new QGraphicsLineItem(locationList[backp].point.x + radius, locationList[backp].point.y + radius, locationList[s.top()].point.x + radius, locationList[s.top()].point.y + radius);
			QPen pen = gl->pen();
			pen.setWidth(2);
			pen.setColor(Qt::yellow);
			gl->setPen(pen);
			gl->setZValue(1);
			gs.addItem(gl);
			PathLines.push(gl);
		}
		backp = s.top();
		s.pop();
		if (!s.empty()) qDebug() << "\\/";
	}
	timer->start(500);
}

int LongestCommonSubstring(wstring &X, wstring &Y, int m, int n) {
	// 动态规划表，大小(m+1)*(n+1)
	vector<vector<int>> table(m + 1, vector<int>(n + 1));

	for (int i = 0; i < m + 1; ++i)
	{
		for (int j = 0; j < n + 1; ++j)
		{
			// 第一行和第一列置0
			if (i == 0 || j == 0)
				table[i][j] = 0;
			else if (X[i - 1] == Y[j - 1])
				table[i][j] = table[i - 1][j - 1] + 1;
			else
				table[i][j] = max(table[i - 1][j], table[i][j - 1]);
		}
	}

	return table[m][n];
}


int GetFileLines(const char* filename)
{
	ifstream ReadFile;
	int n = 0;
	string temp;
	ReadFile.open(filename, ios::in);
	if (!ReadFile.fail())
	{
		while (getline(ReadFile, temp, '\n'))
		{
			n++;
		}
		ReadFile.close();
		return n;
	}
	return -1;
}

std::wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

int MyMap::InitLocation(const char* filename) {
	locationCount = GetFileLines(filename);
	ifstream file(filename, ios::in);
	if (!file.is_open()) {
		cout << "Error opening file";
		return 0;
	}
	locationList = new Location[locationCount];
	string temp;
	int i = 0;
	while (!file.eof())
	{
		getline(file, temp, '\n');
		if (temp == "") continue;
		int index = temp.find(',');
		locationList[i].point.x = stoi(temp.substr(0, index));
		locationList[i].point.y = stoi(temp.substr(index + 1));
		LocationItem* pItem = new LocationItem(i);
		locationList[i].item = pItem;
		// 设置画笔、画刷
		QPen pen = pItem->pen();
		pen.setWidth(1);
		pen.setColor(Qt::white);
		pItem->setPen(pen);
		pItem->setBrush(QBrush(QColor(255, 0, 0)));
		// 矩形区域 起点：(50, 50) 宽：200 高：100
		pItem->setRect(QRectF(locationList[i].point.x, locationList[i].point.y, radius*2, radius*2));
		//pItem->setStartAngle(16 * 0);  // 起始角度
		//pItem->setSpanAngle(16 * 360);  // 跨角
		connect(pItem, SIGNAL(DisplayLocationInfo(int, QPointF)), this, SLOT(DisplayLocationInfo(int, QPointF)));
		connect(pItem, SIGNAL(HideLocationInfo()), this, SLOT(HideLocationInfo()));
		connect(pItem, SIGNAL(SelectFromLocationSignal(int)), this, SIGNAL(SelectFromLocationSignal(int)));
		connect(pItem, SIGNAL(SelectToLocationSignal(int)), this, SIGNAL(SelectToLocationSignal(int)));
		pItem->setZValue(2);//把點點set第二層
		gs.addItem(pItem);
		i++;
	}
	file.close();
	return 1;
}

int MyMap::InitOrganization(const char* filename)
{
	organizationCount = GetFileLines(filename);
	ifstream file(filename, ios::in);
	if (!file.is_open()) {
		cout << "Error opening file";
		return 0;
	}
	organizationList = new string[organizationCount];
	organizationIndexList = new int[organizationCount];
	int i = 0;
	while (!file.eof())
	{
		getline(file, organizationList[i], '\n');
		organizationIndexList[i] = -1;
		i++;
	}
	file.close();
	return 1;
}

int MyMap::InitLocorg(const char* filename)
{
	int count = GetFileLines(filename);
	if (locationCount != count) {
		return 0;
	}
	ifstream file(filename, ios::in);
	if (!file.is_open()) {
		cout << "Error opening file";
		return 0;
	}
	string temp;
	for (int i = 0; i < count; i++) {
		getline(file, temp, '\n');
		if (temp == "") continue;
		int x = 0, head = 0;
		int str_len = int(temp.length());
		for (int tail = 0; tail < str_len; tail++) {
			if (temp[tail] == ',') {
				x = stoi(temp.substr(head, tail - head));
				head = tail + 1;
				Organization* p = locationList[i].organization;
				Organization* bp = p;
				if (bp == NULL) {
					locationList[i].organization = new Organization();
					locationList[i].organization->organization_index = x;
					organizationIndexList[x] = i;
					locationList[i].organization->next = NULL;
				}
				else {
					while (p) {
						bp = p;
						p = p->next;
					}
					p = new Organization();
					p->organization_index = x;
					organizationIndexList[x] = i;
					p->next = NULL;
					bp->next = p;
				}
			}
			else if (tail + 1 == str_len) {
				x = stoi(temp.substr(head));
				Organization* p = locationList[i].organization;
				Organization* bp = p;
				if (bp == NULL) {
					locationList[i].organization = new Organization();
					locationList[i].organization->organization_index = x;
					organizationIndexList[x] = i;
					locationList[i].organization->next = NULL;
				}
				else {
					while (p) {
						bp = p;
						p = p->next;
					}
					p = new Organization();
					p->organization_index = x;
					organizationIndexList[x] = i;
					p->next = NULL;
					bp->next = p;
				}
			}
		}
	}
	file.close();
	return 1;
}

int MyMap::InitEdge(const char* filename)
{
	edgeCount = GetFileLines(filename);
	ifstream file(filename, ios::in);
	if (!file.is_open()) {
		cout << "Error opening file";
		return 0;
	}
	int head, tail;
	head = tail = -1;
	string temp;
	while (!file.eof())
	{
		getline(file, temp, '\n');
		if (temp == "") continue;
		int index = temp.find(',');
		head = stoi(temp.substr(0, index));
		tail = stoi(temp.substr(index + 1));
		int dist = (int)sqrt(pow(locationList[head].point.x - locationList[tail].point.x, 2.0) + pow(locationList[head].point.y - locationList[tail].point.y, 2.0));
		AdjMatrix[head][tail] = dist;
		AdjMatrix[tail][head] = dist;
		QGraphicsLineItem* gl = new QGraphicsLineItem(locationList[head].point.x+radius, locationList[head].point.y+ radius, locationList[tail].point.x+ radius, locationList[tail].point.y+ radius);
		QPen pen = gl->pen();
		pen.setWidth(2);
		pen.setColor(Qt::black);
		gl->setPen(pen);
		gs.addItem(gl);
	}
	file.close();
	return 1;
}

MyMap::MyMap(QGraphicsScene &GS) : gs(GS)
{
	InitLocation(".\\Location.txt");
	InitOrganization(".\\Organization.txt");
	InitLocorg(".\\Locorg.txt");
	AdjMatrix = new int* [locationCount];
	for (int i = 0; i < locationCount; i++) {
		AdjMatrix[i] = new int[locationCount];
		for (int j = 0; j < locationCount; j++) {
			AdjMatrix[i][j] = 0;
		}
	}
	InitEdge(".\\Edge.txt");
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(FlashingPath()));
}

MyMap::~MyMap()
{
	delete timer;
	for (int i = 0; i < locationCount; i++) {
		Organization* p = locationList[i].organization, * np;
		while (p) {
			np = p->next;
			delete p;
			p = np;
		}
	}
	for (int i = 0; i < locationCount; i++) {
		delete locationList[i].item;
		delete[] AdjMatrix[i];
	}
	delete[] AdjMatrix;
	delete[] locationList;
	delete[] organizationList;
	delete[] organizationIndexList;
}

int MyMap::GetLocationCount()
{
	return locationCount;
}

int MyMap::GetOrganizationCount()
{
	return organizationCount;
}

Location MyMap::operator[](int index)
{
	return locationList[index];
}

wstring MyMap::GetOrganizationName(int index)
{
	return s2ws(organizationList[index]);
}

int MyMap::GetOrganizationIndex(int index)
{
	return organizationIndexList[index];
}


vector<int> MyMap::GetOrganizationIndexs(wstring str)
{
	vector<int>vec;
	for (int i = 0; i < organizationCount; i++) {
		wstring str2 = GetOrganizationName(i);
		int t = LongestCommonSubstring(str, str2, str.length(), str2.length());
		if (t >= str.length()) {
			vec.push_back(i);
		}
	}
	return vec;
}

void MyMap::DisplayLocationInfo(int x, QPointF point) {
	gst = new QGraphicsSimpleTextItem();
	Organization* p = this->operator[](x).organization;
	QString temp;
	while (p) {
		temp.append(QString::fromStdWString(GetOrganizationName(p->organization_index)));
		if (p->next) temp.append("\n");
		p = p->next;
	}
	gst->setText(temp);
	QFont font = gst->font();
	font.setFamily("微软雅黑");
	font.setPointSizeF(12 / scale);
	gst->setFont(font);
	gst->setPos(point);
	gs.addItem(gst);
}

void MyMap::HideLocationInfo() {
	if (gst) {
		gs.removeItem(gst);
		gst = NULL;
	}
}

void MyMap::DisplayPath()
{
	int vStart = LocationItem::GetSelectFromLocation(), vEnd = LocationItem::GetSelectToLocation();
	if (vStart == -1) {
		emit ShowError(1);
		return;
	}
	else if(vEnd == -1){
		emit ShowError(2);
		return;
	}
	int* dist = new int[locationCount];
	int* path = new int[locationCount];
	DijkstraPath(dist, path, vStart);
	ShowPath(path, vEnd, vStart);
	delete[] dist;
	delete[] path;
}

void MyMap::FlashingPath()
{
	stack<QGraphicsLineItem*> temp(PathLines);
	while (!temp.empty()) {
		QGraphicsLineItem* l = temp.top();
		FlashingFlag ? l->setZValue(1): l->setZValue(-1);
		temp.pop();
	}
	FlashingFlag = !FlashingFlag;
}
