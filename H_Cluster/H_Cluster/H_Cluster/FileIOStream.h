#pragma once

#ifndef FileIOStream_H
#define FileIOStream_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>

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
	void writeData(std::string filename, std::string label, std::list<cv::Point2i> writeL);

	void writeData(std::string filename, std::string label, std::list<cv::Point2f> writeL);
	void writeData(cv::FileStorage &fs, std::list<cv::Point2i> writeL);
	void writeData(std::string filename, std::string label, std::list< std::list<cv::Point2i> > writeLL);
	void writeData(cv::FileStorage &fs, std::list<cv::Point2f> writeL);
	void writeData(std::string filename, std::string label, std::list< std::list<cv::Point2f> > writeLL);

	void readData(std::string filename, std::string label, cv::Mat &readMat);
	void readData(std::string filename, std::string label, std::list<cv::Point2i> &readL);

	void readData(std::string filename, std::string label, std::list<cv::Point2f> &readL);
	void readData(cv::FileNode &fn, std::list<cv::Point2i> &readL);
	void readData(std::string filename, std::string label, std::list< std::list<cv::Point2i> > &readLL);
	void readData(cv::FileNode &fn, std::list<cv::Point2f> &readL);
	void readData(std::string filename, std::string label, std::list< std::list<cv::Point2f> > &readLL);
};


#endif // !1