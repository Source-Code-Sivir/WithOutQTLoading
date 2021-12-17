#pragma once
#include<osg\Node>
#include <osg/ComputeBoundsVisitor>
#include<vector>
#include <osg/Group>
#include<string>
#include<list>
using std::string;
using std::vector;
typedef std::pair<int, osg::Vec3> ElementInfo;//需要一个int的编号将包围盒与对应的node对应起来。
class getBoundingBoxes 
{
public:
	getBoundingBoxes(){};
	void calculateBoxes(const vector <osg::ref_ptr<osg::Node>> nodes);
	osg::BoundingBox getSingalBoundingBox(const osg::ref_ptr<osg::Node> &node);
	std::vector<ElementInfo> &getBoxes() { return boundingBoxes; }
private:
	std::vector<ElementInfo> boundingBoxes;
};