// OSG_TOUCH.cpp : définit le point d'entrée pour l'application.
//
#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>
#include <ogrsf_frmts.h>

#include <iostream>
#include <fstream>

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>

#include <osgUtil/Optimizer>

#include <osg/Node>
#include <osg/Quat>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Notify>
#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>
#include <osgGA/TrackballManipulator>

#include <process.h>
#include "MapManipulator.h"
#include "MapLineString.h"
#include "LineStringView.h"
#include "MapPolygon.h"
#include "PolygonView.h"
#include "MapPoint.h"
#include "PointView.h"
#include "MapLayer.h"
#include "MapLayerView.h"
#include "OGR.h"
#include "GestureManipulator.h"

using namespace std;
using std::vector;
#define GF_BEGIN                        0x00000001
#define GF_INERTIA                      0x00000002
#define GF_END                          0x00000004

#define GID_BEGIN                       1
#define GID_END                         2
#define GID_ZOOM                        3
#define GID_PAN                         4
#define GID_ROTATE                      5
#define GID_TWOFINGERTAP                6
#define GID_PRESSANDTAP                 7
#define GID_ROLLOVER                    GID_PRESSANDTAP


#define LODWORD(l) ((DWORD)((DWORDLONG)(l)))
#define GID_SCROLL_DIRECTION(x)          ((LONG)(HIWORD(((ULONGLONG)(x)) >> 32) & 0x000F))

osg::ref_ptr<osgViewer::Viewer> viewer;

osg::observer_ptr<osgViewer::GraphicsWindow> window;
osg::ref_ptr<osg::Group> root;

vector<osg::ref_ptr<osg::Geode>> mapLayers; 
osgUtil::Optimizer optimizer;
osg::ref_ptr<osg::GraphicsContext::Traits> traits;
osg::ref_ptr<osg::Referenced> windata;
osg::GraphicsContext* gc;
osg::ref_ptr<osg::Camera> camera;

bool renderok;


osg::ref_ptr<osg::MatrixTransform> matrixTranslate;
osg::ref_ptr<osg::MatrixTransform> matrixRotate;

osg::Matrix* matrixChangeCenter;
osg::Matrix* matrixRotation;
osg::Matrix* matrixChangeBack;
osg::Matrix* matrixZoom;

osg::ref_ptr<osg::Geode> geodeChanged;
int mapCount;

BOOL bResult;

//The width of the window
double windowWidth;

//The height of the window
double windowHeight;

//The X coordinate of the touch in the osg view
double touchX;

//The Y coordinate of the touch in the osg view
double touchY;

osg::ref_ptr<osg::MatrixTransform>nodeMatrix;
osg::ref_ptr<osg::MatrixTransform>nodeMatrixRotate;

GestureManipulator* gestureManipulator=new GestureManipulator;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("HelloWin");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	/*wndclass.hInstance = hInstance;*/
	wndclass.hInstance = 0;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow( szAppName, // window class name
		TEXT ("The Hello Program"), // window caption
		WS_OVERLAPPEDWINDOW, // window style
		100, // initial x position
		100, // initial y position
		800, // initial x size
		600, // initial y size
		NULL, // parent window handle
		NULL, // window menu handle
		0/*hInstance*/, // program instance handle
		NULL); // creation parameters

	GESTURECONFIG gestureConfig;
	gestureConfig.dwID = 0;
	gestureConfig.dwBlock = 0;
	gestureConfig.dwWant = GC_ALLGESTURES;


	BOOL result = SetGestureConfig(hwnd,0,1,&gestureConfig,sizeof(gestureConfig));  

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

void render(void* ptr)
{
	while (!viewer->done())
	{
		viewer->frame();
	}
	renderok = true;
}


void createContext(HWND hwnd)
{
	windata = new osgViewer::GraphicsWindowWin32::WindowData(hwnd);
	traits = new osg::GraphicsContext::Traits;
	traits->x = 0;
	traits->y = 0;
	traits->width = 800;
	traits->height = 600;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->inheritedWindowData = windata;

	gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	
}

