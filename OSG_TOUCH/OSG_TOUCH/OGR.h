#pragma once
#include "Map.h"
#include "MapView.h"

#include <osg/Geode>


class OGR
{
public:
	OGR(void);
	~OGR(void);
	void readMap(Map* map, MapView* mapView,char* source,bool polygonFill);
	
	void pointToDrawable(MapPoint* mapPoint,osg::ref_ptr<osg::Geode> mapNode,osg::Vec4 color);
	void lineStringToDrawable(MapLineString* mapLineString,osg::ref_ptr<osg::Geode> mapNode,osg::Vec4 color);
	void linearRingToDrawable(MapLinearRing* mapLinearRing,osg::ref_ptr<osg::Geode> mapNode);
	void polygonToDrawable(MapPolygon* mapPolygon,osg::ref_ptr<osg::Geode> mapNode,bool polygonFill,osg::Vec4 color);
	void multiLineStringToDrawable(MapMultiLineString* mapMultiLineString,osg::ref_ptr<osg::Geode> mapNode,osg::Vec4 color);
	void multiPolygonToDrawable(MapMultiPolygon* mapMultiPolygon,osg::ref_ptr<osg::Geode> mapNode,bool polygonFill,osg::Vec4 color);
	 osg::ref_ptr<osg::Geode> getMapNode(void);
private:
	vector<PointView*> pointViews;
	vector<MapPoint*> mapPoints;

	vector<LineStringView*> lineStringViews;
	vector<MapLineString*> mapLineStrings;

	vector<LinearRingView*> linearRingViews;
	vector<MapLinearRing*> mapLinearRings;

	vector<PolygonView*> polygonViews;
	vector<MapPolygon*> mapPolygons;

	vector<MultiPolygonView*> multiPolygonViews;
	vector<MapMultiPolygon*> mapMultiPolygons;
	
	vector<MultiLineStringView*> multiLineStringViews;
	vector<MapMultiLineString*> mapMultiLineStrings;
	 
};

