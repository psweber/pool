#ifndef CUSTOMQGRAPHICSVIEW_H
#define CUSTOMQGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QDebug>
class CustomQGraphicsView : public QGraphicsView
{
private:
	int clicks_;
	
protected:
    void mousePressEvent(QMouseEvent *event)
    {
        QGraphicsView::mousePressEvent(event);
    }

public:
	CustomQGraphicsView(QWidget* w)
	:
	QGraphicsView(w), clicks_(0) {}
};

#endif
