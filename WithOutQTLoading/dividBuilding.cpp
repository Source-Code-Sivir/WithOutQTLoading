#include "stdafx.h"
#include"dividBuilding.h"


void splitGeo(osg::Geode* Geode, int n, vector <osg::ref_ptr<osg::Node>>& container);

string dividBuilding::handleOSG()
{
	printf("begin dividing");
	rootNode = osgDB::readNodeFile(fileName);
	if (!rootNode)
	{
		printf("file no exist");
		exit(1);
	}
		
	osg::ref_ptr<osg::Node> rootGroup = rootNode->asGroup();
	if (!rootGroup) return "Error: From root node to group";
	stack<osg::ref_ptr<osg::Node>> pendingNodes;
	pendingNodes.push(rootNode);
	const char *group = "Group";
	const char *switch_ = "Switch";
	const char* geode_ = "Geode";
	while (!pendingNodes.empty())
	{
		osg::ref_ptr<osg::Node>	curNode = pendingNodes.top();
		pendingNodes.pop();
		printf("%s\n",curNode->getName().c_str());
		std::string tempName (curNode->getName());
		if (tempName[0] == '#')
		{
			tempName = tempName.substr(tempName.find('=')+1,tempName.size()- tempName.find('=')-1);
		}
		//printf("%s", tempName.c_str());
		printf("%s", curNode->getName().c_str());
		if (Maps.find(tempName) != Maps.end())//判断是否为ifc实体
		{
			if (isWall(curNode->getName()) || isSlab(curNode->getName()))
			{
				/*std::string curName = curNode->getName();
				curNode->setName(curName.c_str());*/
				externComponents.push_back(curNode);
			}
			else
			{
				curNode->setName(curNode->getName().c_str());
				internComponents.push_back(curNode);
			}
		}
		else
		{
			osg::Group* curGroup = curNode->asGroup();
			if (!curGroup || 0 == curGroup->getNumChildren())
				continue;
			else
			{
				for (unsigned int i = 0; i < curGroup->getNumChildren(); ++i)
				{
					printf("%s ", curGroup->getChild(i)->className());
					if (curGroup->getChild(i) && !strncmp(geode_, curGroup->getChild(i)->className(), 5))
					{
						splitGeo(curGroup->getChild(i)->asGeode(), 9000 ,internComponents);
						//internComponents.push_back(curGroup->getChild(i));
					}
					else if (curGroup->getChild(i) && curGroup->getChild(i) != curNode)
						pendingNodes.push(curGroup->getChild(i));
					else
						continue;
				}
			}
		}
	}
	printf("end dividing");
	return "Done";
}
bool dividBuilding::isWall(std::string ifcname)
{
	for (int i = 0; i < ifcname.size(); ++i)
	{
		if (ifcname[i] != 'W')
			continue;
		if ('a' == ifcname[i + 1] && 'l' == ifcname[i + 2] && 'l' == ifcname[i + 3])
			return true;
	}
	return false;
}
bool dividBuilding::isWindow(std::string ifcname)
{
	for (int i = 0; i < ifcname.size(); ++i)
	{
		if (ifcname[i] != 'W')
			continue;
		if ('i' == ifcname[i + 1] && 'n' == ifcname[i + 2] && 'd' == ifcname[i + 3])
			return true;
	}
	return false;
}
bool dividBuilding::isSlab(std::string ifcname)
{
	for (int i = 0; i < ifcname.size(); ++i)
	{
		if (ifcname[i] != 'S')
			continue;
		if ('l' == ifcname[i + 1] && 'a' == ifcname[i + 2] && 'b' == ifcname[i + 3])
			return true;
	}
	return false;
}

void splitGeo(osg::Geode* Geode, int n, vector <osg::ref_ptr<osg::Node>>& container)
{
	if (!Geode->getNumDrawables()) return;
	osg::ref_ptr<osg::Geometry> Geom = Geode->getDrawable(0)->asGeometry();
	if (Geom == NULL) {
		printf("geom is null\n");
		return;
	}
	osg::Vec3Array* curVecs = dynamic_cast<osg::Vec3Array*>(Geom->getVertexArray());
	int totalSize = curVecs->getNumElements();
	if (totalSize <= n)
	{
		container.push_back(Geode);
		return;
	}
	osg::Vec3Array* curNormal = dynamic_cast<osg::Vec3Array*>(Geom->getNormalArray());
	osg::Vec2Array* curText = dynamic_cast<osg::Vec2Array*>(Geom->getTexCoordArray(0));
	osg::ref_ptr <osg::StateSet> curStatset = Geom->getStateSet();
	for (int i = 0; i < totalSize / n; ++i)
	{
		int curEnd = (i + 1) * n < totalSize-1 ? ((i + 1) * n) : totalSize-1;
		osg::ref_ptr<osg::Vec3Array> splitVecs = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec3Array> splitNormals = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec2Array> splitTexts = new osg::Vec2Array;
		for (int j = i * n; j < curEnd; ++j)
		{
			if(curVecs)
				splitVecs->push_back(curVecs[0][j]);
			if(curNormal)
				splitNormals->push_back(curNormal[0][j]);
			if(curText)
				splitTexts->push_back(curText[0][j]);
		}
		osg::ref_ptr<osg::Geometry> splitGeom = new osg::Geometry;
		splitGeom->setVertexArray(splitVecs);
		splitGeom->setNormalArray(splitNormals,osg::Array::BIND_PER_VERTEX);
		splitGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, splitVecs->size()));
		//splitGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
		if (curText)
		{
			splitGeom->setTexCoordArray(0, splitTexts);
		}
			
		splitGeom->setStateSet(curStatset);
		osg::ref_ptr<osg::Geode> curGeode = new osg::Geode;
		curGeode->addDrawable(splitGeom);
		//osgDB::writeNodeFile(*curGeode, "test.osg");
		container.push_back(curGeode.release());
	}
}