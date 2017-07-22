#include "simulator_cache.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/SimulatorInterface.h"

using Menge::Agents::BaseAgent;
using Menge::Agents::SimulatorInterface;
using Menge::Math::Vector3;

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of SimulatorState
/////////////////////////////////////////////////////////////////////////////////////////////

Vector3 SimulatorState::get_agent_position( const BaseAgent* agent ) const {
  float h = _simulator->getElevation( agent );
  return Vector3( agent->_pos.x(), agent->_pos.y(), h );
}