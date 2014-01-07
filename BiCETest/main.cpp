/**
 * このプログラムは、Binary Coherent Edge Descriptorsの論文を参考に、
 * 当該論文の内容を実装したものです。
 *
 * @author Gen Nishida
 * @version 1.0
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include "GraphUtil.h"
#include "BiCEUtil.h"

int main(int argc, char *argv[]) {
	RoadGraph r;
	GraphUtil::loadRoads(&r, "osm/3x3_simplified/london_3.gsm");

	cv::Size img_size(3000, 3000);
	//cv::Mat img = cv::Mat(img_size, CV_8UC1, cv::Scalar(255));
	cv::Mat_<uchar> img = cv::Mat_<uchar>(img_size, 255);

	// 道路網をcv::Mat行列に置き換える
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(r.graph); ei != eend; ++ei) {
		for (int i = 0; i < r.graph[*ei]->polyLine.size() - 1; i++) {
			QVector2D p0 = r.graph[*ei]->polyLine[i] + QVector2D(1500, 1500);
			QVector2D p1 = r.graph[*ei]->polyLine[i + 1] + QVector2D(1500, 1500);
			cv::line(img, cv::Point(p0.x(), p0.y()), cv::Point(p1.x(), p1.y()), cv::Scalar(0), 1, CV_AA);
		}
	}

	// 500x500行列に分割し、各行列について、BiCEデスクリプタを計算する。
	cv::Mat_<float> desc[121];
	//for (int i = 0; i < 11; i++) {
	for (int i = 0; i < 1; i++) {
		//for (int j = 0; j < 11; j++) {
		for (int j = 0; j < 1; j++) {
			cv::Mat_<uchar> sub = cv::Mat(img, cv::Rect(j * 250, i * 250, 500, 500));
			BiCEUtil::computeBiCEDescriptor(sub, 0, desc[i * 11 + j]);
		}
	}

	cv::namedWindow("original roads", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("original roads", img);

	cv::namedWindow("image", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("image", desc[0]);
	cv::waitKey(0);
}