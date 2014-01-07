#include "RoadEdge.h"

RoadEdge::RoadEdge(unsigned int type, unsigned int lanes, bool oneWay) {
	this->type = type;
	this->lanes = lanes;
	this->oneWay = oneWay;

	// initialize other members
	this->valid = true;
}

RoadEdge::~RoadEdge() {
}

float RoadEdge::getLength() {
	float length = 0.0f;
	for (int i = 0; i < polyLine.size() - 1; i++) {
		length += (polyLine[i + 1] - polyLine[i]).length();
	}

	return length;
}

std::vector<QVector2D> RoadEdge::getPolyLine() {
	return polyLine;
}

/**
 * Add a point to the polyline of the road segment.
 *
 * @param pt		new point to be added
 */
void RoadEdge::addPoint(const QVector2D &pt) {
	polyLine.push_back(pt);
}

float RoadEdge::getWidth(float widthPerLane) {
	if (type == 1) { // local street
		return widthPerLane;
	} else if (type == 2) { // avenue
		return widthPerLane * 1.5f;
	} else if (type == 3) { // high way
		return widthPerLane * 2.0f;
	} else {
		return 0.0f;
	}
}

