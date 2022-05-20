#include <iostream>
#include <fstream>
#include "parseJson.h"
#pragma comment(lib, "jsoncpp.lib")

using namespace std;

void parseJson::parsing_json() {

	string pts_path = "1.pts";

	ifstream json_file;
	json_file.open(pts_path);

	Json::Value root;
	json_file >> root;

	//glob(path, str, false);

	//cout<<root["points"].asString()<<endl;
   // cout << root["world_coords"] << endl;

	Json::Value world_pts = root["world_coords"];
	wpointX[0] = world_pts["X1"].asDouble();
	wpointX[1] = world_pts["X2"].asDouble();
	wpointX[2] = world_pts["X3"].asDouble();
	wpointX[3] = world_pts["X4"].asDouble();
	wpointY[0] = world_pts["Y1"].asDouble();
	wpointY[1] = world_pts["Y2"].asDouble();
	wpointY[2] = world_pts["Y3"].asDouble();
	wpointY[3] = world_pts["Y4"].asDouble();

	/*for (int i = 0; i < 4; i++)
	{
		cout << "world X" << i + 1 << " " << wpointX[i] << endl;
		cout << "world Y" << i + 1 << " " << wpointY[i] << endl;
	}*/


	Json::Value pts = root["points"];
	for (int i = 0; i < pts.size(); i++)
	{
		pointX[i][0] = pts[i]["pts_3d"]["X1"].asDouble();
		pointX[i][1] = pts[i]["pts_3d"]["X2"].asDouble();
		pointX[i][2] = pts[i]["pts_3d"]["X3"].asDouble();
		pointX[i][3] = pts[i]["pts_3d"]["X4"].asDouble();

		pointY[i][0] = pts[i]["pts_3d"]["Y1"].asDouble();
		pointY[i][1] = pts[i]["pts_3d"]["Y2"].asDouble();
		pointY[i][2] = pts[i]["pts_3d"]["Y3"].asDouble();
		pointY[i][3] = pts[i]["pts_3d"]["Y4"].asDouble();

	}
}
