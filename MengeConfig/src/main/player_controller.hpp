#pragma once

/*!
 *  @file   player_controller.hpp
 *  @brief  Defines the widget for manipulating the simulation playback.
 */

#include <QtWidgets/QFrame.h>

QT_BEGIN_NAMESPACE
class QPushButton;
class QSlider;
class QLabel;
QT_END_NAMESPACE

class SimulationPlayer;

namespace Menge {
  namespace Agents {
    class SimulatorInterface;
  }
}

/*!
 *  @brief    The playback widget. Handles playback and pausing of simulation and scrubbing through
 *            the timeline.
 */
class PlayerController : public QFrame {
  Q_OBJECT

public:
  /*! 
   *  @brief    Constructor.
   *  @param    parent    The optional parent widget.
   */
  PlayerController( QWidget* parent = 0x0 );

  /*!
   *  @brief    Sets the simulation for the player. The player doesn't *own* the simulator but the
   *            owner promises that the pointer will be valid for as long as the player has access
   *            to it.
   *  @param    sim   A pointer to the simulator. Setting it to null will, effectively, deactivate
   *                  the player.
   */
  void setSimulation( Menge::Agents::SimulatorInterface* sim );

signals:
  /*! 
   *  @brief    Signals that the state of the player has changed such that a redraw is necessary.
   */
  void needRedraw();

private:
  // Slot for toggling the play button.
  void togglePlayButton( bool checked );
  // Slot for setting the frame from external mechanisms.
  void setFrame( int frame_number );
  // Slot for scrubbing the time line.
  void scrubFrame( int frame_number );
  // Slot for when the playback finishes on its own.
  void playbackFinished();

  // The animation object responsible for advancing the simulation.
  SimulationPlayer* _player;

  // The play/pause/stop button.
  QPushButton* _play_button{ nullptr };
  // The time slider bar.
  QSlider* _time_slider{ nullptr };
  // The time label (in frames).
  QLabel* _time_label{ nullptr };
};