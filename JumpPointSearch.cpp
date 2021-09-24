#include "pch.h"
#include "JumpPointSearch.h"
#include "Visualization.h"
#include "Bresenham.h"

JumpPointSearch::JumpPointSearch(Visualization* visualization)
{
	mVisualization = visualization;
	mBresenham = new Bresenham;
	_ASSERT(mVisualization != nullptr);
	_ASSERT(mBresenham != nullptr);
}

JumpPointSearch::~JumpPointSearch()
{
	Release();
	SafeDelete(mBresenham);
}

//bool JumpPointSearch::Initialize(const WORD tile_MaxNumX, const WORD tile_MaxNumY)
//{
//	gTile_MaxNumX = tile_MaxNumX;
//	mTile_MaxNumY = tile_MaxNumY;
//	wprintf(L"gTile_MaxNumX:%d mTile_MaxNumY:%d", tile_MaxNumX, tile_MaxNumY);
//	return true;
//}


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
	WORD findIndex = 0;
	WORD parentIndex = 0;
	WORD finishCount = 0;
	POINT directionPoint;
	queue<POINT> bresenhamPoint;
	AStarNodeInfo* parent = new AStarNodeInfo(0, 0, mStartIndex, nullptr);
	mCloseList.emplace_back(parent);
	
	while (true)
	{
		index = parent->index;
		// 부모가 없을 경우 전체 8방향 검색 시작
		if (nullptr == parent->parent)
		{
			RandomColorSetting();
			// 윗쪽
			SearchUp_CornerNode(parent, index, tileList, true);
			// 왼쪽
			//RandomColorSetting();
			SearchLeft_CornerNode(parent, index, tileList, true);
			// 왼쪽 위
			//RandomColorSetting();
			SearchLeftUp_CornerNode(parent, index, tileList);
			// 오른쪽
			//RandomColorSetting();
			SearchRight_CornerNode(parent, index, tileList, true);
			// 오른쪽 위
			//RandomColorSetting();
			SearchRightUp_CornerNode(parent, index, tileList);
			// 아래쪽
			//RandomColorSetting();
			SearchDown_CornerNode(parent, index, tileList, true);
			// 왼쪽 아래
			//RandomColorSetting();
			SearchLeftDown_CornerNode(parent, index, tileList);
			// 오른쪽 아래
			//RandomColorSetting();
			SearchRightDown_CornerNode(parent, index, tileList);

		}
		// 부모가 있을 경우
		else
		{
			parentIndex = parent->parent->index;
			directionPoint.x = tileList[index]->point.x - tileList[parentIndex]->point.x;
			directionPoint.y = tileList[index]->point.y - tileList[parentIndex]->point.y;
			RandomColorSetting();
			// 위쪽방향 탐색
			if (directionPoint.x == 0 && directionPoint.y < 0)
			{
				// 최대 3방향 탐색 가능
				SearchUp_CornerNode(parent, index, tileList, true);
				
				// 위쪽으로 갈 수 있는지 체크
				if (index >= gTile_MaxNumX)
				{
					// 위쪽으로 코너 찾기
					findIndex = index - gTile_MaxNumX;

					// 오른쪽 코너 체크
					if (index % gTile_MaxNumX != gTile_MaxNumX - 1 && findIndex % gTile_MaxNumX != gTile_MaxNumX - 1)
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index + 1]->nodeIndex && NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex)
						{
							SearchRightUp_CornerNode(parent, index, tileList);
						}
					}
					// 왼쪽 코너 체크
					if (index % gTile_MaxNumX > 0 && findIndex % gTile_MaxNumX > 0)
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index - 1]->nodeIndex && NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex)
						{
							SearchLeftUp_CornerNode(parent, index, tileList);
						}
					}
				}		
			}
			// 왼쪽 방향 탐색
			else if (directionPoint.x < 0 && directionPoint.y == 0)
			{
				// 최대 3방향 탐색 가능
				SearchLeft_CornerNode(parent, index, tileList, true);
			
				// 왼쪽으로 더이상 갈 수 없는지 체크
				if (index % gTile_MaxNumX > 0)
				{
					// 왼쪽으로 코너 찾기
					findIndex = index - 1;

					// 위쪽 코너 체크
					if (index >= gTile_MaxNumX && 
						findIndex >= gTile_MaxNumX)
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex // 위쪽 체크
							&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex) // 위쪽 체크
						{
							SearchLeftUp_CornerNode(parent, index, tileList);
						}
					}
					// 아래쪽 코너 체크
					if (index / gTile_MaxNumX < gTile_MaxNumY - 1 && 
						findIndex / gTile_MaxNumX < gTile_MaxNumY - 1)
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex // 아래쪽 체크
							&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex) // 아래쪽 체크
						{
							SearchLeftDown_CornerNode(parent, index, tileList);
						}
					}
				}
			}
			// 왼쪽 위 방향 탐색
			else if (directionPoint.x < 0 && directionPoint.y < 0)
			{
				// 최대 5방향 탐색 가능
				SearchLeftUp_CornerNode(parent, index, tileList);
				//RandomColorSetting();
				SearchLeft_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();
				SearchUp_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();
				// 코너 찾기 한칸 위로 체크
				if (index >= gTile_MaxNumX)
				{
					findIndex = index - gTile_MaxNumX;

					// 위쪽 코너 체크
					if (index % gTile_MaxNumX != gTile_MaxNumX - 1 &&  //오른쪽 체크
						findIndex % gTile_MaxNumX != gTile_MaxNumX - 1) // 오른쪽 체크
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index + 1]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex)
						{
							SearchRightUp_CornerNode(parent, index, tileList);
						}
					}
				}

				// 코너 찾기 한칸 왼쪽 체크
				if ((index % gTile_MaxNumX) > 0)
				{
					findIndex = index - 1;

					// 아래쪽 코너 체크
					if (index / gTile_MaxNumX < gTile_MaxNumY - 1 && // 아래쪽 체크
						findIndex / gTile_MaxNumX < gTile_MaxNumY - 1) // 아래쪽 체크
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex)
						{
							SearchLeftDown_CornerNode(parent, index, tileList);
						}
					}
				}
			}
			// 오른쪽 방향 탐색
			else if (directionPoint.x > 0 && directionPoint.y == 0)
			{
				// 최대 3방향 탐색 가능
				SearchRight_CornerNode(parent, index, tileList, true);

				// 오른쪽으로 더이상 갈 수 있는지 체크
				if (index % gTile_MaxNumX != gTile_MaxNumX - 1)
				{
					findIndex = index + 1;
					// 위쪽 코너 체크
					if (index >= gTile_MaxNumX && findIndex >= gTile_MaxNumX)
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex // 위쪽 체크
							&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex) // 위쪽 체크
						{
							SearchRightUp_CornerNode(parent, index, tileList);
						}
					}
					// 아래쪽 코너 체크
					if (index / gTile_MaxNumX < gTile_MaxNumY - 1
						&& findIndex / gTile_MaxNumX < gTile_MaxNumY - 1)
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex // 아래쪽 체크
							&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex) // 아래쪽 체크
						{
							SearchRightDown_CornerNode(parent, index, tileList);
						}
					}
				}		
			}
			// 오른쪽 위 방향 탐색
			else if (directionPoint.x > 0 && directionPoint.y < 0)
			{
				// 최대 5방향 탐색 가능
				SearchRightUp_CornerNode(parent, index, tileList);
				//RandomColorSetting();
				SearchRight_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();
				SearchUp_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();

				// 코너 찾기 한칸 위로 체크
				if (index >= gTile_MaxNumX)
				{
					findIndex = index - gTile_MaxNumX;

					// 왼쪽 코너 체크
					if (index % gTile_MaxNumX > 0 &&  //왼쪽 체크
						findIndex % gTile_MaxNumX > 0) // 왼쪽 체크
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index - 1]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex)
						{
							SearchLeftUp_CornerNode(parent, index, tileList);
						}
					}
				}

				// 코너 찾기 한칸 오른쪽 체크
				if (index % gTile_MaxNumX != gTile_MaxNumX - 1)
				{
					findIndex = index + 1;
					// 아래쪽 코너 체크
					if (index / gTile_MaxNumX < gTile_MaxNumY - 1 && // 아래쪽 체크
						findIndex / gTile_MaxNumX < gTile_MaxNumY - 1) // 아래쪽 체크
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex)
						{
							SearchRightDown_CornerNode(parent, index, tileList);
						}
					}
				}
			}
			// 아래 방향 탐색
			else if (directionPoint.x == 0 && directionPoint.y > 0)
			{
				// 최대 3방향 탐색 가능
				SearchDown_CornerNode(parent, index, tileList, true);

				// 아래쪽으로 코너 찾기
				if ((index / gTile_MaxNumX) < gTile_MaxNumY - 1) // 아래쪽으로 더이상 갈 수 있는지 체크
				{
					findIndex = index + gTile_MaxNumX;

					// 왼쪽 코너 체크
					if (index % gTile_MaxNumX > 0
						&& findIndex % gTile_MaxNumX > 0)
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index - 1]->nodeIndex // 왼쪽 체크
							&& NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex) // 왼쪽 체크
						{
							SearchLeftDown_CornerNode(parent, index, tileList);
						}
					}
					// 오른쪽 코너 체크
					if (index % gTile_MaxNumX != gTile_MaxNumX - 1
						&& findIndex % gTile_MaxNumX != gTile_MaxNumX - 1)
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index + 1]->nodeIndex // 오른쪽 체크
							&& NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex) // 오른쪽 체크
						{
							SearchRightDown_CornerNode(parent, index, tileList);
						}
					}
				}		
			}
			// 왼쪽 아래 방향 탐색
			else if (directionPoint.x < 0 && directionPoint.y > 0)
			{
				// 최대 5방향 탐색 가능
				SearchLeftDown_CornerNode(parent, index, tileList);
				//RandomColorSetting();
				SearchLeft_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();
				SearchDown_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();

				// 코너 찾기 한칸 왼쪽 체크
				if ((index % gTile_MaxNumX) > 0)
				{
					findIndex = index - 1;

					// 위쪽 코너 체크
					if (index >= gTile_MaxNumX && // 위쪽 체크
						findIndex >= gTile_MaxNumX) // 위쪽 체크
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex)
						{
							SearchLeftUp_CornerNode(parent, index, tileList);
						}
					}
				}

				// 코너 찾기 아래로 체크
				if (index / gTile_MaxNumX < gTile_MaxNumY - 1)
				{
					findIndex = index + gTile_MaxNumX;

					// 위쪽 코너 체크
					if (index % gTile_MaxNumX != gTile_MaxNumX - 1 &&  //오른쪽 체크
						findIndex % gTile_MaxNumX != gTile_MaxNumX - 1) // 오른쪽 체크
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index + 1]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex)
						{
							SearchRightDown_CornerNode(parent, index, tileList);
						}
					}
				}
			}
			// 오른쪽 아래 방향 탐색
			else if (directionPoint.x > 0 && directionPoint.y > 0)
			{
				// 최대 5방향 탐색 가능
				SearchRightDown_CornerNode(parent, index, tileList);
				//RandomColorSetting();
				SearchRight_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();
				SearchDown_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();

				// 코너 찾기 한칸 오른쪽 체크
				if ((index % gTile_MaxNumX) != gTile_MaxNumX - 1)
				{
					findIndex = index + 1;

					// 위쪽 코너 체크
					if (index >= gTile_MaxNumX && // 위쪽 체크
						findIndex >= gTile_MaxNumX) // 위쪽 체크
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex)
						{
							SearchRightUp_CornerNode(parent, index, tileList);
						}
					}
				}

				// 코너 찾기 아래로 체크
				if (index / gTile_MaxNumX < gTile_MaxNumY - 1)
				{
					findIndex = index + gTile_MaxNumX;

					// 위쪽 코너 체크
					if (index % gTile_MaxNumX > 0 &&  //왼쪽 체크
						findIndex % gTile_MaxNumX > 0) //왼쪽 체크
					{
						// 코너로 판단
						if (BLOCK_INDEX == tileList[index - 1]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex)
						{
							SearchLeftDown_CornerNode(parent, index, tileList);
						}
					}
				}
			}
		}

		if (mOpenList.empty())
		{
			return false;
		}
			
		// 가중치 기준으로 소팅한다.
		mOpenList.sort(Compare);

		const auto& iterOpenList = mOpenList.begin();
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
			CalBestRoadSpaec(parent, tileList);
			while (true)
			{
				//Visualization AStar 길찾기 경로 표시
				mVisualization->DrawFinishLine(tileList[parent->index]->point, RGB(0, 255, 0));
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
	G = (dx + dy) + tileList[parent->index]->G;

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

bool JumpPointSearch::SearchUp_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList, const bool isNodeCreate)
{
	AStarNodeInfo* node = nullptr;
	short findIndex = 0;

	while (true)
	{
		// 위쪽으로 더이상 갈 수 없음
		if (index < gTile_MaxNumX)
			return false;

		// 윗쪽으로 이동
		index = index - gTile_MaxNumX;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// 탐색한 위치는 노드인덱스로 표기
		SearchIndexRender(index, tileList);

		// 목적지 검색
		if (mFinishIndex == index)
		{
			if (true == isNodeCreate)
			{
				node = CreateNode(parent, index, tileList);
				if (node != nullptr)
					mOpenList.emplace_back(node);
			}
			return true;
		}

		// 위쪽으로 더이상 갈 수 없는지 체크
		if (index < gTile_MaxNumX)
			continue;

		// 위쪽으로 코너 찾기
		findIndex = index - gTile_MaxNumX;

		// 오른쪽 코너 체크
		if (index % gTile_MaxNumX != gTile_MaxNumX - 1 && findIndex % gTile_MaxNumX != gTile_MaxNumX - 1)
		{
			// 코너로 판단
			if (BLOCK_INDEX == tileList[index + 1]->nodeIndex && NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex)
			{
				if (true == isNodeCreate)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);
				}
				return true;
			}
		}
		// 왼쪽 코너 체크
		if (index % gTile_MaxNumX > 0 && findIndex % gTile_MaxNumX > 0)
		{
			// 코너로 판단
			if (BLOCK_INDEX == tileList[index - 1]->nodeIndex && NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex)
			{
				if (true == isNodeCreate)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);
				}
				return true;
			}
		}
	}

	return false;
}

