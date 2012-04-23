#include "StdAfx.h"
#include "MapLayer.h"

MapLayer::MapLayer(void)
{
}


MapLayer::~MapLayer(void)
{
}

void MapLayer::setMapPoints(vector<MapPoint*> mapPoints)
{
	this->mapPoints=mapPoints;
}

vector<MapPoint*> MapLayer::getMapPoints(void)
{
	return this->mapPoints;
}

void MapLayer::setMapLineStrings(vector<MapLineString*> mapLineStrings)
{
	this->mapLineStrings=mapLineStrings;
}

vector<MapLineString*> MapLayer::getMapLineStrings(void)
{
	return this->mapLineStrings;
}

void MapLayer::setMapLinearRings(vector<MapLinearRing*> mapLinearRings)
{
	this->mapLinearRings=mapLinearRings;
}

vector<MapLinearRing*> MapLayer::getMapLinearRings(void)
{
	return this->mapLinearRings;
}

void MapLayer::setMapPolygons(vector<MapPolygon*> mapPolygons)
{
	this->mapPolygons=mapPolygons;
}

vector<MapPolygon*> MapLayer::getMapPolygons(void)
{
	return this->mapPolygons;
}

void MapLayer::setMapMultiLineStrings(vector<MapMultiLineString*> mapMultiLineStrings)
{
	this->mapMultiLineStrings=mapMultiLineStrings;
}

vector<MapMultiLineString*> MapLayer::getMapMultiLineStrings(void)
{
	return this->mapMultiLineStrings;
}



void MapLayer::setMapMultiPolygons(vector<MapMultiPolygon*> mapMultiPolygons)
{
	this->mapMultiPolygons=mapMultiPolygons;
}

vector<MapMultiPolygon*> MapLayer::getMapMultiPolygons(void)
{
	return this->mapMultiPolygons;
}
