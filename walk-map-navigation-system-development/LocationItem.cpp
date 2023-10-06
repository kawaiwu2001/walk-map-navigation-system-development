#include "LocationItem.h"

int LocationItem::SelectFromLocation = -1;
int LocationItem::SelectToLocation = -1;

LocationItem::LocationItem(int x) : QGraphicsEllipseItem(), index(x)
{
	setAcceptHoverEvents(true);
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateHoverTime()));
}

LocationItem::~LocationItem()
{
	delete timer;
}

void LocationItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::MouseButton::LeftButton) {
		if (SelectFromLocation == -1) {
			if (SelectFromLocation != this->index) {
				setFocus();
				QPen pen = this->pen();
				pen.setWidth(2);
				pen.setColor(Qt::black);
				this->setPen(pen);
				SelectFromLocation = this->index;
				emit SelectFromLocationSignal(SelectFromLocation);
			}
		}
		else if (SelectFromLocation == this->index) {
			if (SelectFromLocation != SelectToLocation) {
				QPen pen = this->pen();
				pen.setWidth(1);
				pen.setColor(Qt::white);
				this->setPen(pen);
			}
			SelectFromLocation = -1;
			emit SelectFromLocationSignal(SelectFromLocation);
		}
	}
	else if (event->button() == Qt::MouseButton::RightButton) {
		if (SelectToLocation == -1) {
			if (SelectToLocation != this->index) {
				setFocus();
				QPen pen = this->pen();
				pen.setWidth(2);
				pen.setColor(Qt::black);
				this->setPen(pen);
				SelectToLocation = this->index;
				emit SelectToLocationSignal(SelectToLocation);
			}
		}
		else if (SelectToLocation == this->index) {
			if (SelectFromLocation != SelectToLocation) {
				QPen pen = this->pen();
				pen.setWidth(1);
				pen.setColor(Qt::white);
				this->setPen(pen);
			}
			SelectToLocation = -1;
			emit SelectToLocationSignal(SelectToLocation);
		}
	}
	
	QGraphicsEllipseItem::mousePressEvent(event);
}

void LocationItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	pointPos = event->pos();
	timer->start(1000);
}

void LocationItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	timer->stop();
	HoverTime = 0;
	emit HideLocationInfo();
}

void LocationItem::updateHoverTime() {
	HoverTime += 1;
	if (HoverTime == 5) {           //Ó‹•r•rég
		timer->stop();
		emit DisplayLocationInfo(index, pointPos);
		HoverTime = 0;
	}
}