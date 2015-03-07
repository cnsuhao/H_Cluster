#pragma once

#ifndef cim_h
#define cim_h

#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define N 8

class CIM
{
public:
	CIM();
	~CIM();
	void start();
	void start(cv::Mat aM);

	void compute_sim(cv::Mat adjMatrix, std::list<cv::Point3f> &edge, cv::Mat &ndist);
	
	void compute_qcth(std::list<cv::Point2f> cluster_is, std::list<cv::Point2f> cluster_ds,
		float graph_ts, int cth, cv::Point2f &cq);

	void compute_ts(cv::Mat dist, float &cluster_ts);

	void compute_is(cv::Mat dist, std::list< std::list<cv::Point2i> > cluster, int cth, cv::Point2f &cis);

	void compute_ds(cv::Mat dist, std::list< std::list<cv::Point2i> > cluster, int cth, cv::Point2f &cds);

	void compute_allq(std::list<cv::Point2f> cluster_q, float &q);

	void adjust_cluster(std::list< std::list<cv::Point2i> > &cluster, std::list<cv::Point2f> &cluster_is, 
		std::list<cv::Point2f> &cluster_ds, std::list<cv::Point2f> &cluster_q, std::vector<int> clusterth);

	void sort_edge(std::list<cv::Point3f> &edge_dectct);

	void H_cluster();

	void compute_is_ds_q();

	void compute_is_ds_qth(int cth);

	void check();

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

#endif // !cim.h