#include "MyView.h"
#include <QMouseEvent>
MyView::MyView(QWidget*parent) : QGraphicsView(parent)
{

}

MyView::~MyView()
{

}

void MyView::mouseDoubleClickEvent(QMouseEvent* event)
{
	emit mouseDoubleClick();
}
