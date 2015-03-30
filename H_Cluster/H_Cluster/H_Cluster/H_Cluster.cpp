#include "H_Cluster.h"


//需要加入输入输出
HCluster::HCluster()
{
}

HCluster::~HCluster()
{

}


void
HCluster::check(){
	FILE *checkOut;

	fopen_s(&checkOut, "check data", "a+");
	if (checkOut == NULL){
		fopen_s(&checkOut, "check data", "w+");
	}

	time_t t;
	time(&t);
	fprintf_s(checkOut, "Today'sdateandtime:%s", ctime(&t));

	std::list<cv::Point2i> tmp_list;
	std::list<cv::Point2i>::iterator tmp_itr;
	std::list< std::list<cv::Point2i> >::iterator cluster_itr, cluster_itr2, clusterth_itr_first;
	std::list<cv::Point2f>::iterator clusterth_q_itr, clusterth_is_itr, clusterth_ds_itr;
	std::list<cv::Point3f>::iterator edge_dectct_itr;
	std::list<cv::Point2i>::iterator clusterth_itr1, clusterth_itr2;

	/*fprintf_s(checkOut, "ndistance:\n");
	for (int row = 0; row < NODENUM; row++){
	for (int col = 0; col < NODENUM; col++){
	fprintf_s(checkOut, "%.4f	", ndistance.ptr<float>(row)[col]);
	}
	fprintf_s(checkOut, "\n");
	}*/

	fprintf_s(checkOut, "edge:\n");
	edge_dectct_itr = edge_dectct.begin();
	while (edge_dectct_itr != edge_dectct.end()){
		fprintf_s(checkOut, "%.0f	%.0f	%.5f\n", edge_dectct_itr->x, edge_dectct_itr->y, edge_dectct_itr->z);
		edge_dectct_itr++;
	}
	fprintf_s(checkOut, "\n");

	fprintf_s(checkOut, "ts:\n");
	fprintf_s(checkOut, "%.5f\n", graph_ts);

	fprintf_s(checkOut, "/////////////////////////////////////////////////\n");


	fprintf_s(checkOut, "cluster:\n");
	//std::list< std::list<cv::Point2i> >::iterator cluster_itr;
	//std::list<cv::Point2i>::iterator clusterth_itr;
	cluster_itr = cluster.begin();
	while (cluster_itr != cluster.end()){
		clusterth_itr1 = cluster_itr->begin();
		while (clusterth_itr1 != cluster_itr->end()){
			fprintf_s(checkOut, "%d  ", clusterth_itr1->x);
			clusterth_itr1++;
		}
		fprintf_s(checkOut, "\n");
		cluster_itr++;
	}
	fprintf_s(checkOut, "/////////////////////////////////////////////////\n");


	fprintf_s(checkOut, "is:\n");
	clusterth_is_itr = cluster_is.begin();
	while (clusterth_is_itr != cluster_is.end()){
		fprintf_s(checkOut, "%.5f\n", clusterth_is_itr->x);
		clusterth_is_itr++;
	}
	fprintf_s(checkOut, "/////////////////////////////////////////////////\n");


	fprintf_s(checkOut, "ds:\n");
	clusterth_ds_itr = cluster_ds.begin();
	while (clusterth_ds_itr != cluster_ds.end()){
		fprintf_s(checkOut, "%.5f\n", clusterth_ds_itr->x);
		clusterth_ds_itr++;
	}
	fprintf_s(checkOut, "/////////////////////////////////////////////////\n");

	fprintf_s(checkOut, "cluster q:\n");
	clusterth_q_itr = cluster_q.begin();
	while (clusterth_q_itr != cluster_q.end()){
		fprintf_s(checkOut, "%.5f\n", clusterth_q_itr->x);
		clusterth_q_itr++;
	}
	fprintf_s(checkOut, "/////////////////////////////////////////////////\n");


	fprintf_s(checkOut, "belong to cluster:\n");
	for (int row = 0; row < NODENUM; row++){
		fprintf_s(checkOut, "%.4f	", nodeInfo.ptr<float>(row)[0]);
		fprintf_s(checkOut, "\n");
	}
	fprintf_s(checkOut, "/////////////////////////////////////////////////\n");


	fclose(checkOut);
}

void
HCluster::compute_allq(std::list<cv::Point2f> cluster_q, float &q){
	q = .0f;
	std::list<cv::Point2f>::iterator clusterth_q;
	//std::list< std::list<cv::Point2i> >::iterator clusterth;
	clusterth_q = cluster_q.begin();
	//clusterth = cluster.begin();
	while (clusterth_q != cluster_q.end()){
		//if (clusterth->size()>1){
		q += clusterth_q->x;
		//}
		clusterth_q++;
		//clusterth++;
	}
}


