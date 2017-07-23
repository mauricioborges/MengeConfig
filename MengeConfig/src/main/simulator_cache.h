#pragma once

/**
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
    class SimulatorInterface;
  }  // namespace Agents
}  // namespace Menge

/** The instantaneous state of the simulator. */
class SimulatorState {
public:
  /** Constructor.
   @param agent_count   The number of agents in the state. */
  SimulatorState( const Menge::Agents::SimulatorInterface* simulator );

  /** Returns the agent position for the agent with the given identifier. */
  Menge::Math::Vector3 get_agent_position( const Menge::Agents::BaseAgent* agent ) const;

private:
  friend class SimulationPlayer;

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
public:
  /** Constructor. */
  SimulatorCache() = default;

  /** Adds a state to the cache based on the simulator's current configuration. 
   Returns the index of the added state. */
  size_t AddState( const Menge::Agents::SimulatorInterface* simulator );

  /** Returns the state associated with the given index. */
  const SimulatorState& get_state( size_t index ) const;

  /** Reports the number of stored states. */
  size_t size() const { return _states.size(); }

  /** Clears the cache. */
  void Clear() { _states.clear(); }

private:
  // The sequence of states stored in the order that AddState() was invoked.
  std::vector<SimulatorState> _states;
};