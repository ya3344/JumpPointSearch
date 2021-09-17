#pragma once

class Visualization;

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

		WORD G = 0; // �θ������ ���� �簢�������� ��� ���
		WORD F = 0; // ������� �̵��ϴµ� �ɸ� ���� �������� ��ģ ���(G + H) 
		
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
	AStarNodeInfo* CreateNode(AStarNodeInfo* parent, const WORD index, vector<RectInfo*>& tileList);
	bool CheckList(const WORD index);
	static bool Compare(const AStarNodeInfo* srcNode, const AStarNodeInfo* compareNode);

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
};

