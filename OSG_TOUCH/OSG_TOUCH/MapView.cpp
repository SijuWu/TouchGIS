#include "StdAfx.h"
#include "MapView.h"


MapView::MapView(void)
{
}

MapView::~MapView(void)
{
}

void MapView::setMap(Map* map)
{
	this->map=map;
	
}

vector<PointView*> MapView::getPointViews()
{
	return this->pointViews;
}

void MapView::setPointViews(vector<PointView*> pointViews)
{
	this->pointViews=pointViews;
}

vector<LineStringView*> MapView::getLineStringViews()
{
	return this->lineStringViews;
}

void MapView::setLineStringView(vector<LineStringView*> lineStringViews)
{
	this->lineStringViews=lineStringViews;
}

vector<LinearRingView*> MapView::getLinearRingViews()
{
	return this->linearRingViews;
}

void MapView::setLinearRingView(vector<LinearRingView*> linearRingViews)
{
	this->linearRingViews=linearRingViews;
}

vector<PolygonView*> MapView::getPolygonViews()
{
	return this->PolygonViews;
}


void MapView::setPolygonView(vector<PolygonView*> PolygonViews)
{
	this->PolygonViews=PolygonViews;
}

osg::ref_ptr<osg::Geode>MapView::getMapNode(void)
{
	return this->mapNode;
}

void MapView::setMapNode(osg::ref_ptr<osg::Geode> mapNode)
{
	this->mapNode=mapNode;
}

vector<MultiPolygonView*> MapView::getMultiPolygonViews()
{
	return this->multiPolygonViews;
}
void MapView::setMultiPolygonView(vector<MultiPolygonView*> multiPolygonViews)
{
	this->multiPolygonViews=multiPolygonViews;
}

vector<MultiLineStringView*> MapView::getMultiLineStringViews()
{
	return this->multiLineStringViews;
}
void MapView::setMultiLineStringView(vector<MultiLineStringView*> multiLineStringViews)
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