#include "ObstacleContext.hpp"

#include "AppLogger.hpp"
#include "DrawPolygonContext.h"
#include "EditPolygonContext.h"
#include "LiveObstacleSet.h"
#include "ObstacleContextWidget.hpp"

#include "Agents/ObstacleSets/ExplicitObstacleSet.h"

#include <QtWidgets/qtabwidget.h>


///////////////////////////////////////////////////////////////////////////////
//                    Implementation of New polygon callback
///////////////////////////////////////////////////////////////////////////////

/*!
 *	@brief		Class implementing the PolygonCreatedCB for the DrawPolygonContext.
 *
 *	It ties the draw polygon context together with its parent context, allowing the
 *	ObstacleContext to be informed when a polygon is finished.
 */
class NewPolyCB : public PolygonCreatedCB {
public:
	/*!
	 *	@brief		Constructor.
	 *
	 *	@param		set		The LiveObstacleSet which will have polygons
	 *						appended to.
	 */
	NewPolyCB(LiveObstacleSet * set) : PolygonCreatedCB(), _set(set) {}

	/*!
	*	@brief		The callback when a polygon has been created.
	*
	*	It is the responsibility of the functor to take posession (or serve
	*	as the means of transmission) of the polygon and then report on success.
	*
	*	@param		poly		The new polygon made available.
	*	@returns	True if ownership of the polygon is taken, false if not.
	*				If false is returned, the polygon will be destroyed.
	*/
	virtual bool newPolygon(GLPolygon * poly) {
		if (_set) {
			_set->addPolygon(poly);
			return true;
		}
		return false;
	}

private:

	/*!
	 *	@brief		The cached obstacle set to add polygons to.
	 */
	LiveObstacleSet * _set;
};

///////////////////////////////////////////////////////////////////////////////
//                    Implementation of ObstacleContext
///////////////////////////////////////////////////////////////////////////////

ObstacleContext::ObstacleContext() : _obstacleSet(0x0), _operationContexts(0x0), _callback(0x0), _state(NONE), _editSet(false), _widget(0x0) {
	// draw polygon context
	_obstacleSet = new LiveObstacleSet();
	_callback = new NewPolyCB(_obstacleSet);
	DrawPolygonContext * drawContext = new DrawPolygonContext();
	drawContext->registerNewPolyCallback(_callback);
	_operationContexts.push_back(drawContext);

	// edit polygon context
	EditPolygonContext * editContext = new EditPolygonContext(_obstacleSet);
	_operationContexts.push_back(editContext);

	// TODO: Determine if I *really* need to know the type of widget here.
	_widget = new ObstacleContextWidget(this, (EditPolygonWidget*)editContext->getContextWidget(), (DrawPolygonWidget*)drawContext->getContextWidget());

	// set state
	_state = NEW_OBSTACLE;
}


///////////////////////////////////////////////////////////////////////////////

ObstacleContext::~ObstacleContext() {
	for (size_t i = 0; i < _operationContexts.size(); ++i) {
		delete _operationContexts[i];
	}
	delete _widget;
}

///////////////////////////////////////////////////////////////////////////////

void ObstacleContext::drawGL(int vWidth, int vHeight) {
	QtContext::drawGL(vWidth, vHeight);
	if (_state != NONE) {
		_operationContexts[_state]->drawGL(vWidth, vHeight);
	}
}

///////////////////////////////////////////////////////////////////////////////

Menge::SceneGraph::ContextResult ObstacleContext::handleMouse(QMouseEvent * evt, GLWidget * view) {
	Menge::SceneGraph::ContextResult result(false, false);
	if (_state != NONE) {
		result = _operationContexts[_state]->handleMouse(evt, view);
	}
	if (!result.isHandled()) {
		// The key event is accepted by default -- ignore must be called explicitly to pass it up.
		//	http://doc.qt.io/qt-5/qkeyevent.html#details
		evt->ignore();
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////

Menge::SceneGraph::ContextResult ObstacleContext::handleKeyboard(QKeyEvent * evt, GLWidget * view) {
	Menge::SceneGraph::ContextResult result(false, false);
	if (_state != NONE) {
		result = _operationContexts[_state]->handleKeyboard(evt, view);
	}
	if (!result.isHandled()) {
		Qt::KeyboardModifiers mods = evt->modifiers();
		if (evt->type() == QEvent::KeyPress) {
			if (mods == Qt::NoModifier && evt->key() == Qt::Key_1) {
				result.set(true, setPolygonDraw());
			}
			else if (mods == Qt::NoModifier && evt->key() == Qt::Key_2) {
				result.set(true, setPolygonEdit());
			}
		}
	}
	if (!result.isHandled()) {
		// The key event is accepted by default -- ignore must be called explicitly to pass it up.
		//	http://doc.qt.io/qt-5/qkeyevent.html#details
		evt->ignore();
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////

QWidget * ObstacleContext::getContextWidget() {
	return _widget;
}

///////////////////////////////////////////////////////////////////////////////

bool ObstacleContext::setPolygonDraw() {
	if (_state != NEW_OBSTACLE) {
		((ObstacleContextWidget*)_widget)->_tabWidget->setCurrentIndex(0);
		AppLogger::logStream << AppLogger::INFO_MSG << "Setting to new polygon" << AppLogger::END_MSG;
		_state = NEW_OBSTACLE;
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////

bool ObstacleContext::setPolygonEdit() {
	if (_state != EDIT_OBSTACLE) {
		((ObstacleContextWidget*)_widget)->_tabWidget->setCurrentIndex(1);
		AppLogger::logStream << AppLogger::INFO_MSG << "Setting to edit polygon" << AppLogger::END_MSG;
		_state = EDIT_OBSTACLE;
		// I need to do something with the current obstacle -- commit it or some such thing.

		return true;
	}
	return false;

}

///////////////////////////////////////////////////////////////////////////////

void ObstacleContext::startObstacleSet() {
	// TODO: Remove this deletion -- it's a band-aid until the obstacle
	//		set ownership is truly taken by someone else.
	if (_obstacleSet != 0x0) {
		delete _obstacleSet;
	}
	_obstacleSet = new LiveObstacleSet();
}

///////////////////////////////////////////////////////////////////////////////

bool ObstacleContext::hasLiveObstacleSet() {
	return _obstacleSet != 0x0;
}

///////////////////////////////////////////////////////////////////////////////

void ObstacleContext::finishObstacleSet( bool save ) {
	if (_obstacleSet != 0) {
		if (save) {
			if (_editSet == 0) {
				// save brand new obstacle set
				Menge::Agents::ExplicitObstacleSet * obstacleSet = new Menge::Agents::ExplicitObstacleSet();
				// TODO: Populate the obstacle set from the live obstacle set.
				// notify entity of new obstacle set.
			}
			else {
				// Change the set.
			}
		}
		else {
			delete _obstacleSet;
		}
		_obstacleSet = 0x0;
	}
}

///////////////////////////////////////////////////////////////////////////////

void ObstacleContext::setObstacleSet(Menge::Agents::ObstacleSet * obstacleSet) {
	// TODO: Create a new live set, and populate it from the explicit set.
	// TODO: I need to save the edited obstacle set so I can replace it in the scene data later.
	// TODO: If the input obstacle set is *not* explicit, warn that it will be converted.
}

/////////////////////////////////////////////////////////////////////////////////////////////

void ObstacleContext::draw3DGL(bool select) {
	// TODO: Write status to the view's status.
	if (!select) {
		// draw the live obstacle set.
		_obstacleSet->drawGL();
	}
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
