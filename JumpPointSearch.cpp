#include "pch.h"
#include "JumpPointSearch.h"
#include "Visualization.h"

JumpPointSearch::JumpPointSearch(Visualization* visualization)
{
	mVisualization = visualization;
	_ASSERT(mVisualization != nullptr);
}

JumpPointSearch::~JumpPointSearch()
{
	Release();
}

bool JumpPointSearch::Initialize(const WORD tile_MaxNumX, const WORD tile_MaxNumY)
{
	mTile_MaxNumX = tile_MaxNumX;
	mTile_MaxNumY = tile_MaxNumY;
	wprintf(L"mTile_MaxNumX:%d mTile_MaxNumY:%d", tile_MaxNumX, tile_MaxNumY);
	return true;
}


bool JumpPointSearch::AStarStart(const WORD startIndex, const WORD finishIndex, vector<RectInfo*>& tileList)
{
	if (startIndex == finishIndex)
	{
		return false;
	}

	if (finishIndex < 0 || finishIndex >= tileList.size())
		return false;

	if (BLOCK_INDEX == tileList[finishIndex]->nodeIndex || BLOCK_INDEX == tileList[startIndex]->nodeIndex)
		return false;

	mStartIndex = startIndex;
	mFinishIndex = finishIndex;

	Release();
	
	return FindRoute(tileList);
}

bool JumpPointSearch::FindRoute(vector<RectInfo*>& tileList)
{
	AStarNodeInfo* node = nullptr;
	WORD count = 0;
	WORD index = 0;
	WORD finishCount = 0;

	AStarNodeInfo* parent = new AStarNodeInfo(0, 0, mStartIndex, nullptr);
	mCloseList.emplace_back(parent);
	
	while (true)
	{
		/*if (count >= MAX_NODE_COUNT)
		{
			wprintf(L"MAX_NODE_COUNT ERROR!");
			Release();
			return false;
		}*/
		// 부모가 없을 경우 전체 8방향 검색 시작
		if (parent->parent)
		{

		}
		// 부모가 있을 경우
		else
		{

		}
		// 위
		index = parent->index - mTile_MaxNumX;

		if (parent->index >= mTile_MaxNumX && tileList[index]->nodeIndex != BLOCK_INDEX
			&& CheckList(index))
		{
			node = CreateNode(parent, index, tileList);
			if(node != nullptr)
				mOpenList.emplace_back(node);
		}

		// 오른쪽 위
		index = (parent->index - mTile_MaxNumX) + 1;

		if (parent->index >= mTile_MaxNumX && (parent->index % mTile_MaxNumX) != mTile_MaxNumX - 1
			&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);
		}

		// 오른쪽
		index = parent->index + 1;

		if ((parent->index % mTile_MaxNumX) != mTile_MaxNumX - 1
			&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);
		}

		// 오른쪽 아래 
		index = (parent->index + mTile_MaxNumX) + 1;

		if ((parent->index / mTile_MaxNumX) < mTile_MaxNumY - 1  && (parent->index % mTile_MaxNumX) != mTile_MaxNumX - 1
			&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);
		}

		// 아래
		index = parent->index + mTile_MaxNumX;

		if ((parent->index / mTile_MaxNumX) < mTile_MaxNumY - 1
			&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);
		}

		// 왼쪽 아래
		index = (parent->index + mTile_MaxNumX) - 1;

		if ((parent->index / mTile_MaxNumX) < mTile_MaxNumY - 1 && (parent->index % mTile_MaxNumX) > 0
			&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);
		}

		// 왼쪽
		index = parent->index - 1;

		if ((parent->index % mTile_MaxNumX) > 0
			&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);
		}

		// 왼쪽 위 
		index = (parent->index - mTile_MaxNumX) - 1;
		if ((parent->index % mTile_MaxNumX) > 0 && parent->index >= mTile_MaxNumX
			&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);
		}

		if (mOpenList.empty())
		{
			return false;
		}
			
		// 가중치 기준으로 소팅한다.
		mOpenList.sort(Compare);

		const auto iterOpenList = mOpenList.begin();
		mCloseList.emplace_back(*iterOpenList);

		// Render
		if ((*iterOpenList)->index != mFinishIndex && (*iterOpenList)->index != mStartIndex)
		{
			tileList[(*iterOpenList)->index]->nodeIndex = CLOSE_INDEX;
		}

		parent = *iterOpenList;
		mOpenList.erase(iterOpenList);

		if (parent->index == mFinishIndex)
		{
			wprintf(L"closeListNum:%zd  openListNum:%zd\n", mCloseList.size(), mOpenList.size());

			while (true)
			{
				//Visualization AStar 길찾기 경로 표시
				mVisualization->DrawFinishLine(tileList[parent->index]->point);
				// 경로를 생성해준다.
				if (parent->parent == nullptr)
				{
					wprintf(L"finishCount:%d\n", finishCount);
					//Visualization AStar 길찾기 경로 표시
					mVisualization->RenderBitBlt();
					break;
				}

				parent = parent->parent;
				++finishCount;
			}
			break;
		}

		++count;
	}
	return true;
}

