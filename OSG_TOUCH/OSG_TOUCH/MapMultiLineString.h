#pragma once
#include <ogrsf_frmts.h>

class MapMultiLineString:public OGRMultiLineString
{
public:
	MapMultiLineString(void);
	~MapMultiLineString(void);
};

