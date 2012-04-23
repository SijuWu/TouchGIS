#pragma once

#include "PointView.h"
#include "LineStringView.h"
#include "LinearRingView.h"
#include "PolygonView.h"
#include "MultiPolygonView.h"
#include "MultiLineStringView.h"

#include "MapLayer.h"
#include "MapPoint.h"
#include "MapLineString.h"
#include "MapLinearRing.h"
#include "MapPolygon.h"
#include "MapMultiPolygon.h"
#include "MapMultiLineString.h"

#include <osg/Geode>
#include <osg/Group>

//The view of the map
class MapLayerView
{
public:
	MapLayerView(void);
	~MapLayerView(void);
	void setMapLayer(MapLayer* mapLayer);

	vector<PointView*> getPointViews();
    void setPointViews(vector<PointView*> pointViews);

	vector<LineStringView*> getLineStringViews();
	void setLineStringView(vector<LineStringView*> lineStringViews);

	vector<LinearRingView*> getLinearRingViews();
	void setLinearRingView(vector<LinearRingView*> LinearRingViews);

	vector<PolygonView*> getPolygonViews();
	void setPolygonView(vector<PolygonView*> PolygonViews);

	vector<MultiPolygonView*> getMultiPolygonViews();
	void setMultiPolygonView(vector<MultiPolygonView*> multiPolygonViews);

	vector<MultiLineStringView*> getMultiLineStringViews();
	void setMultiLineStringView(vector<MultiLineStringView*> multiLineStringViews);

	osg::ref_ptr<osg::Geode> getMapLayerNode(void); 
	void setMapLayerNode(osg::ref_ptr<osg::Geode> mapLayerNode);

	/*osg::ref_ptr<osg::Group> getLayer(void);
	void setLayer(osg::ref_ptr<osg::Group> layer);*/
	
private:
	//The model of the map
	MapLayer* mapLayer;

	//The group of all the pointView of the map
	vector<PointView*> pointViews;

	//The group of all the lineView of the map
	vector<LineStringView*> lineStringViews;

	vector<LinearRingView*> linearRingViews;

	//The group of all the polygonView of the map
	vector<PolygonView*> PolygonViews;

	vector<MultiLineStringView*> multiLineStringViews;

	vector<MultiPolygonView*> multiPolygonViews;

	//The geode of the map
	osg::ref_ptr<osg::Geode>mapLayerNode; 
	/*osg::ref_ptr<osg::Group>layer;*/


};

