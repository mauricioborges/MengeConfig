
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class Window;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
	Window * _window;
//private slots:
    //void onAddNew();
};

#endif
