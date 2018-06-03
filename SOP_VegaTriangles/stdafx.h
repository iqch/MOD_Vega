#pragma once


// PRE-DEINES
#ifdef LINUX
#define DLLEXPORT
#else
#define WIN32
#define DLLEXPORT __declspec(dllexport)
#endif

#define MAKING_DSO

#define SIZEOF_VOID_P 8

#define _USE_MATH_DEFINES

#define SESI_LITTLE_ENDIAN 1

#define HBOOST_ALL_NO_LIB

#define UT_DSO_TAGINFO "\"VEGA SYSTEM\""

#define STRICT
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#define _SCL_SECURE_NO_WARNINGS

// CRT
#include <vector>
//#include <sstream>

#include <map>
#include <string>

using namespace std;

// HDK

#include <GU/GU_DetailHandle.h>
#include <GU/GU_Detail.h>
#include <GU/GU_RayIntersect.h>
#include <GU/GU_PrimPoly.h>
#include <GU/GU_PrimPolySoup.h>
#include <GU/GU_PrimMesh.h>
#include <GU/GU_PrimVolume.h>
#include <GU/GU_PrimTetrahedron.h>

//#include <GEO/GEO_PrimVolume.h>
//#include <GU/GU_PrimVolume.h>

#include <OBJ/OBJ_Node.h>

#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <OP/OP_Caller.h>

#include<OP/OP_Director.h>

#include <PRM/PRM_Include.h>
#include <PRM/PRM_Template.h>
#include <PRM/PRM_SpareData.h>
#include <PRM/PRM_ChoiceList.h>

#include <SOP/SOP_Node.h>

#include <UT/UT_HashTable.h>
#include <UT/UT_Hash.h>
#include <UT/UT_IStream.h>
#include <UT/UT_PtrArray.h>
#include <UT/UT_Lock.h>

#include <UT/UT_Vector3.h>
#include <UT/UT_WorkBuffer.h>
#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_Quaternion.h>

#include <UT/UT_Ramp.h>
#include <UT/UT_Interrupt.h>
#include <UT/UT_FastRandom.h>

// VEGA

#include <Vec3d.h>
#include <tetMeshGeo.h>
#include <tetMesh.h>

#include <tetMesher.h>
#include <objMesh.h>

// OPWORKS
#define OP(op) class OP_##op : public OP_Operator \
{  \
public: \
	OP_##op(); \
	virtual ~OP_##op(); \
	static OP_Node* creator(OP_Network *, const char *, OP_Operator*); \
 };

#define DEF_OP(sop,_name,NAME,amin,amax) OP_##sop::OP_##sop() : OP_Operator(#_name, NAME, creator, templateList##sop, (unsigned)amin, amax) {}; OP_Node* OP_##sop ::creator(OP_Network *net, const char *name, OP_Operator *op) { return new SOP_##sop(net, name, op); }; OP_##sop::~OP_##sop() {};

#define OPTABLE(op) table->addOperator(new OP_##op());

#define SOP(sop) class SOP_##sop : public SOP_Node \
{ \
	static int	*myIndirect; \
public: \
	SOP_##sop(OP_Network *net, const char *name, OP_Operator *op) : SOP_Node(net, name, op) \
	{ \
		if (myIndirect == NULL) myIndirect = allocIndirect(15); \
	}; \
	virtual ~SOP_##sop(void) {}; \
	virtual OP_ERROR		 cookMySop(OP_Context &context); \
}; \
int* SOP_##sop::myIndirect = NULL;