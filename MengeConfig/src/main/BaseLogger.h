/*!
*	@file		BaseLogger.h
*	@brief		The definition of the interface for a logging mechanism.
*/

#ifndef __BASE_LOGGER_H__
#define __BASE_LOGGER_H__

#include <string>

/*!
*	@brief		An interface describing a simple logging mechanism for the
*				motion library to report events, warnings, and errors.
*
*	Implementations of this class must ensure that they are thread-safe in
*	processing events.
*/
class BaseLogger {
public:
	/*!
	*	@brief		Retrieves the active logger.
	*
	*	@param		A pointer to a valid logger implementation.
	*/
	static BaseLogger * Singleton();

	/*!
	*	@brief		Sets the active logger.
	*
	*	@param		logger		A pointer to the logger to set as active.
	*							If logger is NULL, the active logger will be reset
	*							to a default logger.
	*/
	static void setLogger(BaseLogger * logger);

	/*!
	*	@brief		Submits an informational event to the logger.
	*
	*	The information has benign impact.  Standard operating events should be
	*	reported as information.
	*
	*	@param		msg		The informational message.
	*/
	virtual void info(const std::string & msg) = 0;

	/*!
	*	@brief		Submits an warning event to the logger.
	*
	*	The warning message is intended to be used when program state is other than
	*	what is expected but the program can proceed in execution.
	*
	*	@param		msg		The warning message.
	*/
	virtual void warning(const std::string & msg) = 0;

	/*!
	*	@brief		Submits an error event to the logger.
	*
	*	The error message represents cessation of normal operation.  An error has been
	*	encountered which actively interferes with the normal execution.
	*
	*	@param		msg		The error message.
	*/
	virtual void error(const std::string & msg) = 0;

protected:
	/*!
	*	@brief		The singleton logger instance.
	*/
	static BaseLogger * _logger;
};

#endif	 // __BASE_LOGGER_H__