#pragma once
#include <ogrsf_frmts.h>
//The model of line object on the map
class MapLineString: public OGRLineString
{
public:
	MapLineString(void);
	~MapLineString(void);
};

