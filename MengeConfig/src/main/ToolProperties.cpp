#include "ToolProperties.hpp"

#include "ContextManager.hpp"
#include "QtContext.h"

#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/QFrame.h>
#include <QtWidgets/QLabel.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of ToolPropertyWidget
/////////////////////////////////////////////////////////////////////////////////////////////

ToolPropertyWidget::ToolPropertyWidget(QWidget * parent) : QWidget(parent), _ctxWidget(0x0),_defaultWidget(0x0), _layout(0x0) {
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

/////////////////////////////////////////////////////////////////////////////////////////////

void ToolPropertyWidget::activated(size_t ctxId) {
	QtContext * ctx = getContext(ctxId);
	if (ctx != 0x0) {
		if (_ctxWidget == 0x0) {
			_layout->removeWidget(_defaultWidget);
		}
		QWidget * widget = ctx->getContextWidget();
		if (widget != 0x0) {
			_layout->addWidget(widget);
		}
	}
	else {
		_layout->addWidget(_defaultWidget);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

void ToolPropertyWidget::deactivated(size_t ctxId) {
	QtContext * ctx = getContext(ctxId);
	if (ctx != 0x0) {
		QWidget * widget = ctx->getContextWidget();
		if (widget != 0x0 && widget == _ctxWidget) {
			_layout->removeWidget(_ctxWidget);
			_ctxWidget = 0x0;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

QtContext * ToolPropertyWidget::getContext(size_t id) {
	ContextManager * mgr = ContextManager::instance();
	return mgr->getContext(id);
}