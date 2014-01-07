#pragma once

#include "RoadGraph.h"
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class GraphUtil {
protected:
	GraphUtil() {}

public:
	// Vertex related functions
	static int getNumVertices(RoadGraph* roads, bool onlyValidVertex = true);
	static int getNumConnectedVertices(RoadGraph* roads, RoadVertexDesc start, bool onlyValidVertex = true);
	static RoadVertexDesc getVertex(RoadGraph* roads, int index, bool onlyValidVertex = true);
	static RoadVertexDesc getVertex(RoadGraph* roads, const QVector2D& pt, bool onlyValidVertex = true);
	static RoadVertexDesc getVertex(RoadGraph* roads, const QVector2D& pt, float angle, float angle_threshold, bool onlyValidVertex = true);
	static bool getVertex(RoadGraph* roads, const QVector2D& pos, float threshold, RoadVertexDesc& desc, bool onlyValidVertex = true);
	static bool getVertex(RoadGraph* roads, RoadVertexDesc v, float threshold, RoadVertexDesc& desc, bool onlyValidVertex = true);
	static bool getVertex(RoadGraph* roads, const QVector2D& pos, float threshold, RoadVertexDesc ignore, RoadVertexDesc& desc, bool onlyValidVertex = true);
	static int getVertexIndex(RoadGraph* roads, RoadVertexDesc desc, bool onlyValidVertex = true);
	static RoadVertexDesc addVertex(RoadGraph* roads, RoadVertexPtr v);
	static void moveVertex(RoadGraph* roads, RoadVertexDesc v, const QVector2D& pt);
	static int getDegree(RoadGraph* roads, RoadVertexDesc v, bool onlyValidEdge = true);
	static std::vector<RoadVertexDesc> getVertices(RoadGraph* roads, bool onlyValidVertex = true);
	static void removeIsolatedVertices(RoadGraph* roads, bool onlyValidVertex = true);
	static void snapVertex(RoadGraph* roads, RoadVertexDesc v1, RoadVertexDesc v2);

	// Edge related functions
	static RoadEdgeDesc getEdge(RoadGraph* roads, int index, bool onlyValidEdge = true);
	static float getTotalEdgeLength(RoadGraph* roads, RoadVertexDesc v);
	//static void collapseEdge(RoadGraph* roads, RoadEdgeDesc e);
	static int getNumEdges(RoadGraph* roads, bool onlyValidEdge = true);
	static RoadEdgeDesc addEdge(RoadGraph* roads, RoadVertexDesc src, RoadVertexDesc tgt, unsigned int type, unsigned int lanes, bool oneWay = false);
	static RoadEdgeDesc addEdge(RoadGraph* roads, RoadVertexDesc src, RoadVertexDesc tgt, RoadEdgePtr ref_edge);
	static bool hasEdge(RoadGraph* roads, RoadVertexDesc desc1, RoadVertexDesc desc2, bool onlyValidEdge = true);
	static RoadEdgeDesc getEdge(RoadGraph* roads, RoadVertexDesc src, RoadVertexDesc tgt, bool onlyValidEdge = true);
	static std::vector<QVector2D> getOrderedPolyLine(RoadGraph* roads, RoadEdgeDesc e);
	static void orderPolyLine(RoadGraph* roads, RoadEdgeDesc e, RoadVertexDesc src);
	static void moveEdge(RoadGraph* roads, RoadEdgeDesc e, QVector2D& src_pos, QVector2D& tgt_pos);
	static std::vector<RoadEdgeDesc> getMajorEdges(RoadGraph* roads, int num);
	static bool removeDeadEnd(RoadGraph* roads);
	static std::vector<QVector2D> interpolateEdges(RoadGraph* roads1, RoadEdgeDesc e1, RoadVertexDesc src1, RoadGraph* roads2, RoadEdgeDesc e2, RoadVertexDesc src2, float t);
	static float computeDissimilarityOfEdges(RoadGraph* roads1, RoadEdgeDesc e1, RoadGraph* roads2, RoadEdgeDesc e2);
	static void removeIsolatedEdges(RoadGraph* roads, bool onlyValidEdge = true);
	static RoadVertexDesc splitEdge(RoadGraph* roads, RoadEdgeDesc edge_desc, const QVector2D& pt);
	static bool hasCloseEdge(RoadGraph* roads, RoadVertexDesc v1, RoadVertexDesc v2, float angle_threshold = 0.3f);
	static std::vector<QVector2D> simplifyPolyLine(std::vector<QVector2D>& polyLine, float threshold);
	static void removeShortEdges(RoadGraph* roads, float threshold);
	static std::vector<QVector2D> finerEdge(RoadGraph* roads, RoadEdgeDesc e, float step = 1.0f);

	// File I/O
	static void loadRoads(RoadGraph* roads, const QString& filename, int roadType = 7);
	static void saveRoads(RoadGraph* roads, const QString& filename);

	// The entire graph related functions
	static RoadGraph* copyRoads(RoadGraph* roads, int roadType = 7);
	static void copyRoads(RoadGraph* roads1, RoadGraph* roads2);
	static void mergeRoads(RoadGraph* roads1, RoadGraph* roads2);
	static RoadGraph* extractMajorRoad(RoadGraph* roads, bool remove = true);
	static float extractMajorRoad(RoadGraph* roads, RoadEdgeDesc root, QList<RoadEdgeDesc>& path);
	static void extractRoads(RoadGraph* roads, int roadType = 7);

	// Connectivity related functions
	static std::vector<RoadVertexDesc> getNeighbors(RoadGraph* roads, RoadVertexDesc v, bool onlyValidVertex = true);
	static bool isNeighbor(RoadGraph* roads, RoadVertexDesc v1, RoadVertexDesc v2);
	static bool isConnected(RoadGraph* roads, RoadVertexDesc desc1, RoadVertexDesc desc2, bool onlyValidEdge = true);
	static RoadVertexDesc findConnectedNearestNeighbor(RoadGraph* roads, const QVector2D &pt, RoadVertexDesc v);

	// The road graph modification functions
	static void clean(RoadGraph* roads);
	static void reduce(RoadGraph* roads);
	static bool reduce(RoadGraph* roads, RoadVertexDesc desc);
	static void normalize(RoadGraph* roads);
	static void singlify(RoadGraph* roads);
	static void skeltonize(RoadGraph* roads);
	static void rotate(RoadGraph* roads, float theta, const QVector2D& rotationCenter = QVector2D(0, 0));
	static void translate(RoadGraph* roads, const QVector2D& offset);
	static RoadGraph* convertToGridNetwork(RoadGraph* roads, RoadVertexDesc start);
	static RoadGraph* approximateToGridNetwork(RoadGraph* roads, float cellLength, QVector2D orig);
	static bool removeDuplicateEdges(RoadGraph* roads);
	static void snapDeadendEdges(RoadGraph* roads, float threshold);
	static void snapDeadendEdges2(RoadGraph* roads, int degree, float threshold);
	static void realize(RoadGraph* roads);

	// Others
	static float computeMinDiffAngle(std::vector<float> *data1, std::vector<float> *data2);
	static float normalizeAngle(float angle);
	static float diffAngle(const QVector2D& dir1, const QVector2D& dir2, bool absolute = true);
	static float diffAngle(float angle1, float angle2, bool absolute = true);

	// Compute similarity
	//static float computeDissimilarity(RoadGraph* roads1, QMap<RoadVertexDesc, RoadVertexDesc>& map1, RoadGraph* roads2, QMap<RoadVertexDesc, RoadVertexDesc>& map2, float w_connectivity, float w_split, float w_angle, float w_distance);
	static float computeSimilarity(RoadGraph* roads1, QMap<RoadVertexDesc, RoadVertexDesc>& map1, RoadGraph* roads2, QMap<RoadVertexDesc, RoadVertexDesc>& map2, float w_connectivity, float w_angle, float w_length);
	static void findCorrespondenceByNearestNeighbor(RoadGraph* roads1, RoadGraph* roads2, QMap<RoadVertexDesc, RoadVertexDesc>& map1, QMap<RoadVertexDesc, RoadVertexDesc>& map2);
	static QMap<RoadVertexDesc, RoadVertexDesc> findCorrespondentEdges(RoadGraph* roads1, RoadVertexDesc parent1, std::vector<RoadVertexDesc> children1, RoadGraph* roads2, RoadVertexDesc parent2, std::vector<RoadVertexDesc> children2);
	static QMap<RoadVertexDesc, RoadVertexDesc> findApproximateCorrespondentEdges(RoadGraph* roads1, RoadVertexDesc parent1, std::vector<RoadVertexDesc> children1, RoadGraph* roads2, RoadVertexDesc parent2, std::vector<RoadVertexDesc> children2);

	// Interpolation
	static RoadGraph* interpolate(RoadGraph* roads1, RoadGraph* roads2, QMap<RoadVertexDesc, RoadVertexDesc>& map, float t);
	static RoadGraph* interpolate(RoadGraph* roads1, const QVector2D& center, float radius, RoadGraph* roads2, QMap<RoadVertexDesc, RoadVertexDesc>& map);

	static bool nextSequence(std::vector<int>& seq, int N);

	// ICP
	static cv::Mat rigidICP(RoadGraph* roads1, RoadGraph* roads2, QMap<RoadVertexDesc, RoadVertexDesc>& map);
	static void transform(RoadGraph* roads, const cv::Mat& transformMat);
	static cv::Mat convertVerticesToCVMatrix(RoadGraph* roads, bool onlyValidVertex = true);
	static cv::Mat convertEdgesToCVMatrix(RoadGraph* roads, bool onlyValidVertex = true);

	// Sample road networks
	static RoadGraph* createGridNetwork(float size, int num);
	static RoadGraph* createCurvyNetwork(float size, int num, float angle);
	static RoadGraph* createRadialNetwork(float size, int num, int degree);

	// For debugging
	static void printStatistics(RoadGraph* roads);
};

