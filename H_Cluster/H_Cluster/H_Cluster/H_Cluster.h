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

	//�������ƶ�asim
	void compute_sim(cv::Mat adjMatrix, std::list<cv::Point3f> &edge, cv::Mat &ndist);
	
	//����������quotaֵ
	void compute_qcth(std::list<cv::Point2f> cluster_is, std::list<cv::Point2f> cluster_ds,
		float graph_ts, int cth, cv::Point2f &cq);
	//����tsֵ���������бߵ�sim���
	void compute_ts(cv::Mat dist, float &cluster_ts);
	//����������is��ֵ
	void compute_is(cv::Mat dist, std::list< std::list<cv::Point2i> > cluster, int cth, cv::Point2f &cis);
	//����������ds��ֵ
	void compute_ds(cv::Mat dist, std::list< std::list<cv::Point2i> > cluster, int cth, cv::Point2f &cds);
	//��������������qֵ�ĺ�
	void compute_allq(std::list<cv::Point2f> cluster_q, float &q);
	//�����������������ϲ�
	void adjust_cluster(std::list< std::list<cv::Point2i> > &cluster, std::list<cv::Point2f> &cluster_is, 
		std::list<cv::Point2f> &cluster_ds, std::list<cv::Point2f> &cluster_q, std::vector<int> clusterth);
	//�Ա߽���������ȡasimֵ���ı�
	void sort_edge(std::list<cv::Point3f> &edge_dectct);
	//�ֲ���࣬ÿ��ѡȡһ��asimֵ���ıߣ��ñ������Ӳ�ͬ�����ģ�����ñ����ӵ������ںϺ�Qֵ�Ƿ������ˣ��������˾��ںϣ���������β�����������
	void H_cluster();
	//��������������is��ds��qֵ
	void compute_is_ds_q();
	//����ĳ��������is��ds��qֵ
	void compute_is_ds_qth(int cth);

	void check();
	//��ȡ�ڽӾ���
	cv::Mat getAdjMat();
	//�������бߵ�simֵ
	cv::Mat getSim();
	//��ȡ�ýڵ����Ϣ
	cv::Mat getNodeInfo();
	//��ȡ��������isֵ
	std::list<cv::Point2f> getCluster_is();
	//��ȡ��������dsֵ
	std::list<cv::Point2f> getCluster_ds();
	//��ȡ��������qֵ
	std::list<cv::Point2f> getCluster_q();
	//��ȡһ�������ڵ�
	std::list< std::list<cv::Point2i> > getCluster();
	//���ƽ�Ҫ�޸ĵ���Ϣ
	void cpyInfo(std::list< std::list<cv::Point2i> > &cluster, std::vector<int> clusterth, std::list< cv::Point3f > &is_ds_qInfo/*, std::vector<float> &nodeInfo_cpy*/, std::list< std::list<cv::Point2i> > &cpyL);
	//��ԭ��Ϣ���ָ�����һ�ε�״̬
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