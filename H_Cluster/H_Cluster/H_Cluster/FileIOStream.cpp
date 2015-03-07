#include "FileIOStream.h"


FileIOStream::FileIOStream()
{
}


FileIOStream::~FileIOStream()
{
}

void
FileIOStream::writeData(std::string filename, std::string label, cv::Mat writeMat){
	cv::FileStorage fs;

	/*filename = "exampleAM.yml";
	fs.open(filename, cv::FileStorage::WRITE);
	fs << CIM::adjMat;
	fs.release();

	filename = "exampleSIM.yml";
	fs.open(filename, cv::FileStorage::WRITE);
	fs << CIM::ndistance;
	fs.release();

	filename = "exampleCluster.yml";
	fs.open(filename, cv::FileStorage::WRITE);
	fs << CIM::nodeInfo;
	fs.release();*/

	fs.open(filename, cv::FileStorage::WRITE);
	fs << label;
	fs << writeMat;
	fs.release();
}

void
FileIOStream::readData(std::string filename, std::string label, cv::Mat &readMat){
	cv::FileStorage fs;

	/*filename = "exampleAM.yml";
	label = "adjacency matrix";
	filename = "exampleSIM.yml";
	label = "similarity matrix";
	filename = "exampleCluster.yml";
	label = "cluster label";*/

	fs.open(filename, cv::FileStorage::READ);
	fs[label] >> readMat;
	fs.release();
}
