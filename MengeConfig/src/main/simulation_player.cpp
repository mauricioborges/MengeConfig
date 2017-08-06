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
}

///////////////////////////////////////////////////////////////////////////

void SimulationPlayer::updateCurrentTime( int ) {
  // NOTE: Currently not using the time value because I'm not tying the behavior to a real-time
  // clock.
  if ( _sim == 0x0 ) stop();

  const int last_frame = static_cast<int>( _cache.size() ) - 1;
  if ( _curr_frame == last_frame ) {
    if ( !finished_sim_ && _sim->step() ) {
      _cache.AddState( _sim );
    } else {
      finished_sim_ = true;
      stop();
      // TODO: If I set the duration value to be equal to currentTime, then stop and finished would
      //  be automatically invoked by QT.
      emit finished();
      return;
    }
  }
  // NOTE: This updates _curr_frame. setCurrentFrame cannot emit playbackChangedFrame because it
  // is also invoked from the outside.
  setCurrentFrame( _curr_frame + 1 );
  emit playbackChangedFrame( _curr_frame );
}

///////////////////////////////////////////////////////////////////////////

void SimulationPlayer::setCurrentFrame( int frame ) {
  const int last_frame = static_cast<int>( _cache.size() ) -1;
  // Values beyond the cache value should be clamped to the last cache index.
  if ( frame > last_frame ) frame = last_frame;

  // If the new frame is different from the current frame.
  if ( _curr_frame != frame ) {
    // Detect if I'm changing playback source.
    bool reading_from_cache = _curr_frame < last_frame;
    bool move_to_cache = frame < last_frame;
    if ( reading_from_cache ^ move_to_cache ) {
      emit playbackSourceChanged( move_to_cache );
    }
    _curr_frame = frame;
  }
}

///////////////////////////////////////////////////////////////////////////

const SimulatorState& SimulationPlayer::get_state() const {
  // This relies on the invariant that _curr_frame is *always* a valid index into the state.
  return _cache.get_state( _curr_frame );
}

///////////////////////////////////////////////////////////////////////////