#include "StdAfx.h"
#include "MultiLineStringView.h"


MultiLineStringView::MultiLineStringView(MapMultiLineString* mapMultiLineString,int width,osg::Vec4 color)
{
	this->geometryMultiLineStrings=new  osg::Geometry;

	vector<LineStringView*> lineStringViews;
	lineStringViews.reserve(0);
	for(int i=0;i<mapMultiLineString->getNumGeometries();i++)
	{
		OGRGeometry* ogrGeom=mapMultiLineString->getGeometryRef(i);
		OGRwkbGeometryType ogrGeomType = ogrGeom->getGeometryType();

		if (wkbLineString != ogrGeomType && wkbLineString25D != ogrGeomType)
                continue; // skip

		MapLineString* mapLineString = static_cast<MapLineString*>(ogrGeom);
        LineStringView* lineStringView=new LineStringView(mapLineString,width,color);
		lineStringViews.push_back(lineStringView);
	}

	for(int i=0;i<lineStringViews.size();i++)
	{
		osg::ref_ptr<osg::Geometry> geometry=lineStringViews[i]->getGeometryLineString();
		if (geometry.valid() && 
                geometry->getVertexArray() && 
                geometry->getNumPrimitiveSets() &&
                geometry->getVertexArray()->getType() == osg::Array::Vec3ArrayType) 
		{
			if (!this->geometryMultiLineStrings->getVertexArray())
                {
                    this->geometryMultiLineStrings->setVertexArray(geometry->getVertexArray());
                    this->geometryMultiLineStrings->setPrimitiveSetList(geometry->getPrimitiveSetList());

                } 
                else 
                {
                    int size = this->geometryMultiLineStrings->getVertexArray()->getNumElements();

                    osg::Vec3Array* arraySrc = static_cast<osg::Vec3Array*>(geometry->getVertexArray());
                    osg::Vec3Array* arrayDst = static_cast<osg::Vec3Array*>(this->geometryMultiLineStrings->getVertexArray());
                    arrayDst->insert(arrayDst->end(), arraySrc->begin(), arraySrc->end());
                    // shift index 
                    this->geometryMultiLineStrings->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, size, arraySrc->size()));
                }
		}
	}
}


MultiLineStringView::~MultiLineStringView(void)
{
}

osg::ref_ptr<osg::Geometry> MultiLineStringView::getGeometryMultiLineStrings(void)
{
	return this->geometryMultiLineStrings;
}

osg::ref_ptr<osg::Vec4Array> MultiLineStringView::getColor(void)
{
	return this->multiLineStringColor;
}

void  MultiLineStringView::setColor(osg::Vec4 color)
{
	this->multiLineStringColor->resize(0);
	this->multiLineStringColor->push_back(color);
}