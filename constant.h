#pragma once

extern HWND gHWnd;
extern RECT gWindowRect;

enum eTileIndex
{
	NORMAL_INDEX = 0,
	BLOCK_INDEX,
	START_INDEX,
	FINISH_INDEX,
	OPEN_INDEX,
	CLOSE_INDEX,
	SEARCH_INDEX,
	RECT_SIZE = 60,
};

enum eWindowINdex
{
	WINDOW_WIDTH = 1680,
	WINDOW_HEIGHT = 960,
};

struct RectInfo
{
	POINT point;
	BYTE nodeIndex;
	WORD G = 0;
	WORD H = 0;
	WORD F = 0;
	BYTE redColor = 0;
	BYTE greenColor = 0;
	BYTE blueColor = 0;
};