#pragma once
#include "json/json.h"

class parseJson {
public:
	void parsing_json();
	double wpointX[4] = {};
	double wpointY[4] = {};

	double pointX[20][4] = { {},{},{},{} };		// dsc 개수만큼 배열 생성
	double pointY[20][4] = { {},{},{},{} };
};