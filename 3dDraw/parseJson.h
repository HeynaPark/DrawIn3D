#pragma once
#include "json/json.h"

class parseJson {
public:
	void parsing_json();
	double wpointX[4] = {};
	double wpointY[4] = {};

	double pointX[20][4] = { {},{},{},{} };		// dsc ������ŭ �迭 ����
	double pointY[20][4] = { {},{},{},{} };
};