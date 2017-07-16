// PolyPartitionLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <list>
#include <vector>

struct TVec2 {
	float x;
	float y;
};

typedef void(*OnNewConvex) (TVec2* Convex, int ConvexLen, void* UserData);

extern "C" __declspec(dllexport) void ConvexPartitionHM(TVec2* Contour, int ContourLen, OnNewConvex CallBack, void* UserData) {
	TPPLPoly poly;

	typedef std::list<TPPLPoly> PolyResult;
	PolyResult result;

	poly.Init(ContourLen);
	for (int i = 0; i < ContourLen; i++) {
		poly[i].x = Contour[i].x;
		poly[i].y = Contour[i].y;
	}

	TPPLPartition pp;
	pp.ConvexPartition_HM(&poly, &result);

	std::vector<TVec2> out;
	for (PolyResult::iterator p = result.begin(); p != result.end(); p++) {
		for (int i = 0; i < (*p).GetNumPoints(); i++){
			TVec2 v;
			v.x = (*p)[i].x;
			v.y = (*p)[i].y;
			out.push_back(v);
		}
		CallBack(&out[0], out.size(), UserData);
		out.clear();
	}
}