void
HCluster::compute_sim(cv::Mat adjMatrix, std::list<cv::Point3f> &edge, cv::Mat &ndist){
	//edge_dectct存放的边startNode < endNode,即存放的是adjMat的上三角

	int startNode, endNode;
	std::list<cv::Point3f>::iterator edge_dectct_itr;
	edge_dectct_itr = edge.begin();
	int sameN, cnt1, cnt2;
	int v1, v2;
	float similarity;

	while (edge_dectct_itr != edge.end()){
		sameN = 0;
		cnt1 = 0;
		cnt2 = 0;
		startNode = (int)(edge_dectct_itr->x + 0.3);
		endNode = (int)(edge_dectct_itr->y + 0.3);
		//计算共同的边
		for (int col = 0; col < adjMatrix.cols; col++){
			v1 = adjMatrix.ptr<float>(startNode)[col];
			v2 = adjMatrix.ptr<float>(endNode)[col];
			if (v1 & v2){
				cnt1++;
				cnt2++;
				sameN++;
			}
			else if (v1){
				cnt1++;
			}
			else if (v2){
				cnt2++;
			}
		}
		similarity = (sameN + 2) / sqrt((cnt1 + 1) * (cnt2 + 1) * 1.0f);
		ndist.ptr<float>(startNode)[endNode] = similarity;
		ndist.ptr<float>(endNode)[startNode] = similarity;
		edge_dectct_itr->z = similarity;
		/*if (startNode == 5){
			printf("%d	%d	%d\n", sameN, cnt1, cnt2);
			}
			printf("%d	%d	%f\n", startNode, endNode, similarity);*/
		//printf("%d	%d	%d	%d	%d	%f\n", startNode, endNode, sameN + 2, (cnt1 + 1), (cnt2 + 1), similarity);
		edge_dectct_itr++;
	}
}


void
HCluster::compute_qcth(std::list<cv::Point2f> cluster_is, std::list<cv::Point2f> cluster_ds,
float graph_ts, int cth, cv::Point2f &cq){
	float cis = 0.f;
	float cds = 0.f;
	std::list<cv::Point2f>::iterator cis_itr;
	std::list<cv::Point2f>::iterator cds_itr;

	if (cth >= cluster_is.size()){
		printf("error/n");
		return;
	}

	int pth = 0;
	cis_itr = cluster_is.begin();
	cds_itr = cluster_ds.begin();
	while (cis_itr != cluster_is.end() && pth != cth){
		pth++;
		cis_itr++;
		cds_itr++;
	}

	cis = cis_itr->x;
	cds = cds_itr->x;

	cq.x = cis / graph_ts - (cds / graph_ts) * (cds / graph_ts);
};


void
HCluster::compute_ts(cv::Mat dist, float &cluster_ts){
	cluster_ts = 0.f;
	for (int row = 0; row < dist.rows; row++){
		for (int col = row + 1; col < dist.cols; col++){
			cluster_ts += dist.ptr<float>(row)[col];
			//if (dist.ptr<float>(row)[col]){
			//	//printf("%.5f\n", dist.ptr<float>(row)[col]);
			//}
		}
	}
	cluster_ts += cluster_ts;
};


void
HCluster::compute_is(cv::Mat dist, std::list< std::list<cv::Point2i> > cluster, int cth, cv::Point2f & cis){
	std::list< std::list<cv::Point2i> >::iterator cluster_itr;
	std::list<cv::Point2i>::iterator clusterth_itr;
	cv::Point2i start_node;

	if (cth >= cluster.size()){
		printf("error\n");
		return;
	}

	int pth = 0;

	cluster_itr = cluster.begin();
	while (cluster_itr != cluster.end() && pth != cth){
		pth++;
		cluster_itr++;
	}

	if (cluster_itr == cluster.end()){
		printf("error\n");
		return;
	}
	clusterth_itr = cluster_itr->begin();

	float sumin = .0f;
	while (clusterth_itr != cluster_itr->end()){
		start_node = *clusterth_itr;
		float start_node_belongto = .0f;
		start_node_belongto = nodeInfo.ptr<float>(start_node.x)[0];

		for (int nodeth = 0; nodeth < dist.cols; nodeth++){
			if (dist.ptr<float>(start_node.x)[nodeth] > .1f){
				float next_node_belongto = .0f;
				next_node_belongto = nodeInfo.ptr<float>(nodeth)[0];
				if (abs(next_node_belongto - start_node_belongto) < 0.1){
					sumin += dist.ptr<float>(start_node.x)[nodeth];
				}
			}
		}
		clusterth_itr++;
	}

	//sumin /= 2;
	cis.x = sumin;
	//printf("is:%.5f\n",sum);
};


