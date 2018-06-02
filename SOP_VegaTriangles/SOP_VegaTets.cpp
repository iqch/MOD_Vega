#include "stdafx.h"

SOP(VegaTets)

OP_ERROR SOP_VegaTets::cookMySop(OP_Context & context)
{
	gdp->clearAndDestroy();
	
	return OP_ERROR();
};

PRM_Template templateListVegaTets[] = {
	PRM_Template()
};

OP(VegaTets)

DEF_OP(VegaTets, vegtetr, "Vega::Get Tetras", 1, 1);

