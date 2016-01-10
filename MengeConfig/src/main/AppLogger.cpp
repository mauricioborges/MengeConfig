#include "AppLogger.hpp"

#include <QtCore/qdatetime.h>
#include <QtWidgets/Qtextedit.h>
#include <QtWidgets/qboxlayout.h>

///////////////////////////////////////////////////////////////////////////////
//                    Implementation of AppLogger
///////////////////////////////////////////////////////////////////////////////

AppLogger::AppLogger(QWidget * parent) : QWidget(parent) {
	//setWindowTitle(tr("Motion Log"));
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
		_editor->append(QString("%1: %2\n").arg(now.toString("dd.MM.yyyy hh:mm:ss.zzz"), QString(msg.substr(0, msg.size() - 1).c_str())));
	}
	else {
		_editor->append(QString("%1: %2").arg(now.toString("dd.MM.yyyy hh:mm:ss.zzz"), QString(msg.c_str())));
	}
}