bool JumpPointSearch::SearchLeft_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList, const bool isNodeCreate)
{
	AStarNodeInfo* node = nullptr;
	short findIndex = 0;

	while (true)
	{
		// 왼쪽으로 더이상 갈 수 없음
		if (index % gTile_MaxNumX <= 0)
			return false;

		// 왼쪽으로 이동
		index = index - 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// 탐색한 위치는 노드인덱스로 표기
		SearchIndexRender(index, tileList);

		// 목적지 검색
		if (mFinishIndex == index)
		{
			if (true == isNodeCreate)
			{
				node = CreateNode(parent, index, tileList);
				if (node != nullptr)
					mOpenList.emplace_back(node);
			}
			return true;
		}


		// 왼쪽으로 더이상 갈 수 없음
		if (index % gTile_MaxNumX <= 0)
			continue;

		// 왼쪽으로 코너 찾기
		findIndex = index - 1;

		// 위쪽 코너 체크
		if (index >= gTile_MaxNumX && findIndex >= gTile_MaxNumX)
		{
			// 코너로 판단
			if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex // 위쪽 체크
				&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex) // 위쪽 체크
			{
				if (true == isNodeCreate)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);
				}
				return true;
			}
		}
		// 아래쪽 코너 체크
		if (index / gTile_MaxNumX < gTile_MaxNumY - 1 
			&& findIndex / gTile_MaxNumX < gTile_MaxNumY - 1)
		{
			// 코너로 판단
			if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex // 아래쪽 체크
				&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex) // 아래쪽 체크
			{
				if (true == isNodeCreate)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);
				}
				return true;
			}
		}
	}
	return false;
}

