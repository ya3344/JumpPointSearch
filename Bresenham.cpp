#include "pch.h"
#include "Bresenham.h"

void Bresenham::Cal_StraightLine(vector<POINT>* outStackPoint, const POINT& startPoint, const POINT& endPoint)
{
	WORD deltaX = (WORD)abs(endPoint.x - startPoint.x);
	WORD deltaY = (WORD)abs(endPoint.y - startPoint.y);
	bool isXPositive = false;
	bool isYPositive = false;
	WORD increaseAxis = 0;
	POINT point = startPoint;

	if (endPoint.x - startPoint.x < 0)
	{
		isXPositive = false;
	}
	else
	{
		isXPositive = true;
	}

	if (endPoint.y - startPoint.y < 0)
	{
		isYPositive = false;
	}
	else
	{
		isYPositive = true;
	}

	if (deltaX > deltaY)
	{
		for (int i = 0; i < deltaX; ++i)
		{
			if (true == isXPositive)
			{
				++point.x;
			}	
			else
			{
				--point.x;
			}

			if (0 == deltaY)
			{
				outStackPoint->emplace_back(point);
				continue;
			}

			increaseAxis += deltaY;

			if (increaseAxis >= deltaX)
			{
				if (true == isYPositive)
				{
					++point.y;
				}
				else
				{
					--point.y;
				}
				increaseAxis -= deltaX;
				outStackPoint->emplace_back(point);
			}
		}
	}
	else
	{
		for (int i = 0; i < deltaY; ++i)
		{
			if (true == isYPositive)
			{
				++point.y;
			}
			else
			{
				--point.y;
			}

			if (0 == deltaX)
			{
				outStackPoint->emplace_back(point);
				continue;
			}
			increaseAxis += deltaX;
			if (increaseAxis >= deltaY)
			{
				if (true == isXPositive)
				{
					++point.x;
				}
				else
				{
					--point.x;
				}
				increaseAxis -= deltaY;
				outStackPoint->emplace_back(point);
			}
		}
	}
}
