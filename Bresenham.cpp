#include "pch.h"
#include "Bresenham.h"

void Bresenham::Cal_StraightLine(list<WORD>* outStraightIndex,
	const WORD startIndex, const WORD endIndex)
{
	WORD deltaX = 0;
	WORD deltaY = 0;
	bool isXPositive = false;
	bool isYPositive = false;
	float increaseAxis = 0.f;
	IndexInfo start_IndexInfo;
	IndexInfo end_IndexInfo;
	WORD index = 0;
	//WORD thresh = 0;
	float thresh = 0.f;

	start_IndexInfo.yIndex = startIndex / gTile_MaxNumX;
	start_IndexInfo.xIndex = startIndex - (start_IndexInfo.yIndex * gTile_MaxNumX);
	end_IndexInfo.yIndex = endIndex / gTile_MaxNumX;
	end_IndexInfo.xIndex = endIndex - (end_IndexInfo.yIndex * gTile_MaxNumX);

	deltaX = abs(end_IndexInfo.xIndex - start_IndexInfo.xIndex);
	deltaY = abs(end_IndexInfo.yIndex - start_IndexInfo.yIndex);

	// 시작점과 목표지점 차이를 구하여 음수 혹은 양수방향 진행 상태 확인
	if (end_IndexInfo.xIndex - start_IndexInfo.xIndex < 0)
	{
		if(start_IndexInfo.xIndex < end_IndexInfo.xIndex)
			isXPositive = true;
		else
		{
			isXPositive = false;
		}
	}
	else
	{
		if (start_IndexInfo.xIndex > end_IndexInfo.xIndex)
			isXPositive = false;
		else
		{
			isXPositive = true;
		}
	}

	// 시작점과 목표지점 차이를 구하여 음수 혹은 양수방향 진행 상태 확인
	if (end_IndexInfo.yIndex - start_IndexInfo.yIndex < 0)
	{
		isYPositive = false;
	}
	else
	{
		isYPositive = true;
	}

	if (deltaX > deltaY)
	{
		/*if (0 == deltaX % 2)
			thresh = (deltaX / 2);
		else
		{
			thresh = deltaX;*/
		//	increaseAxis += (deltaX / 2);
		//}
		thresh = (float)(deltaX / 2);

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
				if(startIndex != index && endIndex != index)
					outStraightIndex->emplace_back(index);
				continue;
			}

			increaseAxis += deltaY;

			if (increaseAxis > thresh)
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
			}
			index = start_IndexInfo.xIndex + (start_IndexInfo.yIndex * gTile_MaxNumX);
			if (startIndex != index && endIndex != index)
			{	
				outStraightIndex->emplace_back(index);
			}
		}
	}
	else
	{
	/*	if (0 == deltaY % 2)
			thresh = (deltaY / 2);
		else
		{
			thresh = deltaY;*/
			//increaseAxis += (deltaY / 2);
		//}
		thresh = (float)(deltaY / 2);
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
				if (startIndex != index && endIndex != index)
					outStraightIndex->emplace_back(index);
				continue;
			}
			increaseAxis += deltaX;
			if (increaseAxis > thresh)
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
						
			}
			index = start_IndexInfo.xIndex + (start_IndexInfo.yIndex * gTile_MaxNumX);
			if (startIndex != index && endIndex != index)
			{
				outStraightIndex->emplace_back(index);
			}
		}
	}
}
