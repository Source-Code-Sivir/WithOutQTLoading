#pragma once
#ifdef _DEBUG
#pragma comment(lib,"osgViewerd.lib")
#pragma comment(lib,"osgDBd.lib")
#pragma comment(lib,"OpenThreadsd.lib")
#pragma comment(lib,"osgd.lib")
#pragma comment(lib,"osgUtild.lib")
#pragma comment(lib,"osgGAd.lib")
#else
#pragma comment(lib,"osgViewer.lib")
#pragma comment(lib,"osgDB.lib")
#pragma comment(lib,"OpenThreads.lib")
#pragma comment(lib,"osg.lib")
#pragma comment(lib,"osgUtil.lib")
#pragma comment(lib,"osgGA.lib")
#endif
#include <osgDB/ReadFile>
#include<osgDB/WriteFile>
#include<osgViewer\Viewer>
#include<vector>
#include<stack>
#include<string>
#include<osg/Array>
#include<unordered_map>
using std::unordered_map;
using std::vector;
using std::string;
using std::stack;

class dividBuilding
{
public:
	dividBuilding(const string fileName_) :fileName(fileName_)
	{ 
		Maps["IfcBuildingElementProxy"] = true;
		Maps["IfcCovering"] = true;
		Maps["IfcBeam"] = true;
		Maps["IfcColumn"] = true;
		Maps["IfcCurtainWall"] = true;
		Maps["IfcDoor"] = true;
		Maps["IfcMember"] = true;
		Maps["IfcPlate"] = 1;
		Maps["IfcRailing"] = 1;
		Maps["IfcRamp"] = 1;
		Maps["IfcSlab"] = 1;
		Maps["IfcStair"] = 1;
		Maps["IfcStairFlight"] = 1;
		Maps["IfcWall"] = 1;
		Maps["IfcWallStandardCase"] = 1;
		Maps["IfcWindow"] = 1;
		Maps["IfcFooting"] = 1;
		Maps["IfcPile"] = 1;
		Maps["IfcBuildingElementComponent"] = 1;
		Maps["IfcBuildingElementProxy"] = 1;
		Maps["IfcFlowSegment"] = 1;
		Maps["IfcFlowFitting"] = 1;
		handleOSG(); 
	};
	string handleOSG();
	const vector<osg::ref_ptr<osg::Node>> getExternEle() { return externComponents; }
	const vector<osg::ref_ptr<osg::Node>> getInternEle() { return internComponents; }
	const osg::ref_ptr<osg::Node> getRootNode() { return rootNode; }
private:
	string fileName;
	osg::ref_ptr<osg::Node> rootNode;
	vector<osg::ref_ptr<osg::Node>> externComponents;
	vector<osg::ref_ptr<osg::Node>> internComponents;
	bool judgeEle() {};//这个函数用来判断墙和板是否为外围构件，现在先不写。
	bool isSlab(std::string ifcname);
	bool isWall(std::string ifcname);
	bool isWindow(std::string ifcname);
	unordered_map<string, bool> Maps;
};