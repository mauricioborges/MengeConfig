#include "simulation_player.hpp"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/SimulatorInterface.h"

using Menge::Agents::BaseAgent;
using Menge::Agents::SimulatorInterface;
using Menge::Math::Vector3;

///////////////////////////////////////////////////////////////////////////
//				IMPLEMENTATION FOR SimulationPlayer
///////////////////////////////////////////////////////////////////////////

SimulationPlayer::SimulationPlayer( QObject* parent ) : 
    QAbstractAnimation( parent ) {}

///////////////////////////////////////////////////////////////////////////

void SimulationPlayer::setSimulation( SimulatorInterface* sim ) {
  // TODO: Set the simulator and reset all the values.
  _sim = sim;
  _cache.Clear();
  _cache.AddState( _sim );
  finished_sim_ = false;
  _curr_frame = 0;
  emit frameChanged( 0 );
}

///////////////////////////////////////////////////////////////////////////

void SimulationPlayer::updateCurrentTime( int ) {
  // NOTE: Currently not using the time value because I'm not tying the behavior to a real-time
  // clock.
  if ( _sim == 0x0 ) stop();
  // TODO: Advance simulation, signal state change.
  if ( _sim->step() ) {
    ++_curr_frame;
    _cache.AddState( _sim );
    emit frameChanged( _curr_frame );
    // Signal state change.
  } else {
    // TODO: Do I need to also emit `finished()`?
    finished_sim_ = true;
    stop();
    // TODO: If I set the duration value to be equal to currentTime, then stop and finished would
    //  be automatically invoked by QT.
    emit finished();
  }
}

///////////////////////////////////////////////////////////////////////////

void SimulationPlayer::start( QAbstractAnimation::DeletionPolicy policy ) {
  if ( !finished_sim_ ) {
    QAbstractAnimation::start( policy );
  }
}

///////////////////////////////////////////////////////////////////////////

const SimulatorState& SimulationPlayer::get_state() const {
  const size_t last_state = _cache.size() - 1;
  return _cache.get_state( last_state );
}

///////////////////////////////////////////////////////////////////////////