JumpPointSearch::AStarNodeInfo* JumpPointSearch::CreateNode(AStarNodeInfo* parent, const WORD index, vector<RectInfo*>& tileList)
{
	//Sleep(50);
	WORD dx = 0;
	WORD dy = 0;
	WORD H = 0;
	WORD G = 0;
	WORD F = 0;
	// Manhattan 방식 사용하여 G(부모 사각형에서 노드 비용 계산)
	dx = (WORD)(abs(tileList[parent->index]->point.x - tileList[index]->point.x)) / RECT_SIZE;
	dy = (WORD)(abs(tileList[parent->index]->point.y - tileList[index]->point.y)) / RECT_SIZE;
	G = (dx + dy);

	// Manhattan 방식 사용하여 H(현재 사각형에서 목적지 비용 계산)
	dx = (WORD)(abs(tileList[mFinishIndex]->point.x - tileList[index]->point.x)) / RECT_SIZE;
	dy = (WORD)(abs(tileList[mFinishIndex]->point.y - tileList[index]->point.y)) / RECT_SIZE;
	H = (dx + dy); 

	// 현재까지 이동하는데 걸린 비용과 예상 비용을 합친 총 비용
	F = G + H;

	// 기존에 오프리스트에 있는 노드와 비교하여 대체 진행
	for (AStarNodeInfo* pNode : mOpenList)
	{
		if (pNode->index == index)
		{
			// G 가 더적기 때문에 G 및 부모 교체
			if (G < pNode->G)
			{
				pNode->G = G;
				pNode->parent = parent;
				return nullptr;
			}
			else
			{
				return nullptr;
			}
		}
	}

	AStarNodeInfo* node = new AStarNodeInfo(G, F, index, parent);

	// Visualization 노드 표시를 위한 정보 기입
	tileList[index]->F = F;
	tileList[index]->G = G;
	tileList[index]->H = H;
	// Visualization 노드 색깔변화 표시
	if(index != mStartIndex && index != mFinishIndex)
		tileList[index]->nodeIndex = OPEN_INDEX;

	mVisualization->Render();

	return node;
}

bool JumpPointSearch::CheckList(const WORD index)
{
	for (const AStarNodeInfo* pNode : mCloseList)
	{
		if (pNode->index == index)
			return false;
	}

	return true;
}

bool JumpPointSearch::Compare(const AStarNodeInfo* srcNode, const AStarNodeInfo* compareNode)
{
	return srcNode->F < compareNode->F;
}


void JumpPointSearch::Release(void)
{
	for_each(mOpenList.begin(), mOpenList.end(), SafeDelete<AStarNodeInfo*>);
	mOpenList.clear();

	for_each(mCloseList.begin(), mCloseList.end(), SafeDelete<AStarNodeInfo*>);
	mCloseList.clear();
}
