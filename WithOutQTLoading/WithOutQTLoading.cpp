// WithOutQTLoading.cpp : 定义控制台应用程序的入口点。
//
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
#include "stdafx.h"
#include"OctreeBuilder.h"
#include"dividBuilding.h"
#include"getBoundingBoxes.h"
#include<osgViewer\Viewer>
#include<time.h>
#include<iostream>
#include<string>

int main()
{
	time_t now = time(0);
	printf("请输入文件路径\n");
	std::string fileName;
	std::getline(std::cin, fileName);
	OctreeBuilder my(fileName,"");
	my.generateLOD();
	my.GenerateIVE();
	osg::ref_ptr<osgViewer::Viewer> myView = new osgViewer::Viewer;
	myView->setUpViewInWindow(100, 100, 1024, 768);
	myView->setSceneData(my.getRoot());
	printf("time %d\n", time(0) - now);
    return myView->run();
}

