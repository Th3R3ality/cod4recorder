#pragma once
#include <vector>

#include "globaldatapointers.h"

#include "types/pmove.h"
#include "types/pml.h"


namespace simulation
{
	inline bool simulating = false;
	inline std::vector<fvec3> trail = {};
	inline std::vector<fvec3> stopTrail = {};

	/*simulation_results*//*, prediction_controller* controller*/

	void SimulateStep(pmove_t* pm, pml_t* pml);
	

	namespace test
	{
		void ForwardSteps(int steps);
	}
}