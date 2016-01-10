/*!
*	@file		AppLogger.h
*	@brief		The implementation of a BaseLogger for the QT application environment.
*/

#ifndef __APP_LOGGER_H__
#define __APP_LOGGER_H__

#include "BaseLogger.h"

#include <QtWidgets/qWidget.h>
#include <QtGui/qcolor.h>

// forward declaration
QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

/*!
*	@brief		An implementation of the base logger interface which stores the event
*				log in a QTextEdit object.
*/
class AppLogger : public BaseLogger, public QWidget {
public:
	/*!
	*	@brief		Constructor.
	*
	*	@param		parent		The optional parent widget.
	*/
	AppLogger(QWidget * parent = 0x0);

	/*!
	*	@brief		Clears the log.
	*/
	void clear();

	/*!
	*	@brief		Submits an informational event to the logger.
	*
	*	The information has benign impact.  Standard operating events should be
	*	reported as information.
	*
	*	@param		msg		The informational message.
	*/
	virtual void info(const std::string & msg);

	/*!
	*	@brief		Submits an warning event to the logger.
	*
	*	The warning message is intended to be used when program state is other than
	*	what is expected but the program can proceed in execution.
	*
	*	@param		msg		The warning message.
	*/
	virtual void warning(const std::string & msg);

	/*!
	*	@brief		Submits an error event to the logger.
	*
	*	The error message represents cessation of normal operation.  An error has been
	*	encountered which actively interferes with the normal execution.
	*
	*	@param		msg		The error message.
	*/
	virtual void error(const std::string & msg);

protected:

	/*!
	*	@brief		Writes the message to the logger using the current state.
	*
	*	@param		msg		The message to write.
	*/
	void message(const std::string & msg);

	/*!
	*	@brief		The text editor to which all logs will be written.
	*/
	QTextEdit * _editor;

	/*!
	*	@brief		The text color for info.
	*/
	QColor _infoColor;

	/*!
	*	@brief		The text color for warning.
	*/
	QColor _warnColor;

	/*!
	*	@brief		The text color for errors.
	*/
	QColor _errColor;

};

#endif // __APP_LOGGER_H__