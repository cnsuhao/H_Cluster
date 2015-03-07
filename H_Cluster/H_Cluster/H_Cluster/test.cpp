#include "CIM.h"
#include "FileIOStream.h"

int main(){
	FileIOStream fios;
	cv::Mat m;
	std::string filename = "exampleAM.yml";
	std::string label = "adjacency matrix";
	fios.readData(filename, label, m);

	CIM cim, *cim_ptr;
	
	cim.start(m);

	//cim_ptr = new CIM();
	//cim = *cim_ptr;

	//cim.check();

	getchar();
	return 0;
}

int readExample(){
	cv::FileStorage fs("test.xml", cv::FileStorage::WRITE);
	fs << "frameCount" << 5;
	time_t rawtime; 
	time(&rawtime);
	fs << "calibrationDate" << asctime(localtime(&rawtime));
	cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 1000, 0, 320, 0, 1000, 240, 0, 0, 1); //��һ��Mat��ʼ����ʽ
	cv::Mat distCoeffs = (cv::Mat_<double>(5, 1) << 0.1, 0.01, -0.001, 0, 0);
	fs << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;

	//featuresΪһ����СΪ3������,����ÿ��Ԫ���������x,y�ʹ�СΪ8��uchar�������
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
