#pragma once

#include <opencv2/opencv.hpp>

class BiCEUtil {
protected:
	BiCEUtil() {}
	~BiCEUtil() {}

public:
	static void computeBiCEDescriptor(cv::Mat_<uchar>& mat, int xnum, int ynum, int thetanum, int n, int k, cv::Mat_<int>& desc, const char* file_temp, int index);
};

