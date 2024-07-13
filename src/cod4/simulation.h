#pragma once
#include "globaldatapointers.h"

#include "types/pmove.h"
#include "types/pml.h"


namespace simulation
{
	inline bool simulating = false;

	/*simulation_results*//*, prediction_controller* controller*/

	void SimulateStep(pmove_t* pm, pml_t* pml);

	namespace test
	{
		void Forward3Steps();
	}
}