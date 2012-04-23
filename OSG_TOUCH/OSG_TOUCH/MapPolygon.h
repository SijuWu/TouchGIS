#pragma once
#include <ogrsf_frmts.h>
//The model of polygon object on the map
class MapPolygon:public OGRPolygon
{
public:
	MapPolygon(void);
	~MapPolygon(void);
};

