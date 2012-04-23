#include "StdAfx.h"
#include "Map.h"

Map::Map(void)
{
}


Map::~Map(void)
{
}

void Map::setMapPoints(vector<MapPoint*> mapPoints)
{
	this->mapPoints=mapPoints;
}

vector<MapPoint*> Map::getMapPoints(void)
{
	return this->mapPoints;
}

void Map::setMapLineStrings(vector<MapLineString*> mapLineStrings)
{
	this->mapLineStrings=mapLineStrings;
}

vector<MapLineString*> Map::getMapLineStrings(void)
{
	return this->mapLineStrings;
}

void Map::setMapLinearRings(vector<MapLinearRing*> mapLinearRings)
{
	this->mapLinearRings=mapLinearRings;
}

vector<MapLinearRing*> Map::getMapLinearRings(void)
{
	return this->mapLinearRings;
}

void Map::setMapPolygons(vector<MapPolygon*> mapPolygons)
{
	this->mapPolygons=mapPolygons;
}

vector<MapPolygon*> Map::getMapPolygons(void)
{
	return this->mapPolygons;
}

void Map::setMapMultiLineStrings(vector<MapMultiLineString*> mapMultiLineStrings)
{
	this->mapMultiLineStrings=mapMultiLineStrings;
}

vector<MapMultiLineString*> Map::getMapMultiLineStrings(void)
{
	return this->mapMultiLineStrings;
}



void Map::setMapMultiPolygons(vector<MapMultiPolygon*> mapMultiPolygons)
{
	this->mapMultiPolygons=mapMultiPolygons;
}

vector<MapMultiPolygon*> Map::getMapMultiPolygons(void)
{
	return this->mapMultiPolygons;
}
