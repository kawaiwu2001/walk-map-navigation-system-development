#pragma once
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QCursor>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>

class LocationItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
signals:
    void DisplayLocationInfo(int x, QPointF point);
    void HideLocationInfo();
    void SelectFromLocationSignal(int x);
    void SelectToLocationSignal(int x);
public slots:
    void updateHoverTime(); //Ë¢ÐÂÊó˜ËÍ£Áô•régµÄ²Û
public:
	LocationItem(int x);
	~LocationItem();
    static int GetSelectFromLocation(){ return LocationItem::SelectFromLocation; };
    static int GetSelectToLocation() { return LocationItem::SelectToLocation; };
    static void SetSelectFromLocation(int x) { SelectFromLocation = x; };
    static void SetSelectToLocation(int x) { SelectToLocation = x; };
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
    //void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    //void keyPressEvent(QKeyEvent* event);
    //void keyReleaseEvent(QKeyEvent* event);
    //void focusInEvent(QFocusEvent* event);
    //void focusOutEvent(QFocusEvent* event);
    //void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
    //void dragLeaveEvent(QGraphicsSceneDragDropEvent* event);
    //void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
    //void dropEvent(QGraphicsSceneDragDropEvent* event);
    //inputMethodEvent(QInputMethodEvent* event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    //void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
private:
    int HoverTime = 0;
    const int index = -1;
    QTimer* timer;
    QPointF pointPos;
    static int SelectFromLocation;
    static int SelectToLocation;
};