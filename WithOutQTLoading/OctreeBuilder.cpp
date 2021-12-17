#include "stdafx.h"
#include"OctreeBuilder.h"
#include<thread>
#include<mutex>
#include<osg/ShapeDrawable>
#include<osg/material>
//#include<osgUtil/Statics>
std::mutex mut;
int OctreeBuilder::eleNum = 0;
void OctreeBuilder::build(int depth, const osg::BoundingBox &total, std::vector<ElementInfo> &LastBoxelements)
{
	if (depth == 1)
		printf("thread\n");
	//printf("begin octree");
	int s[3];
	osg::Vec3 extentSet[3] = { total._min,(total._min + total._max)*0.5f,total._max };
	std::vector<ElementInfo> curBoxElements;//用来存储当前包围盒下的构件包围盒信息
	if (depth == 0)
	{
		curBoxElements = LastBoxelements;
	}
	else
	{
		for (auto i = LastBoxelements.begin(); i != LastBoxelements.end();++i)
		{
			const osg::Vec3& curEleBox = (*i).second;
			if (total.contains(curEleBox))
			{
				curBoxElements.push_back(*i);
			}
		}
	}
	
	bool isLeaf = false;
	if (curBoxElements.size() <= maxChildNum || depth > maxTreeDepth)
	{
		isLeaf = true;
	}
	osg::ref_ptr<osg::Group> curLevelGroup = new osg::Group;
	if (!isLeaf)//不是叶子节点继续划分
	{
		time_t initialDivid = time(0);
		for (s[0] = 0; s[0] < 2; ++s[0])
		{
			for (s[1] = 0; s[1] < 2; ++s[1])
			{
				for (s[2] = 0; s[2] < 2; ++s[2])
				{
					osg::Vec3 min, max;
					for (int a = 0; a < 3; ++a)
					{
						min[a] = (extentSet[s[a] + 0])[a];
						max[a] = (extentSet[s[a] + 1])[a];//extentSet 是整个大包围盒
					}
					build(depth + 1, osg::BoundingBox(min, max), curBoxElements);//迭代进行划分，观察是否还能继续拆分  每个孩子继续执行这个操作，判断是否能继续进行划分
				}
			}
		}
	}
	
	else//如果这个节点是叶子节点，就不要进行再次划分
	{
		time_t initial_ = time(0);
		for (const auto &i : curBoxElements)
		{
			curLevelGroup->addChild(getIFCElement(i.first));
		}
		//int num = time(0);
		//string myFile = fileName.substr(fileName.find_last_of('\\')+1, fileName.find_last_of('.')- fileName.find_last_of('\\')-1);//找到file的名称 即当前ive文件的名称
		string myFile = "model";
		mut.lock();
		++num;
		string childFileName = floder+"/"+ myFile + "/" + myFile + std::to_string(depth) + "-" + std::to_string(num) + ".nmo";
		/*if (depth == 1)
			printf("抓到你了！");*/
		if(!osgDB::writeNodeFile(*curLevelGroup, childFileName))
			printf(childFileName.c_str(),"\n");
		pagedLODs.push_back("/" + myFile + "/" + myFile+std::to_string(depth) + "-"  + std::to_string(num) + ".nmo");
		mut.unlock();
		float radius = (total._max - total._min).length()*0.5f;
		radius_.push_back(radius);
		centers.push_back((total._max + total._min)*0.5);
		//resultNode->addChild(generateBox(total._min, total._max));
	}
	//接下来是生成pageLOD部分
}
void OctreeBuilder::generateLOD()
{
	time_t begin = time(0);
	createFloder();
	my = new dividBuilding(fileName);
	getBoundingBoxes mybox;
	mybox.calculateBoxes(my->getInternEle());
	for (auto i : my->getExternEle())
	{
		resultNode->addChild(i);
	}
	//build(0, mybox.getSingalBoundingBox(my->getRootNode()), mybox.getBoxes());
	osg::BoundingBox totalBox = mybox.getSingalBoundingBox(my->getRootNode());
	PreHandle(totalBox, mybox.getBoxes());
	int j = my->getExternEle().size();
	//printf("\n%d", j);
	if (j == 0)
	{
		resultNode->addChild(generateBox(totalBox._min, totalBox._max));
	}
	printf("total time %d", time(0) - begin);

	for (int i=0;i<pagedLODs.size();++i)
	{
		osg::ref_ptr<osg::PagedLOD> newPage = new osg::PagedLOD;
		newPage->setFileName(0, pagedLODs[i]);
		//printf("%s\n",pagedLODs[i].c_str());
		//float radius = (total._max - total._min).length()*0.5f;
		float dis = radius_[i] * 3.0f;
		//float total_len = (totalBox._max - totalBox._min).length()/15;
		if (dis > 80) dis = 100;
		newPage->setRange(0, 0, dis);
		newPage->setCenter(centers[i]);
		newPage->setRadius(radius_[i]);
		resultNode->addChild(newPage);
	}
}
const osg::ref_ptr<osg::Node> OctreeBuilder::getIFCElement(int i)
{
	return my->getInternEle()[i];
}
void OctreeBuilder::GenerateIVE()
{
	//string myFile = fileName.substr(fileName.find_last_of('\\')+1, fileName.find_last_of('.')- fileName.find_last_of('\\')-1)+".nmo";
	string myFile = "model.nmo";
	//string myFile = "test.ive";
	string tempUSe = floder + "\\" + myFile;
	string temp2 = floder + "\\" + "model";
	osgDB::writeNodeFile(*resultNode, tempUSe);
	string myNmi = fileName.substr(fileName.find_last_of('\\')+1, fileName.find_last_of('.')- fileName.find_last_of('\\')-1)+".nmi";
}

