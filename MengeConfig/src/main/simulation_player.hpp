#pragma once

/*!
 *  @file   simulation_player.hpp
 *  @brief  Defines the QT animation object for managing simulation playback.
 */

#include <QtCore/QAbstractAnimation.h>

#include "simulator_cache.h"

// Forward declaration.
namespace Menge {
  namespace Agents {
    class SimulatorInterface;
  }
}

/*!
 *  @brief    The animation object for simulation playback. Responsible for advancing the
 *            simulation clock and synchronizing with the GUI. The playback in no way attempts
 *            to do *real time* playback. It will simply crank through as fast as it can.
 *            Every event trigger to evaluate will advance the simulation the simulation's
 *            time step.
 */
class SimulationPlayer : public QAbstractAnimation {
  Q_OBJECT

public:
  /*!
   *  @brief    Constructor.
   *  @param    parent    The optional parent widget.
   */
  SimulationPlayer( QObject* parent = 0x0 );

  /*!
   *  @brief    Sets the simulation for the player. The player doesn't *own* the simulator but the
   *            owner promises that the pointer will be valid for as long as the player has access
   *            to it.
   *  @param    sim   A pointer to the simulator. Setting it to null will, effectively, deactivate
   *                  the player.
   */
  void setSimulation( Menge::Agents::SimulatorInterface* sim );

  // TODO: Determine if I would be better off dynamically redefining this value to be < current
  // time after finishing. That would get `stop()` to be called automatically (as well as the
  // `finished()` signal getting emitted.
  /*!
   *  @brief    Reports a never ending animation (of no defined duration). Relies on the frame
   *            evaluation to stop the animation when the simulation has completed.
   */
  int duration() const override { return -1; }

  /*!
   *  @brief    Called when the time is changed.
   *  @param    currentTIme   The current time, in milliseconds.
   */
  void updateCurrentTime( int currentTime ) override;

  // TODO: Allow resetting the state of the simulator to the beginning and simulating anew.
  /*!
   *  @brief    Attempts to start playback. This is a no-op if the current simulation has already
   *            run its full course (either hiting the duration limit *or* reaching a final
   *            state. This renders stop/start to be similar to pause/resume in behavior.
   */
  void start( QAbstractAnimation::DeletionPolicy policy = QAbstractAnimation::KeepWhenStopped );

  /*! Returns the current simulator state. */
  const SimulatorState& get_state() const;

signals:
  /*!
   *  @brief    Reports that the current frame has changed.
   */
  void frameChanged( int frame );

private:
  // The cache of simulator state.
  SimulatorCache _cache;

  // The index of the frame that has last been processed. The simulation global time should be
  // _curr_frame * sim->getTimeStep().
  int _curr_frame{ 0 };

  // The simulator to run from.
  Menge::Agents::SimulatorInterface* _sim{ nullptr };

  // Indicator on whether the current simulation has run its course.
  bool finished_sim_{ false };
};