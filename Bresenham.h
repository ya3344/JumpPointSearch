#pragma once


class Bresenham
{
public:
	Bresenham() = default;
	~Bresenham() = default;
public:
	struct IndexInfo
	{
		WORD xIndex = 0;
		WORD yIndex = 0;
	};

public:
	void Cal_StraightLine(list<WORD>* outStraightIndex,
		const WORD startIndex, const WORD endIndex);
};

