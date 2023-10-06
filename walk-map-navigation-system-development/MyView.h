#pragma once

#include <QGraphicsView>
#include <QDebug>

class MyView : public QGraphicsView
{
	Q_OBJECT
signals:
	void mouseDoubleClick();
public:
	explicit MyView(QWidget*parent = 0);
	~MyView();
protected:
	void mouseDoubleClickEvent(QMouseEvent* event);
};
