#include "pch.h"
#include "Bresenham.h"

void Bresenham::Cal_StraightLine(list<WORD>* outStraightIndex,
	const WORD startIndex, const WORD endIndex)
{
	WORD deltaX = 0;
	WORD deltaY = 0;
	bool isXPositive = false;
	bool isYPositive = false;
	WORD increaseAxis = 0;
	IndexInfo start_IndexInfo;
	IndexInfo end_IndexInfo;
	WORD index = 0;

	start_IndexInfo.yIndex = startIndex / gTile_MaxNumX;
	start_IndexInfo.xIndex = startIndex - (start_IndexInfo.yIndex * gTile_MaxNumX);
	end_IndexInfo.yIndex = endIndex / gTile_MaxNumX;
	end_IndexInfo.xIndex = endIndex - (end_IndexInfo.yIndex * gTile_MaxNumX);

	deltaX = abs(end_IndexInfo.xIndex - start_IndexInfo.xIndex);
	deltaY = abs(end_IndexInfo.yIndex - start_IndexInfo.yIndex);

	if (end_IndexInfo.xIndex - start_IndexInfo.xIndex < 0)
	{
		if(start_IndexInfo.xIndex < end_IndexInfo.xIndex)
			isXPositive = true;
		else
		{
			isXPositive = false;
		}
	}
	{
		if (start_IndexInfo.xIndex > end_IndexInfo.xIndex)
			isXPositive = false;
		else
		{
			isXPositive = true;
		}
	}

	if (end_IndexInfo.yIndex - start_IndexInfo.yIndex < 0)
	{
		if (start_IndexInfo.yIndex < end_IndexInfo.yIndex)
			isYPositive = false;
		else
			isYPositive = true;
	}
	else
	{
		if (start_IndexInfo.yIndex > end_IndexInfo.yIndex)
			isYPositive = false;
		else
			isYPositive = true;
	}

	if (deltaX > deltaY)
	{
		//increaseAxis += (deltaX / 2);
		for (int i = 0; i < deltaX; ++i)
		{
			if (true == isXPositive)
			{
				++start_IndexInfo.xIndex;
			}	
			else
			{
				--start_IndexInfo.xIndex;
			}

			if (0 == deltaY)
			{
				index = start_IndexInfo.xIndex + (start_IndexInfo.yIndex * gTile_MaxNumX);
				outStraightIndex->emplace_back(index);
				continue;
			}

			increaseAxis += deltaY;

			if (increaseAxis >= deltaX)
			{
				if (true == isYPositive)
				{
					++start_IndexInfo.yIndex;
				}
				else
				{
					--start_IndexInfo.yIndex;
				}
				increaseAxis -= deltaX;
				index = start_IndexInfo.xIndex + (start_IndexInfo.yIndex * gTile_MaxNumX);
				outStraightIndex->emplace_back(index);
			}
		}
	}
	else
	{
		//increaseAxis += (deltaY / 2);
		for (int i = 0; i < deltaY; ++i)
		{
			if (true == isYPositive)
			{
				++start_IndexInfo.yIndex;
			}
			else
			{
				--start_IndexInfo.yIndex;
			}

			if (0 == deltaX)
			{
				index = start_IndexInfo.xIndex + (start_IndexInfo.yIndex * gTile_MaxNumX);
				outStraightIndex->emplace_back(index);
				continue;
			}
			increaseAxis += deltaX;
			if (increaseAxis >= deltaY)
			{
				if (true == isXPositive)
				{
					++start_IndexInfo.xIndex;
				}
				else
				{
					--start_IndexInfo.xIndex;
				}
				increaseAxis -= deltaY;
				index = start_IndexInfo.xIndex + (start_IndexInfo.yIndex * gTile_MaxNumX);
				outStraightIndex->emplace_back(index);
			}
		}
	}
}
