#include "StdAfx.h"
#include "MapManipulator.h"

const osg::Vec4 colorPicked(0.5,0.5,0.5,1); 
MapManipulator::MapManipulator(void)
{
	this->geometryPickeds.reserve(0);
}


MapManipulator::~MapManipulator(void)
{
}

bool MapManipulator::handle(const osgGA::GUIEventAdapter &ea,osgGA::GUIActionAdapter& aa)
{

	this->viewer=dynamic_cast<osgViewer::Viewer*>(&aa);

	osg::ref_ptr<osg::MatrixTransform>mt=dynamic_cast<osg::MatrixTransform*>(this->viewer->getSceneData());




	switch(ea.getEventType())
	{
		
	case(GUIEventAdapter::PUSH):
		{
			flushMouseEventStack();
			addMouseEvent(ea);
			x=ea.getWindowWidth();
			y=ea.getWindowHeight();
			double eaX=ea.getXnormalized();
			double eaY=ea.getYnormalized();
			pick(ea.getXnormalized(),ea.getYnormalized(),this->viewer);
			//pick(ea.getXnormalized(),ea.getYnormalized(),this->viewer,ea);
			if (calcMovement())aa.requestRedraw();
			aa.requestContinuousUpdate(false);
			_thrown=false;
			return true;
		}

	case(GUIEventAdapter::RELEASE):
		{
			if (ea.getButtonMask()==0)
			{
				if (isMouseMoving())
				{
					if(calcMovement())
					{
						aa.requestRedraw();
						aa.requestContinuousUpdate(true);
						_thrown=true;
					}
				}
				else
				{
					flushMouseEventStack();
					addMouseEvent(ea);
					if (calcMovement())aa.requestRedraw();
					_thrown=false;
				}
			}
			else
			{
				flushMouseEventStack();
				addMouseEvent(ea);
				if (calcMovement())aa.requestRedraw();
				aa.requestContinuousUpdate(false);
				_thrown=false;
			}
			this->nodeMatrix=NULL;
			return true;
		}

	case (GUIEventAdapter::DRAG):
		{
			addMouseEvent(ea);
			if (calcMovement())aa.requestRedraw();
			aa.requestContinuousUpdate(false);
			_thrown=false;
			return true;
		}

	case (GUIEventAdapter::MOVE):
		{
			return false;
		}

	case (GUIEventAdapter::FRAME):
		if (_thrown)
		{
			if (calcMovement())aa.requestRedraw();
		}
		return false;

	default:
		return false;
	}
}
bool MapManipulator::isMouseMoving()
{
	if (_ga_t0.get()==NULL||_ga_t1.get()==NULL)return false;

	static const float velocity=0.1f;

	float dx=_ga_t0->getXnormalized()-_ga_t1->getXnormalized();
	float dy=_ga_t0->getYnormalized()-_ga_t1->getYnormalized();
	float len=sqrt(dx*dx+dy*dy);
	float dt=_ga_t0->getTime()-_ga_t1->getTime();

	return (len>dt*velocity);

}
void MapManipulator::addMouseEvent(const GUIEventAdapter& ea)
{
	_ga_t1 = _ga_t0;
	_ga_t0 = &ea;
}
void MapManipulator::flushMouseEventStack()
{
	_ga_t1 = NULL;
	_ga_t0 = NULL;
}

