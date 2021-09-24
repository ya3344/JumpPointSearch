#pragma once
class Visualization
{
public:
	Visualization() = default;
	~Visualization();

public:
	bool Initialize();
	void Release();
	void DrawTile(HDC hdc);
	void DrawFinishLine(const POINT& point, const COLORREF& colorRef);
	void DrawBresenhamLine(const POINT& point, const COLORREF& colorRef);
	void SetMoveTo(const bool isMoveTo) { mIsMoveTo = isMoveTo; }
	bool GetMoveTo() const { return mIsMoveTo; }
	void SetTilePicking(const RectInfo& rectInfo);
	void SetBlockIndexClear();

public:
	void RandomColorSetting();
	void AStarWorking();
	void Render();
	void RenderBitBlt();
private:
	inline RECT RectPointPlus(const RECT rect, const POINT& point);

private:
	WORD mPrevStart_TileIndex = 0;
	WORD mPrevFinish_TileIndex = 0;
	RECT mRect;

private:
	vector<RectInfo*> mTileList;
	class JumpPointSearch* mJumpPointSearch = nullptr;
	bool mIsAStarStart = false;
	bool mIsMoveTo = false;
	bool mIsBresenham_MoveTo = false;

//API Render 
private:
	HDC mhDC;
	HDC mhMemDC;
	HBITMAP mBitmap;
	HBITMAP mOldBitmap;

// RandomColorSetting
private:
	BYTE mRed;
	BYTE mGreen;
	BYTE mBlue;
};