void
HCluster::compute_ds(cv::Mat dist, std::list< std::list<cv::Point2i> > cluster, int cth, cv::Point2f & ds){
	std::list< std::list<cv::Point2i> >::iterator cluster_itr;
	std::list<cv::Point2i>::iterator clusterth_itr;
	cv::Point2i start_node;

	if (cth >= cluster.size()){
		printf("error\n");
		return;
	}

	//找到社区地址
	int pth = 0;
	cluster_itr = cluster.begin();
	while (cluster_itr != cluster.end() && pth != cth){
		pth++;
		cluster_itr++;
	}

	if (cluster_itr == cluster.end()){
		printf("error\n");
		return;
	}

	clusterth_itr = cluster_itr->begin();
	float sumin = .0f;
	float sumout = .0f;
	//对于社区中的每个节点
	while (clusterth_itr != cluster_itr->end()){
		start_node = *clusterth_itr;
		float start_node_belongto = .0f;
		start_node_belongto = nodeInfo.ptr<float>(start_node.x)[0];

		for (int nodeth = 0; nodeth < dist.cols; nodeth++){
			if (dist.ptr<float>(start_node.x)[nodeth] > .1f){
				float end_node_belongto = .0f;
				end_node_belongto = nodeInfo.ptr<float>(nodeth)[0];
				//float next_node_belongto = .0f;
				//next_node_belongto = nodeInfo.ptr(nodeth)[0];
				if (abs(end_node_belongto - start_node_belongto) < 0.1f){
					sumin += dist.ptr<float>(start_node.x)[nodeth];
				}
				else{
					sumout += dist.ptr<float>(start_node.x)[nodeth];
				}

			}
		}
		clusterth_itr++;
	}

	//sum /= 2;
	//ds.x = sumin / 2 + sumout;
	ds.x = sumin + sumout;
	//printf("ds:%.5f\n", sum);
};


void
HCluster::adjust_cluster(std::list< std::list<cv::Point2i> > &cluster, std::list<cv::Point2f> &cluster_is,
std::list<cv::Point2f> &cluster_ds, std::list<cv::Point2f> &cluster_q, std::vector<int> cluster_arr){
	if (cluster_arr.size() <= 1){
		return;
	}

	sort(cluster_arr.begin(), cluster_arr.end());

	std::list< std::list<cv::Point2i> >::iterator cluster_itr, cluster_itr2, clusterth_itr_first;
	std::list<cv::Point2f>::iterator clusterth_q_itr, clusterth_is_itr, clusterth_ds_itr/*, clusterth_q_itr2, clusterth_is_itr2, clusterth_ds_itr2*/;
	std::list<cv::Point2i>::iterator clusterth_itr1, clusterth_itr2;

	std::list<cv::Point2i> tmp_cluster;
	int pth = 0;
	int cth = HCluster::nodeInfo.ptr<float>(cluster_arr[0])[0];

	cluster_itr = cluster.begin();
	clusterth_is_itr = cluster_is.begin();
	clusterth_ds_itr = cluster_ds.begin();
	clusterth_q_itr = cluster_q.begin();

	cth = 0;
	while (cluster_itr != cluster.end() && cth < cluster_arr.size()){
		if (pth == cluster_arr[0]){
			clusterth_itr_first = cluster_itr;
			pth++;
			cth++;
			cluster_itr++;
			clusterth_is_itr++;
			clusterth_ds_itr++;
			clusterth_q_itr++;
			continue;
		}
		else if (pth != cluster_arr[cth]){
			cluster_itr++;
			clusterth_is_itr++;
			clusterth_ds_itr++;
			clusterth_q_itr++;
			pth++;
			continue;
		}

		std::cout << "pth:" << pth << std::endl;
		clusterth_itr1 = cluster_itr->begin();

		while (clusterth_itr1 != cluster_itr->end()){
			clusterth_itr_first->push_back(*clusterth_itr1);
			nodeInfo.ptr<float>(clusterth_itr1->x)[0] = 0.0f + cluster_arr[0];
			clusterth_itr1++;
		}
		nodeInfo.ptr<float>(cluster_arr[cth])[0] = 0.0f + cluster_arr[0];
		/*cluster_itr2 = cluster_itr;
		clusterth_is_itr2 = clusterth_is_itr;
		clusterth_ds_itr2 = clusterth_ds_itr;
		clusterth_q_itr2 = clusterth_q_itr;*/

		clusterth_is_itr->x = 0;
		clusterth_ds_itr->x = 0;
		clusterth_q_itr->x = 0;
		cluster_itr->clear();

		cluster_itr++;
		clusterth_is_itr++;
		clusterth_ds_itr++;
		clusterth_q_itr++;
		pth++;
		cth++;

		/*if (pth - 1 != cluster_arr[0]){
			cluster.erase(cluster_itr2);
			cluster_q.erase(clusterth_q_itr2);
			cluster_ds.erase(clusterth_ds_itr2);
			cluster_is.erase(clusterth_is_itr2);
			}*/
	}
}


