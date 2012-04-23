#include "StdAfx.h"
#include "PointView.h"


PointView::PointView(void)
{
}

PointView::PointView(MapPoint* mapPoint)
{
	this->mapPoint=mapPoint;
	this->radius=0.1;
	circlePoints=new osg::Vec3Array;

	double theta=0;
	while(theta<2*osg::PI)
	{
		circlePoints->push_back(osg::Vec3(mapPoint->getX()+this->radius*cos(theta),0,mapPoint->getY()+this->radius*sin(theta)));
		theta+=osg::PI_2/4;
	}

	drawArrayCircle=new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,circlePoints->size());
	drawArrayCircle->setFirst(0);
	drawArrayCircle->setCount(circlePoints->size());

	this->geometryPoint=new osg::Geometry;
	this->geometryPoint->addPrimitiveSet(this->drawArrayCircle);
	this->geometryPoint->setVertexArray(this->circlePoints);

	this->normals=new osg::Vec3Array;
	this->normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
	this->geometryPoint->setNormalArray(this->normals);
	this->geometryPoint->setNormalBinding(osg::Geometry::BIND_OVERALL);
	this->pointColor=new osg::Vec4Array;
	this->pointColor->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	this->geometryPoint->setColorArray(this->pointColor);
	this->geometryPoint->setColorBinding(osg::Geometry::BIND_OVERALL);
	

}


PointView::PointView(MapPoint* mapPoint,int size,osg::Vec4 color)
{
	point=new osg::Vec3Array;
	point->push_back(osg::Vec3(mapPoint->getX(),0,mapPoint->getY()));
	this->geometryPoint=new osg::Geometry;
	this->geometryPoint->setVertexArray(point);
	this->geometryPoint->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,point->size()));

	this->normals=new osg::Vec3Array;
	this->normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
	this->geometryPoint->setNormalArray(this->normals);
	this->geometryPoint->setNormalBinding(osg::Geometry::BIND_OVERALL);
	this->pointColor=new osg::Vec4Array;
	//this->pointColor->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	this->pointColor->push_back(color);
	this->geometryPoint->setColorArray(this->pointColor);
	this->geometryPoint->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::Point* point=new osg::Point;
	point->setSize(size);
	this->geometryPoint->getOrCreateStateSet()->setAttributeAndModes(point);
}

osg::ref_ptr<osg::Geometry> PointView::getGeometryPoint(void)
{
	return this->geometryPoint;
}

PointView::~PointView(void)
{
}

osg::ref_ptr<osg::Vec4Array> PointView::getColor(void)
{
	return this->pointColor;
}

void  PointView::setColor(osg::Vec4 color)
{
	this->pointColor->resize(0);
	this->pointColor->push_back(color);
}