#include "StdAfx.h"
#include "OGR.h"

const int pointSize=2;
const int lineWidth=2;
const osg::Vec4 color0(0.0f,0.0f,1.0f,1.0f);
const osg::Vec4 color1(0.0f,1.0f,0.0f,1.0f);
const osg::Vec4 color2(1.0f,0.0f,0.0f,1.0f);
const osg::Vec4 color3(0.0f,1.0f,1.0f,1.0f);
const osg::Vec4 color4(1.0f,0.0f,1.0f,1.0f);
const osg::Vec4 color5(1.0f,1.0f,0.0f,1.0f);

static int colorCount=0;

OGR::OGR(void)
{
}

OGR::~OGR(void)
{
}

void OGR::readMap(Map* map, MapView* mapView,char* source,bool polygonFill)
{
	osg::ref_ptr<osg::Geode>mapNode=new osg::Geode;
	/*osg::ref_ptr<osg::Group>maplayer=new osg::Group;
	maplayer->addChild(mapNode.get());*/

	OGRRegisterAll();

	OGRDataSource       *poDS;

	poDS = OGRSFDriverRegistrar::Open( source, FALSE );

	if( poDS == NULL )
	{
		printf( "Open failed.\n" );
		exit( 1 );
	}

	OGRLayer  *poLayer;

	int layer=poDS->GetLayerCount();

	this->mapPoints.reserve(0);
	this->pointViews.reserve(0);
	
	this->mapLineStrings.reserve(0);
	this->lineStringViews.reserve(0);

	this->mapLinearRings.reserve(0);
	this->linearRingViews.reserve(0);

	this->mapPolygons.reserve(0);
	this->polygonViews.reserve(0);

	this->mapMultiLineStrings.reserve(0);
	this->multiLineStringViews.reserve(0);

	this->mapMultiPolygons.reserve(0);
	this->multiPolygonViews.reserve(0);


	for (int layerCount=0;layerCount<poDS->GetLayerCount();layerCount++)
	{
		poLayer = poDS->GetLayer(layerCount);
		OGRFeature *poFeature=NULL;

		poLayer->ResetReading();

		int featureCount=0;
		osg::Vec4 color;
		switch(colorCount)
		{
		case 0:
			color=color0;
			break;
		case 1:
			color=color1;
			break;
		case 2:
			color=color2;
			break;
		case 3:
			color=color3;
			break;
		case 4:
			color=color4;
			break;
		case 5:
			color=color5;
			break;
		default:
			color=osg::Vec4(0,0,0,0);
			break;
		}
		colorCount++;

		while((poFeature=poLayer->GetNextFeature()) !=NULL)
		{
			featureCount=poLayer->GetFeatureCount();
			
			if(!poFeature || !poFeature->GetGeometryRef())
				break;

			switch(poFeature->GetGeometryRef()->getGeometryType())
			{
			case wkbPoint:
			case wkbPoint25D:
				 pointToDrawable(static_cast<MapPoint*>(poFeature->GetGeometryRef()->clone()),mapNode,color);
				break;

			case wkbLinearRing:
				linearRingToDrawable(static_cast<MapLinearRing*>(poFeature->GetGeometryRef()->clone()),mapNode);
				break;

			case wkbLineString:
			case wkbLineString25D:
				lineStringToDrawable(static_cast<MapLineString*>(poFeature->GetGeometryRef()->clone()),mapNode,color);
				break;
            
			case wkbPolygon:
            case wkbPolygon25D:
				polygonToDrawable(static_cast<MapPolygon*>(poFeature->GetGeometryRef()->clone()),mapNode, polygonFill,color);
			    break; 

		    case wkbMultiPoint:
            case wkbMultiPoint25D:
                break;

			case wkbMultiLineString:
			case wkbMultiLineString25D:
				multiLineStringToDrawable(static_cast<MapMultiLineString*>(poFeature->GetGeometryRef()->clone()),mapNode,color);
				break;

			case wkbMultiPolygon:
			case wkbMultiPolygon25D:
				//multiPolygonToDrawable(static_cast<MapMultiPolygon*>(poFeature->GetGeometryRef()->clone()),mapNode,polygonFill,color);
				break;

			case wkbGeometryCollection:
			case wkbGeometryCollection25D:
				osg::notify(osg::WARN) << "This geometry is not yet implemented " << OGRGeometryTypeToName(poFeature->GetGeometryRef()->getGeometryType()) << std::endl;
				break;

			case wkbNone:
				osg::notify(osg::WARN) << "No WKB Geometry " << OGRGeometryTypeToName(poFeature->GetGeometryRef()->getGeometryType()) << std::endl;
				break;

			case wkbUnknown:
			default:
				osg::notify(osg::WARN) << "Unknown WKB Geometry " << OGRGeometryTypeToName(poFeature->GetGeometryRef()->getGeometryType()) << std::endl;
				break;

			}
			
			OGRFeature::DestroyFeature(poFeature);
		}
		
	}

	OGRDataSource::DestroyDataSource( poDS );
	
	map->setMapPoints(mapPoints);
	map->setMapLineStrings(mapLineStrings);
	map->setMapLinearRings(mapLinearRings);
	map->setMapPolygons(mapPolygons);
	map->setMapMultiPolygons(mapMultiPolygons);
	map->setMapMultiLineStrings(mapMultiLineStrings);

	mapView->setMap(map);
	mapView->setPointViews(pointViews);
	mapView->setLineStringView(lineStringViews);
	mapView->setLinearRingView(linearRingViews);
	mapView->setPolygonView(polygonViews);
	mapView->setMultiPolygonView(multiPolygonViews);
	mapView->setMultiLineStringView(multiLineStringViews);
	mapView->setMapNode(mapNode);
	//mapView->setLayer(maplayer);

}



