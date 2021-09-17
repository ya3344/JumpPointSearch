#pragma once

class Visualization;
//#define NODE_TRADE

class JumpPointSearch
{
public:
	explicit JumpPointSearch(Visualization* visualization);
	~JumpPointSearch();

private:
	enum eAStarIndex
	{
		MAX_NODE_COUNT = 1000,
	};

private:
	struct AStarNodeInfo
	{
		explicit AStarNodeInfo(const float G, const float F, const WORD index, AStarNodeInfo* parent)
		{
			this->G = G;
			this->F = F;
			this->index = index;
			this->parent = parent;
		}

		float G = 0.f; // ������ ���� ���� �簢�������� ��� ���
		float F = 0.f; // ������� �̵��ϴµ� �ɸ� ���� �������� ��ģ ���(G + H) 
		
		WORD index = 0;
		AStarNodeInfo* parent = nullptr;
	};

public:
	bool Initialize(const WORD tile_MaxNumX, const WORD tile_MaxNumY);
	std::stack<AStarNodeInfo*>& GetBesRoadSpace(void) { return mBestRoadSpace; }
	void Release(void);

public:
	bool AStarStart(const WORD startIndex, const WORD finishIndex, vector<RectInfo*>& tileList);

private:
	bool FindRoute(vector<RectInfo*>& tileList);
	AStarNodeInfo* CreateNode(AStarNodeInfo* parent, const WORD index, const bool isDiagonal, vector<RectInfo*>& tileList);
	bool CheckList(const WORD index);
	static bool Compare(const AStarNodeInfo* srcNode, const AStarNodeInfo* compareNode);

private:
	list<AStarNodeInfo*> mOpenList;
	list<AStarNodeInfo*> mCloseList;
	stack<AStarNodeInfo*> mBestRoadSpace;

private:
	WORD mStartIndex = 0;
	WORD mFinishIndex = 0;
	WORD mTile_MaxNumX = 0;
	WORD mTile_MaxNumY = 0;

private:
	Visualization* mVisualization = nullptr;
};