bool JumpPointSearch::SearchLeftUp_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList)
{
	AStarNodeInfo* node = nullptr;
	short findIndex = 0;
	bool isCorner = false;

	while (true)
	{
		// 왼쪽 위로 더이상 갈 수 없음
		if (index % gTile_MaxNumX <= 0 || index < gTile_MaxNumX)
			return false;

		// 왼쪽 위 대각선 이동
		index = (index - gTile_MaxNumX) - 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// 탐색한 위치는 노드인덱스로 표기
		SearchIndexRender(index, tileList);

		// 목적지 검색
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// 코너 찾기 한칸 위로 체크
		if (index >= gTile_MaxNumX)
		{
			findIndex = index - gTile_MaxNumX;

			// 위쪽 코너 체크
			if (index % gTile_MaxNumX != gTile_MaxNumX - 1 &&  //오른쪽 체크
				findIndex % gTile_MaxNumX != gTile_MaxNumX - 1) // 오른쪽 체크
			{
				// 코너로 판단
				if (BLOCK_INDEX == tileList[index + 1]->nodeIndex
					&& NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}
		
		// 코너 찾기 한칸 왼쪽 체크
		if ((index % gTile_MaxNumX) > 0)
		{
			findIndex = index - 1;

			// 아래쪽 코너 체크
			if (index / gTile_MaxNumX < gTile_MaxNumY - 1 && // 아래쪽 체크
				findIndex / gTile_MaxNumX < gTile_MaxNumY - 1) // 아래쪽 체크
			{
				// 코너로 판단
				if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex
					&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}
		
		// 대각선 코너를 못찾으면 보조 옵션 검색
		if (true == SearchLeft_CornerNode(parent, index, tileList, false)) // 왼쪽 방향에 코너가 있는지 체크
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
		if (true == SearchUp_CornerNode(parent, index, tileList, false)) // 위쪽 방향에 코너가 있는지 체크
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
	}
}

bool JumpPointSearch::SearchRight_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList, const bool isNodeCreate)
{
	AStarNodeInfo* node = nullptr;
	short findIndex = 0;

	while (true)
	{
		// 오른쪽으로 더이상 갈 수 없음
		if (index % gTile_MaxNumX == gTile_MaxNumX - 1)
			return false;

		// 오른쪽으로 이동
		index = index + 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// 탐색한 위치는 노드인덱스로 표기
		SearchIndexRender(index, tileList);

		// 목적지 검색
		if (mFinishIndex == index)
		{
			if (true == isNodeCreate)
			{
				node = CreateNode(parent, index, tileList);
				if (node != nullptr)
					mOpenList.emplace_back(node);
			}
			return true;
		}

		// 오른쪽으로 코너 찾기
		if (index % gTile_MaxNumX == gTile_MaxNumX - 1) // 오른쪽으로 더이상 갈 수 없음
			continue;

		findIndex = index + 1;
		// 위쪽 코너 체크
		if (index >= gTile_MaxNumX && findIndex >= gTile_MaxNumX)
		{
			// 코너로 판단
			if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex // 위쪽 체크
				&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex) // 위쪽 체크
			{
				if (true == isNodeCreate)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);
				}
				return true;
			}
		}
		// 아래쪽 코너 체크
		if (index / gTile_MaxNumX < gTile_MaxNumY - 1
			&& findIndex / gTile_MaxNumX < gTile_MaxNumY - 1)
		{
			// 코너로 판단
			if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex // 아래쪽 체크
				&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex) // 아래쪽 체크
			{
				if (true == isNodeCreate)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);
				}
				return true;
			}
		}
	}
	return false;
}