bool
HCluster::cmp_edge(const cv::Point3f &a, const cv::Point3f &b){
	if (a.z > b.z){
		return true;
	}
	else {
		return false;
	}
};


void
HCluster::sort_edge(std::list<cv::Point3f> &edge_dectct){
	edge_dectct.sort(cmp_edge);
}

void
HCluster::compute_is_ds_q(){
	std::list<cv::Point2f>::iterator clusterth_is, clusterth_ds, clusterth_q;
	std::list<cv::Point3f>::iterator edge_dectct_itr;
	int cth = 0;

	clusterth_is = cluster_is.begin();
	clusterth_ds = cluster_ds.begin();

	cth = 0;
	while (clusterth_is != cluster_is.end()){
		compute_is(HCluster::ndistance, HCluster::cluster, cth, *clusterth_is);
		compute_ds(HCluster::ndistance, HCluster::cluster, cth, *clusterth_ds);

		//减去了重复计算的部分
		//clusterth_ds->x -= clusterth_is->x;
		clusterth_is++;
		clusterth_ds++;
		cth++;
	}
	//check();
	clusterth_q = cluster_q.begin();

	cth = 0;
	while (clusterth_q != cluster_q.end()){
		compute_qcth(HCluster::cluster_is, HCluster::cluster_ds, HCluster::graph_ts, cth, *clusterth_q);
		clusterth_q++;
		cth++;
	}

	compute_allq(HCluster::cluster_q, current_allq);
	//HCluster::pre_allq = HCluster::current_allq;
}


void
HCluster::compute_is_ds_qth(int clusterth){
	std::list<cv::Point2f>::iterator clusterth_is, clusterth_ds, clusterth_q;
	std::list<cv::Point3f>::iterator edge_dectct_itr;
	int cth = 0;

	clusterth_is = cluster_is.begin();
	clusterth_ds = cluster_ds.begin();

	//切换到对应类对应的ds 和 is
	cth = 0;
	while (clusterth_is != cluster_is.end() && cth != clusterth){
		cth++;
		clusterth_is++;
		clusterth_ds++;
	}

	if (clusterth_is == cluster_is.end()){
		printf("error\n");
		return;
	}

	//计算该类的is 和 ds
	compute_is(HCluster::ndistance, HCluster::cluster, cth, *clusterth_is);
	compute_ds(HCluster::ndistance, HCluster::cluster, cth, *clusterth_ds);
	clusterth_ds->x -= clusterth_is->x;

	clusterth_q = cluster_q.begin();

	//切换到对应类q
	cth = 0;
	while (clusterth_q != cluster_q.end() && cth != clusterth){
		cth++;
		clusterth_q++;
	}

	//计算q值
	compute_qcth(HCluster::cluster_is, HCluster::cluster_ds, HCluster::graph_ts, cth, *clusterth_q);

	//更新sum q
	compute_allq(HCluster::cluster_q, HCluster::current_allq);
	//HCluster::pre_allq = HCluster::current_allq;
}

