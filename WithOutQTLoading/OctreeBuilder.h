#pragma once
#include"getBoundingBoxes.h"
#include"dividBuilding.h"
#include<osg\PagedLOd>
#include<osgViewer\Viewer>
#include<time.h>
#include<fstream>
class OctreeBuilder
{
public:
	OctreeBuilder(const string &fileName,const string &fol) :num(0),my(nullptr),maxChildNum(128), maxTreeDepth(6), maxLevel(0), fileName(fileName) ,floder(fol){ resultNode = new osg::Group; };
	const osg::ref_ptr<osg::Node> getIFCElement(int i);
	void generateLOD();
	static int eleNum;
	void GenerateIVE();
	osg::ref_ptr<osg::Group> getRoot()const { return resultNode; };
	//void showModel();
	const osg::ref_ptr<osg::Node> getRoot() { return resultNode; }
	void MultiBuilder();
protected:
	int num;
	string fileName, floder;
	vector<string> pagedLODs;
	string createFloder();
	std::vector<double>radius_;
	std::vector<osg::Vec3> centers;
	dividBuilding *my;
	osg::ref_ptr<osg::Group> resultNode;
	int maxChildNum;
	int maxTreeDepth;
	int maxLevel;
	//int TimeDivid,TimeWriteFile;
	void build(int depth, const osg::BoundingBox &total, std::vector<ElementInfo> &elements);
	void PreHandle(const osg::BoundingBox &total, std::vector<ElementInfo> &elements);
	int getVerticsCount(osg::ref_ptr<osg::Group> node) const ;
	osg::ref_ptr<osg::Geode> generateBox(const osg::Vec3& min, const osg::Vec3& max);
};
