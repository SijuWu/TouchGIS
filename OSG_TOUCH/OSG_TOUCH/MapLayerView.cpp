#include "StdAfx.h"
#include "MapLayerView.h"


MapLayerView::MapLayerView(void)
{
}

MapLayerView::~MapLayerView(void)
{
}

void MapLayerView::setMapLayer(MapLayer* mapLayer)
{
	this->mapLayer=mapLayer;
	
}

vector<PointView*> MapLayerView::getPointViews()
{
	return this->pointViews;
}

void MapLayerView::setPointViews(vector<PointView*> pointViews)
{
	this->pointViews=pointViews;
}

vector<LineStringView*> MapLayerView::getLineStringViews()
{
	return this->lineStringViews;
}

void MapLayerView::setLineStringView(vector<LineStringView*> lineStringViews)
{
	this->lineStringViews=lineStringViews;
}

vector<LinearRingView*> MapLayerView::getLinearRingViews()
{
	return this->linearRingViews;
}

void MapLayerView::setLinearRingView(vector<LinearRingView*> linearRingViews)
{
	this->linearRingViews=linearRingViews;
}

vector<PolygonView*> MapLayerView::getPolygonViews()
{
	return this->PolygonViews;
}


void MapLayerView::setPolygonView(vector<PolygonView*> PolygonViews)
{
	this->PolygonViews=PolygonViews;
}

osg::ref_ptr<osg::Geode>MapLayerView::getMapLayerNode(void)
{
	return this->mapLayerNode;
}

void MapLayerView::setMapLayerNode(osg::ref_ptr<osg::Geode> mapLayerNode)
{
	this->mapLayerNode=mapLayerNode;
}

vector<MultiPolygonView*> MapLayerView::getMultiPolygonViews()
{
	return this->multiPolygonViews;
}
void MapLayerView::setMultiPolygonView(vector<MultiPolygonView*> multiPolygonViews)
{
	this->multiPolygonViews=multiPolygonViews;
}

vector<MultiLineStringView*> MapLayerView::getMultiLineStringViews()
{
	return this->multiLineStringViews;
}
void MapLayerView::setMultiLineStringView(vector<MultiLineStringView*> multiLineStringViews)
{
	this->multiLineStringViews=multiLineStringViews;
}

//osg::ref_ptr<osg::Group> MapView::getLayer(void)
//{
//	return this->layer;
//}
//
//void MapView::setLayer(osg::ref_ptr<osg::Group> layer)
//{
//	this->layer=layer;
//}