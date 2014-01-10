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
	cv::Mat_<float> grad = cv::Mat_<float>::zeros(mat.rows, mat.cols);

	// compute the gradient magnitude
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			float fx = 0.0f;
			if (j < mat.cols - 1) {
				fx = mat.at<uchar>(i, j + 1) - mat.at<uchar>(i, j);
			}

			float fy = 0.0f;
			if (i < mat.rows - 1) {
				fy = mat.at<uchar>(i + 1, j) - mat.at<uchar>(i, j);
			}

			grad(i, j) = sqrtf(fx * fx + fy * fy);
		}
	}

	// パッチを保存
	char filename[256];
	sprintf(filename, "patch\\%s_%d_grad.png", file_temp, index);
	cv::imwrite(filename, grad);
	
	// compute the Gaussian weighted average
	cv::Mat_<float> grad2(mat.rows, mat.cols);
	cv::GaussianBlur(grad, grad2, cv::Size(0, 0), 3, 3, cv::BORDER_DEFAULT);

	// パッチを保存
	sprintf(filename, "patch\\%s_%d_grad2.png", file_temp, index);
	cv::imwrite(filename, grad2);

	// blur
	cv::Mat_<float> blur(grad2.rows, grad2.cols);
	cv::GaussianBlur(grad2, blur, cv::Size(0, 0), 1, 3, cv::BORDER_DEFAULT);

	// sub-sampling
	cv::Mat_<float> subsampled(xnum, ynum);
	cv::resize(blur, subsampled, cv::Size(xnum, ynum));

	// パッチを保存
	sprintf(filename, "patch\\%s_%d_subsampled.png", file_temp, index);
	cv::imwrite(filename, subsampled);

	// binarize the values by assigning a value of 1 to the top 20 percent of the bins with highest values, and 0 to the others.
	//cv::Mat_<uchar> binalized = cv::Mat_<uchar>::zeros(subsampled.rows, subsampled.cols);
	binalized = cv::Mat::zeros(subsampled.rows, subsampled.cols, CV_8U);
	TopNSearch<int> tns;
	for (int i = 0; i < subsampled.rows; i++) {
		for (int j = 0; j < subsampled.cols; j++) {
			tns.add(subsampled(i, j), i * subsampled.cols + j);
		}
	}

	QList<int> topN = tns.topN(subsampled.rows * subsampled.cols * 0.2, TopNSearch<int>::ORDER_DESC);

	for (int i = 0; i < topN.size(); i++) {
		int c = topN[i] % subsampled.cols;
		int r = (topN[i] - c) / subsampled.cols;

		if (subsampled(r, c) == 0) break;

		binalized.at<uchar>(r, c) = 1;
	}

	// パッチを保存
	sprintf(filename, "patch\\%s_%d_binalized.png", file_temp, index);
	cv::imwrite(filename, binalized * 255);

	srand(1234567);
	desc = cv::Mat_<int>(n, k);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < k; j++) {
			desc(i, j) = topN[rand() % topN.size()];
		}
	}
}

float BiCEUtil::computeJaccarSimilarity(cv::Mat& mat1, cv::Mat& mat2) {
	// まずは、AND
	cv::Mat m1 = mat1.mul(mat2);
	m1.convertTo(m1, CV_32F);

	cv::reduce(m1, m1, 0, CV_REDUCE_SUM);	// 1行の行列になる
	cv::reduce(m1, m1, 1, CV_REDUCE_SUM);	// 1行1列になる

	// 次に、OR
	cv::Mat m2 = mat1 + mat2;
	m2 = m2 * 255;
	m2.convertTo(m2, CV_8U);
	m2 = m2 / 255;
	m2.convertTo(m2, CV_32F);
	cv::reduce(m2, m2, 0, CV_REDUCE_SUM);	// 1行の行列になる
	cv::reduce(m2, m2, 1, CV_REDUCE_SUM);	// 1行1列になる

	// Jaccar similarityを計算する
	return m1.at<float>(0, 0) / m2.at<float>(0, 0);
}
