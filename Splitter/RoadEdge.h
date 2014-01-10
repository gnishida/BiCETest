#pragma once

#include <QVector2D>
#include <vector>
#include <boost/shared_ptr.hpp>

class RoadEdge {
public:
	int type;
	int lanes;
	bool oneWay;
	std::vector<QVector2D> polyLine;
	bool valid;			// if this edge is valid

public:
	RoadEdge(unsigned int type, unsigned int lanes, bool oneWay);
	~RoadEdge();
	
	float getLength();
	std::vector<QVector2D> getPolyLine();

	void addPoint(const QVector2D &pt);
	float getWidth(float widthPerLane);
};

typedef boost::shared_ptr<RoadEdge> RoadEdgePtr;