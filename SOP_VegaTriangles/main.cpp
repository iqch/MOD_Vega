#include "stdafx.h"

OP(VegaTriangles)
OP(VegaTets)
OP(Fade3DTets)
OP(IGLTets)
OP(CGALMiddle)

void newSopOperator(OP_OperatorTable *table)
{
	OPTABLE(VegaTriangles);
	OPTABLE(VegaTets);
	OPTABLE(Fade3DTets);
	OPTABLE(IGLTets);
	OPTABLE(CGALMiddle);
};

#include <UT/UT_DSOVersion.h>
