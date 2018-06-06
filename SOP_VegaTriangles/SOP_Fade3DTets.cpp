#include "stdafx.h"

// VEGA

#include <Vec3d.h>
#include <tetMeshGeo.h>
#include <tetMesh.h>

#include <tetMesher.h>
#include <objMesh.h>

// FADE3D
#include <Fade_3D.h>
using namespace FADE3D;

SOP(Fade3DTets)


OP_ERROR SOP_Fade3DTets::cookMySop(OP_Context & context)
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

	Fade_3D dt;
	//dt.insert(vInputPoints);

	vector<Point3*> vVertices;
	dt.getVertices(vVertices);

	vector<Tet3*> vTets;
	dt.getTetrahedra(vTets);

	//GA_Offset ptoff = gdp->appendPointBlock(numVertices);

	//UT_Vector3D* data = (UT_Vector3D*)vertices;

	//GA_RWHandleV3D ha(gdp->findPointAttribute("P"));
	//ha.setBlock(ptoff, numVertices, data);

	//// T
	//for (int i = 0; i < numTets; i++)
	//{
	//	GEO_PrimTetrahedron* T = GU_PrimTetrahedron::build(gdp, false);

	//	T->setVertexPoint(0, tets[i * 4 + 0]);
	//	T->setVertexPoint(1, tets[i * 4 + 1]);
	//	T->setVertexPoint(2, tets[i * 4 + 2]);
	//	T->setVertexPoint(3, tets[i * 4 + 3]);
	//};

	return OP_ERROR();
};

PRM_Template templateListFade3DTets[] = {
	PRM_Template()
};

OP(Fade3DTets)
DEF_OP(Fade3DTets, fade3dtets, "Fade3D::Get Tets", 1, 1)
