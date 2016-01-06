
#include "mainwindow.hpp"
#include "window.hpp"
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
//#include <QtWidgets/QMessageBox>

MainWindow::MainWindow()
{
	_window = new Window(this);
	setCentralWidget(_window);

	// Set up the main window
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuView = menuBar->addMenu(tr("&View"));
	QAction *addNew = new QAction(menuView);
    addNew->setText(tr("Log"));
	addNew->setCheckable(true);
	addNew->setChecked(true);
    menuView->addAction(addNew);
    connect(addNew, &QAction::triggered, _window, &Window::toggleLog);
    setMenuBar(menuBar);

	
}
