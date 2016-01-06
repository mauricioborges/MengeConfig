#include "SceneViewer.hpp"
#include <QtWidgets/qToolbar.h>
#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qaction.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of SceneViewer
/////////////////////////////////////////////////////////////////////////////////////////////

SceneViewer::SceneViewer(QWidget * parent) : QWidget(parent) {
	QVBoxLayout * mainLayout = new QVBoxLayout();
	_toolBar = new QToolBar();
	QAction * action = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
	_toolBar->addAction(action);
	mainLayout->addWidget(_toolBar);
	_someChild = new QTextEdit();
	_someChild->setText(tr("This is some text"));
	mainLayout->addWidget(_someChild);

	setLayout(mainLayout);
}