#pragma once

/*!
 *  @file   simulator_cache.h
 *  @brief  Defines the interface between the program and Menge. All *state* of the simulator is
 *          directly queried from the cache.
 */

#include "MengeCore/Math/Vector3.h"

#include <vector>

// forward declarations
namespace Menge {
  namespace Agents {
    class BaseAgent;
  }  // namespace Agents
}  // namespace Menge

/*! The instantaneous state of the simulator. */
class SimulatorState {
public:
  /** Constructor.
   @param agent_count   The number of agents in the state. */
  SimulatorState( size_t agent_count );

  /** Returns the agent position for the agent with the given identifier. */
  Menge::Math::Vector3 get_agent_position( const Menge::Agents::BaseAgent* agent ) const;

private:
  friend class SimulationPlayer;

  // Resets the state for the given agent_count;
  void reset( size_t agent_count );

  // Agent positions -- where agent id serves as index into the vector.
  std::vector<Menge::Math::Vector3> _positions;
};

/*!
 *  @brief  The embodiment of the state of the simulator. Everything that the app knows about the
 *          state of the simulation is queried from this cache. It is responsible for supporting
 *          queries on the "current" time (i.e., the state of the simulator) as well as all 
 *          previous states of the simulator (to the resolution of the time step size).
 */
class SimulatorCache {
};