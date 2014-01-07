#pragma once

#include <opencv2/opencv.hpp>

class BiCEUtil {
protected:
	BiCEUtil() {}
	~BiCEUtil() {}

public:
	static void computeBiCEDescriptor(cv::Mat_<uchar>& mat, float theta, cv::Mat_<uchar>& desc);
};

