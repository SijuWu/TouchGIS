#pragma once
#include "stdafx.h"

#include <osgViewer/Viewer>

#include <osg/Geode>
#include <osg/Node>
#include <osg/Group>

#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Quat>
#include <osg/Notify>
#include <osg/BoundsChecking>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>

using namespace osg;
using namespace osgGA;
using namespace std;
using std::vector;
class MapManipulator: public osgGA::GUIEventHandler
{
public:
	MapManipulator(void);
	~MapManipulator(void);
	bool handle(const osgGA::GUIEventAdapter &ea,osgGA::GUIActionAdapter& aa);
	bool isMouseMoving();
	void addMouseEvent(const GUIEventAdapter& ea);
	void flushMouseEventStack();
	bool pick(const double x,const double y, osgViewer::Viewer* viewer);
	bool pick(const double x,const double y, osgViewer::Viewer* viewer,const osgGA::GUIEventAdapter &ea);
	bool calcMovement();

private:
	osgViewer::Viewer* viewer;
	// mouse state
	bool _thrown;
	float _mouseCenterX, _mouseCenterY;

	// internal event stack comprising last two mouse events.
	osg::ref_ptr< const osgGA::GUIEventAdapter > _ga_t1;
	osg::ref_ptr< const osgGA::GUIEventAdapter > _ga_t0;

	/** The approximate amount of time it is currently taking to draw a frame.
	* This is used to compute the delta in translation/rotation during a thrown display update.
	* It allows us to match an delta in position/rotation independent of the rendering frame rate.
	*/
	double _delta_frame_time;

	/** The time the last frame started.
	* Used when _rate_sensitive is true so that we can match display update rate to rotation/translation rate.
	*/
	double _last_frame_time;

	// scene data

	osg::ref_ptr<osg::Node> nodeSelected;
	osg::ref_ptr<osg::MatrixTransform>nodeMatrix;
	vector<osg::ref_ptr<osg::Geometry>> geometryPickeds;
	vector<osg::Vec4>colorPickeds;
	double x;
	double y;
};

