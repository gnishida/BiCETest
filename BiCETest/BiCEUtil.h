#pragma once

#include <opencv2/opencv.hpp>
#include <QHash>

class BiCESketch {
public:
	int m1;
	int m2;
	int m3;

public:
	BiCESketch(int m1, int m2, int m3);
	bool operator==(const BiCESketch& ref);
};

inline bool operator==(const BiCESketch& s1, const BiCESketch& s2) {
	return (s1.m1 == s2.m1 && s1.m2 == s2.m2 && s1.m3 == s2.m3);
}

inline uint qHash(const BiCESketch& key) {
	return qHash(key.m1 + key.m2 + key.m3);
}



class BiCEUtil {
protected:
	BiCEUtil() {}
	~BiCEUtil() {}

public:
	static void computeBiCEDescriptor(cv::Mat& mat, int xnum, int ynum, int thetanum, int n, int k, cv::Mat& binalized, const char* file_temp, int index);
	static void binalize(cv::Mat& srcMat, cv::Mat& dstMat, float threshold);
	static float computeJaccarSimilarity(cv::Mat& mat1, cv::Mat& mat2);
	static cv::Mat and(cv::Mat& mat1, cv::Mat& mat2);
	static cv::Mat or(cv::Mat& mat1, cv::Mat& mat2);
	static int count(cv::Mat& mat);
};

