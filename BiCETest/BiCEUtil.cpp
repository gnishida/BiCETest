#include "BiCEUtil.h"

void BiCEUtil::computeBiCEDescriptor(cv::Mat_<uchar>& mat, float theta, cv::Mat_<float>& desc) {
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
	cv::GaussianBlur(grad, desc, cv::Size(0, 0), 3, 3, cv::BORDER_DEFAULT);
}
