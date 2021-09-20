#include "pch.h"
#include "Visualization.h"
#include "JumpPointSearch.h"

Visualization::~Visualization()
{
	SafeDelete(mJumpPointSearch);
	Release();
}

bool Visualization::Initialize()
{
	RectInfo* rectInfo = nullptr;

	// Render Init
	mhDC = GetDC(gHWnd);
	mhMemDC = CreateCompatibleDC(mhDC);

	mBitmap = CreateCompatibleBitmap(mhDC, WINDOW_WIDTH, WINDOW_HEIGHT);
	mOldBitmap = (HBITMAP)SelectObject(mhMemDC, mBitmap);
	
	//AStar Class 할당
	mJumpPointSearch = new JumpPointSearch(this);
	_ASSERT(mJumpPointSearch != nullptr);

	mRect = { -RECT_SIZE, -RECT_SIZE, RECT_SIZE, RECT_SIZE };
	mTile_MaxNumX = WINDOW_WIDTH / RECT_SIZE;
	mTile_MaxNumY = WINDOW_HEIGHT / RECT_SIZE;
	mJumpPointSearch->Initialize(mTile_MaxNumX, mTile_MaxNumY);

	// 타일 세팅
	for (WORD i = 1; i <= mTile_MaxNumY; ++i)
	{
		for (WORD j = 1; j <= mTile_MaxNumX; ++j)
		{
			rectInfo = new RectInfo;
			_ASSERT(rectInfo != nullptr);

			rectInfo->point.x = j * RECT_SIZE;
			rectInfo->point.y = i * RECT_SIZE;
			rectInfo->nodeIndex = NORMAL_INDEX;
			mTileList.emplace_back(rectInfo);
		}
	}

	Render();

	return true;
}

void Visualization::Release()
{
	SelectObject(mhMemDC, mOldBitmap);
	DeleteObject(mBitmap);
	DeleteDC(mhMemDC);

	for_each(mTileList.begin(), mTileList.end(), SafeDelete<RectInfo*>);
	mTileList.clear();
}