bool MapManipulator::pick(const double x,const double y, osgViewer::Viewer* viewer)
{
	if (!viewer->getSceneData())
		return false;

	double w=0.01;
	double h=0.01;
	osgUtil::LineSegmentIntersector* picker=new osgUtil::LineSegmentIntersector(osgUtil::Intersector::PROJECTION,x,y);
	//osgUtil::PolytopeIntersector* picker=new osgUtil::PolytopeIntersector(osgUtil::Intersector::PROJECTION,x-w, y-h, x+w, y+h);
	osgUtil::IntersectionVisitor intersectionVisitor(picker);
	viewer->getCamera()->accept(intersectionVisitor);

	if (picker->containsIntersections())
	{
		for(int i=0;i<this->geometryPickeds.size();i++)
		{
			osg::Vec4Array* color=new osg::Vec4Array;
			color->push_back(this->colorPickeds[i]);
			this->geometryPickeds[i]->setColorArray(color);
		}
		this->colorPickeds.resize(0);
		this->geometryPickeds.resize(0);

		osg::ref_ptr<osg::Geometry> geometry=picker->getFirstIntersection().drawable->asGeometry();

		osg::Vec4Array* colorBefore=(osg::Vec4Array*)geometry->getColorArray();
		this->colorPickeds.push_back((*colorBefore)[0]);
		osg::Vec4Array* lineColor=new osg::Vec4Array;
		lineColor->push_back(colorPicked);
		geometry->setColorArray(lineColor);
		geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
		this->geometryPickeds.push_back(geometry);

	
		
		

		const osg::NodePath& nodePath=picker->getFirstIntersection().nodePath;
		unsigned int idx=nodePath.size();
		while (idx--)
		{
			// Find the LAST MatrixTransform in the node path;
			// this will be the MatrixTransform to attach our callback to.
			osg::MatrixTransform* mt =dynamic_cast<osg::MatrixTransform*>(nodePath[idx]);
			if (mt == NULL)
				continue;

			//nodeSelected=mt;
			this->nodeMatrix=mt;
			break;
		}
	}
}
bool MapManipulator::pick(const double x,const double y, osgViewer::Viewer* viewer,const osgGA::GUIEventAdapter &ea)
{
	if (!viewer->getSceneData())
		return false;
	
	osgUtil::LineSegmentIntersector::Intersections intersections;
	
	if (viewer->computeIntersections(ea.getX(),ea.getY(),intersections))
	{
		for (osgUtil::LineSegmentIntersector::Intersections::iterator hitr=intersections.begin();hitr!=intersections.end();++hitr)
		{
			bool i1=hitr->nodePath.empty();
			std::string name=hitr->nodePath.back()->getName();
			if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))
			{
				
				std::string name=hitr->nodePath.back()->getName();
				osg::Node* nodePicked=hitr->nodePath.back();
				int nodePathSize=hitr->nodePath.size();
				while (nodePathSize--)
				{
					// Find the LAST MatrixTransform in the node path;
					// this will be the MatrixTransform to attach our callback to.
					osg::MatrixTransform* mt =dynamic_cast<osg::MatrixTransform*>(hitr->nodePath[nodePathSize--]);
					if (mt == NULL)
						continue;

					//nodeSelected=mt;
					this->nodeMatrix=mt;
					break;
				}
				break;
				
			}
			else if (hitr->drawable.valid())
			{
				std::cout<<"Geo: name="<<hitr->drawable->getName()
					<<std::endl;
				
			}
		}
	}

	
}
bool MapManipulator::calcMovement()
{
	// return if less then two events have been added.
	if (_ga_t0.get()==NULL || _ga_t1.get()==NULL) return false;

	float dx = _ga_t0->getXnormalized()-_ga_t1->getXnormalized();
	float dy = _ga_t0->getYnormalized()-_ga_t1->getYnormalized();

	// return if there is no movement.
	if (dx==0.0f && dy==0.0f)
	{
		return false;
	}

	unsigned int buttonMask = _ga_t1->getButtonMask();
	if (buttonMask==GUIEventAdapter::LEFT_MOUSE_BUTTON)
	{



		float scale = 100.0f;
		osg::Vec3 dv(dx*scale,0.0f,dy*scale);
		//osg::ref_ptr<osg::MatrixTransform>mt=dynamic_cast<osg::MatrixTransform*>(this->viewer->getSceneData());
		//osg::Matrix matrix=mt->getMatrix();
		if (!this->nodeMatrix==NULL)
		{
			osg::Matrix matrix=this->nodeMatrix->getMatrix();
			osg::Vec3 position(0.0f,0.0f,0.0f);
			//position=matrix.getTrans();
			//position=position+dv;
			osg::Matrix translateMatrix;
			translateMatrix.makeTranslate(dv);
			matrix=matrix*translateMatrix;
			// 			mt->setMatrix(matrix);

			this->nodeMatrix->setMatrix(matrix);
		}

		return true;

	}
	else if (buttonMask==GUIEventAdapter::MIDDLE_MOUSE_BUTTON ||
		buttonMask==(GUIEventAdapter::LEFT_MOUSE_BUTTON|GUIEventAdapter::RIGHT_MOUSE_BUTTON))
	{

		// pan model.

		/*osg::ref_ptr<osg::MatrixTransform>mt=dynamic_cast<osg::MatrixTransform*>(this->viewer->getSceneData());
		osg::Matrix matrix=mt->getMatrix();*/
		if (!this->nodeMatrix==0)
		{
			osg::Matrix matrix=this->nodeMatrix->getMatrix();
			if (dy>=0)
			{
				osg::Matrix rotateMatrix;
				rotateMatrix.makeRotate(osg::Quat(osg::PI_2/90,osg::Vec3(0,1,0)));
				matrix=matrix*rotateMatrix;
			}

			else
			{
				osg::Matrix rotateMatrix;
				rotateMatrix.makeRotate(osg::Quat(-osg::PI_2/90,osg::Vec3(0,1,0)));;
				matrix=matrix*rotateMatrix;
			}
			this->nodeMatrix->setMatrix(matrix);
		}



		return true;

	}
	else if (buttonMask==GUIEventAdapter::RIGHT_MOUSE_BUTTON)
	{

		// zoom model.
		float scale =1.1;
		/*osg::ref_ptr<osg::MatrixTransform>mt=dynamic_cast<osg::MatrixTransform*>(this->viewer->getSceneData());
		osg::Matrix matrix=mt->getMatrix();*/
		if (!this->nodeMatrix==0)
		{
			osg::Matrix matrix=this->nodeMatrix->getMatrix();
			if (dy>=0)
			{
				osg::Matrix scaleMatrix;
				scaleMatrix.makeScale(scale,1.0,scale);
				matrix=matrix*scaleMatrix;
			}

			else
			{
				osg::Matrix scaleMatrix;
				scaleMatrix.makeScale(1/scale,1.0,1/scale);
				matrix=matrix*scaleMatrix;
			}
			this->nodeMatrix->setMatrix(matrix);
			return true;
		}
	}

	return false;
}
