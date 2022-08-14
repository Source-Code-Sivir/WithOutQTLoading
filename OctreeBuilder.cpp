#include"OctreeBuilder.h"
#include"stdafx.h"
void OctreeBuilder::build(int depth, const osg::BoundingBox &total, vector<ElementInfo> LastBoxelements)
{
	OctreeBuilder::eleNum = 0;
	int s[3];
	osg::Vec3 extentSet[3] = { total._min,(total._min + total._max)*0.5f,total._max };
	vector<ElementInfo> curBoxElements;//�����洢��ǰ��Χ���µĹ�����Χ����Ϣ
	int num{ 0 };
	for (unsigned int i = 0; i < LastBoxelements.size(); ++i)
	{
		const osg::BoundingBox& curEleBox = LastBoxelements[i].second;
		if (total.contains(curEleBox._min) && total.contains(curEleBox._max))
		{
			curBoxElements.push_back(LastBoxelements[i]);//
		}
		else if (total.intersects(curEleBox))
		{
			osg::Vec3 center = (curEleBox._max + curEleBox._min)*0.5f;
			if (total.contains(center))
			{
				curBoxElements.push_back(LastBoxelements[i]);
			}
		}
	}
	bool isLeaf = false;
	if (curBoxElements.size() <= maxChildNum || depth > maxTreeDepth)
	{
		isLeaf = true;
	}
	osg::ref_ptr<osg::Group> curLevelGroup = new osg::Group;
	if (!isLeaf)//����Ҷ�ӽڵ��������
	{
		osg::ref_ptr<osg::Group> curLevelChildren[8];//��һ����εİ˸����ӽڵ㣻
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
						max[a] = (extentSet[s[a] + 1])[a];//extentSet ���������Χ��
					}
					int id = s[0] + (2 * s[1]) + (4 * s[2]);//���ӵ�id 1 2 3 4 5 6 7 8
					build(depth + 1, osg::BoundingBox(min, max), curBoxElements);//�������л��֣��۲��Ƿ��ܼ������  ÿ�����Ӽ���ִ������������ж��Ƿ��ܼ������л���
				}
			}
		}
		for (unsigned int i = 0; i < 8; ++i)
		{
			curLevelGroup->addChild(curLevelChildren[i]);
		}
	}
	else//�������ڵ���Ҷ�ӽڵ㣬�Ͳ�Ҫ�����ٴλ���
	{
		for (auto i :curBoxElements)
		{
			++eleNum;
			curLevelGroup->addChild(getIFCElement(i.first));
		}
	}
	//������������pageLOD����
	string childFileName = std::to_string(eleNum) + ".ive";
	osgDB::writeNodeFile(*curLevelGroup, childFileName);
	osg::ref_ptr<osg::PagedLOD> newPage = new osg::PagedLOD;
	newPage->setFileName(0, childFileName);
	float radius = (total._max - total._min).length()*0.5f;
	newPage->setRange(0, 0,10*radius);
	newPage->setCenter((total._max - total._min)*0.5f);
	newPage->setRadius(radius);
	resultNode->addChild(newPage);
}
void OctreeBuilder::generateLOD()
{
	my = new dividBuilding(fileName);
	getBoundingBoxes mybox;
	mybox.calculateBoxes(my->getInternEle());
	build(0, mybox.getSingalBoundingBox(my->getRootNode()), mybox.getBoxes());
	for (auto i : my->getExternEle())
	{
		resultNode->addChild(i);
	}
}
const osg::ref_ptr<osg::Node> OctreeBuilder::getIFCElement(int i)
{
	return my->getInternEle()[i];
}
void OctreeBuilder::GenerateIVE()
{
	string myFile = "test.ive";
	osgDB::writeNodeFile(*resultNode, myFile);
}
void OctreeBuilder::showModel()
{
	osg::ref_ptr<osgViewer::Viewer> myViewer;
	myViewer->setUpViewInWindow(100, 100, 1024, 768);
	myViewer->setSceneData(resultNode);
	myViewer->run();
}