void Visualization::DrawTile(HDC hdc)
{
	RECT resultRect;
	RECT textRect;
	HBRUSH brush;
	HBRUSH oldBrush;
	TCHAR inputText[20] = { 0, };
	static bool isMoveTo = true;

	for (const RectInfo* rectInfo : mTileList)
	{
		resultRect = RectPointPlus(mRect, rectInfo->point);
		switch (rectInfo->nodeIndex)
		{
			case NORMAL_INDEX:
				{
					brush = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
				}
				break;
			case BLOCK_INDEX:
				{
					brush = (HBRUSH)CreateSolidBrush(RGB(125, 125, 125));
				}
				break;
			case START_INDEX:
				{
					brush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
					wsprintf(inputText, L"Start");
				}
				break;
			case FINISH_INDEX:
				{
					brush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
					wsprintf(inputText, L"Finish");					
				}
				break;
			case OPEN_INDEX:
				{
					brush = (HBRUSH)CreateSolidBrush(RGB(125, 125, 255));
				}
				break;
			case CLOSE_INDEX:
				{
					brush = (HBRUSH)CreateSolidBrush(RGB(235, 200, 0));
				}
				break;
			case SEARCH_INDEX:
				{
					brush = (HBRUSH)CreateSolidBrush(RGB(rectInfo->redColor, rectInfo->greenColor, rectInfo->blueColor));
				}
				break;
			default:
				{
					brush = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
				}
				break;
		}

		oldBrush = (HBRUSH)SelectObject(hdc, brush);
		
		Rectangle(hdc, resultRect.left, resultRect.top, resultRect.right, resultRect.bottom);

		// 텍스트
		textRect.left = resultRect.left - RECT_SIZE;
		textRect.top  = resultRect.top - RECT_SIZE;
		textRect.right = resultRect.right;
		textRect.bottom = resultRect.bottom;

		switch (rectInfo->nodeIndex)
		{
			case START_INDEX:
				{
					wsprintf(inputText, L"Start");
					DrawText(hdc, inputText, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				break;
			case FINISH_INDEX:
				{
					wsprintf(inputText, L"Finish");
					DrawText(hdc, inputText, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		
				}
				break;
#ifdef NODE_INFO_VIEW
			case OPEN_INDEX:
			case CLOSE_INDEX:
				{
					RECT tempRect;
					tempRect = textRect;
					tempRect.top = (LONG)(textRect.top - (RECT_SIZE * 0.7f));
					swprintf_s(inputText, _countof(inputText), L"%d   %d", rectInfo->G, rectInfo->H);
					DrawText(hdc, inputText, -1, &tempRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					tempRect.top = (LONG)(textRect.top + (RECT_SIZE * 0.7f));
					swprintf_s(inputText, _countof(inputText), L"%d", rectInfo->F);
					DrawText(hdc, inputText, -1, &tempRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
#endif
			default:
				{
					wsprintf(inputText, L"");
				}
				break;
		}
		

		SelectObject(hdc, oldBrush);
		DeleteObject(brush);
	}
}

void Visualization::DrawFinishLine(const POINT& point)
{
	HPEN pen;
	HPEN oldPen;

	pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	oldPen = (HPEN)SelectObject(mhMemDC, pen);
	if (true == mIsMoveTo)
	{
		MoveToEx(mhMemDC, point.x - (RECT_SIZE / 2), point.y - (RECT_SIZE / 2), nullptr);
		mIsMoveTo = false;
	}
	else
	{
		LineTo(mhMemDC, point.x - (RECT_SIZE / 2), point.y - (RECT_SIZE / 2));
	}

	SelectObject(mhMemDC, oldPen);
	DeleteObject(pen);
}

void Visualization::SetTilePicking(const RectInfo& rectInfo)
{
	WORD tileIndex;
	WORD x;
	WORD y;

	if (rectInfo.point.x > WINDOW_WIDTH || rectInfo.point.y > WINDOW_HEIGHT)
	{
		return;
	}
	x = (WORD)rectInfo.point.x / RECT_SIZE;
	y = (WORD)rectInfo.point.y / RECT_SIZE;
	wprintf(L"xindex:%d yindex:%d\n", x, y);

	tileIndex = (y * mTile_MaxNumX) + x;
	wprintf(L"tildIndex:%d\n", tileIndex);
	if (tileIndex >= mTileList.size())
	{
		wprintf(L"tildIndex overflow!!:%d\n", tileIndex);
		return;
	}

	switch (rectInfo.nodeIndex)
	{
		case START_INDEX:
			{
				for (RectInfo* rectInfo : mTileList)
				{
					if(rectInfo->nodeIndex != BLOCK_INDEX)
						rectInfo->nodeIndex = NORMAL_INDEX;
				}
				mTileList[mPrevStart_TileIndex]->nodeIndex = NORMAL_INDEX;
				mTileList[mPrevFinish_TileIndex]->nodeIndex = NORMAL_INDEX;
				mPrevStart_TileIndex = tileIndex;
				mIsAStarStart = false;
			}
			break;
		case FINISH_INDEX:
			{
				mPrevFinish_TileIndex = tileIndex;
				// 길찾기 시작
				mIsAStarStart = true;
			}
			break;
		default:
			{
				
			}
			break;
	}

	mTileList[tileIndex]->nodeIndex = rectInfo.nodeIndex;
	Render();
	
}

void Visualization::SetBlockIndexClear()
{
	for (RectInfo* rectInfo : mTileList)
	{
		if (rectInfo->nodeIndex == BLOCK_INDEX)
			rectInfo->nodeIndex = NORMAL_INDEX;
	}
	Render();
}

void Visualization::RandomColorSetting()
{
	// random setting
	static random_device randDevice;
	static mt19937_64 rand(randDevice());
	static uniform_int_distribution<__int16> randDist(0, 255);

	mRed = (BYTE)randDist(rand);
	mGreen = (BYTE)randDist(rand);
	mBlue = (BYTE)randDist(rand);
}

void Visualization::AStarWorking()
{
	if (true == mIsAStarStart)
	{
		mIsMoveTo = true; // 출발지와 목적지 간의 경로를 선으로 표시 하기 위한 작업 진행
		if (false == mJumpPointSearch->AStarStart(mPrevStart_TileIndex, mPrevFinish_TileIndex, mTileList))
		{
			MESSAGE_BOX(L"해당 위치로는 목적지를 설정 할 수 없습니다.!");
			mIsAStarStart = false;
			return;
		}

		mIsAStarStart = false;
	}
}

void Visualization::Render()
{
	// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
	SetBkMode(mhMemDC, TRANSPARENT);
	SetTextColor(mhMemDC, RGB(255, 255, 255));
	FillRect(mhMemDC, &gWindowRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DrawTile(mhMemDC);
	BitBlt(mhDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mhMemDC, 0, 0, SRCCOPY);
}

void Visualization::RenderBitBlt()
{
	BitBlt(mhDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mhMemDC, 0, 0, SRCCOPY);
}

inline RECT Visualization::RectPointPlus(const RECT rect, const POINT& point)
{
	RECT rectResult =
	{
		point.x + rect.left,
		point.y + rect.top,
		point.x + rect.right,
		point.y + rect.bottom
	};
	return rectResult;
}