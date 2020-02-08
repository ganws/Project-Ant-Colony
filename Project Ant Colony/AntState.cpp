#include "AntState.h"

std::string printActivity(Activity target_activity)
{
	switch (target_activity)
	{
	default:
		return "UNDEFINED ACTIVITY";
	case Activity::IDLE:
		return "IDLE";
	case Activity::FORAGING:
		return "FORAGING";
	case Activity::GATHERING:
		return "GATHERING";
	case Activity::ATTACKING:
		return "ATTACKING";
	case Activity::PATROLLING:
		return "PATROLLING";
	case Activity::EGGLAYING:
		return "EGGLAYING";
	case Activity::DEAD:
		return "DEAD";
	}
}
