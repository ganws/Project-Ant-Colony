#include "SensoryCircle.h"

void SensoryCircle::updateCi()
{
	Ci = coeff_weight * coeff_pathing * sum_clvl_food;
}