bool JumpPointSearch::SearchRightUp_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList)
{
	AStarNodeInfo* node = nullptr;
	short findIndex = 0;
	bool isCorner = false;

	while (true)
	{
		// 오른쪽 위로 더이상 갈 수 없음
		if (index % gTile_MaxNumX == gTile_MaxNumX - 1 || index < gTile_MaxNumX)
			return false;

		// 오른쪽 위 대각선 이동
		index = (index - gTile_MaxNumX) + 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// 탐색한 위치는 노드인덱스로 표기
		SearchIndexRender(index, tileList);

		// 목적지 검색
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// 코너 찾기 한칸 위로 체크
		if (index >= gTile_MaxNumX)
		{
			findIndex = index - gTile_MaxNumX;

			// 왼쪽 코너 체크
			if (index % gTile_MaxNumX > 0 &&  //왼쪽 체크
				findIndex % gTile_MaxNumX > 0) // 왼쪽 체크
			{
				// 코너로 판단
				if (BLOCK_INDEX == tileList[index - 1]->nodeIndex
					&& NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}
		
		// 코너 찾기 한칸 오른쪽 체크
		if (index % gTile_MaxNumX != gTile_MaxNumX - 1)
		{
			findIndex = index + 1;
			// 아래쪽 코너 체크
			if (index / gTile_MaxNumX < gTile_MaxNumY - 1 && // 아래쪽 체크
				findIndex / gTile_MaxNumX < gTile_MaxNumY - 1) // 아래쪽 체크
			{
				// 코너로 판단
				if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex
					&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}
		

		// 대각선 코너를 못찾으면 보조 옵션 검색
		if (true == SearchRight_CornerNode(parent, index, tileList, false)) // 오른쪽 방향에 코너가 있는지 체크
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
		if (true == SearchUp_CornerNode(parent, index, tileList, false)) // 위쪽 방향에 코너가 있는지 체크
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
	}
}

bool JumpPointSearch::SearchDown_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList, const bool isNodeCreate)
{
	AStarNodeInfo* node = nullptr;
	short findIndex = 0;

	while (true)
	{
		// 아래쪽으로 더이상 갈 수 없음
		if ((index / gTile_MaxNumX) >= gTile_MaxNumY - 1)
			return false;

		// 오른쪽으로 이동
		index = index + gTile_MaxNumX;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// 탐색한 위치는 노드인덱스로 표기
		SearchIndexRender(index, tileList);

		// 목적지 검색
		if (mFinishIndex == index)
		{
			if (true == isNodeCreate)
			{
				node = CreateNode(parent, index, tileList);
				if (node != nullptr)
					mOpenList.emplace_back(node);
			}
			return true;
		}

		// 아래쪽으로 코너 찾기
		if ((index / gTile_MaxNumX) >= gTile_MaxNumY - 1) // 아래쪽으로 더이상 갈 수 없음
			continue;

		findIndex = index + gTile_MaxNumX;

		// 왼쪽 코너 체크
		if (index % gTile_MaxNumX > 0 
			&& findIndex % gTile_MaxNumX > 0)
		{
			// 코너로 판단
			if (BLOCK_INDEX == tileList[index - 1]->nodeIndex // 왼쪽 체크
				&& NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex) // 왼쪽 체크
			{
				if (true == isNodeCreate)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);
				}
				return true;
			}
		}
		// 오른쪽 코너 체크
		if (index % gTile_MaxNumX != gTile_MaxNumX - 1
			&& findIndex % gTile_MaxNumX != gTile_MaxNumX - 1)
		{
			// 코너로 판단
			if (BLOCK_INDEX == tileList[index + 1]->nodeIndex // 오른쪽 체크
				&& NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex) // 오른쪽 체크
			{
				if (true == isNodeCreate)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);
				}
				return true;
			}
		}
	}
	return false;
}