void
HCluster::H_cluster(){
	std::list<cv::Point2f>::iterator clusterth_is, clusterth_ds, clusterth_q;
	std::list<cv::Point3f>::iterator edge_dectct_itr;
	std::list< std::list<cv::Point2i> > cpyL;
	std::list< cv::Point3f > is_ds_qInfo;
	std::vector<float> nodeInfo_cpy;

	std::vector<int> nodeSet;
	cpyL.clear();
	nodeSet.clear();
	int cth = 0;
	sort_edge(HCluster::edge_dectct);

	compute_ts(HCluster::ndistance, HCluster::graph_ts);

	compute_is_ds_q();
	HCluster::pre_allq = HCluster::current_allq;
	//check();


	/*clusterth_is = cluster_is.begin();
	clusterth_ds = cluster_ds.begin();

	cth = 0;
	while (clusterth_is != cluster_is.end()){
	compute_is(HCluster::ndistance, HCluster::cluster, cth, *clusterth_is);
	compute_ds(HCluster::ndistance, HCluster::cluster, cth, *clusterth_ds);
	}

	clusterth_q = cluster_q.begin();

	cth = 0;
	while (clusterth_q != cluster_q.end()){
	compute_qcth(HCluster::cluster_is, HCluster::cluster_ds, HCluster::graph_ts, cth, *clusterth_q);
	}

	compute_allq(HCluster::cluster_q, current_allq);
	HCluster::pre_allq = HCluster::current_allq;*/

	edge_dectct_itr = edge_dectct.begin();

	if (edge_dectct_itr == edge_dectct.end()){
		return;
	}

	pre_allq = current_allq - 1;
	while ((current_allq >= pre_allq) && (edge_dectct_itr != edge_dectct.end())){

		//std::cout << ( (current_allq - pre_allq > 0.00001) && (edge_dectct_itr != edge_dectct.end()) ) << std::endl;

		int startNode = (int)(edge_dectct_itr->x + 0.3f);
		int endNode = (int)(edge_dectct_itr->y + 0.3f);

		int startNBlong = nodeInfo.ptr<float>(startNode)[0];
		int endNBlong = nodeInfo.ptr<float>(endNode)[0];

		//printf("%d %d\n", startNode, endNode);

		if (endNBlong == startNBlong){
			edge_dectct_itr++;
			continue;
		}

		//需要将这些社区合并
		nodeSet.clear();
		nodeSet.push_back(startNBlong);
		nodeSet.push_back(endNBlong);
		sort(nodeSet.begin(), nodeSet.end());

		//if (startNode == 5 && endNode == 6){
		//getchar();
		//}

		cv::Mat tmpMat = HCluster::nodeInfo.clone();
		std::cout << tmpMat.t() << std::endl;

		cpyInfo(cluster, nodeSet, is_ds_qInfo/*, nodeInfo_cpy*/, cpyL);

		adjust_cluster(HCluster::cluster, HCluster::cluster_is, HCluster::cluster_ds, HCluster::cluster_q, nodeSet);

		/*cv::Mat*/ tmpMat = HCluster::nodeInfo.clone();
		std::cout << tmpMat.t() << std::endl;

		pre_allq = current_allq;
		int clusterth = /*(int)(nodeInfo.ptr<float>(startNode)[0] + 0.3f)*/nodeSet[0];

		std::cout << "clusterth:" << clusterth << std::endl;

		compute_is_ds_q();
		//compute_is_ds_qth这个有问题先注释
		//compute_is_ds_qth(clusterth);
		edge_dectct_itr++;

		printf("%f %f %f\n", current_allq, pre_allq, current_allq - pre_allq);
	}

	if (current_allq < pre_allq){
		HCluster::recoveryInfo(cluster, nodeSet, is_ds_qInfo/*, nodeInfo_cpy*/, cpyL);
	}
	cv::Mat tmpMat = HCluster::nodeInfo.clone();
	std::cout << tmpMat.t() << std::endl;
	//compute_is_ds_q();

	check();
}

