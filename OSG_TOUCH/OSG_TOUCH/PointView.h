#pragma once
#include "MapPoint.h"
#include <osg/Geometry>
#include <osg/Point>

//The view of point object
class PointView
{
public:
	PointView(void);
	PointView(MapPoint* mapPoint);
	PointView::PointView(MapPoint* mapPoint,int size,osg::Vec4 color);
	~PointView(void);

	//Get the geometry of the circle
	osg::ref_ptr<osg::Geometry> getGeometryPoint(void);

	osg::ref_ptr<osg::Vec4Array> getColor(void);
	void setColor(osg::Vec4 color);
private:
	//The model of point object
	MapPoint* mapPoint;

	//The points of circle which is used to represent the point object
    osg::ref_ptr<osg::Vec3Array> circlePoints;

	osg::ref_ptr<osg::Vec3Array> point;
	//The drawArray of the circle
	osg::ref_ptr<osg::DrawArrays> drawArrayCircle;

	//The geometry of the circle
	osg::ref_ptr<osg::Geometry>geometryPoint;

	//The normal of the circle
	osg::ref_ptr<osg::Vec3Array>normals;

	//The color of the circle
	osg::ref_ptr<osg::Vec4Array> pointColor;

    //The radius of the circle
	double radius;
};

