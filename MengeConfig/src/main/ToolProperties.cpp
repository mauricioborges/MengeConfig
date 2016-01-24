#include "ToolProperties.hpp"
#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/QFrame.h>
#include <QtWidgets/QLabel.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of ToolPropertyWidget
/////////////////////////////////////////////////////////////////////////////////////////////

ToolPropertyWidget::ToolPropertyWidget(QWidget * parent) : QWidget(parent) {
	_layout = new QVBoxLayout();

	_layout->setMargin(0);

	_defaultWidget = new QFrame();
	QVBoxLayout * defLayout = new QVBoxLayout();
	QLabel * label = new QLabel(tr("No tool properties available"));
	defLayout->addWidget(label, Qt::AlignCenter);
	_defaultWidget->setLayout(defLayout);

	_layout->addWidget(_defaultWidget);
	
	setLayout(_layout);
}