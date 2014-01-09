/**
 * このプログラムは、Binary Coherent Edge Descriptorsの論文を参考に、
 * 当該論文の内容を実装したものです。
 *
 * @author Gen Nishida
 * @version 1.0
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <QVector2D>
#include <QHash>
#include "GraphUtil.h"
#include "BiCEUtil.h"

int main(int argc, char *argv[]) {
	int patch_size = 60;

	// 道路データを読み込む
	RoadGraph r;
	GraphUtil::loadRoads(&r, "osm/3x3/london.gsm");

	// 道路データを、行列に変換
	cv::Mat_<uchar> img;
	GraphUtil::convertToMat(r, img, cv::Size(1000, 1000), QVector2D(patch_size, patch_size));

	// 500x500行列に分割し、各行列について、BiCEデスクリプタを計算する。
	//cv::Mat_<int> desc[121];
	QHash<int, QList<int> > lookup;
	int img_cols = img.cols * 2 / patch_size - 1;
	for (int i = 0; i < img.rows * 2 / patch_size - 1; i++) {
	//for (int i = 0; i < 1; i++) {
		for (int j = 0; j < img_cols; j++) {
		//for (int j = 0; j < 1; j++) {
			cv::Mat_<int> desc;
			cv::Mat_<uchar> sub = cv::Mat(img, cv::Rect(j * patch_size / 2, i * patch_size / 2, patch_size, patch_size));

			// パッチを保存
			cv::Mat_<uchar> v1, v2;
			cv::reduce(sub, v1, 0, CV_REDUCE_AVG);
			cv::reduce(v1, v2, 1, CV_REDUCE_AVG);

			// ほとんど真っ黒のパッチは、データベースに入れないで、スキップする
			if (v2(0, 0) < 10) continue;

			char filename[256];
			sprintf(filename, "patch\\london_%d.png", i * img_cols + j);
			cv::imwrite(filename, sub);

			BiCEUtil::computeBiCEDescriptor(sub, 18, 6, 4, 20, 3, desc, "london", i * img_cols + j);

			for (int k = 0; k < desc.rows; k++) {
				lookup[desc(k, 0) + desc(k, 1) * 432 + desc(k, 2) * 432 * 432].push_back(i * img_cols + j);
			}
		}
	}

	// クエリ用の画像を読込む
	RoadGraph sketch;
	GraphUtil::loadRoads(&sketch, "osm/sketch.gsm");
	
	// クエリ用の画像を、行列に変換
	cv::Mat_<uchar> sketch_mat;
	GraphUtil::convertToMat(sketch, sketch_mat, cv::Size(480, 480));

	// offset計算用のハッシュテーブル
	QHash<int, int> offsetX;
	QHash<int, int> offsetY;

	int sketch_cols = sketch_mat.cols * 4 / patch_size - 3;
	for (int i = 0; i < sketch_mat.rows * 4 / patch_size - 3; i++) {
		for (int j = 0; j < sketch_cols; j++) {
			cv::Mat_<uchar> sub = cv::Mat(sketch_mat, cv::Rect(j * patch_size / 4, i * patch_size / 4, patch_size, patch_size));

			// スケッチを保存
			char filename[256];
			sprintf(filename, "patch\\sketch_%d.png", i * sketch_cols + j);
			cv::imwrite(filename, sub);

			cv::Mat_<int> sketch_desc;
			BiCEUtil::computeBiCEDescriptor(sub, 18, 6, 4, 20, 3, sketch_desc, "sketch", i * sketch_cols + j);

			// クエリ検索
			QList<int> locations = lookup[sketch_desc(0, 0) + sketch_desc(0, 1) * 432 + sketch_desc(0, 2) * 432 * 432];

			if (locations.size() > 0) {
				for (int k = 0; k < locations.size(); k++) {
					int c = locations[k] % img_cols;
					int r = (locations[k] - c) / img_cols;

					offsetX[c * patch_size / 2 - j * patch_size / 4]++;
					offsetY[r * patch_size / 2 - i * patch_size / 4]++;
					std::cout << "sketch_" << (i * sketch_cols + j) << " / london_" << locations[k] << std::endl;
				}
			}
		}
	}

	// ベストオフセット値を計算する
	int bestOffsetX;
	int bestOffsetY;
	int voteX = 0;
	int voteY = 0;
	for (QHash<int, int>::iterator it = offsetX.begin(); it != offsetX.end(); ++it) {
		if (it.value() > voteX) {
			voteX = it.value();
			bestOffsetX = it.key();
		}
	}
	for (QHash<int, int>::iterator it = offsetY.begin(); it != offsetY.end(); ++it) {
		if (it.value() > voteY) {
			voteY = it.value();
			bestOffsetY = it.key();
		}
	}

	std::cout << bestOffsetX << "," << bestOffsetY << std::endl;


	// 上下反転して表示
	/*
	cv::flip(img, img, 0);
	cv::namedWindow("original roads", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("original roads", img);
	*/

	cv::waitKey(0);
}