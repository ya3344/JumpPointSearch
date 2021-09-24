#pragma once

extern HWND gHWnd;
extern RECT gWindowRect;

enum eTileIndex
{
	START_INDEX,
	FINISH_INDEX,
	NORMAL_INDEX,
	BLOCK_INDEX,
	OPEN_INDEX,
	CLOSE_INDEX,
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

extern WORD gTile_MaxNumX;
extern WORD gTile_MaxNumY;