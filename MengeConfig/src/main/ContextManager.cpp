#include "ContextManager.hpp"
#include "QtContext.h"

///////////////////////////////////////////////////////////////////////////////
//                    Implementation of ContextManager
///////////////////////////////////////////////////////////////////////////////

ContextManager * ContextManager::_instance = 0x0;
size_t ContextManager::_nextId = 1;

///////////////////////////////////////////////////////////////////////////////

ContextManager::ContextManager() : _active(0x0), _idToContext(), _contextToId() {

}

///////////////////////////////////////////////////////////////////////////////

QtContext * ContextManager::getContext(size_t id) {
	std::unordered_map<size_t, QtContext *>::const_iterator itr = _idToContext.find(id);
	if (itr != _idToContext.end()) {
		return itr->second;
	} 
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

ContextManager * ContextManager::instance() {
	if (_instance == 0x0) {
		_instance = new ContextManager();
	}
	return _instance;
}

///////////////////////////////////////////////////////////////////////////////

size_t ContextManager::registerContext(QtContext * ctx) {
	std::unordered_map<QtContext *, size_t>::const_iterator itr = _contextToId.find(ctx);
	size_t id = 0;
	if (itr == _contextToId.end()) {
		id = _nextId;
		++_nextId;
		_contextToId[ctx] = id;
		_idToContext[id] = ctx;
	}
	else {
		id = itr->second;
	}
	return id;
}

///////////////////////////////////////////////////////////////////////////////

void ContextManager::unregisterContext(QtContext * ctx) {
	std::unordered_map<QtContext *, size_t>::const_iterator itr = _contextToId.find(ctx);
	if (itr != _contextToId.end()) {
		_idToContext.erase(itr->second);
		_contextToId.erase(itr);
	}
}

///////////////////////////////////////////////////////////////////////////////

size_t ContextManager::activate(QtContext * ctx) {
	if (_active != 0x0) {
		size_t id = _contextToId[_active];
		_active->deactivate();
		emit deactivated(id);
		_active = 0x0;
	}
	size_t id = 0;
	if (ctx != 0x0) {
		std::unordered_map<QtContext *, size_t>::iterator itr = _contextToId.find(ctx);
		if (itr == _contextToId.end()) {
			return 0;
		}
		id = itr->second;
		_active = ctx;
		_active->activate();
		emit activated(id);
	}
	return id;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////


