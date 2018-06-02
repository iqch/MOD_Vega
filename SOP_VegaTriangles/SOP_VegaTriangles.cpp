#include "stdafx.h"

SOP(VegaTriangles)

#define PRM_FILE_NAME "file"

OP_ERROR SOP_VegaTriangles::cookMySop(OP_Context & context)
{
	gdp->clearAndDestroy();

	UT_String path = "";
	evalString(path, PRM_FILE_NAME, 0, 0);

	if (path == "") return OP_ERROR();

	TetMesh mesh(path.c_str(), VolumetricMesh::ASCII, 0);

	// P
	int numVertices, numTets, numEleVtx;
	double * vertices;
	int * tets;
	mesh.exportMeshGeometry(&numVertices, &vertices, &numTets, &numEleVtx, &tets);

	GA_Offset ptoff = gdp->appendPointBlock(numVertices);

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

PRM_Name	 theFileName(PRM_FILE_NAME, "File Name");
PRM_Default	theFileNameDef(0, "");

PRM_Template templateListVegaTriangles[] = {
	PRM_Template(PRM_FILE_E,			1, &theFileName, &theFileNameDef),
	PRM_Template()
};

OP(VegaTriangles)
DEF_OP(VegaTriangles, vegtr2geo, "Vega::Get Triangles", 0, 0)
