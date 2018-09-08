#include "AppLogger.hpp"

#include <QtCore/qdatetime.h>
#include <QTextEdit>
#include <QBoxLayout>



///////////////////////////////////////////////////////////////////////////////
//                    Implementation of AppLogger
///////////////////////////////////////////////////////////////////////////////

char AppLogger::INFO_MSG = 1;
char AppLogger::WARN_MSG = 2;
char AppLogger::ERROR_MSG = 3;
char AppLogger::END_MSG = 0;
std::ostream AppLogger::logStream(0x0);

///////////////////////////////////////////////////////////////////////////////


AppLogger::AppLogger(QWidget * parent) : QWidget(parent) {
	logStream.rdbuf(new LogBuffer(this));
	QVBoxLayout * lyt = new QVBoxLayout(this);
	lyt->setMargin(0);
	_editor = new QTextEdit();
	_editor->setReadOnly(true);
	_editor->setAcceptRichText(true);

	_infoColor.setRgb(0, 0, 0);
	_warnColor.setRgb(255, 128, 0);
	_errColor.setRgb(255, 0, 0);

	lyt->addWidget(_editor);
}

///////////////////////////////////////////////////////////////////////////////

void AppLogger::clear() {
	_editor->clear();
}

///////////////////////////////////////////////////////////////////////////////

void AppLogger::info(const std::string & msg) {
	_editor->setTextColor(_infoColor);
	message(msg);
}

///////////////////////////////////////////////////////////////////////////////

void AppLogger::warning(const std::string & msg) {
	_editor->setTextColor(_warnColor);
	message(msg);
}

///////////////////////////////////////////////////////////////////////////////

void AppLogger::error(const std::string & msg) {
	_editor->setTextColor(_errColor);
	message(msg);
}

///////////////////////////////////////////////////////////////////////////////

void AppLogger::message(const std::string & msg) {
	QDateTime now(QDate::currentDate(), QTime::currentTime());
	if (msg[msg.size() - 1] == '\n') {
		_editor->append(QString("%1: %2\n").arg(now.toString("dd.MM.yyyy hh:mm:ss"), QString(msg.substr(0, msg.size() - 1).c_str())));
	}
	else {
		_editor->append(QString("%1: %2").arg(now.toString("dd.MM.yyyy hh:mm:ss"), QString(msg.c_str())));
	}
}


///////////////////////////////////////////////////////////////////////////////
//                    Implementation of LogBuffer
///////////////////////////////////////////////////////////////////////////////

LogBuffer::LogBuffer(AppLogger * logger) : _logger(logger), _currType(AppLogger::END_MSG), _ss() {

}
///////////////////////////////////////////////////////////////////////////////

int LogBuffer::overflow(int ch) {
	int result(EOF);
	if (ch == EOF) result = sync();
	else result = process(ch);
	return result;
}

///////////////////////////////////////////////////////////////////////////////

int LogBuffer::process(int ch){
	if (ch == AppLogger::INFO_MSG) {
		_currType = AppLogger::INFO_MSG;
	}
	else if (ch == AppLogger::WARN_MSG) {
		_currType = AppLogger::WARN_MSG;
	}
	else if (ch == AppLogger::ERROR_MSG) {
		_currType = AppLogger::ERROR_MSG;
	}
	else if (ch == AppLogger::END_MSG) {
		if (_currType == AppLogger::INFO_MSG) {
			_logger->info(_ss.str());
		}
		else if (_currType == AppLogger::WARN_MSG) {
			_logger->warning(_ss.str());

		}
		else if (_currType == AppLogger::ERROR_MSG) {
			_logger->error(_ss.str());

		}
		_currType = AppLogger::END_MSG;
		_ss.str("");
	}
	else {
		_ss.put((char)ch);
	}
	return ch;
}
