#include "StdAfx.h"
#include "PolygonView.h"
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


PolygonView::PolygonView(MapPolygon* mapPolygon,bool polygonFill,osg::Vec4 color)
{

	this->mapPolygon=mapPolygon;
	this->geometryEdgePolygon=new osg::Geometry;
	/*this->geometryPolygon=new osg::Geometry;*/
	this->vertices=new osg::Vec3Array;

	/*this->geometryPolygon->setVertexArray(this->vertices);*/

	this->geometryEdgePolygon->setVertexArray(this->vertices);
	OGRLinearRing *poExteriorRing=mapPolygon->getExteriorRing();

	normals=new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));

	this->polygonFillColor=new osg::Vec4Array;
	this->polygonEdgeColor=new osg::Vec4Array;
	this->polygonFillColor->push_back(color);
	this->polygonEdgeColor->push_back(edgeColor);

	{	
		OGRPoint point;
		for(int i = 0; i < poExteriorRing->getNumPoints(); i++)
		{
			poExteriorRing->getPoint(i, &point);
			vertices->push_back(osg::Vec3(point.getX(), 0, point.getY()));
		}
		this->geometryEdgePolygon->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vertices->size()));
		/*this->geometryPolygon->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vertices->size()));*/
	}
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
			}
			this->geometryEdgePolygon->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, vertices->size()-ring->getNumPoints() , ring->getNumPoints()));
			/*this->geometryPolygon->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, vertices->size()-ring->getNumPoints() , ring->getNumPoints()));*/
		}
	}

	this->geometryEdgePolygon->setNormalArray(normals);
	this->geometryEdgePolygon->setNormalBinding(osg::Geometry::BIND_OVERALL);

	this->geometryEdgePolygon->setColorArray(this->polygonEdgeColor);
	this->geometryEdgePolygon->setColorBinding(osg::Geometry::BIND_OVERALL);

	if (polygonFill==true)
	{
		this->geometryPolygon=new osg::Geometry;
		this->geometryPolygon->setVertexArray(this->vertices);
		for(int i=0;i<this->geometryEdgePolygon->getNumPrimitiveSets();i++)
		{
			this->geometryPolygon->addPrimitiveSet(this->geometryEdgePolygon->getPrimitiveSet(i));
		}

		osgUtil::Tessellator tsl;
		tsl.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
		tsl.setBoundaryOnly(false);
		tsl.retessellatePolygons(*this->geometryPolygon);

		osg::Vec3Array* array = triangulizeGeometry(this->geometryPolygon);
		this->geometryPolygon->setVertexArray(array);
		this->geometryPolygon->removePrimitiveSet(0,this->geometryPolygon->getNumPrimitiveSets());
		this->geometryPolygon->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, array->size()));

		this->geometryPolygon->setNormalArray(normals);
		this->geometryPolygon->setNormalBinding(osg::Geometry::BIND_OVERALL);

		this->geometryPolygon->setColorArray(this->polygonFillColor);
		this->geometryPolygon->setColorBinding(osg::Geometry::BIND_OVERALL);
	}
}

PolygonView::~PolygonView(void)
{
}

osg::ref_ptr<osg::Geometry> PolygonView::getGeometryPolygon(void)
{
	return this->geometryPolygon;
}

osg::ref_ptr<osg::Geometry> PolygonView::getGeometryEdgePolygon(void)
{
	return this->geometryEdgePolygon;
}

osg::ref_ptr<osg::Vec3Array> PolygonView::getVertices(void)
{
	return this->vertices;
}

MapPolygon* PolygonView::getMapPolygon(void)
{
	return this->mapPolygon;
}

osg::ref_ptr<osg::Vec4Array> PolygonView::getColor(void)
{
	return this->polygonFillColor;
}

void  PolygonView::setColor(osg::Vec4 color)
{
	this->polygonFillColor->resize(0);
	this->polygonFillColor->push_back(color);
}