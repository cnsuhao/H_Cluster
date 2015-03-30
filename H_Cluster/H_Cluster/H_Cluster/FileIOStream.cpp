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

	if (!fs.isOpened())
	{
		std::cerr << "Failed to open " << std::endl;
		return;
	}

	fs << label;
	fs << writeMat;
	fs.release();
}

void
FileIOStream::writeData(std::string filename, std::string label, std::list<cv::Point2i> _writeL){
	cv::FileStorage fs;
	std::list<cv::Point2i> writeL = _writeL;
	std::list<cv::Point2i>::iterator writeL_itr;
	fs.open(filename, cv::FileStorage::WRITE);

	if (!fs.isOpened())
	{
		std::cerr << "Failed to open " << std::endl;
		return;
	}

	fs << label;
	FileIOStream::writeData(fs, _writeL);
	fs.release();
}


void
FileIOStream::writeData(std::string filename, std::string label, std::list<cv::Point2f> _writeL){
	cv::FileStorage fs;
	std::list<cv::Point2f> writeL = _writeL;
	std::list<cv::Point2f>::iterator writeL_itr;
	fs.open(filename, cv::FileStorage::WRITE);

	if (!fs.isOpened())
	{
		std::cerr << "Failed to open " << std::endl;
		return;
	}

	fs << label;
	FileIOStream::writeData(fs, _writeL);
	fs.release();
}

void
FileIOStream::writeData(cv::FileStorage &fs, std::list<cv::Point2i> _writeL){
	std::list<cv::Point2i> writeL = _writeL;
	std::list<cv::Point2i>::iterator writeL_itr;

	if (!fs.isOpened())
	{
		std::cerr << "Failed to open " << std::endl;
		return;
	}

	fs << "[:";
	writeL_itr = writeL.begin();
	while (writeL_itr != writeL.end()){
		fs << *writeL_itr;
		writeL_itr++;
	}
	fs << "]";
}

void
FileIOStream::writeData(std::string filename, std::string label, std::list< std::list<cv::Point2i> > _write2L){
	cv::FileStorage fs;
	std::list< std::list<cv::Point2i> > writeLL = _write2L;
	std::list< std::list<cv::Point2i> >::iterator write2L_itr;
	std::list<cv::Point2i> writeL;
	std::list<cv::Point2i>::iterator writeL_itr;
	
	write2L_itr = writeLL.begin();
	fs.open(filename, cv::FileStorage::WRITE);
	fs << label;

	fs << "[:";
	while (write2L_itr != writeLL.end()){
		FileIOStream::writeData(fs, *write2L_itr);

		/*writeL = *writeLL_itr;

		fs << "[:";
		writeL_itr = writeL.begin();
		while (writeL_itr != writeL.end()){
			fs << *writeL_itr;
			writeL_itr++;
		}
		fs << "]";*/


		write2L_itr++;
		//fs << ", ";
	}
	fs << "]";
	fs.release();
}


void
FileIOStream::writeData(cv::FileStorage &fs, std::list<cv::Point2f> _writeL){
	std::list<cv::Point2f> writeL = _writeL;
	std::list<cv::Point2f>::iterator writeL_itr;

	if (!fs.isOpened())
	{
		std::cerr << "Failed to open " << std::endl;
		return;
	}

	fs << "[:";
	writeL_itr = writeL.begin();
	while (writeL_itr != writeL.end()){
		fs << *writeL_itr;
		writeL_itr++;
	}
	fs << "]";
}

void
FileIOStream::writeData(std::string filename, std::string label, std::list< std::list<cv::Point2f> > _write2L){
	cv::FileStorage fs;
	std::list< std::list<cv::Point2f> > writeLL = _write2L;
	std::list< std::list<cv::Point2f> >::iterator write2L_itr;
	std::list<cv::Point2f> writeL;
	std::list<cv::Point2f>::iterator writeL_itr;

	write2L_itr = writeLL.begin();
	fs.open(filename, cv::FileStorage::WRITE);
	fs << label;

	fs << "[:";
	while (write2L_itr != writeLL.end()){
		FileIOStream::writeData(fs, *write2L_itr);

		/*writeL = *writeLL_itr;

		fs << "[:";
		writeL_itr = writeL.begin();
		while (writeL_itr != writeL.end()){
		fs << *writeL_itr;
		writeL_itr++;
		}
		fs << "]";*/


		write2L_itr++;
		//fs << ", ";
	}
	fs << "]";
	fs.release();
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

	if (!fs.isOpened())
	{
		std::cerr << "Failed to open " << std::endl;
		return;
	}

	fs[label] >> readMat;
	fs.release();
}

void
FileIOStream::readData(std::string filename, std::string label, std::list<cv::Point2i> &_readL){
	cv::FileStorage fs;
	cv::FileNode fn;
	cv::FileNodeIterator it, it_end;
	std::list<cv::Point2i>  &readL = _readL;
	//std::list<cv::Point2i>::iterator readL_itr;

	fs.open(filename, cv::FileStorage::READ);
	if (!fs.isOpened())
	{
		std::cerr << "Failed to open " << std::endl;
		return;
	}

	fn = fs[label];
	//it = fn.begin();
	//it_end = fn.end();

	//readL.clear();
	//while (it != it_end){
	//	cv::Point2i tmp;
	//	it >> tmp;
	//	readL.push_back(tmp);
	//	//it++;
	//}

	FileIOStream::readData(fn,readL);

	fs.release();
}


