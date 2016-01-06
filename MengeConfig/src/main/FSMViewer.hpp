#ifndef __FSM_VIEWER_H__
#define __FSM_VIEWER_H__

#include <QtWidgets/qwidget.h>

QT_BEGIN_NAMESPACE
class QToolBar;
class QGraphicsView;
QT_END_NAMESPACE

class FSMViewer : public QWidget {
	Q_OBJECT

public:
	/*!
	*	@brief		Constructor.
	*
	*	@param		parent		The optional parent
	*/
	FSMViewer(QWidget * parent = 0x0);

private:

	/*!
	*	@brief		The tool bar for this window.
	*/
	QToolBar * _toolBar;

	/*!
	*	@brief		A child widget.
	*/
	QGraphicsView * _view;
};

#endif	// __FSM_VIEWER_H__