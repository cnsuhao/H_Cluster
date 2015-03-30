#pragma once

#ifndef HCluster_h
#define HCluster_h

#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <time.h>

//#define NODENUM 8
//#define NODENUM 5

//#define NODENUM 14

#define NODENUM 100

class HCluster
{
public:
	HCluster();
	~HCluster();
	void start();
	void start(cv::Mat aM);

	//计算相似度asim
	void compute_sim(cv::Mat adjMatrix, std::list<cv::Point3f> &edge, cv::Mat &ndist);
	
	//计算社区的quota值
	void compute_qcth(std::list<cv::Point2f> cluster_is, std::list<cv::Point2f> cluster_ds,
		float graph_ts, int cth, cv::Point2f &cq);
	//计算ts值，即将所有边的sim求和
	void compute_ts(cv::Mat dist, float &cluster_ts);
	//计算社区的is的值
	void compute_is(cv::Mat dist, std::list< std::list<cv::Point2i> > cluster, int cth, cv::Point2f &cis);
	//计算社区的ds的值
	void compute_ds(cv::Mat dist, std::list< std::list<cv::Point2i> > cluster, int cth, cv::Point2f &cds);
	//计算所有社区的q值的和
	void compute_allq(std::list<cv::Point2f> cluster_q, float &q);
	//调整社区，将社区合并
	void adjust_cluster(std::list< std::list<cv::Point2i> > &cluster, std::list<cv::Point2f> &cluster_is, 
		std::list<cv::Point2f> &cluster_ds, std::list<cv::Point2f> &cluster_q, std::vector<int> clusterth);
	//对边进行排序，求取asim值最大的边
	void sort_edge(std::list<cv::Point3f> &edge_dectct);
	//分层聚类，每次选取一个asim值最大的边，该边是连接不同社区的，计算该边连接的社区融合后Q值是否增加了，若增加了就融合，否则撤销这次操作，并结束
	void H_cluster();
	//计算所有社区的is，ds和q值
	void compute_is_ds_q();
	//计算某个社区的is，ds和q值
	void compute_is_ds_qth(int cth);

	void check();
	//获取邻接矩阵
	cv::Mat getAdjMat();
	//返回所有边的sim值
	cv::Mat getSim();
	//获取该节点的信息
	cv::Mat getNodeInfo();
	//获取该社区的is值
	std::list<cv::Point2f> getCluster_is();
	//获取该社区的ds值
	std::list<cv::Point2f> getCluster_ds();
	//获取该社区的q值
	std::list<cv::Point2f> getCluster_q();
	//获取一个社区节点
	std::list< std::list<cv::Point2i> > getCluster();
	//复制将要修改的信息
	void cpyInfo(std::list< std::list<cv::Point2i> > &cluster, std::vector<int> clusterth, std::list< cv::Point3f > &is_ds_qInfo/*, std::vector<float> &nodeInfo_cpy*/, std::list< std::list<cv::Point2i> > &cpyL);
	//还原信息，恢复到上一次的状态
	void recoveryInfo(std::list< std::list<cv::Point2i> > &cluster, std::vector<int> cluster_arr, std::list< cv::Point3f > is_ds_qInfo/*, std::vector<float> nodeInfo_cpy*/, std::list< std::list<cv::Point2i> > cpyL);


private:
	cv::Mat ndistance;
	cv::Mat nodeInfo;
	cv::Mat adjMat;
	std::list<cv::Point2f> cluster_is;
	std::list<cv::Point2f> cluster_ds;
	std::list<cv::Point2f> cluster_q;
	std::list< std::list<cv::Point2i> > cluster;
	std::list<cv::Point3f> edge_dectct;
	
	float graph_ts;
	float current_allq;
	float pre_allq;
	static bool cmp_edge(const cv::Point3f &a, const cv::Point3f &b);
};

#endif // !HCluster.h