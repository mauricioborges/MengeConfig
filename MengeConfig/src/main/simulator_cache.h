#pragma once

/*!
 *  @file   simulator_cache.h
 *  @brief  Defines the interface between the program and Menge. All *state* of the simulator is
 *          directly queried from the cache.
 */

#include "MengeCore/Math/Vector3.h"

// forward declarations
namespace Menge {
  namespace Agents {
    class BaseAgent;
    class SimulatorInterface;
  }  // namespace Agents
}  // namespace Menge

/*! The instantaneous state of the simulator. */
class SimulatorState {
public:
  /** Constructor. Stores the provided simulator pointer; the pointer must have a lifespan greater
   than this state instance. */
  SimulatorState( const Menge::Agents::SimulatorInterface* simulator ) : _simulator( simulator ) {}

  /** Returns the agent position for the agent with the given identifier. */
  Menge::Math::Vector3 get_agent_position( const Menge::Agents::BaseAgent* agent ) const;

  void set_simulator( const Menge::Agents::SimulatorInterface* simulator ) {
    _simulator = simulator;
  }

private:
  const Menge::Agents::SimulatorInterface* _simulator;
};

/*!
 *  @brief  The embodiment of the state of the simulator. Everything that the app knows about the
 *          state of the simulation is queried from this cache. It is responsible for supporting
 *          queries on the "current" time (i.e., the state of the simulator) as well as all 
 *          previous states of the simulator (to the resolution of the time step size).
 */
class SimulatorCache {
};