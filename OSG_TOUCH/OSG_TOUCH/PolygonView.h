#pragma once
#include "MapPolygon.h"
#include <osg/Geometry>
#include <vector>
#include <osgUtil/Tessellator>
#include <osg/TriangleFunctor>
using std::vector;

//The view of the polygon
class PolygonView
{
public:
	PolygonView(MapPolygon* mapPolygon,bool polygonFill,osg::Vec4 color);
	~PolygonView(void);

	osg::ref_ptr<osg::Geometry> getGeometryPolygon(void);
	osg::ref_ptr<osg::Geometry> getGeometryEdgePolygon(void);
	osg::ref_ptr<osg::Vec3Array> getVertices(void);
	MapPolygon* getMapPolygon(void);

	osg::ref_ptr<osg::Vec4Array> getColor(void);
	void setColor(osg::Vec4 color);

private:
	//The model of the polygon object
	MapPolygon* mapPolygon;

	//The points of the polygon
	osg::ref_ptr<osg::Vec3Array> vertices;

	//The geometry of the polygon
	osg::ref_ptr<osg::Geometry> geometryPolygon;

	//The geometry of edges of the polygon
	osg::ref_ptr<osg::Geometry> geometryEdgePolygon;
	
	//The normal of the polygon
	osg::ref_ptr<osg::Vec3Array>normals;

	//The color of edges of the polygon
	osg::ref_ptr<osg::Vec4Array> polygonEdgeColor;

	//The color used to fill the polygon
	osg::ref_ptr<osg::Vec4Array> polygonFillColor;
};

