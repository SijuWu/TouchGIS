#pragma once
#include "MapPoint.h"
#include "MapLinearRing.h"
#include "MapLineString.h"
#include "MapPolygon.h"
#include "MapMultiPolygon.h"
#include "MapMultiLineString.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <ogrsf_frmts.h>
#include "LineStringView.h"
using std::vector;

//The model of map
class MapLayer
{
public:
	MapLayer(void);
	~MapLayer(void);

	void setMapPoints(vector<MapPoint*> mapPoints);
	vector<MapPoint*> getMapPoints(void);

	void setMapLineStrings(vector<MapLineString*> mapLineStrings);
	vector<MapLineString*> getMapLineStrings(void);

	void setMapLinearRings(vector<MapLinearRing*> mapLinearRings);
	vector<MapLinearRing*> getMapLinearRings(void);

	void setMapPolygons(vector<MapPolygon*> mapPolygons);
	vector<MapPolygon*> getMapPolygons(void);

	void setMapMultiLineStrings(vector<MapMultiLineString*> mapMultiLineStrings);
	vector<MapMultiLineString*> getMapMultiLineStrings(void);

	void setMapMultiPolygons(vector<MapMultiPolygon*> mapMultiPolygons);
	vector<MapMultiPolygon*> getMapMultiPolygons(void);

private:
	//The group of point objects 
	vector<MapPoint*> mapPoints;

	//The group of line objects
	vector<MapLineString*> mapLineStrings;
	vector<MapLinearRing*> mapLinearRings;

	//The group of polygon objects
	vector<MapPolygon*> mapPolygons;

	vector<MapMultiLineString*> mapMultiLineStrings;
	vector<MapMultiPolygon*> mapMultiPolygons;
};

