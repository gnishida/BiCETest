#include "BiCEUtil.h"
#include "TopNSearch.h"
#include <algorithm>
#include <random>

BiCESketch::BiCESketch(int m1, int m2, int m3) {
	std::vector<int> list;
	list.push_back(m1);
	list.push_back(m2);
	list.push_back(m3);
	std::sort(list.begin(), list.end());

	this->m1 = list[0];
	this->m2 = list[1];
	this->m3 = list[2];
}

/**
 * 与えられた行列に対して、BiCEデスクリプタを計算する。
 * サブサンプリングでは、xnum x ynum 次元に落とす。
 * 角度は、thetanumでサンプリング。
 * k個のmin-hash値を行列の1つの行とし（つまり、列数=k）、合計n行の行列にして返却する。
 */
void BiCEUtil::computeBiCEDescriptor(cv::Mat& mat, int xnum, int ynum, int thetanum, int n, int k, cv::Mat& binalized, cv::Mat_<int>& desc, const char* file_temp, int index) {
	// blur
	cv::Mat blur(mat.rows, mat.cols, CV_32F);
	mat.convertTo(blur, CV_32F);
	//cv::Mat blur(mat.rows, mat.cols, CV_8U);
	cv::GaussianBlur(mat, blur, cv::Size(0, 0), 3, 3, cv::BORDER_DEFAULT);

	// sub-sampling
	cv::Mat subsampled(xnum, ynum, CV_32F);
	cv::resize(blur, subsampled, cv::Size(xnum, ynum));

	// パッチを保存
	char filename[256];
	sprintf(filename, "patch\\%s_%d_subsampled.png", file_temp, index);
	cv::imwrite(filename, subsampled);

	/*
	// TOP 20%を1、それ以外は0にして、バイナリ化する
	binalized = cv::Mat::zeros(subsampled.rows, subsampled.cols, CV_8U);
	TopNSearch<int> tns;
	for (int i = 0; i < subsampled.rows; i++) {
		for (int j = 0; j < subsampled.cols; j++) {
			tns.add(subsampled(i, j), i * subsampled.cols + j);
		}
	}

	QList<int> topN = tns.topN(subsampled.rows * subsampled.cols * 0.3, TopNSearch<int>::ORDER_DESC);

	for (int i = 0; i < topN.size(); i++) {
		int c = topN[i] % subsampled.cols;
		int r = (topN[i] - c) / subsampled.cols;

		if (subsampled(r, c) == 0) break;

		binalized.at<uchar>(r, c) = 1;
	}
	*/

	/*
	subsampled = subsampled * 2.0f;
	subsampled.convertTo(binalized, CV_8U);
	binalized = binalized / 255;
	*/

	binalize(subsampled, binalized, 128);

	// パッチを保存
	sprintf(filename, "patch\\%s_%d_binalized.png", file_temp, index);
	cv::imwrite(filename, binalized * 255);

	/*
	srand(1234567);
	desc = cv::Mat_<int>(n, k);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < k; j++) {
			desc(i, j) = topN[rand() % topN.size()];
		}
	}
	*/
}

void BiCEUtil::binalize(cv::Mat& srcMat, cv::Mat& dstMat, float threshold) {
	dstMat = srcMat - threshold;
	dstMat = dstMat * 100000.0f;
	dstMat.convertTo(dstMat, CV_8U);
	dstMat = dstMat / 255;
}

/**
 * Jaccard similarityに基づいて、２つの行列の類似度を計算し、返却する。mat1、mat2は、0, 1のバイナリ行列という前提。
 */
float BiCEUtil::computeJaccarSimilarity(cv::Mat& mat1, cv::Mat& mat2) {
	// まずは、AND
	cv::Mat m1 = and(mat1, mat2);

	// 1x1行列に縮退する
	m1.convertTo(m1, CV_32F);
	cv::reduce(m1, m1, 0, CV_REDUCE_SUM);	// 1行の行列になる
	cv::reduce(m1, m1, 1, CV_REDUCE_SUM);	// 1行1列になる

	// 次に、OR
	cv::Mat m2 = or(mat1, mat2);

	// 1x1行列に縮退する
	m2.convertTo(m2, CV_32F);
	cv::reduce(m2, m2, 0, CV_REDUCE_SUM);	// 1行の行列になる
	cv::reduce(m2, m2, 1, CV_REDUCE_SUM);	// 1行1列になる

	// Jaccar similarityを計算する
	return (float)count(m1) / (float)count(m2);
	//return (float)count(m1);
}

/**
 * mat1、mat2は、0, 1のバイナリ行列という前提。
 * mat1とmat2の各要素について、共に1の要素のみ1、残りは0となる行列を生成して、返却する。
 */
cv::Mat BiCEUtil::and(cv::Mat& mat1, cv::Mat& mat2) {
	cv::Mat m1 = mat1.mul(mat2);
	m1.convertTo(m1, CV_32F);

	return m1;
}

/**
 * mat1、mat2は、0, 1のバイナリ行列という前提。
 * mat1とmat2の各要素について、どちらかが1の要素は1、残りは0となる行列を生成して、返却する。
 */
cv::Mat BiCEUtil::or(cv::Mat& mat1, cv::Mat& mat2) {
	cv::Mat m1 = mat1 + mat2;
	m1 = m1 * 255;
	m1.convertTo(m1, CV_8U);
	m1 = m1 / 255;
	
	return m1;
}

/**
 * matは、0, 1のバイナリ行列という前提。
 * 1となっている要素の数をカウントし、返却する。
 */
int BiCEUtil::count(cv::Mat& mat) {
	cv::Mat m;
	mat.convertTo(m, CV_32F);
	cv::reduce(m, m, 0, CV_REDUCE_SUM);	// 1行の行列になる
	cv::reduce(m, m, 1, CV_REDUCE_SUM);	// 1行1列になる

	return (int)m.at<float>(0, 0);
}