void HCluster::start(){
	//NODENUM = 8
	/*float dis[NODENUM][NODENUM] =
	{
	{ 0, 0.1340, 0.1340, 0.2500, 1.0000, 1.0000, 1.0000, 1.0000 },
	{ 0.1340, 0, 0.1340, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000 },
	{ 0.1340, 0.1340, 0, 0.2500, 1.0000, 1.0000, 1.0000, 1.0000 },
	{ 0.2500, 1.0000, 0.2500, 0, 0.4230, 1.0000, 1.0000, 1.0000 },
	{ 1.0000, 1.0000, 1.0000, 0.4230, 0, 0.4230, 1.0000, 1.0000 },
	{ 1.0000, 1.0000, 1.0000, 1.0000, 0.4230, 0, 0.2500, 0.2500 },
	{ 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 0.2500, 0, 1.0000 },
	{ 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 0.2500, 1.0000, 0 }
	};*/

	/*float dis[NODENUM][NODENUM] =
	{
	{ 0, 0.8660, 1.0000, 0.7500, 0, 0, 0, 0 },
	{ 0.8660, 0, 0.8660, 0, 0, 0, 0, 0 },
	{ 1.0000, 0.8660, 0, 0.7500, 0, 0, 0, 0 },
	{ 0.7500, 0, 0.7500, 0, 0.5770, 0, 0, 0 },
	{ 0, 0, 0, 0.5770, 0, 0.5770, 0, 0 },
	{ 0, 0, 0, 0, 0.5770, 0, 0.8660, 0.8660 },
	{ 0, 0, 0, 0, 0, 0.8660, 0, 1.0000 },
	{ 0, 0, 0, 0, 0, 0.8660, 1.0000, 0 }
	};*/

	/*float dis[NODENUM][NODENUM] =
	{
	{ 0, 1, 1, 1, 0, 0, 0, 0 },
	{ 1, 0, 1, 0, 0, 0, 0, 0 },
	{ 1, 1, 0, 1, 0, 0, 0, 0 },
	{ 1, 0, 1, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 1, 0, 1, 0, 0 },
	{ 0, 0, 0, 0, 1, 0, 1, 1 },
	{ 0, 0, 0, 0, 0, 1, 0, 1 },
	{ 0, 0, 0, 0, 0, 1, 1, 0 }
	};*/

	/*float dis[NODENUM][NODENUM] =
	{
	{ 0, 1, 1, 1, 1},
	{ 1, 0, 0, 0, 0},
	{ 1, 0, 0, 0, 0},
	{ 1, 0, 0, 0, 0},
	{ 1, 0, 0, 0, 0},
	};*/

	//float dis[NODENUM][NODENUM] =
	//{
	//	{ 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	//	{ 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
	//	{ 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
	//	{ 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
	//	{ 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
	//	{ 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
	//	{ 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
	//	{ 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
	//	{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0 },
	//	{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0 },
	//	{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1 },
	//	{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1 },
	//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1 },
	//	{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0 }
	//};

	//float dis[NODENUM][NODENUM];



	ndistance = cv::Mat(cv::Size(NODENUM, NODENUM), CV_32F, cv::Scalar::all(0));
	nodeInfo = cv::Mat(cv::Size(1, NODENUM), CV_32F, cv::Scalar::all(0));
	adjMat = cv::Mat(cv::Size(NODENUM, NODENUM), CV_32F, cv::Scalar::all(0));


	srand(time(NULL));
	for (int i = 0; i < 6300; i++){
		int x = rand() % NODENUM;
		int y = rand() % NODENUM;
		adjMat.ptr<float>(y)[x] = 1;
	}

	//std::list<cv::Point2i>::iterator tmp_itr;
	for (int i = 0; i < NODENUM; i++){
		cluster_q.push_back(cv::Point2f(0, 0));
		cluster_is.push_back(cv::Point2f(0, 0));
		cluster_ds.push_back(cv::Point2f(0, 0));
		nodeInfo.ptr<float>(i)[0] = i + 0.0f;

		std::list<cv::Point2i> tmp_list;
		tmp_list.push_back(cv::Point2i(i, 0));
		cluster.push_back(tmp_list);
	}

	/*for (int row = 0; row < NODENUM; row++){
	for (int col = 0; col < NODENUM; col++){
	ndistance.ptr<float>(row)[col] = dis[row][col];
	if (col > row && dis[row][col] > 0){
	edge_dectct.push_back(cv::Point3f(row, col, dis[row][col]));
	}
	}
	}*/

	for (int row = 0; row < NODENUM; row++){
		for (int col = 0; col < NODENUM; col++){
			//adjMat.ptr<float>(row)[col] = dis[row][col];
			if (col > row && /*dis[row][col]*/adjMat.ptr<float>(row)[col] > 0.1f){
				edge_dectct.push_back(cv::Point3f(row, col, /*dis[row][col]*/adjMat.ptr<float>(row)[col]));
			}
		}
	}

	compute_sim(adjMat, edge_dectct, ndistance);
	/*std::cout << ndistance << std::endl;*/
	//sort_edge(HCluster::edge_dectct);

	//compute_ts(HCluster::ndistance, HCluster::graph_ts);

	//compute_is_ds_q();
	H_cluster();
	//std::cout << ndistance << std::endl;
}