void
FileIOStream::readData(std::string filename, std::string label, std::list<cv::Point2f> &_readL){
	cv::FileStorage fs;
	cv::FileNode fn;
	cv::FileNodeIterator it, it_end;
	std::list<cv::Point2f>  &readL = _readL;
	//std::list<cv::Point2i>::iterator readL_itr;

	fs.open(filename, cv::FileStorage::READ);
	if (!fs.isOpened())
	{
		std::cerr << "Failed to open " << std::endl;
		return;
	}

	fn = fs[label];
	//it = fn.begin();
	//it_end = fn.end();

	//readL.clear();
	//while (it != it_end){
	//	cv::Point2i tmp;
	//	it >> tmp;
	//	readL.push_back(tmp);
	//	//it++;
	//}

	FileIOStream::readData(fn, readL);

	fs.release();
}

void
FileIOStream::readData(cv::FileNode &_fn, std::list<cv::Point2i> &_readL){
	cv::FileNode &fn = _fn;
	cv::FileNodeIterator it, it_end;
	std::list<cv::Point2i>  &readL = _readL;
	//std::list<cv::Point2i>::iterator readL_itr;

	it = fn.begin();
	it_end = fn.end();

	readL.clear();
	while (it != it_end){
		cv::Point2i tmp;
		it >> tmp;
		readL.push_back(tmp);
		//it++;
	}
}

void
FileIOStream::readData(std::string filename, std::string label, std::list< std::list<cv::Point2i> > &_read2L){
	cv::FileStorage fs;
	cv::FileNode fn, fn2;
	cv::FileNodeIterator it, it_end, it2, it_end2;
	std::list< std::list<cv::Point2i> >  &read2L = _read2L;
	std::list<cv::Point2i>::iterator readL_itr;

	fs.open(filename, cv::FileStorage::READ);
	if (!fs.isOpened())
	{
		std::cerr << "Failed to open " << std::endl;
		return;
	}

	fn = fs[label];
	it = fn.begin();
	it_end = fn.end();

	read2L.clear();

	while (it != it_end){
	//for (int i = 0; i < fn.size(); i++){
		std::list<cv::Point2i> tmp;
		tmp.clear();
		//tmp.push_back(cv::Point2i(1, 1));
		/*FileIOStream::readData((*it), "", tmp);*/
		
		fn2 = *it;
		//fn2 = fn[i];
		it2 = fn2.begin();
		it_end2 = fn2.end();

		while (it2 != it_end2){
			cv::Point2i tmp2;
			it2 >> tmp2;
			//std::cout << tmp2 << " ";
			tmp.push_back(tmp2);
			//it++;
		}

		//std::cout << std::endl;
		read2L.push_back(tmp);
		it++;
	}
	

	/*while (it != it_end){
		std::list<cv::Point2i> tmp;
		FileIOStream::readData(, "", tmp);
		readLL.push_back(tmp);
		it++;
	}*/
	fs.release();
}



void
FileIOStream::readData(cv::FileNode &_fn, std::list<cv::Point2f> &_readL){
	cv::FileNode &fn = _fn;
	cv::FileNodeIterator it, it_end;
	std::list<cv::Point2f>  &readL = _readL;
	//std::list<cv::Point2i>::iterator readL_itr;

	it = fn.begin();
	it_end = fn.end();

	readL.clear();
	while (it != it_end){
		cv::Point2f tmp;
		it >> tmp;
		readL.push_back(tmp);
		//it++;
	}
}


void
FileIOStream::readData(std::string filename, std::string label, std::list< std::list<cv::Point2f> > &_read2L){
	cv::FileStorage fs;
	cv::FileNode fn, fn2;
	cv::FileNodeIterator it, it_end, it2, it_end2;
	std::list< std::list<cv::Point2f> >  &read2L = _read2L;
	std::list<cv::Point2i>::iterator readL_itr;

	fs.open(filename, cv::FileStorage::READ);
	if (!fs.isOpened())
	{
		std::cerr << "Failed to open " << std::endl;
		return;
	}

	fn = fs[label];
	it = fn.begin();
	it_end = fn.end();

	read2L.clear();

	while (it != it_end){
		//for (int i = 0; i < fn.size(); i++){
		std::list<cv::Point2f> tmp;
		tmp.clear();
		//tmp.push_back(cv::Point2i(1, 1));
		/*FileIOStream::readData((*it), "", tmp);*/

		fn2 = *it;
		//fn2 = fn[i];
		it2 = fn2.begin();
		it_end2 = fn2.end();

		while (it2 != it_end2){
			cv::Point2f tmp2;
			it2 >> tmp2;
			//std::cout << tmp2 << " ";
			tmp.push_back(tmp2);
			//it++;
		}

		//std::cout << std::endl;
		read2L.push_back(tmp);
		it++;
	}


	/*while (it != it_end){
	std::list<cv::Point2i> tmp;
	FileIOStream::readData(, "", tmp);
	readLL.push_back(tmp);
	it++;
	}*/
	fs.release();
}