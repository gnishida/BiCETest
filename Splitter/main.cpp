/**
 * 与えられた道路網を行列にし、指定されたサイズに分割して、画像として保存する。
 */

#include <iostream>

int main(int argc, char* argv[]) {
	if (argc < 3) {
		std::cerr << "Usage: BiCETest <road GSM file> <size>" << std::endl;
		return 1;
	}

	// 道路データを読み込む
	RoadGraph r;
	GraphUtil::loadRoads(&r, "osm/03x03/london.gsm");

	return 0;
}
