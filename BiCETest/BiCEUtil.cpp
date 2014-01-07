#include "BiCEUtil.h"
#include "TopNSearch.h"

void BiCEUtil::computeBiCEDescriptor(cv::Mat_<uchar>& mat, float theta, cv::Mat_<uchar>& desc) {
	cv::Mat_<float> grad = cv::Mat_<float>::zeros(mat.rows, mat.cols);

	// compute the gradient magnitude
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			float fx = 0.0f;
			if (j < mat.cols - 1) {
				fx = mat(i, j + 1) - mat(i, j);
			}

			float fy = 0.0f;
			if (i < mat.rows - 1) {
				fy = mat(i + 1, j) - mat(i, j);
			}

			grad(i, j) = sqrtf(fx * fx + fy * fy);
		}
	}

	// compute the Gaussian weighted average
	cv::Mat_<float> grad2(mat.rows, mat.cols);
	cv::GaussianBlur(grad, grad2, cv::Size(0, 0), 3, 3, cv::BORDER_DEFAULT);

	// binarize the values by assigning a value of 1 to the top 20 percent of the bins with highest values, and 0 to the others.
	cv::Mat_<uchar> binalized = cv::Mat_<uchar>::zeros(grad2.rows, grad2.cols);
	TopNSearch<int> tns;
	for (int i = 0; i < grad2.rows; i++) {
		for (int j = 0; j < grad2.cols; j++) {
			tns.add(grad2(i, j), i * grad2.cols + j);
		}
	}
	QList<int> topN = tns.topN(grad2.rows * grad2.cols * 0.2, TopNSearch<int>::ORDER_DESC);
	for (int i = 0; i < topN.size(); i++) {
		int c = topN[i] % grad2.cols;
		int r = (topN[i] - c) / grad2.cols;
		binalized(r, c) = 255;//1;
	}

	desc = binalized;
}
