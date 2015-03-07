#pragma once

#ifndef FileIOStream_H
#define FileIOStream_H

#include <opencv2/opencv.hpp>
#include <iostream>

/*filename = "exampleAM.yml";
label = "adjacency matrix";
filename = "exampleSIM.yml";
label = "similarity matrix";
filename = "exampleCluster.yml";
label = "cluster label"; */

/*
writeData("exampleAM.yml", "adjacency matrix", CIM::adjMat);
writeData("exampleSIM.yml", "similarity matrix", CIM::ndistance);
writeData("exampleCluster.yml", "cluster label", CIM::nodeInfo);
*/

class FileIOStream
{
public:
	FileIOStream();
	~FileIOStream();
	//Must only a yaml file, content only a adjacency Matrix, label is "adjacency Matrix"
	void writeData(std::string filename, std::string label, cv::Mat writeMat);
	void readData(std::string filename, std::string label, cv::Mat &readMat);
};

#endif // !1