void HCluster::start(cv::Mat aM){
	//NODENUM = 8
	/*float dis[NODENUM][NODENUM] =
	{
	{ 0, 0.1340, 0.1340, 0.2500, 1.0000, 1.0000, 1.0000, 1.0000 },
	{ 0.1340, 0, 0.1340, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000 },
	{ 0.1340, 0.1340, 0, 0.2500, 1.0000, 1.0000, 1.0000, 1.0000 },
	{ 0.2500, 1.0000, 0.2500, 0, 0.4230, 1.0000, 1.0000, 1.0000 },
	{ 1.0000, 1.0000, 1.0000, 0.4230, 0, 0.4230, 1.0000, 1.0000 },
	{ 1.0000, 1.0000, 1.0000, 1.0000, 0.4230, 0, 0.2500, 0.2500 },
	{ 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 0.2500, 0, 1.0000 },
	{ 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 0.2500, 1.0000, 0 }
	};*/

	/*float dis[NODENUM][NODENUM] =
	{
	{ 0, 0.8660, 1.0000, 0.7500, 0, 0, 0, 0 },
	{ 0.8660, 0, 0.8660, 0, 0, 0, 0, 0 },
	{ 1.0000, 0.8660, 0, 0.7500, 0, 0, 0, 0 },
	{ 0.7500, 0, 0.7500, 0, 0.5770, 0, 0, 0 },
	{ 0, 0, 0, 0.5770, 0, 0.5770, 0, 0 },
	{ 0, 0, 0, 0, 0.5770, 0, 0.8660, 0.8660 },
	{ 0, 0, 0, 0, 0, 0.8660, 0, 1.0000 },
	{ 0, 0, 0, 0, 0, 0.8660, 1.0000, 0 }
	};*/

	/*float dis[NODENUM][NODENUM] =
	{
	{ 0, 1, 1, 1, 0, 0, 0, 0 },
	{ 1, 0, 1, 0, 0, 0, 0, 0 },
	{ 1, 1, 0, 1, 0, 0, 0, 0 },
	{ 1, 0, 1, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 1, 0, 1, 0, 0 },
	{ 0, 0, 0, 0, 1, 0, 1, 1 },
	{ 0, 0, 0, 0, 0, 1, 0, 1 },
	{ 0, 0, 0, 0, 0, 1, 1, 0 }
	};*/

	int n = aM.rows;

	ndistance = cv::Mat(cv::Size(n, n), CV_32F, cv::Scalar::all(0));
	nodeInfo = cv::Mat(cv::Size(1, n), CV_32F);
	//adjMat = cv::Mat(cv::Size(n, n), CV_32F);
	adjMat = aM.clone();

	//std::list<cv::Point2i>::iterator tmp_itr;
	for (int i = 0; i < n; i++){
		cluster_q.push_back(cv::Point2f(0, 0));
		cluster_is.push_back(cv::Point2f(0, 0));
		cluster_ds.push_back(cv::Point2f(0, 0));
		nodeInfo.ptr<float>(i)[0] = i + 0.0f;

		std::list<cv::Point2i> tmp_list;
		tmp_list.push_back(cv::Point2i(i, 0));
		cluster.push_back(tmp_list);
	}

	/*for (int row = 0; row < NODENUM; row++){
	for (int col = 0; col < NODENUM; col++){
	ndistance.ptr<float>(row)[col] = dis[row][col];
	if (col > row && dis[row][col] > 0){
	edge_dectct.push_back(cv::Point3f(row, col, dis[row][col]));
	}
	}
	}*/

	for (int row = 0; row < n; row++){
		for (int col = 0; col < n; col++){
			//adjMat.ptr<float>(row)[col] = dis[row][col];
			if (col > row && aM.ptr<float>(row)[col] > 0){
				edge_dectct.push_back(cv::Point3f(row, col, aM.ptr<float>(row)[col]));
			}
		}
	}

	compute_sim(adjMat, edge_dectct, ndistance);

	//sort_edge(HCluster::edge_dectct);

	//compute_ts(HCluster::ndistance, HCluster::graph_ts);

	//compute_is_ds_q();
	H_cluster();
}

cv::Mat HCluster::getSim(){
	return ndistance.clone();
}

cv::Mat HCluster::getNodeInfo(){
	return nodeInfo.clone();
}

cv::Mat  HCluster::getAdjMat(){
	return  HCluster::adjMat;
}


std::list<cv::Point2f> HCluster::getCluster_is(){
	return cluster_is;
}

std::list<cv::Point2f> HCluster::getCluster_ds(){
	return cluster_ds;
}

std::list<cv::Point2f> HCluster::getCluster_q(){
	return cluster_q;
}

std::list< std::list<cv::Point2i> > HCluster::getCluster(){
	return cluster;
}

