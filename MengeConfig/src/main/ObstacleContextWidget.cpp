#include "ObstacleContextWidget.hpp"
#include "DrawPolygonWidget.hpp"
#include "EditPolygonWidget.hpp"
#include "ObstacleContext.hpp"

#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/qtabwidget.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of ObstacleContextWidget
/////////////////////////////////////////////////////////////////////////////////////////////

ObstacleContextWidget::ObstacleContextWidget(ObstacleContext * context, EditPolygonWidget * editWidget, DrawPolygonWidget * drawWidget, QWidget * parent) : QWidget(parent), _context(context), _editWidget(editWidget), _drawWidget(drawWidget) {
	QVBoxLayout * mainLayout = new QVBoxLayout();
	mainLayout->setMargin(0);

	_tabWidget = new QTabWidget();

	_tabWidget->addTab(drawWidget, tr("Draw Obstacle"));
	_tabWidget->addTab(editWidget, tr("Edit Obstacle"));
	_tabWidget->setTabToolTip(0, tr("Enter Drawing Mode (1)"));
	_tabWidget->setTabToolTip(1, tr("Enter Editing Mode (2)"));
	mainLayout->addWidget(_tabWidget);

	connect(_tabWidget, &QTabWidget::tabBarClicked, this, &ObstacleContextWidget::tabChanged);

	setLayout(mainLayout);
}

/////////////////////////////////////////////////////////////////////////////////////////////

ObstacleContextWidget::~ObstacleContextWidget() {
	// Disconnect the child widgets -- the contexts should be responsible for deleting them.
	_tabWidget->removeTab(0);
	_tabWidget->removeTab(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void ObstacleContextWidget::tabChanged(int index) {
	if (index == 0) {
		_context->setPolygonDraw();
	}
	else if (index == 1) {
		_context->setPolygonEdit();
	}
}