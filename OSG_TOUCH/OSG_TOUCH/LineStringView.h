#pragma once
#include "MapLineString.h"
#include <osg/Geometry>
#include <osg/LineWidth>
using namespace osg;

//The view of the line object 
class LineStringView
{
public:
	LineStringView(void);
	LineStringView(MapLineString* mapLineString);
	LineStringView(MapLineString* mapLineString,int width,osg::Vec4 color);
	
	~LineStringView(void);
	osg::ref_ptr<osg::Geometry> getGeometryLineString(void);

	osg::ref_ptr<osg::Vec4Array> getColor(void);
	void setColor(osg::Vec4 color);
	

private:
	//The model of the line object
	MapLineString* mapLineString;

	//The points of the line
	osg::ref_ptr<osg::Vec3Array> linePoints;

	//The drawArray of the line
	osg::ref_ptr<osg::DrawArrays> drawArrayLine;

	//The geometry of the line
	osg::ref_ptr<osg::Geometry>geometryLine;

	//The normal of the line
	osg::ref_ptr<osg::Vec3Array>normals;

	//The color of the line
	osg::ref_ptr<osg::Vec4Array> lineColor;

	
	
};

