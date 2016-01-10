#include "BaseLogger.h"

///////////////////////////////////////////////////////////////////////////////
//                    Implementation of DummyLogger
///////////////////////////////////////////////////////////////////////////////

/*!
*	@brief		The dummy logger is a valid implementaiton of the MotionLogger
*				which simply swallows all messages.  There is no record.
*/
class DummyLogger : public BaseLogger {
public:
	/*!
	*	@brief		Submits an informational event to the logger.
	*
	*	The information has benign impact.  Standard operating events should be
	*	reported as information.
	*
	*	@param		msg		The informational message.
	*/
	virtual void info(const std::string & msg) {}

	/*!
	*	@brief		Submits an warning event to the logger.
	*
	*	The warning message is intended to be used when program state is other than
	*	what is expected but the program can proceed in execution.
	*
	*	@param		msg		The warning message.
	*/
	virtual void warning(const std::string & msg) {}

	/*!
	*	@brief		Submits an error event to the logger.
	*
	*	The error message represents cessation of normal operation.  An error has been
	*	encountered which actively interferes with the normal execution.
	*
	*	@param		msg		The error message.
	*/
	virtual void error(const std::string & msg) {}
};

///////////////////////////////////////////////////////////////////////////////
//                    Implementation of BaseLogger
///////////////////////////////////////////////////////////////////////////////

BaseLogger * BaseLogger::_logger = new DummyLogger();

///////////////////////////////////////////////////////////////////////////////

BaseLogger * BaseLogger::Singleton() {
	return _logger;
}

///////////////////////////////////////////////////////////////////////////////

void BaseLogger::setLogger(BaseLogger * logger) {
	if (logger == 0x0) {
		if (dynamic_cast< DummyLogger * >(_logger) == 0x0) {
			delete _logger;
			_logger = new DummyLogger();
		}
	}
	else if (logger != _logger) {
		delete _logger;
		_logger = logger;
	}
}

///////////////////////////////////////////////////////////////////////////////


