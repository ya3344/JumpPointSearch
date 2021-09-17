#pragma once

extern HWND gHWnd;
extern RECT gWindowRect;
extern bool gIsNodeTrade;

enum eTileIndex
{
	NORMAL_INDEX = 0,
	BLOCK_INDEX,
	START_INDEX,
	FINISH_INDEX,
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
	float G = 0.f;
	WORD H = 0;
	float F = 0.f;
};