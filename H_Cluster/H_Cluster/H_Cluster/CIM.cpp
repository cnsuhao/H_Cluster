#include "CIM.h"


//需要加入输入输出
CIM::CIM()
{
}

CIM::~CIM()
{

}


void
CIM::check(){
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
	for (int row = 0; row < N; row++){
	for (int col = 0; col < N; col++){
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
	for (int row = 0; row < N; row++){
		fprintf_s(checkOut, "%.4f	", nodeInfo.ptr<float>(row)[0]);
		fprintf_s(checkOut, "\n");
	}
	fprintf_s(checkOut, "/////////////////////////////////////////////////\n");


	fclose(checkOut);
}

void
CIM::compute_allq(std::list<cv::Point2f> cluster_q, float &q){
	q = .0f;
	std::list<cv::Point2f>::iterator clusterth_q;
	clusterth_q = cluster_q.begin();
	while (clusterth_q != cluster_q.end()){
		q += clusterth_q->x;
		clusterth_q++;
	}
}


void
CIM::compute_sim(cv::Mat adjMatrix, std::list<cv::Point3f> &edge, cv::Mat &ndist){
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
		for (int col = 0; col < adjMatrix.cols; col++){
			v1 = adjMatrix.ptr<float>(startNode)[col];
			v2 = adjMatrix.ptr<float>(endNode)[col];
			if (v1 & v2){
				cnt1++;
				cnt2++;
				sameN++;
			}else if (v1){
				cnt1++;
			}else if (v2){
				cnt2++;
			}
		}
		similarity = (sameN + 2) / sqrt((cnt1 + 1) * (cnt2 + 1) * 1.0f);
		ndist.ptr<float>(startNode)[endNode] = similarity;
		ndist.ptr<float>(endNode)[endNode] = similarity;
		edge_dectct_itr->z = similarity;
		if (startNode == 5){
			printf("%d	%d	%d\n", sameN, cnt1, cnt2);
		}
		printf("%d	%d	%f\n",startNode, endNode, similarity);

		edge_dectct_itr++;
	}
}


void
CIM::compute_qcth(std::list<cv::Point2f> cluster_is, std::list<cv::Point2f> cluster_ds,
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
CIM::compute_ts(cv::Mat dist, float &cluster_ts){
	cluster_ts = 0.f;
	for (int row = 0; row < dist.rows; row++){
		for (int col = row + 1; col < dist.cols; col++){
			cluster_ts += dist.ptr<float>(row)[col];
			if (dist.ptr<float>(row)[col]){
				//printf("%.5f\n", dist.ptr<float>(row)[col]);
			}
		}
	}
};


void
CIM::compute_is(cv::Mat dist, std::list< std::list<cv::Point2i> > cluster, int cth, cv::Point2f & cis){
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

	float sum = .0f;
	while (clusterth_itr != cluster_itr->end()){
		start_node = *clusterth_itr;
		float start_node_belongto = .0f;
		start_node_belongto = nodeInfo.ptr<float>(start_node.x)[0];

		for (int nodeth = 0; nodeth < dist.cols; nodeth++){
			if (dist.ptr<float>(start_node.x)[nodeth] > .0f){
				float next_node_belongto = .0f;
				next_node_belongto = nodeInfo.ptr<float>(nodeth)[0];
				if (next_node_belongto == start_node_belongto){
					sum += dist.ptr<float>(start_node.x)[nodeth];
				}
			}
		}
		clusterth_itr++;
	}

	sum /= 2;
	cis.x = sum;
	//printf("is:%.5f\n",sum);
};


void
CIM::compute_ds(cv::Mat dist, std::list< std::list<cv::Point2i> > cluster, int cth, cv::Point2f & ds){
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

	float sum = .0f;
	while (clusterth_itr != cluster_itr->end()){
		start_node = *clusterth_itr;
		float start_node_belongto = .0f;
		start_node_belongto = nodeInfo.ptr<float>(start_node.x)[0];

		for (int nodeth = 0; nodeth < dist.cols; nodeth++){
			if (dist.ptr<float>(start_node.x)[nodeth] > .0f){
				//float next_node_belongto = .0f;
				//next_node_belongto = nodeInfo.ptr(nodeth)[0];
				sum += dist.ptr<float>(start_node.x)[nodeth];
			}
		}
		clusterth_itr++;
	}

	//sum /= 2;
	ds.x = sum;
	//printf("ds:%.5f\n", sum);
};


void
CIM::adjust_cluster(std::list< std::list<cv::Point2i> > &cluster, std::list<cv::Point2f> &cluster_is,
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
	int cth = cluster_arr[0];

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
CIM::cmp_edge(const cv::Point3f &a, const cv::Point3f &b){
	if (a.z > b.z){
		return true;
	}
	else {
		return false;
	}
};


void
CIM::sort_edge(std::list<cv::Point3f> &edge_dectct){
	edge_dectct.sort(cmp_edge);
}

void
CIM::compute_is_ds_q(){
	std::list<cv::Point2f>::iterator clusterth_is, clusterth_ds, clusterth_q;
	std::list<cv::Point3f>::iterator edge_dectct_itr;
	int cth = 0;

	clusterth_is = cluster_is.begin();
	clusterth_ds = cluster_ds.begin();

	cth = 0;
	while (clusterth_is != cluster_is.end()){
		compute_is(CIM::ndistance, CIM::cluster, cth, *clusterth_is);
		compute_ds(CIM::ndistance, CIM::cluster, cth, *clusterth_ds);

		//减去了重复计算的部分
		clusterth_ds->x -= clusterth_is->x;
		clusterth_is++;
		clusterth_ds++;
		cth++;
	}
	//check();
	clusterth_q = cluster_q.begin();

	cth = 0;
	while (clusterth_q != cluster_q.end()){
		compute_qcth(CIM::cluster_is, CIM::cluster_ds, CIM::graph_ts, cth, *clusterth_q);
		clusterth_q++;
		cth++;
	}

	compute_allq(CIM::cluster_q, current_allq);
	CIM::pre_allq = CIM::current_allq;
}


void
CIM::compute_is_ds_qth(int clusterth){
	std::list<cv::Point2f>::iterator clusterth_is, clusterth_ds, clusterth_q;
	std::list<cv::Point3f>::iterator edge_dectct_itr;
	int cth = 0;

	clusterth_is = cluster_is.begin();
	clusterth_ds = cluster_ds.begin(); 

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

	compute_is(CIM::ndistance, CIM::cluster, cth, *clusterth_is);
	compute_ds(CIM::ndistance, CIM::cluster, cth, *clusterth_ds);
	clusterth_ds->x -= clusterth_is->x;

	clusterth_q = cluster_q.begin();

	cth = 0;
	while (clusterth_q != cluster_q.end() && cth != clusterth){
		cth++;
		clusterth_q++;
	}

	compute_qcth(CIM::cluster_is, CIM::cluster_ds, CIM::graph_ts, cth, *clusterth_q);

	compute_allq(CIM::cluster_q, CIM::current_allq);
	//CIM::pre_allq = CIM::current_allq;
}

void
CIM::H_cluster(){
	std::list<cv::Point2f>::iterator clusterth_is, clusterth_ds, clusterth_q;
	std::list<cv::Point3f>::iterator edge_dectct_itr;
	int cth = 0;
	sort_edge(CIM::edge_dectct);

	compute_ts(CIM::ndistance, CIM::graph_ts);

	compute_is_ds_q();

	check();


	/*clusterth_is = cluster_is.begin();
	clusterth_ds = cluster_ds.begin();

	cth = 0;
	while (clusterth_is != cluster_is.end()){
	compute_is(CIM::ndistance, CIM::cluster, cth, *clusterth_is);
	compute_ds(CIM::ndistance, CIM::cluster, cth, *clusterth_ds);
	}

	clusterth_q = cluster_q.begin();

	cth = 0;
	while (clusterth_q != cluster_q.end()){
	compute_qcth(CIM::cluster_is, CIM::cluster_ds, CIM::graph_ts, cth, *clusterth_q);
	}

	compute_allq(CIM::cluster_q, current_allq);
	CIM::pre_allq = CIM::current_allq;*/

	edge_dectct_itr = edge_dectct.begin();

	if (edge_dectct_itr == edge_dectct.end()){
		return;
	}

	pre_allq = current_allq - 1;
	while ( (current_allq >= pre_allq) && (edge_dectct_itr != edge_dectct.end()) ){

		//std::cout << ( (current_allq - pre_allq > 0.00001) && (edge_dectct_itr != edge_dectct.end()) ) << std::endl;

		int startNode = (int)(edge_dectct_itr->x + 0.3f);
		int endNode = (int)(edge_dectct_itr->y + 0.3f);

		int startNBlong = nodeInfo.ptr<float>(startNode)[0];
		int endNBlong = nodeInfo.ptr<float>(endNode)[0];

		printf("%d %d\n", startNode, endNode);

		if (endNBlong == startNBlong){
			edge_dectct_itr++;
			continue;
		}

		std::vector<int> nodeSet;
		nodeSet.push_back(startNode);
		nodeSet.push_back(endNode);

		adjust_cluster(CIM::cluster, CIM::cluster_is, CIM::cluster_ds, CIM::cluster_q, nodeSet);
		
		pre_allq = current_allq;
		int clusterth = (int)(nodeInfo.ptr<float>(startNode)[0] + 0.3f);

		std::cout << (int)(nodeInfo.ptr<float>(endNode)[0] + 0.3f) << std::endl;

		compute_is_ds_qth(clusterth);
		edge_dectct_itr++;
		
		printf("%f %f %f\n", current_allq, pre_allq, current_allq - pre_allq);
	}
	check();
}

void CIM::start(){
	//N = 8
	/*float dis[N][N] =
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

	/*float dis[N][N] =
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

	float dis[N][N] =
	{
		{ 0, 1, 1, 1, 0, 0, 0, 0 },
		{ 1, 0, 1, 0, 0, 0, 0, 0 },
		{ 1, 1, 0, 1, 0, 0, 0, 0 },
		{ 1, 0, 1, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 1, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 1, 1 },
		{ 0, 0, 0, 0, 0, 1, 0, 1 },
		{ 0, 0, 0, 0, 0, 1, 1, 0 }
	};

	ndistance = cv::Mat(cv::Size(N, N), CV_32F, cv::Scalar::all(0));
	nodeInfo = cv::Mat(cv::Size(1, N), CV_32F);
	adjMat = cv::Mat(cv::Size(N, N), CV_32F);

	//std::list<cv::Point2i>::iterator tmp_itr;
	for (int i = 0; i < N; i++){
		cluster_q.push_back(cv::Point2f(0, 0));
		cluster_is.push_back(cv::Point2f(0, 0));
		cluster_ds.push_back(cv::Point2f(0, 0));
		nodeInfo.ptr<float>(i)[0] = i + 0.0f;

		std::list<cv::Point2i> tmp_list;
		tmp_list.push_back(cv::Point2i(i, 0));
		cluster.push_back(tmp_list);
	}

	/*for (int row = 0; row < N; row++){
	for (int col = 0; col < N; col++){
	ndistance.ptr<float>(row)[col] = dis[row][col];
	if (col > row && dis[row][col] > 0){
	edge_dectct.push_back(cv::Point3f(row, col, dis[row][col]));
	}
	}
	}*/

	for (int row = 0; row < N; row++){
		for (int col = 0; col < N; col++){
			adjMat.ptr<float>(row)[col] = dis[row][col];
			if (col > row && dis[row][col] > 0){
				edge_dectct.push_back(cv::Point3f(row, col, dis[row][col]));
			}
		}
	}

	compute_sim(adjMat, edge_dectct, ndistance);

	//sort_edge(CIM::edge_dectct);

	//compute_ts(CIM::ndistance, CIM::graph_ts);

	//compute_is_ds_q();
	H_cluster();
}

void CIM::start(cv::Mat aM){
	//N = 8
	/*float dis[N][N] =
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

	/*float dis[N][N] =
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

	/*float dis[N][N] =
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

	/*for (int row = 0; row < N; row++){
	for (int col = 0; col < N; col++){
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

	//sort_edge(CIM::edge_dectct);

	//compute_ts(CIM::ndistance, CIM::graph_ts);

	//compute_is_ds_q();
	H_cluster();
}