void OGR::pointToDrawable(MapPoint* mapPoint,osg::ref_ptr<osg::Geode> mapNode,osg::Vec4 color)
{
	mapPoints.push_back(mapPoint);
	PointView* pointView=new PointView(mapPoint,pointSize,color);
	pointViews.push_back(pointView);
	mapNode->addDrawable(pointView->getGeometryPoint());
}

void OGR::lineStringToDrawable(MapLineString* mapLineString,osg::ref_ptr<osg::Geode> mapNode,osg::Vec4 color)
{
	
	mapLineStrings.push_back(mapLineString);
	LineStringView* lineStringView=new LineStringView(mapLineString,lineWidth,color);
	lineStringViews.push_back(lineStringView);
	mapNode->addDrawable(lineStringView->getGeometryLineString());
}

void OGR::linearRingToDrawable(MapLinearRing* mapLinearRing,osg::ref_ptr<osg::Geode> mapNode)
{
	
	mapLinearRings.push_back(mapLinearRing);
	LinearRingView* linearRingView=new LinearRingView(mapLinearRing,lineWidth);
	linearRingViews.push_back(linearRingView);
	mapNode->addDrawable(linearRingView->getGeometryLinearRing());
}

void OGR::polygonToDrawable(MapPolygon* mapPolygon,osg::ref_ptr<osg::Geode> mapNode,bool polygonFill,osg::Vec4 color)
{
	mapPolygons.push_back(mapPolygon);

	PolygonView* polygonView=new PolygonView(mapPolygon,polygonFill,color);
	polygonViews.push_back(polygonView);
	
	mapNode->addDrawable(polygonView->getGeometryEdgePolygon());
	if (polygonFill==true)
	{
		mapNode->addDrawable(polygonView->getGeometryPolygon());
	}
		
}

void OGR::multiLineStringToDrawable(MapMultiLineString* mapMultiLineString,osg::ref_ptr<osg::Geode> mapNode,osg::Vec4 color)
{
	mapMultiLineStrings.push_back(mapMultiLineString);

	MultiLineStringView* multiLineStringView=new MultiLineStringView(mapMultiLineString,lineWidth,color);
	multiLineStringViews.push_back(multiLineStringView);
	
	mapNode->addDrawable(multiLineStringView->getGeometryMultiLineStrings());

}

void OGR::multiPolygonToDrawable(MapMultiPolygon* mapMultiPolygon,osg::ref_ptr<osg::Geode> mapNode,bool polygonFill,osg::Vec4 color)
{
	MultiPolygonView* multiPolygonView=new MultiPolygonView(mapMultiPolygon,polygonFill,color);
	multiPolygonViews.push_back(multiPolygonView);
	
	mapNode->addDrawable(multiPolygonView->getGeometryEdgePolygon());
	/*if (polygonFill==true)
	{*/
		mapNode->addDrawable(multiPolygonView->getGeometryPolygon());
	/*}*/	

}