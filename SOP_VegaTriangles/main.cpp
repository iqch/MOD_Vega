#include "stdafx.h"

OP(VegaTriangles)
OP(VegaTets)

void newSopOperator(OP_OperatorTable *table)
{
	OPTABLE(VegaTriangles)
	OPTABLE(VegaTets)
};

#include <UT/UT_DSOVersion.h>
