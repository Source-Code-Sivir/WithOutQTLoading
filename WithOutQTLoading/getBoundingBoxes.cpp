#include "stdafx.h"
#include"getBoundingBoxes.h"
#include<osg/ShapeDrawable>
#include<osg/PolygonMode>
#include<osg/LineWidth>
#include<osg/Geode>

void getBoundingBoxes::calculateBoxes(const vector<osg::ref_ptr<osg::Node>> nodes)
{
	for (unsigned int i =0;i<nodes.size();++i)
	{
		osg::ComputeBoundsVisitor boundingVisitor;
		nodes[i]->accept(boundingVisitor);
		ElementInfo curInfor;
		curInfor.first = i;
		osg::BoundingBox boundingBox = boundingVisitor.getBoundingBox();
		curInfor.second = boundingVisitor.getBoundingBox().center();
		boundingBoxes.push_back(curInfor);

		/*float length = boundingBox.xMax() - boundingBox.xMin();
		float width = boundingBox.yMax() - boundingBox.yMin();
		float height = boundingBox.zMax() - boundingBox.zMin();
		osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(
			new osg::Box(boundingBox.center(), length, width, height));
		drawable->setColor(osg::Vec4(1.0, 1.0, 0.0, 1.0));

		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
		stateset = drawable->getOrCreateStateSet();
		osg::ref_ptr<osg::PolygonMode> polygonMode = new osg::PolygonMode(
			osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		stateset->setAttributeAndModes(polygonMode);

		osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth(3.0);
		stateset->setAttribute(linewidth);

		osg::Geode *geode = nodes[i]->asGeode();
		geode->addDrawable(drawable);*/
		//return geode;
	}
}
osg::BoundingBox getBoundingBox(const osg::ref_ptr<osg::Node> &node)
{
	osg::ComputeBoundsVisitor boundingVisitor;
	node->accept(boundingVisitor);
	return boundingVisitor.getBoundingBox();//先暂定返回一个包围盒，剩下的以后再说
}
osg::BoundingBox getBoundingBoxes::getSingalBoundingBox(const osg::ref_ptr<osg::Node> &node)
{
	osg::ComputeBoundsVisitor my;	
	node->accept(my);
	return my.getBoundingBox();	
}
