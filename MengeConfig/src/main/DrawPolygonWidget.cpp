#include "DrawPolygonWidget.hpp"
#include "DrawPolygonContext.h"

#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/qlabel.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of DrawPolygonContext
/////////////////////////////////////////////////////////////////////////////////////////////

DrawPolygonWidget::DrawPolygonWidget(DrawPolygonContext * context, QWidget * parent) : QWidget(parent), _context(context)  {
	QVBoxLayout * mainLayout = new QVBoxLayout();

	QLabel * label = new QLabel(tr("Draw polygon widget"));
	mainLayout->addWidget(label, Qt::AlignCenter);

	setLayout(mainLayout);
}

/////////////////////////////////////////////////////////////////////////////////////////////

