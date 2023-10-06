#include "QWalkingMaps.h"

QWalkingMaps::QWalkingMaps(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QPixmap pixmap("./bg.bmp");
    QGraphicsPixmapItem* gpitem = new QGraphicsPixmapItem(pixmap);
    gpitem->setPos(QPointF(map->radius, map->radius)); 
    scene.addItem(gpitem);
    //gpitem->setZValue(-2);
    ui.graphicsView->setScene(&scene);
    double scale = 2;
    ui.graphicsView->scale(scale, scale);
    map->scale = scale;
    ui.graphicsView->show();
    FromLabel = new QLabel(this);
    ToLabel = new QLabel(this);
    FromLabel->setText(QString::fromStdWString(L"起点:无"));
    FromLabel->setContentsMargins(QMargins(10, 0, 10, 0));
    ToLabel->setText(QString::fromStdWString(L"终点:无"));
    ToLabel->setContentsMargins(QMargins(10, 0, 10, 0));
    ui.statusBar->addPermanentWidget(FromLabel, 1);
    ui.statusBar->addPermanentWidget(ToLabel, 1);
    connect(map, SIGNAL(SelectFromLocationSignal(int)), this, SLOT(SelectFromLocationSignal(int)));
    connect(map, SIGNAL(SelectToLocationSignal(int)), this, SLOT(SelectToLocationSignal(int)));
    connect(map, SIGNAL(ShowError(int)), this, SLOT(ShowError(int)));
    connect(ui.graphicsView, SIGNAL(mouseDoubleClick()), map, SLOT(DisplayPath()));
    connect(ui.FromSearchLine, SIGNAL(textChanged(const QString&)), this, SLOT(ShowSameFromOrganizations(const QString&)));
    connect(ui.FromSearchLine, SIGNAL(SelectLocation()), this, SLOT(EnterFromOrganizations()));
    connect(ui.ToSearchLine, SIGNAL(textChanged(const QString&)), this, SLOT(ShowSameToOrganizations(const QString&)));
    connect(ui.ToSearchLine, SIGNAL(SelectLocation()), this, SLOT(EnterToOrganizations()));
}

QWalkingMaps::~QWalkingMaps()
{
    delete map;
}

void QWalkingMaps::SelectFromLocationSignal(int x) {
    if(x == -1) FromLabel->setText(QString::fromStdWString(L"起点:无"));
    else {
        wstring temp=L"";
        Organization* p = map->operator[](x).organization;
        while (p) {
            temp += map->GetOrganizationName(p->organization_index);
            if (p->next) temp += L",";
            p = p->next;
        }
        FromLabel->setText(QString::fromStdWString(L"起点:" + temp));
    }
}

void QWalkingMaps::SelectToLocationSignal(int x) {
    if (x == -1) ToLabel->setText(QString::fromStdWString(L"终点:无"));
    else {
        wstring temp = L"";
        Organization* p = map->operator[](x).organization;
        while (p) {
            temp += map->GetOrganizationName(p->organization_index);
            if (p->next) temp += L",";
            p = p->next;
        }
        ToLabel->setText(QString::fromStdWString(L"终点:" + temp));
    }
}

void QWalkingMaps::ShowError(int msg)
{
    switch (msg)
    {
    case 1:
        QMessageBox::information(this, u8"提示",QString::fromStdWString(L"起点不能为空"), QMessageBox::Ok);
        break;
    case 2:
        QMessageBox::information(this, u8"提示", QString::fromStdWString(L"终点不能为空"), QMessageBox::Ok);
        break;
    default:
        break;
    }
}

void QWalkingMaps::ShowSameFromOrganizations(const QString& str)
{
    if (str.isEmpty()) return;
    SameFromOrganizations.clear();
    vector<int> SameOrganizations;
    SameOrganizations = map->GetOrganizationIndexs(str.toStdWString());
    QStringList strList = {};
    for (vector<int>::iterator ite = SameOrganizations.begin(); ite != SameOrganizations.end(); ++ite)
    {
        wstring temp = map->GetOrganizationName(*ite);
        strList << QString::fromStdWString(temp);
        SameFromOrganizations[temp] = *ite;
    }
    //QCompleter* completer = new QCompleter(strList, this);
    //completer->setCaseSensitivity(Qt::CaseInsensitive);
    //completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    //connect(completer, SIGNAL())
    ui.FromSearchLine->setCompleter(str, strList);
}

void QWalkingMaps::ShowSameToOrganizations(const QString& str)
{
    if (str.isEmpty()) return;
    SameToOrganizations.clear();
    vector<int> SameOrganizations;
    SameOrganizations = map->GetOrganizationIndexs(str.toStdWString());
    QStringList strList = {};
    for (vector<int>::iterator ite = SameOrganizations.begin(); ite != SameOrganizations.end(); ++ite)
    {
        wstring temp = map->GetOrganizationName(*ite);
        strList << QString::fromStdWString(temp);
        SameToOrganizations[temp] = *ite;
    }
    //QCompleter* completer = new QCompleter(strList, this);
    //completer->setCaseSensitivity(Qt::CaseInsensitive);
    //completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui.ToSearchLine->setCompleter(str, strList);
}

void QWalkingMaps::EnterFromOrganizations()
{
    std::map<wstring, int>::iterator iter;
    iter = SameFromOrganizations.find(ui.FromSearchLine->text().toStdWString());
    if (iter != SameFromOrganizations.end()) {
        if (LastSelectFromLocationIndex != -1) {
            LocationItem::SetSelectFromLocation(-1);
            SelectFromLocationSignal(-1);
            LocationItem* p = map->operator[](LastSelectFromLocationIndex).item;
            QPen pen = p->pen();
            pen.setWidth(1);
            pen.setColor(Qt::white);
            p->setPen(pen);
        }
        int t = map->GetOrganizationIndex(iter->second);
        LocationItem::SetSelectFromLocation(t);
        SelectFromLocationSignal(t);
        LocationItem* p = map->operator[](t).item;
        QPen pen = p->pen();
        pen.setWidth(2);
        pen.setColor(Qt::black);
        p->setPen(pen);
        LastSelectFromLocationIndex = t;
    }
}

void QWalkingMaps::EnterToOrganizations()
{
    std::map<wstring, int>::iterator iter;
    iter = SameToOrganizations.find(ui.ToSearchLine->text().toStdWString());
    if (iter != SameToOrganizations.end()) {
        if (LastSelectToLocationIndex != -1) {
            LocationItem::SetSelectToLocation(-1);
            SelectToLocationSignal(-1);
            LocationItem* p = map->operator[](LastSelectToLocationIndex).item;
            QPen pen = p->pen();
            pen.setWidth(1);
            pen.setColor(Qt::white);
            p->setPen(pen);
        }
        int t = map->GetOrganizationIndex(iter->second);
        LocationItem::SetSelectToLocation(t);
        SelectToLocationSignal(t);
        LocationItem* p = map->operator[](t).item;
        QPen pen = p->pen();
        pen.setWidth(2);
        pen.setColor(Qt::black);
        p->setPen(pen);
        LastSelectToLocationIndex = t;
    }
}
