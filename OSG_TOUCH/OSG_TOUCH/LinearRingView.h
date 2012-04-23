#pragma once
#include "MapLinearRing.h"
#include <osg/Geometry>
#include <osg/LineWidth>
class LinearRingView
{
public:
	LinearRingView(MapLinearRing* mapLinearRing,int width);
	~LinearRingView(void);
	osg::ref_ptr<osg::Geometry> getGeometryLinearRing(void);
	private:
	//The model of the line object
	MapLinearRing* mapLinearRing;

	//The points of the line
	osg::ref_ptr<osg::Vec3Array> ringPoints;

	//The drawArray of the line
	osg::ref_ptr<osg::DrawArrays> drawArrayRing;

	//The geometry of the line
	osg::ref_ptr<osg::Geometry>geometryRing;

	//The normal of the line
	osg::ref_ptr<osg::Vec3Array>normals;

	//The color of the line
	osg::ref_ptr<osg::Vec4Array> ringColor;


};

