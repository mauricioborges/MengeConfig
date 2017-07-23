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
    QAbstractAnimation( parent ), _state(0) {}

///////////////////////////////////////////////////////////////////////////

void SimulationPlayer::setSimulation( SimulatorInterface* sim ) {
  // TODO: Set the simulator and reset all the values.
  _sim = sim;
  _state.reset( _sim->getNumAgents() );
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
    updateState();
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
  return _state;
}

///////////////////////////////////////////////////////////////////////////

void SimulationPlayer::updateState() {
  // TODO: I should actually instantiate a SimulatorState from the simulator. That way the state
  // can grow, but this class doesn't have to know *how* it grows.
  for ( size_t i = 0; i < _sim->getNumAgents(); ++i ) {
    const BaseAgent* agt = _sim->getAgent( i );
    float h = _sim->getElevation( agt );
    _state._positions[i] = Vector3( agt->_pos.x(), agt->_pos.y(), h );
  }
}