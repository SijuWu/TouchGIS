#pragma once
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <osgUtil/SceneView>

//Manipulator of the gesture command
class GestureManipulator
{
public:
	GestureManipulator(void);
	~GestureManipulator(void);

	//Scale the object chosen
	void scale(HWND hwnd,GESTUREINFO gi,POINT lastPoint,ULONGLONG lastArguments);
	void scale(HWND hwnd,GESTUREINFO gi,POINT lastPoint,ULONGLONG lastArguments,osg::Matrix* matrixChangeCenter,osg::Matrix* matrixRotation,osg::Matrix* matrixChangeBack,osg::Matrix* matrixZoom);

	//Rotate the object chosen
	void rotate(HWND hwnd,GESTUREINFO gi,ULONGLONG lastArguments);
	void rotate(HWND hwnd,GESTUREINFO gi,ULONGLONG lastArguments,osg::Matrix* matrixChangeCenter,osg::Matrix* matrixRotation,osg::Matrix* matrixChangeBack,osg::Matrix* matrixZoom);

	//Translate the object chosen
	void translate(HWND hwnd,GESTUREINFO gi,POINT lastPoint);

	void twoFingerTap(HWND hwnd);
	
	//Pick an object by the gesture
	bool pick(const double x,const double y, osgViewer::Viewer* viewer);
	bool pick(const double x,const double y, osgViewer::Viewer* viewer,osg::ref_ptr<osg::MatrixTransform>matrixTranslate);
	osg::ref_ptr<osg::MatrixTransform>getMatrixTransformTranslate();
	void setMatrixTransformTranslate(osg::ref_ptr<osg::MatrixTransform> nodeMatrixTranslate);

	osg::ref_ptr<osg::MatrixTransform>getMatrixTransformRotate();
	void setMatrixTransformRotate(osg::ref_ptr<osg::MatrixTransform> nodeMatrixRotate);

	

private:
	//The matrixTransform of translation of the object chosen by the gesture
	osg::ref_ptr<osg::MatrixTransform>nodeMatrixTranslate;

	//The matrixTransform of rotation of the object chosen by the gesture
	osg::ref_ptr<osg::MatrixTransform>nodeMatrixRotate;

	osg::Vec3 centerRotateZoom;

	

	int count;
};
