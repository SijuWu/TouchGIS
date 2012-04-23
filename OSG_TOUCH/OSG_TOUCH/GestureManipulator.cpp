#include "StdAfx.h"
#include "GestureManipulator.h"


GestureManipulator::GestureManipulator(void)
{
	
}


GestureManipulator::~GestureManipulator(void)
{
}

void GestureManipulator::scale(HWND hwnd,GESTUREINFO gi,POINT lastPoint,ULONGLONG lastArguments)
{
	double zoomFactor = (double)gi.ullArguments/lastArguments;

	// zoom model.
	float scale =1.1;
	if (!nodeMatrixTranslate==0)
	{
		osg::Matrix matrix=nodeMatrixRotate->getMatrix();
		osg::Matrix scaleMatrix;
		if (zoomFactor>1)
			scaleMatrix.makeScale(scale,scale,scale);
		else if (zoomFactor<1)
			scaleMatrix.makeScale(1/scale,1/scale,1/scale);

		matrix=matrix*scaleMatrix;

		nodeMatrixRotate->setMatrix(matrix);
	}

	InvalidateRect(hwnd,NULL,TRUE);
}
void GestureManipulator::scale(HWND hwnd,GESTUREINFO gi,POINT lastPoint,ULONGLONG lastArguments,osg::Matrix* matrixChangeCenter,osg::Matrix* matrixRotation,osg::Matrix* matrixChangeBack,osg::Matrix* matrixZoom)
{
	double zoomFactor = (double)gi.ullArguments/lastArguments;

	// zoom model.
	float scale =1.1;
	if (!nodeMatrixTranslate==0)
	{
		osg::Matrix inverse;
		osg::Matrix rotate=nodeMatrixRotate->getMatrix();
		inverse.invert(rotate);


		if (this->count==0)
		{

			osg::Vec3 touch=this->centerRotateZoom*inverse;
			osg::Vec3 back=this->centerRotateZoom;
			(*matrixChangeBack).identity();
			(*matrixChangeBack).makeTranslate(back);
			touch.x()=-touch.x();
			touch.y()=-touch.y();
			touch.z()=-touch.z();
			(*matrixChangeCenter).identity();

			(*matrixChangeCenter).makeTranslate(touch);

			this->count++;
		}

		osg::Matrix scaleMatrix;
		if (zoomFactor>1)
			scaleMatrix.makeScale(scale,1,scale);
		else if (zoomFactor<1)
			scaleMatrix.makeScale(1/scale,1,1/scale);
		*matrixZoom=(*matrixZoom)*scaleMatrix;

		nodeMatrixRotate->setMatrix((*matrixChangeCenter)*(*matrixRotation)*(*matrixZoom)*(*matrixChangeBack));

	}

	InvalidateRect(hwnd,NULL,TRUE);
}
void GestureManipulator::rotate(HWND hwnd,GESTUREINFO gi,ULONGLONG lastArguments)
{
	//The angle is the rotation delta from the last message, 
	//or from 0 if it is a new message
	ULONGLONG lastAngle = ((gi.dwFlags & GF_BEGIN) != 0) ? 0 : lastArguments;

	double angle=GID_ROTATE_ANGLE_FROM_ARGUMENT(gi.ullArguments)-GID_ROTATE_ANGLE_FROM_ARGUMENT(lastAngle);

	if (!nodeMatrixRotate==0)
	{
		osg::Matrix matrix=nodeMatrixRotate->getMatrix();
		osg::Matrix rotateMatrix;
		rotateMatrix.makeRotate(-angle,osg::Vec3(0,1,0));
		matrix=matrix*rotateMatrix;
		nodeMatrixRotate->setMatrix(matrix);
	}
	InvalidateRect (hwnd, NULL, TRUE);
}

void GestureManipulator::rotate(HWND hwnd,GESTUREINFO gi,ULONGLONG lastArguments,osg::Matrix* matrixChangeCenter,osg::Matrix* matrixRotation,osg::Matrix* matrixChangeBack,osg::Matrix* matrixZoom)
{
	//The angle is the rotation delta from the last message, 
	//or from 0 if it is a new message
 	ULONGLONG lastAngle = ((gi.dwFlags & GF_BEGIN) != 0) ? 0 : lastArguments;

	double angle=GID_ROTATE_ANGLE_FROM_ARGUMENT(gi.ullArguments)-GID_ROTATE_ANGLE_FROM_ARGUMENT(lastAngle);

	if (!nodeMatrixRotate==0)
	{
		osg::Matrix inverse;
		osg::Matrix rotate=nodeMatrixRotate->getMatrix();
		inverse.invert(rotate);


		if (this->count==0)
		{

			osg::Vec3 touch=this->centerRotateZoom*inverse;
			osg::Vec3 back=this->centerRotateZoom;
			(*matrixChangeBack).identity();
			(*matrixChangeBack).makeTranslate(back);
			touch.x()=-touch.x();
			touch.y()=-touch.y();
			touch.z()=-touch.z();
			(*matrixChangeCenter).identity();
			
			(*matrixChangeCenter).makeTranslate(touch);

			this->count++;
		}

		osg::Matrix rotateMatrix;
		rotateMatrix.makeRotate(-angle,osg::Vec3(0,1,0));
		*matrixRotation=(*matrixRotation)*rotateMatrix;

		nodeMatrixRotate->setMatrix((*matrixChangeCenter)*(*matrixRotation)*(*matrixZoom)*(*matrixChangeBack));


	}
	InvalidateRect (hwnd, NULL, TRUE);
}

