#pragma once
#include <ogrsf_frmts.h>
//The model of point object on the map
class MapPoint:public OGRPoint
{
public:
	MapPoint(void);
	~MapPoint(void);
};