void createMapView(HWND hwnd)
{		
	
	mapLayers.reserve(0);

	MapLayer* mapLayer1=new MapLayer;
	MapLayerView* mapLayerView1=new MapLayerView;

	MapLayer* mapLayer2=new MapLayer;
	MapLayerView* mapLayerView2=new MapLayerView;

	OGR* ogr=new OGR;
	
	ogr->readMapLayer(mapLayer1, mapLayerView1,"D:/GIS data/FRA_adm/FRA_adm1.shp",true);
	ogr->readMapLayer(mapLayer2, mapLayerView2,"D:/GIS data/FRA_adm/FRA_adm2.shp",true);
	mapLayers.push_back(mapLayerView1->getMapLayerNode());
	mapLayers.push_back(mapLayerView2->getMapLayerNode());

	matrixTranslate=new osg::MatrixTransform();
	matrixRotate=new osg::MatrixTransform();
	
	matrixChangeCenter=new osg::Matrix;
	matrixChangeCenter->identity();

	matrixRotation=new osg::Matrix;
	matrixRotation->identity();

	matrixChangeBack=new osg::Matrix;
	matrixChangeBack->identity();

	matrixZoom=new osg::Matrix;
	matrixZoom->identity();

	matrixRotate->setMatrix((*matrixChangeCenter)*(*matrixRotation)*(*matrixZoom)*(*matrixChangeBack));

	matrixTranslate->addChild(matrixRotate);

	osg::ref_ptr<osg::Group>groupMap=new osg::Group();
	
	for(int i=0;i<mapLayers.size();i++)
	{
		matrixRotate->addChild(mapLayers[i]);
	}

	root = new osg::Group;
	root->addChild(matrixTranslate);
	
	createContext(hwnd);

	camera = new osg::Camera;
	camera->setGraphicsContext(gc);
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setClearMask( GL_DEPTH_BUFFER_BIT |
		GL_COLOR_BUFFER_BIT );
	camera->setClearColor( osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f) );
	camera->setProjectionMatrixAsPerspective(
		30.0f,(double)traits->width/(double)traits
		->height,1.0,1000.0 );
	
	viewer = new osgViewer::Viewer;
	viewer->setCamera(camera.get());
	viewer->setSceneData(root.get());
	
	viewer->setKeyEventSetsDone(0);
	viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	viewer->addEventHandler(new MapManipulator);

	osg::Matrix viewMatrix=camera->getViewMatrix();
	osg::Matrix projectionMatrix=camera->getProjectionMatrix();
	osg::Matrix windowMatrix=camera->getViewport()->computeWindowMatrix();
	viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);	
	viewer->realize();

	renderok = false;
	_beginthread(render, 0, NULL);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_GESTURE:
		static POINT lastPoint;
		static ULONGLONG lastArguments;

		GESTUREINFO gi;

		gi.cbSize = sizeof(GESTUREINFO);
		gi.dwFlags       = 0;
		gi.ptsLocation.x = 0;
		gi.ptsLocation.y = 0;
		gi.dwID          = 0;
		gi.dwInstanceID  = 0;
		gi.dwSequenceID  = 0;

		bResult = GetGestureInfo((HGESTUREINFO)lParam, &gi);
		POINT pt;
		pt.x=gi.ptsLocation.x;
		pt.y=gi.ptsLocation.y;
		ScreenToClient(hwnd,&pt);
		RECT rect;
		::GetWindowRect(hwnd, &rect);
		windowWidth=rect.right-rect.left;
		windowHeight=rect.bottom-rect.top;
		touchX=2*pt.x/windowWidth-1;
		touchY=-2*pt.y/windowHeight+1;

		switch(gi.dwID)
		{
		case GID_BEGIN:

			if(gestureManipulator->pick(touchX,touchY,viewer))
			{
				gestureManipulator->setMatrix(matrixChangeCenter,matrixRotation,matrixChangeBack, matrixZoom);
			}

			break;
		case GID_END:

			gestureManipulator->setMatrixTransformTranslate(0);
			gestureManipulator->setMatrixTransformRotate(0);
			gestureManipulator->setMatrix(0,0,0,0);
			
			break;
		case GID_PAN:
			if ((gi.dwFlags & GF_BEGIN) == 0) //not the first message
			{		
				gestureManipulator->translate(hwnd,gi,lastPoint);		
			}
			break;

		case GID_ZOOM:            //not the first message
			if ((gi.dwFlags & GF_BEGIN) == 0 && lastArguments != 0)
			{
				gestureManipulator->scale(hwnd,gi,lastPoint,lastArguments);			
			}
			break;
		case GID_ROTATE:
			{
				gestureManipulator->rotate(hwnd,gi,lastArguments);
			}

			break;
		case GID_PRESSANDTAP:
			
			break;

		case GID_TWOFINGERTAP:

			gestureManipulator->twoFingerTap(hwnd);
			break;
		}

		lastPoint.x = gi.ptsLocation.x;
		lastPoint.y = gi.ptsLocation.y;
		lastArguments = gi.ullArguments;
		CloseGestureInfoHandle((HGESTUREINFO)lParam);
		return 0;

	case WM_CREATE:
		createMapView(hwnd);
		return 0;

	case WM_PAINT:
		return 0;

	case WM_DESTROY:
		viewer->setDone(true);
		while (!renderok)
		{
			Sleep(10);
		}
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

