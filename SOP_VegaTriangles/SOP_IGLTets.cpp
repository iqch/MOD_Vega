#include "stdafx.h"

// IGL
#include <igl/copyleft/tetgen/tetrahedralize.h>
using namespace igl::copyleft::tetgen;

// Eigen
#include <Eigen/Core>
using namespace Eigen;

SOP(IGLTets)

#define PRM_CLAUSE_NAME "parameters"

OP_ERROR SOP_IGLTets::cookMySop(OP_Context & context)
{
	gdp->clearAndDestroy();

	UT_String clause = "";
	evalString(clause, PRM_CLAUSE_NAME, 0, 0);

	lockInputs(context);

	const GU_Detail* input0 = inputGeo(0);

	const GA_Primitive* prim = NULL;

	GA_FOR_ALL_PRIMITIVES(input0, prim)
	{
		if (prim->getVertexCount() != 3)
		{
			addFatal(SOP_ErrorCodes::SOP_ERR_INVALID_SRC, "Input");
			return error(context);
		};
	}

	//MatrixXd P(input0->getNumPoints(),3);

	vector<vector<REAL > > P(input0->getNumPoints());

	GA_Offset ptoff;
	GA_FOR_ALL_PTOFF(input0, ptoff)
	{
		UT_Vector3 p = input0->getPos3(ptoff);
		P[ptoff].resize(3);
		P[ptoff][0] = p[0];
		P[ptoff][1] = p[1];
		P[ptoff][2] = p[2];
	};

	vector<vector<int> > F(input0->getNumPrimitives());

	int index = 0;
	GA_FOR_ALL_PRIMITIVES(input0, prim)
	{
		F[index].resize(3);
		F[index][0]=prim->getPointIndex(0);
		F[index][1] = prim->getPointIndex(1);
		F[index][2] = prim->getPointIndex(2);
		index++;
	};

	unlockInputs();

	vector<vector<REAL > > TV;
	vector<vector<int > > TT;
	vector<vector<int> > TF;

	int res = tetrahedralize(P,F,clause.c_str(),TV,TT,TF);

	// P
	ptoff = gdp->appendPointBlock(TV.size());

	GA_RWHandleV3D ha(gdp->findPointAttribute("P"));
	index = 0;
	for (vector<REAL>& p : TV)
	{
		ha.set(ptoff+index, UT_Vector3D(p.data()));
		index++;
	};

	// T
	index = 0;
	for (vector<int> f : TT)
	{
		GEO_PrimTetrahedron* T = GU_PrimTetrahedron::build(gdp, false);

		T->setVertexPoint(0, f[0]);
		T->setVertexPoint(1, f[1]);
		T->setVertexPoint(2, f[2]);
		T->setVertexPoint(3, f[3]);

		index++;
	};

	return OP_ERROR();
};

PRM_Name	 theClauseName(PRM_CLAUSE_NAME, "Parameters");
PRM_Default	theClauseDef(0, "pq1.414Y");

PRM_Template templateListIGLTets[] = {
	PRM_Template(PRM_STRING, 1, &theClauseName, &theClauseDef),
	PRM_Template()
};

OP(IGLTets)
DEF_OP(IGLTets, igltets, "IGL::Get Tets", 1, 1)
