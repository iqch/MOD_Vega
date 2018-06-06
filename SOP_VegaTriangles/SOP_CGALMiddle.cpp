// CGALMiddle

#include "stdafx.h"

#define CGAL_EIGEN3_ENABLED 1

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include <CGAL/extract_mean_curvature_flow_skeleton.h>
#include <CGAL/boost/graph/split_graph_into_polylines.h>

typedef CGAL::Simple_cartesian<double>	Kernel;
typedef CGAL::Polyhedron_3<Kernel>		Polyhedron;
typedef Polyhedron::HalfedgeDS		HalfedgeDS;

typedef Kernel::Point_3				Point;

typedef CGAL::Mean_curvature_flow_skeletonization<Polyhedron> Skeletonization;
typedef Skeletonization::Skeleton                             Skeleton;
typedef Skeleton::vertex_descriptor                           Skeleton_vertex;
typedef Skeleton::edge_descriptor                             Skeleton_edge;


SOP(CGALMiddle)

template <class HDS>
class Build : public CGAL::Modifier_base<HDS>
{
public:
	vector<vector<double > > P;
	vector<vector<int> > F;

	Build() {};
	void operator()(HDS& hds)
	{
		// Postcondition: hds is a valid polyhedral surface.
		CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
		B.begin_surface(P.size(), F.size(), F.size()*3);
		typedef typename HDS::Vertex   Vertex;
		typedef typename Vertex::Point Point;
		for (vector<double>& p : P) B.add_vertex(Point(p[0], p[1], p[2]));

		for (vector<int>& f : F)
		{
			B.begin_facet();
			B.add_vertex_to_facet(f[0]);
			B.add_vertex_to_facet(f[1]);
			B.add_vertex_to_facet(f[2]);
			B.end_facet();
		};

		B.end_surface();
	}
};

struct get_polylines
{
	const Skeleton& skeleton;
	vector<vector<Point> > P;
	get_polylines(const Skeleton& skeleton) : skeleton(skeleton) {};
	void start_new_polyline() { vector<Point> p; P.push_back(p); };
	void add_node(Skeleton_vertex v) { P.back().push_back(skeleton[v].point); };
	void end_polyline() {};
};


OP_ERROR SOP_CGALMiddle::cookMySop(OP_Context & context)
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

	Build<HalfedgeDS> B;

	B.P.resize(input0->getNumPoints());

	GA_Offset ptoff;
	GA_FOR_ALL_PTOFF(input0, ptoff)
	{
		UT_Vector3 p = input0->getPos3(ptoff);
		B.P[ptoff].resize(3);
		B.P[ptoff][0] = p[0];
		B.P[ptoff][1] = p[1];
		B.P[ptoff][2] = p[2];
	};

	B.F.resize(input0->getNumPrimitives());

	int index = 0;
	GA_FOR_ALL_PRIMITIVES(input0, prim)
	{
		B.F[index].resize(3);
		B.F[index][0] = prim->getPointIndex(0);
		B.F[index][1] = prim->getPointIndex(1);
		B.F[index][2] = prim->getPointIndex(2);
		index++;
	};

	unlockInputs();

	Polyhedron tmesh;
	tmesh.delegate(B);

	Skeletonization::Skeleton S;

	CGAL::extract_mean_curvature_flow_skeleton(tmesh, S);

	get_polylines get(S);
	CGAL::split_graph_into_polylines(S, get);

	GA_RWHandleV3D ha(gdp->findPointAttribute("P"));

	for (vector<Point> p : get.P)
	{
		ptoff = gdp->appendPointBlock(p.size());

		GU_PrimPoly* PP = GU_PrimPoly::build(gdp, p.size(), 1, 0);

		for (int i = 0; i < p.size(); i++)
		{
			ha.set(ptoff + i, UT_Vector3D(p[i][0], p[i][1], p[i][2]));
			PP->setVertexPoint(i, ptoff + i);
		};
	};

	return OP_ERROR();
};

PRM_Template templateListCGALMiddle[] = {
	PRM_Template()
};

OP(CGALMiddle)
DEF_OP(CGALMiddle, cgalmddl, "CGAL::Middles", 1, 1)