void HCluster::cpyInfo(std::list< std::list<cv::Point2i> > &cluster, std::vector<int> cluster_arr, std::list< cv::Point3f > &is_ds_qInfo/*, std::vector<float> &nodeInfo_cpy*/, std::list< std::list<cv::Point2i> > &cpyL){
	if (cluster_arr.size() <= 1){
		return;
	}

	//排序不能少，和recoveryInfo约定好的
	sort(cluster_arr.begin(), cluster_arr.end());

	std::list< std::list<cv::Point2i> >::iterator cluster_itr, cluster_itr2, clusterth_itr_first;
	std::list<cv::Point2f>::iterator clusterth_q_itr, clusterth_is_itr, clusterth_ds_itr/*, clusterth_q_itr2, clusterth_is_itr2, clusterth_ds_itr2*/;
	std::list<cv::Point2i>::iterator clusterth_itr1, clusterth_itr2;

	std::list<cv::Point2i> tmp_cluster;

	int pth = 0;
	int cth = cluster_arr[0];

	cluster_itr = cluster.begin();
	clusterth_is_itr = HCluster::cluster_is.begin();
	clusterth_ds_itr = HCluster::cluster_ds.begin();
	clusterth_q_itr = HCluster::cluster_q.begin();


	is_ds_qInfo.clear();
	cpyL.clear();
	//nodeInfo_cpy.clear();
	cth = 0;
	while (cluster_itr != cluster.end() && cth < cluster_arr.size()){
		//切换到对应簇的信息
		if (pth != cluster_arr[cth]/*HCluster::nodeInfo.ptr<float>(cluster_arr[cth])[0]*/){
			cluster_itr++;
			clusterth_is_itr++;
			clusterth_ds_itr++;
			clusterth_q_itr++;
			pth++;
			continue;
		}
		//clusterth_itr_first = cluster_itr;
		clusterth_itr1 = cluster_itr->begin();

		tmp_cluster.clear();
		while (clusterth_itr1 != cluster_itr->end()){
			tmp_cluster.push_back(*clusterth_itr1);
			clusterth_itr1++;
		}
		cpyL.push_back(tmp_cluster);

		cv::Point3f tmpInfo;
		tmpInfo.x = clusterth_is_itr->x;
		tmpInfo.y = clusterth_ds_itr->x;
		tmpInfo.z = clusterth_q_itr->x;
		is_ds_qInfo.push_back(tmpInfo);

		/*nodeInfo_cpy.push_back(HCluster::nodeInfo.ptr<float>(cluster_arr[cth])[0]);*/

		cluster_itr++;
		clusterth_is_itr++;
		clusterth_ds_itr++;
		clusterth_q_itr++;
		pth++;
		cth++;
	}
}

void HCluster::recoveryInfo(std::list< std::list<cv::Point2i> > &cluster, std::vector<int> cluster_arr, std::list< cv::Point3f > is_ds_qInfo/*, std::vector<float> nodeInfo_cpy*/, std::list< std::list<cv::Point2i> > cpyL){
	if (cluster_arr.size() <= 1){
		return;
	}

	sort(cluster_arr.begin(), cluster_arr.end());

	std::list< std::list<cv::Point2i> >::iterator cluster_itr, cluster_itr2, clusterth_itr_first, cpyL_itr;
	std::list<cv::Point2f>::iterator clusterth_q_itr, clusterth_is_itr, clusterth_ds_itr/*, clusterth_q_itr2, clusterth_is_itr2, clusterth_ds_itr2*/;
	std::list<cv::Point2i>::iterator clusterth_itr1;
	std::list<cv::Point3f>::iterator is_ds_qInfo_itr;

	std::list<cv::Point2i> tmp_cluster;
	std::list<cv::Point2i>::iterator itr;

	int pth = 0;
	int cth = cluster_arr[0];

	cluster_itr = cluster.begin();

	cpyL_itr = cpyL.begin();
	clusterth_is_itr = HCluster::cluster_is.begin();
	clusterth_ds_itr = HCluster::cluster_ds.begin();
	clusterth_q_itr = HCluster::cluster_q.begin();
	is_ds_qInfo_itr = is_ds_qInfo.begin();

	//cpyL.clear();
	//is_ds_qInfo.clear()
	cth = 0;
	while (cluster_itr != cluster.end() && cpyL_itr != cpyL.end() && cth < cluster_arr.size()){
		if (pth != cluster_arr[cth]/*nodeInfo_cpy[cth]*/){
			cluster_itr++;
			clusterth_is_itr++;
			clusterth_ds_itr++;
			clusterth_q_itr++;
			pth++;
			continue;
		}
		//clusterth_itr_first = cluster_itr;
		itr = cpyL_itr->begin();
		cluster_itr->clear();

		//tmp_cluster.clear();
		while (itr != cpyL_itr->end()){
			cluster_itr->push_back(*itr);
			nodeInfo.ptr<float>(itr->x)[0] = pth;
			itr++;
		}

		clusterth_is_itr->x = is_ds_qInfo_itr->x;
		clusterth_ds_itr->x = is_ds_qInfo_itr->y;
		clusterth_q_itr->x = is_ds_qInfo_itr->z;
		/*HCluster::nodeInfo.ptr<float>(cluster_arr[cth])[0] = nodeInfo_cpy[cth];*/

		if (cth >= cluster_arr.size()){
			printf("error\n");
			return;
		}

		cpyL_itr++;
		cluster_itr++;
		is_ds_qInfo_itr++;
		pth++;
		cth++;
	}
}
