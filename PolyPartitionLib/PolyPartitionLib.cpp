// PolyPartitionLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <list>
#include <vector>
#include <cstdint>

struct TVec2 {
	float x;
	float y;
};

struct Contour {
    TVec2* points;
    int32_t numPoints;
    int32_t hole;
};

typedef std::list<TPPLPoly> PolyList;

typedef void(*OnNewConvex) (const TPPLPoint* Convex, int ConvexLen, void* UserData);

TPPLPoly PolyFromContour(const Contour& Contour) {
    TPPLPoly result;
    result.Init(Contour.numPoints);
    for (int i = 0; i < Contour.numPoints; i++) {
        result[i].x = tppl_float(Contour.points[i].x);
        result[i].y = tppl_float(Contour.points[i].y);
    }
    result.SetHole(Contour.hole!=0);
    result.SetOrientation(Contour.hole ? TPPL_CW : TPPL_CCW);
    return result;
}

void CallBackResults(const PolyList& OutPolygons, OnNewConvex CallBack, void* UserData) {
    for (const TPPLPoly& poly : OutPolygons) {
        CallBack(poly.GetPoints(), poly.GetNumPoints(), UserData);
    }
}

extern "C" __declspec(dllexport) void ConvexPartitionHM_withHoles_f(const Contour* Contours, int32_t CountoursCount, OnNewConvex CallBack, void* UserData) {
    PolyList result;
    PolyList inPoly;
    for (int j = 0; j < CountoursCount; j++) {
        if (Contours[j].numPoints > 2) {
            inPoly.push_back(PolyFromContour(Contours[j]));
        }
    }

    TPPLPartition pp;
    pp.ConvexPartition_HM(&inPoly, &result);

    CallBackResults(result, CallBack, UserData);
}

extern "C" __declspec(dllexport) void ConvexPartitionHM_f(TVec2* aContour, int32_t aContourLen, OnNewConvex CallBack, void* UserData) {
    Contour cntr;
    cntr.hole = 0;
    cntr.numPoints = aContourLen;
    cntr.points = aContour;

    TPPLPoly poly = PolyFromContour(cntr);

    PolyList result;
	TPPLPartition pp;
	pp.ConvexPartition_HM(&poly, &result);

    CallBackResults(result, CallBack, UserData);
}