
#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
class QSplitter;
class QTreeView;
QT_END_NAMESPACE

class GLWidget;
class MainWindow;
class SceneViewer;
class FSMViewer;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(MainWindow *mw);

	friend class MainWindow;

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void dockUndock();

	/*!
	 * Slot for when the state of the log visibility changes.
	 */
	void toggleSceneViewer(bool state);

private:
    QSlider *createSlider();

    GLWidget *glWidget;
	SceneViewer * _sceneViewer;
	FSMViewer * _fsmViewer;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;
	QSplitter *splitter;
    QPushButton *dockBtn;
    MainWindow *mainWindow;
};

#endif
