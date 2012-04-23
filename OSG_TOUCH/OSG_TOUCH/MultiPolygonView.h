#pragma once
#include "MapMultiPolygon.h"
#include "PolygonView.h"
#include <osg/Geometry>
#include <vector>
#include <osgUtil/Tessellator>
#include <osg/TriangleFunctor>
using std::vector;

class MultiPolygonView
{
public:
	MultiPolygonView(MapMultiPolygon* mapMultiPolygon,bool polygonFill,osg::Vec4 color);
	~MultiPolygonView(void);

	osg::ref_ptr<osg::Geometry> getGeometryPolygon(void);
	osg::ref_ptr<osg::Geometry> getGeometryEdgePolygon(void);	

	osg::ref_ptr<osg::Vec4Array> getColor(void);
	void setColor(osg::Vec4 color);

private:
	//The model of the polygon object
	MapMultiPolygon* mapMultiPolygon;

	//The points of the polygon
	osg::ref_ptr<osg::Vec3Array> vertices;

	//The geometry of the polygon
	osg::ref_ptr<osg::Geometry> geometryMultiPolygons;

	//The geometry of edges of the polygon
	osg::ref_ptr<osg::Geometry> geometryEdgeMultiPolygons;

	//The normal of the polygon
	osg::ref_ptr<osg::Vec3Array>normals;

	//The color of edges of the polygon
	osg::ref_ptr<osg::Vec4Array> polygonEdgeColor;

	//The color used to fill the polygon
	osg::ref_ptr<osg::Vec4Array> polygonFillColor;

};

