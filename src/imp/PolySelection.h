#pragma once

#include <QPoint>
#include <QVector>
#include <QPolygon>
#include <QMenu>
#include <deque>
#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>

#include <iostream>

class Logger{
public:
	static void logout(const char* _msg)
	{
#ifdef _DEBUG
		std::cout<<_msg<<std::endl;
#endif
	}
};

namespace JuHistory
{
	class Action;
	class State;
	class Manager;

	typedef Action* ActionPtr;


	class Action
	{
	public:

		std::string name;
		virtual bool doAct(){ return true;}
		ActionPtr clone(){ return new Action(*this);}
	};

	class State
	{
	};

	class Manager
	{
	public:

		bool  doAction(ActionPtr _act)
		{
			valid_hist_.push_back(_act);
			invalid_hist_.clear();
			return _act->doAct();
		}

		ActionPtr undo()
		{
			invalid_hist_.push_front(valid_hist_.back());
			valid_hist_.pop_back();
			return invalid_hist_.front();
		}

		//redo, recycle hist.
		bool redo()
		{
			ActionPtr act = invalid_hist_.front();
			valid_hist_.push_back(act);
			invalid_hist_.pop_front();
			return 	act->doAct();
		}

		//do it again
		bool doAgain()
		{
			ActionPtr act = valid_hist_.back()->clone();
			return doAction(act);
		}

		std::deque<ActionPtr> valid_hist_;
		std::deque<ActionPtr> invalid_hist_;
	};
}

class PolygonNotClosable:public std::exception{
public:
 virtual const char* what() const throw()
  {
    return "too few vertex for the current \"polygon\" to close";
  }
};


template <typename T>
class ClosePolygon
{
public:
	void addPoint(const T& _pt)
	{
		pts_.push_back(_pt);
	}

	void closePolygon()
	{
		if (pts_.size() <3) throw PolygonNotClosable();
		close_ = true;
	}

	void cancle()
	{
		pts_.clear();
	}

	bool closed_;
	std::vector<T> pts_;

	std::vector<T> get_list(){
		vector::vector<T> pts = pts_;
		if (closed_) pts.push_back(pts_[0]);
		return pts;
	}
};

class PolySelectionWidget : public QGraphicsScene{
	Q_OBJECT

public:
	PolySelectionWidget(QMenu *itemMenu=0, QObject *parent=0)
	{
	};

protected:
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
	{
		if (mouseEvent->modifiers() & Qt::ControlModifier)
		{
			if (mouseEvent->button() == Qt::LeftButton){
				std::cout<<mouseEvent->scenePos().x()<<' '<<mouseEvent->scenePos().y()<<std::endl;
				addPolygonCorner(mouseEvent->scenePos());
			}else if (mouseEvent->button() == Qt::RightButton){				
				std::cout<<mouseEvent->scenePos().x()<<' '<<mouseEvent->scenePos().y()<<std::endl;
				addPolygonCorner(mouseEvent->scenePos());
				closeCurrentPolygon();
				add_poly();
			}
		}
	}

public:
	void closeCurrentPolygon(){}
	void addPolygonCorner(const QPointF& _pt){
		unclosed_pts_.push_back(QPoint(_pt.x(),_pt.y()));
	}
	void add_poly()
	{
		QPolygon poly(unclosed_pts_);
		unclosed_pts_.clear();
		polys_.push_back(poly);
		Logger::logout("poly begin:");
		for (int i=0; i<poly.size(); ++i)
			std::cout<<poly.point(i).x()<<' '<<poly.point(i).y()<<std::endl;
		Logger::logout("poly end");
		addItem(new QGraphicsPolygonItem(poly));
	}

	QVector<QPoint> unclosed_pts_;
	std::vector<QPolygon> polys_;
};
