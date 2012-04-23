#include "StdAfx.h"
#include "MultiPolygonView.h"
const osg::Vec4 edgeColor(0.0,0.0,0.0,0.0);

struct TriangulizeFunctor 
{
	osg::Vec3Array* _vertexes;

	// do nothing
	void operator ()(const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, bool treatVertexDataAsTemporary) {
		_vertexes->push_back(v1);
		_vertexes->push_back(v2);
		_vertexes->push_back(v3);
	}
};

static osg::Vec3Array* triangulizeGeometry(osg::Geometry* src)
{
	if (src->getNumPrimitiveSets() == 1 && 
		src->getPrimitiveSet(0)->getType() == osg::PrimitiveSet::DrawArraysPrimitiveType &&
		src->getVertexArray() &&
		src->getVertexArray()->getType() == osg::Array::Vec3ArrayType)
		return static_cast<osg::Vec3Array*>(src->getVertexArray());

	osg::TriangleFunctor<TriangulizeFunctor> functor;
	osg::Vec3Array* array = new osg::Vec3Array;
	functor._vertexes = array;
	src->accept(functor);
	return array;
}

MultiPolygonView::MultiPolygonView(MapMultiPolygon* mapMultiPolygon,bool polygonFill,osg::Vec4 color)
{
	this->geometryMultiPolygons=new osg::Geometry;
	this->geometryEdgeMultiPolygons=new osg::Geometry;

	this->vertices=new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> verticesCopy=new osg::Vec3Array;
	this->geometryEdgeMultiPolygons->setVertexArray(this->vertices);
	this->geometryMultiPolygons->setVertexArray(verticesCopy);
	normals=new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));

	this->polygonFillColor=new osg::Vec4Array;
	this->polygonEdgeColor=new osg::Vec4Array;
	this->polygonFillColor->push_back(color);
	this->polygonEdgeColor->push_back(edgeColor);

	
	for (int i=0;i<mapMultiPolygon->getNumGeometries();i++)
	{


		OGRGeometry* ogrGeom=mapMultiPolygon->getGeometryRef(i);
		OGRwkbGeometryType ogrGeomType=ogrGeom->getGeometryType();

		if (wkbPolygon != ogrGeomType && wkbPolygon25D != ogrGeomType)
			continue; // skip



		MapPolygon* mapPolygon=static_cast< MapPolygon*>(ogrGeom);


		OGRLinearRing *poExteriorRing=mapPolygon->getExteriorRing();

			
		OGRPoint point;
		for(int i = 0; i < poExteriorRing->getNumPoints(); i++)
		{
			poExteriorRing->getPoint(i, &point);
			vertices->push_back(osg::Vec3(point.getX(), 0, point.getY()));
			verticesCopy->push_back(osg::Vec3(point.getX(), 0, point.getY()));
		}
		this->geometryEdgeMultiPolygons->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, vertices->size()-poExteriorRing->getNumPoints(), poExteriorRing->getNumPoints()));
		this->geometryMultiPolygons->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, verticesCopy->size()-poExteriorRing->getNumPoints(), poExteriorRing->getNumPoints()));


		if (mapPolygon->getNumInteriorRings())
		{
			for (int i = 0; i < mapPolygon->getNumInteriorRings(); i++)
			{
				OGRLinearRing *ring = mapPolygon->getInteriorRing(i);
				OGRPoint point;

				for (int j = 0; j < ring->getNumPoints(); j++) 
				{
					ring->getPoint(j, &point);
					vertices->push_back(osg::Vec3(point.getX(), 0, point.getY()));
					verticesCopy->push_back(osg::Vec3(point.getX(), 0, point.getY()));
				}
				this->geometryEdgeMultiPolygons->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, vertices->size()-ring->getNumPoints() , ring->getNumPoints()));
				this->geometryMultiPolygons->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, verticesCopy->size()-ring->getNumPoints() , ring->getNumPoints()));
			}
		}
	}


	this->geometryEdgeMultiPolygons->setNormalArray(normals);
	this->geometryEdgeMultiPolygons->setNormalBinding(osg::Geometry::BIND_OVERALL);

	this->geometryEdgeMultiPolygons->setColorArray(this->polygonEdgeColor);
	this->geometryEdgeMultiPolygons->setColorBinding(osg::Geometry::BIND_OVERALL);

	/*if(polygonFill==true)
	{*/


		osgUtil::Tessellator tsl;
		tsl.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
		tsl.setBoundaryOnly(false);
		tsl.retessellatePolygons(*this->geometryMultiPolygons);

		osg::Vec3Array* array = triangulizeGeometry(this->geometryMultiPolygons);
		this->geometryMultiPolygons->setVertexArray(array);
		this->geometryMultiPolygons->removePrimitiveSet(0,this->geometryMultiPolygons->getNumPrimitiveSets());
		this->geometryMultiPolygons->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, array->size()));

		this->geometryMultiPolygons->setNormalArray(normals);
		this->geometryMultiPolygons->setNormalBinding(osg::Geometry::BIND_OVERALL);

		this->geometryMultiPolygons->setColorArray(this->polygonFillColor);
		this->geometryMultiPolygons->setColorBinding(osg::Geometry::BIND_OVERALL);
	//}

}


MultiPolygonView::~MultiPolygonView(void)
{
}


osg::ref_ptr<osg::Geometry> MultiPolygonView::getGeometryPolygon(void)
{
	return this->geometryMultiPolygons;
}

osg::ref_ptr<osg::Geometry> MultiPolygonView::getGeometryEdgePolygon(void)
{
	return this->geometryEdgeMultiPolygons;
}

osg::ref_ptr<osg::Vec4Array> MultiPolygonView::getColor(void)
{
	return this->polygonFillColor;
}

void  MultiPolygonView::setColor(osg::Vec4 color)
{
	this->polygonFillColor->resize(0);
	this->polygonFillColor->push_back(color);
}