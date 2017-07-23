#include "simulator_cache.h"

#include <cassert>

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/SimulatorInterface.h"

using Menge::Agents::BaseAgent;
using Menge::Math::Vector3;

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of SimulatorState
/////////////////////////////////////////////////////////////////////////////////////////////

SimulatorState::SimulatorState( const Menge::Agents::SimulatorInterface* simulator ) {
  const size_t AGT_COUNT = simulator->getNumAgents();
  _positions.reserve( AGT_COUNT );
  for ( size_t i = 0; i < AGT_COUNT; ++i ) {
    const BaseAgent* agt = simulator->getAgent( i );
    float h = simulator->getElevation( agt );
    _positions.emplace_back( agt->_pos.x(), agt->_pos.y(), h );
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////

Vector3 SimulatorState::get_agent_position( const BaseAgent* agent ) const {
  size_t id = agent->_id;
  assert( id < _positions.size() );
  return _positions[ id ];
}

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of SimulatorCache
/////////////////////////////////////////////////////////////////////////////////////////////

size_t SimulatorCache::AddState( const Menge::Agents::SimulatorInterface* simulator ) {
  size_t index = _states.size();
  _states.emplace_back( simulator );
  return index;
}

/////////////////////////////////////////////////////////////////////////////////////////////

const SimulatorState& SimulatorCache::get_state( size_t index ) const {
  assert( index < _states.size() && "Requesting a state that has *not* been cached." );
  return _states.at( index );
}