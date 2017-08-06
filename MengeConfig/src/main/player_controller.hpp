#pragma once

/*!
 *  @file   player_controller.hpp
 *  @brief  Defines the widget for manipulating the simulation playback.
 */

#include <QtWidgets/QFrame.h>

#include "simulator_cache.h"

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

  /*! Returns the current simulator state. */
  const SimulatorState& get_state() const;

signals:
  /*! 
   *  @brief    Signals that the state of the player has changed such that a redraw is necessary.
   */
  void needRedraw();

private:
  // Slot for toggling the play button.
  void togglePlayButton( bool checked );
  // Slot for setting the frame from the player.
  void playerSetFrame( int frame_number );
  // Slot for handling the action when a user scrubs the timeline to the given frame.
  void scrubFrame( int frame_number );
  // Slot for when the playback finishes on its own.
  void playbackFinished();
  // Slot for when the simulation player changes the source. True --> cache playback, false -->
  // simulation playback.
  void sourceChanged( bool from_cache );
  // Does the work for when time changes.
  void timeChanged( int frame );

  // The animation object responsible for advancing the simulation.
  SimulationPlayer* _player;

  // The play/pause/stop button.
  QPushButton* _play_button{ nullptr };
  // The time slider bar.
  QSlider* _time_slider{ nullptr };
  // The time label (in frames).
  QLabel* _time_label{ nullptr };
};