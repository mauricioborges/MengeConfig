#include "FSMViewer.hpp"
#include <QtWidgets/qToolbar.h>
#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/QGraphicsView.h>
#include <QtWidgets/qaction.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of FSMViewer
/////////////////////////////////////////////////////////////////////////////////////////////

FSMViewer::FSMViewer(QWidget * parent) : QWidget(parent) {
	QVBoxLayout * mainLayout = new QVBoxLayout();
	mainLayout->setMargin(0);
	_toolBar = new QToolBar();
	QAction * action = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
	_toolBar->addAction(action);
	mainLayout->addWidget(_toolBar);
	_view = new QGraphicsView();
	mainLayout->addWidget(_view);

	setLayout(mainLayout);
}