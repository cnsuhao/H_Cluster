#include "H_Cluster.h"
#include "FileIOStream.h"

int main(){
	FileIOStream fios;
	cv::Mat m;
	std::string filename1 = "exampleAM.yml";
	std::string label1 = "adjacency matrix";
	std::string filename2 = "exampleSim.yml";
	std::string label2 = "similarity";
	std::string filename3 = "exampleLabel.yml";
	std::string label3 = "cluster label";

	std::string filename4 = "exampleCluster_q.yml";
	std::string label4 = "cluster q";

	std::string filename5 = "exampleCluster_is.yml";
	std::string label5 = "cluster is";

	std::string filename6 = "exampleCluster_ds.yml";
	std::string label6 = "cluster ds";

	std::string filename7 = "exampleCluster.yml";
	std::string label7 = "cluster cluster";

	//fios.readData(filename1, label1, m);

	HCluster HCluster, *HCluster_ptr;
	
	//HCluster.start(m);
	HCluster.start();


	fios.writeData(filename1, label1, HCluster.getAdjMat());
	fios.writeData(filename2, label2, HCluster.getSim());
	fios.writeData(filename3, label3, HCluster.getNodeInfo());

	fios.writeData(filename4, label4, HCluster.getCluster_q());
	fios.writeData(filename5, label5, HCluster.getCluster_is());
	fios.writeData(filename6, label6, HCluster.getCluster_ds());
	fios.writeData(filename7, label7, HCluster.getCluster());


	//HCluster_ptr = new HCluster();
	//HCluster = *HCluster_ptr;

	//HCluster.check();

	std::cout << "h_cluster end" << std::endl;
	getchar();
	return 0;
}

int readExample(){
	cv::FileStorage fs("test.xml", cv::FileStorage::WRITE);
	fs << "frameCount" << 5;
	time_t rawtime; 
	time(&rawtime);
	fs << "calibrationDate" << asctime(localtime(&rawtime));
	cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 1000, 0, 320, 0, 1000, 240, 0, 0, 1); //又一种Mat初始化方式
	cv::Mat distCoeffs = (cv::Mat_<double>(5, 1) << 0.1, 0.01, -0.001, 0, 0);
	fs << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;

	//features为一个大小为3的向量,其中每个元素由随机数x,y和大小为8的uchar数组组成
	fs << "features" << "[";
	for (int i = 0; i < 3; i++)
	{
		int x = rand() % 640;
		int y = rand() % 480;
		uchar lbp = rand() % 256;
		fs << "{:" << "x" << x << "y" << y << "lbp" << "[:";
		for (int j = 0; j < 8; j++)
			fs << ((lbp >> j) & 1);
		fs << "]" << "}";
	}
	fs << "]";
	fs.release();
	return 0;
}

int test(){
	std::string filename;
	filename = "test.yml";
	cv::FileStorage fs;
	fs.open(filename, cv::FileStorage::WRITE);

	cv::Mat wM = (cv::Mat_<float>(3, 3) << 1.0, 2.0, 3.0, 4.5, 5.6, 6.7, 7.7, 8.8, 9.9);

	fs << "adjacency Matrix";
	fs << wM;
	fs.release();

	fs.open(filename, cv::FileStorage::READ);

	cv::Mat rM;
	fs["adjacency Matrix"] >> rM;
	fs.release();

	std::cout << rM << std::endl;
	return 0;
}

int main3(){
	test();
	return 0;
}
