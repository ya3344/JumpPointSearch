#pragma once

class Visualization;

class JumpPointSearch
{
public:
	JumpPointSearch() = delete;
	explicit JumpPointSearch(Visualization* visualization);
	~JumpPointSearch();

private:
	enum eAStarIndex
	{
		MAX_NODE_COUNT = 1000,
	};
	enum eNodeDirection
	{
		UP,
		LEFT,
		RIGHT,
		LEFT_UP,
		RIGHT_UP,
		DOWN,
		LEFT_DOWN,
		RIGHTR_DOWN,
	};

private:
	struct AStarNodeInfo
	{
		explicit AStarNodeInfo(const WORD G, const WORD F, const WORD index, AStarNodeInfo* parent)
		{
			this->G = G;
			this->F = F;
			this->index = index;
			this->parent = parent;
		}

		WORD G = 0; // 부모노드부터 현재 사각형까지의 경로 비용
		WORD F = 0; // 현재까지 이동하는데 걸린 비용과 예상비용을 합친 비용(G + H) 
		
		WORD index = 0;
		AStarNodeInfo* parent = nullptr;
	};

public:
	bool Initialize(const WORD tile_MaxNumX, const WORD tile_MaxNumY);
	void Release(void);

public:
	bool AStarStart(const WORD startIndex, const WORD finishIndex, vector<RectInfo*>& tileList);

private:
	bool FindRoute(vector<RectInfo*>& tileList);
	inline void SearchCornerNode(const BYTE direction, AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList);
	AStarNodeInfo* CreateNode(AStarNodeInfo* parent, const WORD index, vector<RectInfo*>& tileList);
	bool CheckList(const WORD index);
	static bool Compare(const AStarNodeInfo* srcNode, const AStarNodeInfo* compareNode);
	
// Search Corner Node Function
private:
	bool SearchUp_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList, const bool isNodeCreate = true);
	bool SearchLeft_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList, const bool isNodeCreate = true);
	bool SearchLeftUp_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList);
	bool SearchRight_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList, const bool isNodeCreate = true);
	bool SearchRightUp_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList);
	bool SearchDown_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList, const bool isNodeCreate = true);
	bool SearchLeftDown_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList);
	bool SearchRightDown_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList);

private:
	void SearchIndexRender(const BYTE nodeIndex, const WORD index, vector<RectInfo*>& tileList);
	void RandomColorSetting();

private:
	list<AStarNodeInfo*> mOpenList;
	list<AStarNodeInfo*> mCloseList;

private:
	WORD mStartIndex = 0;
	WORD mFinishIndex = 0;
	WORD mTile_MaxNumX = 0;
	WORD mTile_MaxNumY = 0;

private:
	Visualization* mVisualization = nullptr;

	// RandomColorSetting
private:
	BYTE mRed = 0;
	BYTE mGreen = 0;
	BYTE mBlue = 0;
};

