#pragma once
#include <ogrsf_frmts.h>

class MapLinearRing: public OGRLinearRing
{
public:
	MapLinearRing(void);
	~MapLinearRing(void);
};