bool JumpPointSearch::SearchLeftDown_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList)
{
	AStarNodeInfo* node = nullptr;
	short findIndex = 0;
	bool isCorner = false;
	
	while (true)
	{
		// 왼쪽 아래로 더이상 갈 수 없음
		if ((index % gTile_MaxNumX) <= 0 || (index / gTile_MaxNumX) >= gTile_MaxNumY - 1)
			return false;

		// 왼쪽 아래 대각선 이동
		index = (index + gTile_MaxNumX) - 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// 탐색한 위치는 노드인덱스로 표기
		SearchIndexRender(index, tileList);

		// 목적지 검색
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// 코너 찾기 한칸 왼쪽 체크
		if ((index % gTile_MaxNumX) > 0)
		{
			findIndex = index - 1;

			// 위쪽 코너 체크
			if (index >= gTile_MaxNumX && // 위쪽 체크
				findIndex >= gTile_MaxNumX) // 위쪽 체크
			{
				// 코너로 판단
				if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex
					&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}

		// 코너 찾기 아래로 체크
		if (index / gTile_MaxNumX < gTile_MaxNumY - 1)
		{
			findIndex = index + gTile_MaxNumX;

			// 위쪽 코너 체크
			if (index % gTile_MaxNumX != gTile_MaxNumX - 1 &&  //오른쪽 체크
				findIndex % gTile_MaxNumX != gTile_MaxNumX - 1) // 오른쪽 체크
			{
				// 코너로 판단
				if (BLOCK_INDEX == tileList[index + 1]->nodeIndex
					&& NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}

		// 대각선 코너를 못찾으면 보조 옵션 검색
		if (true == SearchLeft_CornerNode(parent, index, tileList, false)) // 왼쪽 방향에 코너가 있는지 체크
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
		if (true == SearchDown_CornerNode(parent, index, tileList, false)) // 아래쪽 방향에 코너가 있는지 체크
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
	}

	return false;
}

bool JumpPointSearch::SearchRightDown_CornerNode(AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList)
{
	AStarNodeInfo* node = nullptr;
	short findIndex = 0;
	bool isCorner = false;
	WORD originaIndex = index;

	while (true)
	{
		// 오른쪽 아래로 더이상 갈 수 없음
		if ((index % gTile_MaxNumX) == gTile_MaxNumX -1 || (index / gTile_MaxNumX) >= gTile_MaxNumY - 1)
			break;

		// 왼쪽 아래 대각선 이동
		index = (index + gTile_MaxNumX) + 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			break;

		// 탐색한 위치는 노드인덱스로 표기
		SearchIndexRender(index, tileList);

		// 목적지 검색
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// 코너 찾기 한칸 오른쪽 체크
		if ((index % gTile_MaxNumX) != gTile_MaxNumX - 1)
		{
			findIndex = index + 1;

			// 위쪽 코너 체크
			if (index >= gTile_MaxNumX && // 위쪽 체크
				findIndex >= gTile_MaxNumX) // 위쪽 체크
			{
				// 코너로 판단
				if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex
					&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}

		// 코너 찾기 아래로 체크
		if (index / gTile_MaxNumX < gTile_MaxNumY - 1)
		{
			findIndex = index + gTile_MaxNumX;

			// 위쪽 코너 체크
			if (index % gTile_MaxNumX > 0 &&  //왼쪽 체크
				findIndex % gTile_MaxNumX > 0) //왼쪽 체크
			{
				// 코너로 판단
				if (BLOCK_INDEX == tileList[index - 1]->nodeIndex
					&& NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}

		// 대각선 코너를 못찾으면 보조 옵션 검색
		if (true == SearchRight_CornerNode(parent, index, tileList, false)) // 오른쪽 방향에 코너가 있는지 체크
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
		if (true == SearchDown_CornerNode(parent, index, tileList, false)) // 아래쪽 방향에 코너가 있는지 체크
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
	}

	//while (true)
	//{
	//	// 오른쪽 아래로 더이상 갈 수 없음
	//	if ((originaIndex % gTile_MaxNumX) == gTile_MaxNumX - 1 || (originaIndex / gTile_MaxNumX) >= mTile_MaxNumY - 1)
	//		return false;

	//	// 왼쪽 아래 대각선 이동
	//	originaIndex = (originaIndex + gTile_MaxNumX) + 1;

	//	if (tileList[originaIndex]->nodeIndex == BLOCK_INDEX)
	//		return false;

	//	// 목적지 검색
	//	if (mFinishIndex == originaIndex)
	//	{
	//		node = CreateNode(parent, originaIndex, tileList);
	//		if (node != nullptr)
	//			mOpenList.emplace_back(node);

	//		return true;
	//	}

	//	// 대각선 코너를 못찾으면 보조 옵션 검색
	//	if (true == SearchRight_CornerNode(parent, originaIndex, tileList, false)) // 오른쪽 방향에 코너가 있는지 체크
	//	{
	//		node = CreateNode(parent, originaIndex, tileList);
	//		if (node != nullptr)
	//			mOpenList.emplace_back(node);

	//		return true;
	//	}
	//	if (true == SearchDown_CornerNode(parent, originaIndex, tileList, false)) // 아래쪽 방향에 코너가 있는지 체크
	//	{
	//		node = CreateNode(parent, originaIndex, tileList);
	//		if (node != nullptr)
	//			mOpenList.emplace_back(node);

	//		return true;
	//	}
	//}

	return false;
}

void JumpPointSearch::SearchIndexRender(const WORD index, vector<RectInfo*>& tileList)
{
	// 탐색한 위치는 노드인덱스로 표기
	if (index != mStartIndex && index != mFinishIndex)
	{
		tileList[index]->redColor = mRed;
		tileList[index]->greenColor = mGreen;
		tileList[index]->blueColor = mBlue;
		mVisualization->Render();
	}
}

void JumpPointSearch::RandomColorSetting()
{
	// random setting
	static random_device randDevice;
	static mt19937_64 rand(randDevice());
	static uniform_int_distribution<__int16> randDist(1, 255);

	mRed = (BYTE)randDist(rand);
	mGreen = (BYTE)randDist(rand);
	mBlue = (BYTE)randDist(rand);
}

bool JumpPointSearch::CalBestRoadSpaec(AStarNodeInfo* node, const vector<RectInfo*>& tileList)
{
	int beforeIndex = -1;
	WORD xIndex = 0;
	WORD yIndex = 0;
	//int index = 0;
	WORD finishCount = 0;
	list<WORD> bresenhamIndex;
	AStarNodeInfo* finishNode = node;
	AStarNodeInfo* childNode = finishNode->parent;
	AStarNodeInfo* tempNode = nullptr;
	bool isBlock = false;
	
	_ASSERT(finishNode != nullptr);
	mBestRoadSpace.emplace(finishNode);

	while (childNode != nullptr && finishNode != nullptr)
	{
		isBlock = false;
		beforeIndex = -1;
		bresenhamIndex.clear();
		mBresenham->Cal_StraightLine(&bresenhamIndex, childNode->index, finishNode->index);
		wprintf(L"startPointIndex:%d endPointIndex:%d \n", childNode->index,finishNode->index);

		for (WORD index : bresenhamIndex)
		{
			wprintf(L"bresenhamIndex:%d\n", index);
			//mVisualization->DrawBresenhamLine(tileList[index]->point, RGB(0, 0, 255));
			if (beforeIndex != index)
			{
				beforeIndex = index;
				// 장애물 노드가 있으면 바로 무한루프를 정지하고 finishNode 갱신 및 노드 포인트 생성
				if (index >= gTile_MaxNumX * gTile_MaxNumY)
				{
					wprintf(L"index:%d\n", index);
				}

				if (tileList[index]->nodeIndex == BLOCK_INDEX)
				{
					isBlock = true;
					if (tempNode != nullptr) // 정상적인 길이 이전에 있었으면 그 노드를 finishNode 로 갱신
					{
						finishNode = tempNode;
						mBestRoadSpace.emplace(finishNode);
						tempNode = nullptr;
					}
					else
					{
						finishNode = finishNode->parent;
					}
					
					if (finishNode == childNode)
						childNode = finishNode->parent;
					break;
				}
			}
		}

		// 점과 점사이에 BlockNode가 없으면 다음 노드와 연결하여 이용 가능한지 확인, BlockNode가 있으면 바로 그 지점에 노드 포인트 생성.
		if (false == isBlock)
		{
			if (childNode->parent != nullptr)
			{
				tempNode = childNode;
				childNode = childNode->parent;
			}
			else
			{
				mBestRoadSpace.emplace(childNode);
				childNode = childNode->parent;
			}
		}
	}

	BestRoadRender(tileList);
	//mVisualization->RenderBitBlt();
	return true;
}

void JumpPointSearch::BestRoadRender(const vector<RectInfo*>& tileList)
{
	while (mBestRoadSpace.empty() == false)
	{
		mVisualization->DrawBresenhamLine(tileList[mBestRoadSpace.top()->index]->point, RGB(0, 0, 255));
		mBestRoadSpace.pop();
	}
	mVisualization->RenderBitBlt();
}


void JumpPointSearch::Release(void)
{
	for_each(mOpenList.begin(), mOpenList.end(), SafeDelete<AStarNodeInfo*>);
	mOpenList.clear();

	for_each(mCloseList.begin(), mCloseList.end(), SafeDelete<AStarNodeInfo*>);
	mCloseList.clear();

	while (mBestRoadSpace.empty() == false)
	{
		mBestRoadSpace.pop();
	}
}
