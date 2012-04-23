#pragma once
#include "MapMultiLineString.h"
#include "LineStringView.h"
#include <osg/Geometry>
#include <vector>
#include <osgUtil/Tessellator>
#include <osg/TriangleFunctor>
using std::vector;

class MultiLineStringView
{
public:
	MultiLineStringView(MapMultiLineString* mapMultiLineString,int width,osg::Vec4 color);
	~MultiLineStringView(void);

	osg::ref_ptr<osg::Geometry> getGeometryMultiLineStrings(void);

	osg::ref_ptr<osg::Vec4Array> getColor(void);
	void setColor(osg::Vec4 color);
	
private:
	//The model of the polygon object
	MapMultiLineString* mapMultiLineString;

	//The points of the polygon
	osg::ref_ptr<osg::Vec3Array> vertices;

	//The geometry of the polygon
	osg::ref_ptr<osg::Geometry> geometryMultiLineStrings;



	//The normal of the polygon
	osg::ref_ptr<osg::Vec3Array>normals;


	osg::ref_ptr<osg::Vec4Array> multiLineStringColor;

	


};

