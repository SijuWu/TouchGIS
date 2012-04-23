#pragma once
#include <ogrsf_frmts.h>

class MapMultiPolygon: public OGRMultiPolygon
{
public:
	MapMultiPolygon(void);
	~MapMultiPolygon(void);
};

