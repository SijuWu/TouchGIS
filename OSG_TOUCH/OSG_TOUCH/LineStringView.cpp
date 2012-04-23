#include "StdAfx.h"
#include "LineStringView.h"


LineStringView::LineStringView(void)
{
}

LineStringView::LineStringView(MapLineString* mapLineString)
{
	this->mapLineString=mapLineString;

	linePoints=new osg::Vec3Array;

	for (int i=0;i<mapLineString->getNumPoints();i++)
	{
		OGRPoint *pointOfLine=new OGRPoint;
		mapLineString->getPoint(i,pointOfLine);

		linePoints->push_back(osg::Vec3(pointOfLine->getX(),0,pointOfLine->getY()));
	}
	drawArrayLine=new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,linePoints->size());
	drawArrayLine->setFirst(0);
	drawArrayLine->setCount(linePoints->size()); 

	geometryLine=new osg::Geometry;
	geometryLine->addPrimitiveSet(drawArrayLine);
	geometryLine->setVertexArray(linePoints);

	normals=new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
	geometryLine->setNormalArray(normals);
	geometryLine->setNormalBinding(osg::Geometry::BIND_OVERALL);
	lineColor=new osg::Vec4Array;
	lineColor->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	geometryLine->setColorArray(lineColor);
	geometryLine->setColorBinding(osg::Geometry::BIND_OVERALL);
}

LineStringView::LineStringView(MapLineString* mapLineString,int width,osg::Vec4 color)
{
	this->mapLineString=mapLineString;

	linePoints=new osg::Vec3Array;

	for (int i=0;i<mapLineString->getNumPoints();i++)
	{
		OGRPoint *pointOfLine=new OGRPoint;
		mapLineString->getPoint(i,pointOfLine);

		linePoints->push_back(osg::Vec3(pointOfLine->getX(),0,pointOfLine->getY()));
	}
	drawArrayLine=new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,linePoints->size());
	drawArrayLine->setFirst(0);
	drawArrayLine->setCount(linePoints->size()); 

	geometryLine=new osg::Geometry;
	geometryLine->addPrimitiveSet(drawArrayLine);
	geometryLine->setVertexArray(linePoints);

	normals=new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
	geometryLine->setNormalArray(normals);
	geometryLine->setNormalBinding(osg::Geometry::BIND_OVERALL);
	lineColor=new osg::Vec4Array;
	//lineColor->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	lineColor->push_back(color);
	geometryLine->setColorArray(lineColor);
	geometryLine->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::LineWidth* lineWidth=new osg::LineWidth;
	lineWidth->setWidth(width);
	geometryLine->getOrCreateStateSet()->setAttributeAndModes(lineWidth);
}

LineStringView::~LineStringView(void)
{
}

osg::ref_ptr<osg::Geometry>LineStringView::getGeometryLineString(void)
{
	return this->geometryLine;
}

osg::ref_ptr<osg::Vec4Array> LineStringView::getColor(void)
{
	return this->lineColor;
}
void  LineStringView::setColor(osg::Vec4 color)
{
	this->lineColor->resize(0);
	this->lineColor->push_back(color);
}