string OctreeBuilder::createFloder()	
{
	int pos1 = fileName.find_last_of('.');
	int pos2 = fileName.find_last_of('\\');
	string filePath = "mkdir -p	";
	//filePath = filePath +"D:\\NMBIM\\fileData" + fileName.substr(pos2,pos1);
	floder = "D:\\NMBIM\\fileData\\total";
	filePath = filePath + floder;
	string floderPathOrder = filePath + "\\model";
	if (system(filePath.c_str())) 
		floder = filePath.substr(filePath.find_first_of('D'), filePath.size());
	system(floderPathOrder.c_str());
	return "false";
}
void OctreeBuilder::MultiBuilder()
{

}
void OctreeBuilder::PreHandle(const osg::BoundingBox &total, std::vector<ElementInfo> &elements)
{
	vector< vector<ElementInfo> > SubElement(8);
	vector<osg::BoundingBox> SubBoxes;
	int s[3];
	osg::Vec3 extentSet[3] = { total._min,(total._min + total._max)*0.5f,total._max };
	for (s[0] = 0; s[0] < 2; ++s[0])
	{
		for (s[1] = 0; s[1] < 2; ++s[1])
		{
			for (s[2] = 0; s[2] < 2; ++s[2])
			{
				osg::Vec3 min, max;
				for (int a = 0; a < 3; ++a)
				{
					min[a] = (extentSet[s[a] + 0])[a];
					max[a] = (extentSet[s[a] + 1])[a];//extentSet 是整个大包围盒
				}
				SubBoxes.push_back(osg::BoundingBox(min, max));//迭代进行划分，观察是否还能继续拆分  每个孩子继续执行这个操作，判断是否能继续进行划分
			}
		}
	}
	for (auto i = elements.begin();i!=elements.end();++i)
	{
		for (unsigned int k = 0; k < 8; ++k)
		{
			if (SubBoxes[k].contains((*i).second))
			{
				SubElement[k].push_back(*i);
				break;
			}
		}
	}
	//build(0, total, elements);
	std::thread oc1(&OctreeBuilder::build, this, 1, SubBoxes[0], SubElement[0]);
	std::thread oc2(&OctreeBuilder::build, this, 1, SubBoxes[1], SubElement[1]);
	std::thread oc3(&OctreeBuilder::build, this, 1, SubBoxes[2], SubElement[2]);
	std::thread oc4(&OctreeBuilder::build, this, 1, SubBoxes[3], SubElement[3]);
	std::thread oc5(&OctreeBuilder::build, this, 1, SubBoxes[4], SubElement[4]);
	std::thread oc6(&OctreeBuilder::build, this, 1, SubBoxes[5], SubElement[5]);
	std::thread oc7(&OctreeBuilder::build, this, 1, SubBoxes[6], SubElement[6]);
	std::thread oc8(&OctreeBuilder::build, this, 1, SubBoxes[7], SubElement[7]);
	oc1.join();
	oc2.join();
	oc3.join();
	oc4.join();
	oc5.join();
	oc6.join();
	oc7.join();
	oc8.join();
}

int OctreeBuilder::getVerticsCount(osg::ref_ptr<osg::Group> node) const
{
	//osg::ref_ptr <osgUtil::StatsVisitor>
	return 0;
}
osg::ref_ptr<osg::Geode> OctreeBuilder::generateBox(const osg::Vec3& min, const osg::Vec3& max)
{
	osg::ref_ptr<osg::Box> box = new osg::Box();
	osg::Vec3 center = (min + max) / 2;
	//osg::BoundingBox totalBox = mybox.getSingalBoundingBox(my->getRootNode());
	box->setCenter(center);
	box->setHalfLengths((max - min) / 2);
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(box.get());

	osg::ref_ptr<osg::Geode> boxGeode = new osg::Geode();
	boxGeode->addDrawable(sd.get());

	osg::Vec4 color(0.5, 0.5, 0.5, 0.2f);
	osg::ref_ptr < osg::Material> material = new osg::Material();
	material->setDiffuse(osg::Material::FRONT_AND_BACK, color);
	material->setSpecular(osg::Material::FRONT_AND_BACK, color);
	material->setAmbient(osg::Material::Face::FRONT_AND_BACK, color);//环境光
	//material->setDiffuse(osg::Material::, osg::Vec4(0.5, 0.5, 0.5, 0.2f));
	//material->setSpecular(osg::Material::FRONT, osg::Vec4(0.5, 0.5, 0.5, 0.2f));

	osg::ref_ptr<osg::StateSet>  m_glass_stateset;
	m_glass_stateset = new osg::StateSet();
	m_glass_stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
	m_glass_stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	m_glass_stateset->setAttributeAndModes(material, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);

	boxGeode->setStateSet(m_glass_stateset);
	//boxGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, text, osg::StateAttribute::ON);
	return boxGeode.release();
}