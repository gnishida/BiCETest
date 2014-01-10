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
#include "TopNSearch.h"

int main(int argc, char *argv[]) {
	int patch_size = 60;
	
	// 道路データを読み込む
	cv::Mat_<uchar> img = cv::imread("area/paris_1800_600.png", 0);

	// パッチに分割し、各行列について、BiCEデスクリプタを計算する。
	int count = 0;
	int img_rows = img.rows * 4 / patch_size - 3;
	int img_cols = img.cols * 4 / patch_size - 3;
	std::vector<cv::Mat> subs(img_rows * img_cols);
	std::vector<cv::Mat> binalized(img_rows * img_cols);
	for (int i = 0; i < img_rows; i++) {
		for (int j = 0; j < img_cols; j++) {
			subs[count] = cv::Mat(img, cv::Rect(j * patch_size / 4, i * patch_size / 4, patch_size, patch_size));

			// パッチを保存
			char filename[256];
			sprintf(filename, "patch\\london_%d.png", count);
			cv::imwrite(filename, subs[count]);

			cv::Mat_<int> desc;
			BiCEUtil::computeBiCEDescriptor(subs[count], 18, 18, 4, 20, 3, binalized[count], desc, "london", count);

			count++;
		}
	}

	// クエリ用の画像を読込む
	cv::Mat_<uchar> sketch_mat = cv::imread("osm/sketch_circle.png", 0);

	int sketch_rows = sketch_mat.rows * 4 / patch_size - 3;
	int sketch_cols = sketch_mat.cols * 4 / patch_size - 3;
	for (int i = 0; i < sketch_rows; i++) {
		for (int j = 0; j < sketch_cols; j++) {
			cv::Mat sub = cv::Mat(sketch_mat, cv::Rect(j * patch_size / 4, i * patch_size / 4, patch_size, patch_size));

			cv::Mat v1;
			cv::reduce(sub, v1, 0, CV_REDUCE_AVG);
			cv::reduce(v1, v1, 1, CV_REDUCE_AVG);
			if (v1.at<uchar>(0, 0) < 3) continue;

			// スケッチを保存
			char filename[256];
			sprintf(filename, "patch\\sketch_%d.png", i * sketch_cols + j);
			cv::imwrite(filename, sub);

			cv::Mat_<int> sketch_desc;
			cv::Mat_<uchar> sketch_binalized;
			BiCEUtil::computeBiCEDescriptor(sub, 18, 18, 4, 20, 3, sketch_binalized, sketch_desc, "sketch", i * sketch_cols + j);

			// 各パッチとのJaccar similarityを計算し、上位5件を保存する
			TopNSearch<int> tns;
			for (int k = 0; k < binalized.size(); k++) {
				float similarity = BiCEUtil::computeJaccarSimilarity(binalized[k], sketch_binalized);
				tns.add(similarity, k);
			}
			QList<int> results = tns.topN(15, TopNSearch<int>::ORDER_DESC);
			
			for (int k = 0; k < results.size(); k++) {
				float similarity = BiCEUtil::computeJaccarSimilarity(binalized[results[k]], sketch_binalized);
				int isim = similarity * 100.0f;

				char filename[256];
				sprintf(filename, "results\\%d_%d_%d_%d.png", i * sketch_cols + j, k, results[k], isim);
				cv::imwrite(filename, subs[results[k]]);
			}
		}
	}

	// 表示
	cv::namedWindow("sketch", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("sketch", sketch_mat);

	cv::waitKey(0);
}