void GestureManipulator::translate(HWND hwnd,GESTUREINFO gi,POINT lastPoint)
{
	float scale = 100.0f;
	osg::Vec3 dv((gi.ptsLocation.x-lastPoint.x),0.0f,(-gi.ptsLocation.y+lastPoint.y));

	if (!nodeMatrixTranslate==0)
	{

		osg::Matrix matrix=nodeMatrixTranslate->getMatrix();
		osg::Vec3 position(0.0f,0.0f,0.0f);

		osg::Matrix translateMatrix;
		translateMatrix.makeTranslate(dv/10);
		matrix=matrix*translateMatrix;

		nodeMatrixTranslate->setMatrix(matrix);
	}
	InvalidateRect(hwnd, NULL, TRUE);
}

void GestureManipulator::twoFingerTap(HWND hwnd)
{
	if (!nodeMatrixTranslate==0)
	{
		osg::ref_ptr<osg::Geode>geode1=(osg::Geode*)this->nodeMatrixRotate->getChild(0);
		osg::ref_ptr<osg::Geode>geode2=(osg::Geode*)this->nodeMatrixRotate->getChild(1);
		this->nodeMatrixRotate->setChild(0,geode2);
		this->nodeMatrixRotate->setChild(1,geode1);
	}

	InvalidateRect(hwnd, NULL, TRUE);
}
bool GestureManipulator::pick(const double x,const double y, osgViewer::Viewer* viewer)
{
	if (!viewer->getSceneData())
		return false;

	osgUtil::LineSegmentIntersector* picker=new osgUtil::LineSegmentIntersector(osgUtil::Intersector::PROJECTION,x,y);
	osgUtil::IntersectionVisitor intersectionVisitor(picker);
	viewer->getCamera()->accept(intersectionVisitor);

	if (picker->containsIntersections())
	{
		const osg::NodePath& nodePath=picker->getFirstIntersection().nodePath;
		unsigned int idx=nodePath.size();
		while (idx--)
		{
			osg::Geode* geodePicked=dynamic_cast<osg::Geode*>(nodePath[idx]);

			// Find the LAST MatrixTransform in the node path;
			// this will be the MatrixTransform to attach our callback to.
			osg::MatrixTransform* mr =dynamic_cast<osg::MatrixTransform*>(nodePath[idx]);

			if (mr == NULL)
				continue;
			osg::MatrixTransform* mt =dynamic_cast<osg::MatrixTransform*>(nodePath[idx-1]);
			this->nodeMatrixRotate=mr;
			this->nodeMatrixTranslate=mt;
			break;
		}

		this->count=0;

		osg::Camera* camera=viewer->getCamera();
		osg::Matrix viewMatrix=camera->getViewMatrix();
		osg::Matrix projectionMatrix=camera->getProjectionMatrix();

		osg::Matrix VPW=viewMatrix*projectionMatrix;

		osg::Matrix inverseVPW;
		inverseVPW.invert(VPW);
		osg::Vec3d window(x,y,0);
		this->centerRotateZoom=window*inverseVPW;

		osg::Matrix matrixTran=this->nodeMatrixTranslate->getMatrix();
		osg::Vec3 translation=matrixTran.getTrans();

		osg::Matrix inverse;
		inverse.invert(matrixTran);
		this->centerRotateZoom=this->centerRotateZoom*inverse;
	}
}


osg::ref_ptr<osg::MatrixTransform>GestureManipulator::getMatrixTransformTranslate()
{
	return this->nodeMatrixTranslate;
}

void GestureManipulator::setMatrixTransformTranslate(osg::ref_ptr<osg::MatrixTransform> nodeMatrixTranslate)
{
	this->nodeMatrixTranslate=nodeMatrixTranslate;
}

osg::ref_ptr<osg::MatrixTransform>GestureManipulator::getMatrixTransformRotate()
{
	return this->nodeMatrixRotate;
}

void GestureManipulator::setMatrixTransformRotate(osg::ref_ptr<osg::MatrixTransform> nodeMatrixRotate)
{
	this->nodeMatrixRotate=nodeMatrixRotate;
}

