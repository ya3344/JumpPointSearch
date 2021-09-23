#pragma once

class Bresenham
{
public:
	Bresenham() = default;
	~Bresenham() = default;

public:
	void Cal_StraightLine(vector<POINT>* outStackPoint, const POINT& startPoint, const POINT& endPoint);
};

