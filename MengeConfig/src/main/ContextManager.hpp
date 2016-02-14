/*!
 *	@file		ContextManager.hpp
 *	@brief		This manages all contexts and serves as the single point
 *				through which contexts are accessed/activated/etc.
 */


#ifndef __CONTEXT_MANAGER_H__
#define	__CONTEXT_MANAGER_H__

#include <QtCore/qobject.h>

#include <unordered_map>

// forward declarations
class QtContext;

class ContextManager : public QObject {
	Q_OBJECT

public:
	/*!
	 *	@brief		Retrieves the singleton member.
	 *	
	 *	@returns	A pointer to the singleton instance.
	 */
	static ContextManager * instance();

	/*!
	 *	@brief		Retrieves the context associated with the given id
	 *				if it exists.
	 *
	 *	@param		id		The context identifier.
	 *	@returns	A pointer to the corresponding context, or null if none
	 *				exists.
	 */
	QtContext * getContext(size_t id);

	/*!
	 *	@brief		Registers the given context to the manager.
	 *
	 *	@param		ctx		A pointer to the context to register.
	 *	@returns	Returns the identifier for the context.  If no valid
	 *				identifier, 0 is returned.
	 */
	size_t registerContext(QtContext * ctx);

	/*!
	 *	@brief		Unregisters the given context to the manager.
	 *
	 *	@param		ctx		A pointer to the context to remove.
	 */
	void unregisterContext(QtContext * ctx);

	/*!
	 *	@brief		Activate the given context (implicitly deactivating 
	 *				the previous active context.
	 *
	 *	@param		ctx		The context to activate -- if null, there will
	 *						be no active context.
	 *	@returns	The identifier for the active context -- 0 if no
	 *				active context.
	 */
	size_t activate(QtContext * ctx);

signals:

	/*!
	 *	@brief		Emits a signal when a context gets activated.
	 *
	 *	@param		ctxId		The identifier of the activated context.
	 */
	void activated(size_t ctxId);

	/*!
	*	@brief		Emits a signal when a context gets deactivated.
	*
	*	@param		ctxId		The identifier of the deactivated context.
	*/
	void deactivated(size_t ctxId);

private:
	/*!
	 *	@brief		Private construtor enables static/singleton access.
	 */
	ContextManager();

	/// The singleton instance
	static ContextManager * _instance;

	/// The identifier for the next registered context.
	static size_t _nextId;

	/// The active context
	QtContext * _active;

	/// Maps a context identifier to its context.
	std::unordered_map<size_t, QtContext *>	_idToContext;
	
	/// Maps the context identifier to its identifier.
	std::unordered_map<QtContext *, size_t >	_contextToId;

};

#endif	// __CONTEXT_MANAGER_H__