/*!
*	@file		MCException.h
*	@brief		The base definition for exceptions in MengeConfig.
*/

#ifndef __MC_EXCEPTION_H__
#define __MC_EXCEPTION_H__

#include <string>
#include <exception>


/*!
*	@brief		Base exception class for menge config application.
*/
class MCException : public std::exception {
public:

	/*!
	*	@brief		Default constructor.
	*/
	MCException() : std::exception() {}

	/*!
	*	@brief		Constructor with message.
	*
	*	@param		s		The exception-specific message.
	*/
	MCException(const std::string & s) : std::exception(), _msg(s) {}

	/*!
	*	@brief		Destructor.
	*/
	~MCException() throw(){}

	/*!
	*	@brief		Provides the exception message.
	*
	*	@returns	A pointer to the exception message.
	*/
	virtual const char * what() const throw() {
		return _msg.c_str();
	}

	/*!
	*	@brief		The exception message.
	*/
	std::string _msg;
};

#endif // __MC_EXCEPTION_H__