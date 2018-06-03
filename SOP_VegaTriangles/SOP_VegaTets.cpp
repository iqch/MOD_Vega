#include "stdafx.h"

SOP(VegaTets)

OP_ERROR SOP_VegaTets::cookMySop(OP_Context & context)
{
	gdp->clearAndDestroy();

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

	vector<Vec3d> P;

	GA_Offset ptoff;
	GA_FOR_ALL_PTOFF(input0, ptoff)
	{
		UT_Vector3 p = input0->getPos3(ptoff);
		P.push_back(Vec3d(p[0], p[1], p[2]));
	};

	vector<Vec3i> I;
	GA_FOR_ALL_PRIMITIVES(input0, prim)
	{
		I.push_back(Vec3i(prim->getPointIndex(0), prim->getPointIndex(1), prim->getPointIndex(2)));
	};

	unlockInputs();

	ObjMesh objMesh(P,I);

	TetMesher TM;
	TetMesh* _mesh = TM.compute(&objMesh);

	if (_mesh == NULL)
	{
		addFatal(SOP_ErrorCodes::SOP_ERR_INVALID_SRC, "Input");
		return error(context);
	};

	TetMesh& mesh = *_mesh;

	// P
	int numVertices, numTets, numEleVtx;
	double * vertices;
	int * tets;
	mesh.exportMeshGeometry(&numVertices, &vertices, &numTets, &numEleVtx, &tets);

	ptoff = gdp->appendPointBlock(numVertices);

	UT_Vector3D* data = (UT_Vector3D*)vertices;

	GA_RWHandleV3D ha(gdp->findPointAttribute("P"));
	ha.setBlock(ptoff, numVertices, data);

	// T
	for (int i = 0; i < numTets; i++)
	{
		GEO_PrimTetrahedron* T = GU_PrimTetrahedron::build(gdp, false);

		T->setVertexPoint(0, tets[i * 4 + 0]);
		T->setVertexPoint(1, tets[i * 4 + 1]);
		T->setVertexPoint(2, tets[i * 4 + 2]);
		T->setVertexPoint(3, tets[i * 4 + 3]);
	};


	
	return OP_ERROR();
};

PRM_Template templateListVegaTets[] = {
	PRM_Template()
};

OP(VegaTets)

DEF_OP(VegaTets, vegtetr, "Vega::Get Tetras", 1, 1);

