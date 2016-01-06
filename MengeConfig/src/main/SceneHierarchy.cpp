#include "SceneHierarchy.hpp"
#include <QtWidgets/qToolbar.h>
#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/qtreewidget.h>
#include <QtWidgets/qaction.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of SceneHierarchy
/////////////////////////////////////////////////////////////////////////////////////////////

SceneHierarchy::SceneHierarchy(QWidget * parent) : QWidget(parent) {
	QVBoxLayout * mainLayout = new QVBoxLayout();
	_toolBar = new QToolBar();
	QAction * action = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
	_toolBar->addAction(action);
	mainLayout->addWidget(_toolBar);
	_sceneTree = new QTreeWidget();
	QList<QTreeWidgetItem *> items;
	items.append(new QTreeWidgetItem((QTreeWidget *)0x0, QStringList(tr("Scene Elements"))));
	items.append(new QTreeWidgetItem((QTreeWidget *)0x0, QStringList(tr("Behavior FSM"))));
	_sceneTree->addTopLevelItems(items);
	mainLayout->addWidget(_sceneTree);

	setLayout(mainLayout);
}