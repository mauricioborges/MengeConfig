#include "EditPolygonWidget.hpp"
#include "EditPolygonContext.h"

#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/qlabel.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of EditPolygonContext
/////////////////////////////////////////////////////////////////////////////////////////////

EditPolygonWidget::EditPolygonWidget(EditPolygonContext * context, QWidget * parent) : QWidget(parent), _context(context)  {
	QVBoxLayout * mainLayout = new QVBoxLayout();

	QLabel * label = new QLabel(tr("Edit polygon widget"));
	mainLayout->addWidget(label, Qt::AlignCenter);

	setLayout(mainLayout);
}

/////////////////////////////////////////////////////////////////////////////////////////////

