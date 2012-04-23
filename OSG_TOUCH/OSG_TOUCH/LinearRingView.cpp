#include "StdAfx.h"
#include "LinearRingView.h"


LinearRingView::LinearRingView(MapLinearRing* mapLinearRing,int width)
{
	this->mapLinearRing=mapLinearRing;

	ringPoints=new osg::Vec3Array;
	

	for (int i=0;i<mapLinearRing->getNumPoints();i++)
	{
		OGRPoint *pointOfLine=new OGRPoint;
		mapLinearRing->getPoint(i,pointOfLine);

		ringPoints->push_back(osg::Vec3(pointOfLine->getX(),0,pointOfLine->getY()));
		
	}
	drawArrayRing=new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP,0,ringPoints->size());
	drawArrayRing->setFirst(0);
	drawArrayRing->setCount(ringPoints->size()); 

	geometryRing=new osg::Geometry;
	geometryRing->addPrimitiveSet(drawArrayRing);
	geometryRing->setVertexArray(ringPoints);

	normals=new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
	geometryRing->setNormalArray(normals);
	geometryRing->setNormalBinding(osg::Geometry::BIND_OVERALL);
	ringColor=new osg::Vec4Array;
	ringColor->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	geometryRing->setColorArray(ringColor);
	geometryRing->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::LineWidth* lineWidth=new osg::LineWidth;
	lineWidth->setWidth(width);
	geometryRing->getOrCreateStateSet()->setAttributeAndModes(lineWidth);
}


LinearRingView::~LinearRingView(void)
{
}

osg::ref_ptr<osg::Geometry> LinearRingView::getGeometryLinearRing(void)
{
	return this->geometryRing;
}