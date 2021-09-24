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
		// �θ� ���� ��� ��ü 8���� �˻� ����
		if (nullptr == parent->parent)
		{
			RandomColorSetting();
			// ����
			SearchUp_CornerNode(parent, index, tileList, true);
			// ����
			//RandomColorSetting();
			SearchLeft_CornerNode(parent, index, tileList, true);
			// ���� ��
			//RandomColorSetting();
			SearchLeftUp_CornerNode(parent, index, tileList);
			// ������
			//RandomColorSetting();
			SearchRight_CornerNode(parent, index, tileList, true);
			// ������ ��
			//RandomColorSetting();
			SearchRightUp_CornerNode(parent, index, tileList);
			// �Ʒ���
			//RandomColorSetting();
			SearchDown_CornerNode(parent, index, tileList, true);
			// ���� �Ʒ�
			//RandomColorSetting();
			SearchLeftDown_CornerNode(parent, index, tileList);
			// ������ �Ʒ�
			//RandomColorSetting();
			SearchRightDown_CornerNode(parent, index, tileList);

		}
		// �θ� ���� ���
		else
		{
			parentIndex = parent->parent->index;
			directionPoint.x = tileList[index]->point.x - tileList[parentIndex]->point.x;
			directionPoint.y = tileList[index]->point.y - tileList[parentIndex]->point.y;
			RandomColorSetting();
			// ���ʹ��� Ž��
			if (directionPoint.x == 0 && directionPoint.y < 0)
			{
				// �ִ� 3���� Ž�� ����
				SearchUp_CornerNode(parent, index, tileList, true);
				
				// �������� �� �� �ִ��� üũ
				if (index >= gTile_MaxNumX)
				{
					// �������� �ڳ� ã��
					findIndex = index - gTile_MaxNumX;

					// ������ �ڳ� üũ
					if (index % gTile_MaxNumX != gTile_MaxNumX - 1 && findIndex % gTile_MaxNumX != gTile_MaxNumX - 1)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + 1]->nodeIndex && NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex)
						{
							SearchRightUp_CornerNode(parent, index, tileList);
						}
					}
					// ���� �ڳ� üũ
					if (index % gTile_MaxNumX > 0 && findIndex % gTile_MaxNumX > 0)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index - 1]->nodeIndex && NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex)
						{
							SearchLeftUp_CornerNode(parent, index, tileList);
						}
					}
				}		
			}
			// ���� ���� Ž��
			else if (directionPoint.x < 0 && directionPoint.y == 0)
			{
				// �ִ� 3���� Ž�� ����
				SearchLeft_CornerNode(parent, index, tileList, true);
			
				// �������� ���̻� �� �� ������ üũ
				if (index % gTile_MaxNumX > 0)
				{
					// �������� �ڳ� ã��
					findIndex = index - 1;

					// ���� �ڳ� üũ
					if (index >= gTile_MaxNumX && 
						findIndex >= gTile_MaxNumX)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex // ���� üũ
							&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex) // ���� üũ
						{
							SearchLeftUp_CornerNode(parent, index, tileList);
						}
					}
					// �Ʒ��� �ڳ� üũ
					if (index / gTile_MaxNumX < gTile_MaxNumY - 1 && 
						findIndex / gTile_MaxNumX < gTile_MaxNumY - 1)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex // �Ʒ��� üũ
							&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex) // �Ʒ��� üũ
						{
							SearchLeftDown_CornerNode(parent, index, tileList);
						}
					}
				}
			}
			// ���� �� ���� Ž��
			else if (directionPoint.x < 0 && directionPoint.y < 0)
			{
				// �ִ� 5���� Ž�� ����
				SearchLeftUp_CornerNode(parent, index, tileList);
				//RandomColorSetting();
				SearchLeft_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();
				SearchUp_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();
				// �ڳ� ã�� ��ĭ ���� üũ
				if (index >= gTile_MaxNumX)
				{
					findIndex = index - gTile_MaxNumX;

					// ���� �ڳ� üũ
					if (index % gTile_MaxNumX != gTile_MaxNumX - 1 &&  //������ üũ
						findIndex % gTile_MaxNumX != gTile_MaxNumX - 1) // ������ üũ
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + 1]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex)
						{
							SearchRightUp_CornerNode(parent, index, tileList);
						}
					}
				}

				// �ڳ� ã�� ��ĭ ���� üũ
				if ((index % gTile_MaxNumX) > 0)
				{
					findIndex = index - 1;

					// �Ʒ��� �ڳ� üũ
					if (index / gTile_MaxNumX < gTile_MaxNumY - 1 && // �Ʒ��� üũ
						findIndex / gTile_MaxNumX < gTile_MaxNumY - 1) // �Ʒ��� üũ
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex)
						{
							SearchLeftDown_CornerNode(parent, index, tileList);
						}
					}
				}
			}
			// ������ ���� Ž��
			else if (directionPoint.x > 0 && directionPoint.y == 0)
			{
				// �ִ� 3���� Ž�� ����
				SearchRight_CornerNode(parent, index, tileList, true);

				// ���������� ���̻� �� �� �ִ��� üũ
				if (index % gTile_MaxNumX != gTile_MaxNumX - 1)
				{
					findIndex = index + 1;
					// ���� �ڳ� üũ
					if (index >= gTile_MaxNumX && findIndex >= gTile_MaxNumX)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex // ���� üũ
							&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex) // ���� üũ
						{
							SearchRightUp_CornerNode(parent, index, tileList);
						}
					}
					// �Ʒ��� �ڳ� üũ
					if (index / gTile_MaxNumX < gTile_MaxNumY - 1
						&& findIndex / gTile_MaxNumX < gTile_MaxNumY - 1)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex // �Ʒ��� üũ
							&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex) // �Ʒ��� üũ
						{
							SearchRightDown_CornerNode(parent, index, tileList);
						}
					}
				}		
			}
			// ������ �� ���� Ž��
			else if (directionPoint.x > 0 && directionPoint.y < 0)
			{
				// �ִ� 5���� Ž�� ����
				SearchRightUp_CornerNode(parent, index, tileList);
				//RandomColorSetting();
				SearchRight_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();
				SearchUp_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();

				// �ڳ� ã�� ��ĭ ���� üũ
				if (index >= gTile_MaxNumX)
				{
					findIndex = index - gTile_MaxNumX;

					// ���� �ڳ� üũ
					if (index % gTile_MaxNumX > 0 &&  //���� üũ
						findIndex % gTile_MaxNumX > 0) // ���� üũ
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index - 1]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex)
						{
							SearchLeftUp_CornerNode(parent, index, tileList);
						}
					}
				}

				// �ڳ� ã�� ��ĭ ������ üũ
				if (index % gTile_MaxNumX != gTile_MaxNumX - 1)
				{
					findIndex = index + 1;
					// �Ʒ��� �ڳ� üũ
					if (index / gTile_MaxNumX < gTile_MaxNumY - 1 && // �Ʒ��� üũ
						findIndex / gTile_MaxNumX < gTile_MaxNumY - 1) // �Ʒ��� üũ
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex)
						{
							SearchRightDown_CornerNode(parent, index, tileList);
						}
					}
				}
			}
			// �Ʒ� ���� Ž��
			else if (directionPoint.x == 0 && directionPoint.y > 0)
			{
				// �ִ� 3���� Ž�� ����
				SearchDown_CornerNode(parent, index, tileList, true);

				// �Ʒ������� �ڳ� ã��
				if ((index / gTile_MaxNumX) < gTile_MaxNumY - 1) // �Ʒ������� ���̻� �� �� �ִ��� üũ
				{
					findIndex = index + gTile_MaxNumX;

					// ���� �ڳ� üũ
					if (index % gTile_MaxNumX > 0
						&& findIndex % gTile_MaxNumX > 0)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index - 1]->nodeIndex // ���� üũ
							&& NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex) // ���� üũ
						{
							SearchLeftDown_CornerNode(parent, index, tileList);
						}
					}
					// ������ �ڳ� üũ
					if (index % gTile_MaxNumX != gTile_MaxNumX - 1
						&& findIndex % gTile_MaxNumX != gTile_MaxNumX - 1)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + 1]->nodeIndex // ������ üũ
							&& NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex) // ������ üũ
						{
							SearchRightDown_CornerNode(parent, index, tileList);
						}
					}
				}		
			}
			// ���� �Ʒ� ���� Ž��
			else if (directionPoint.x < 0 && directionPoint.y > 0)
			{
				// �ִ� 5���� Ž�� ����
				SearchLeftDown_CornerNode(parent, index, tileList);
				//RandomColorSetting();
				SearchLeft_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();
				SearchDown_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();

				// �ڳ� ã�� ��ĭ ���� üũ
				if ((index % gTile_MaxNumX) > 0)
				{
					findIndex = index - 1;

					// ���� �ڳ� üũ
					if (index >= gTile_MaxNumX && // ���� üũ
						findIndex >= gTile_MaxNumX) // ���� üũ
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex)
						{
							SearchLeftUp_CornerNode(parent, index, tileList);
						}
					}
				}

				// �ڳ� ã�� �Ʒ��� üũ
				if (index / gTile_MaxNumX < gTile_MaxNumY - 1)
				{
					findIndex = index + gTile_MaxNumX;

					// ���� �ڳ� üũ
					if (index % gTile_MaxNumX != gTile_MaxNumX - 1 &&  //������ üũ
						findIndex % gTile_MaxNumX != gTile_MaxNumX - 1) // ������ üũ
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + 1]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex)
						{
							SearchRightDown_CornerNode(parent, index, tileList);
						}
					}
				}
			}
			// ������ �Ʒ� ���� Ž��
			else if (directionPoint.x > 0 && directionPoint.y > 0)
			{
				// �ִ� 5���� Ž�� ����
				SearchRightDown_CornerNode(parent, index, tileList);
				//RandomColorSetting();
				SearchRight_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();
				SearchDown_CornerNode(parent, index, tileList, true);
				//RandomColorSetting();

				// �ڳ� ã�� ��ĭ ������ üũ
				if ((index % gTile_MaxNumX) != gTile_MaxNumX - 1)
				{
					findIndex = index + 1;

					// ���� �ڳ� üũ
					if (index >= gTile_MaxNumX && // ���� üũ
						findIndex >= gTile_MaxNumX) // ���� üũ
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex
							&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex)
						{
							SearchRightUp_CornerNode(parent, index, tileList);
						}
					}
				}

				// �ڳ� ã�� �Ʒ��� üũ
				if (index / gTile_MaxNumX < gTile_MaxNumY - 1)
				{
					findIndex = index + gTile_MaxNumX;

					// ���� �ڳ� üũ
					if (index % gTile_MaxNumX > 0 &&  //���� üũ
						findIndex % gTile_MaxNumX > 0) //���� üũ
					{
						// �ڳʷ� �Ǵ�
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
			
		// ����ġ �������� �����Ѵ�.
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
				//Visualization AStar ��ã�� ��� ǥ��
				mVisualization->DrawFinishLine(tileList[parent->index]->point, RGB(0, 255, 0));
				if (parent->parent == nullptr)
				{
					wprintf(L"finishCount:%d\n", finishCount);
					//Visualization AStar ��ã�� ��� ǥ��
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
	// Manhattan ��� ����Ͽ� G(�θ� �簢������ ��� ��� ���)
	dx = (WORD)(abs(tileList[parent->index]->point.x - tileList[index]->point.x)) / RECT_SIZE;
	dy = (WORD)(abs(tileList[parent->index]->point.y - tileList[index]->point.y)) / RECT_SIZE;
	G = (dx + dy) + tileList[parent->index]->G;

	// Manhattan ��� ����Ͽ� H(���� �簢������ ������ ��� ���)
	dx = (WORD)(abs(tileList[mFinishIndex]->point.x - tileList[index]->point.x)) / RECT_SIZE;
	dy = (WORD)(abs(tileList[mFinishIndex]->point.y - tileList[index]->point.y)) / RECT_SIZE;
	H = (dx + dy); 

	// ������� �̵��ϴµ� �ɸ� ���� ���� ����� ��ģ �� ���
	F = G + H;

	// ������ ��������Ʈ�� �ִ� ���� ���Ͽ� ��ü ����
	for (AStarNodeInfo* pNode : mOpenList)
	{
		if (pNode->index == index)
		{
			// G �� ������ ������ G �� �θ� ��ü
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

	// Visualization ��� ǥ�ø� ���� ���� ����
	tileList[index]->F = F;
	tileList[index]->G = G;
	tileList[index]->H = H;
	// Visualization ��� ����ȭ ǥ��
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
		// �������� ���̻� �� �� ����
		if (index < gTile_MaxNumX)
			return false;

		// �������� �̵�
		index = index - gTile_MaxNumX;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(index, tileList);

		// ������ �˻�
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

		// �������� ���̻� �� �� ������ üũ
		if (index < gTile_MaxNumX)
			continue;

		// �������� �ڳ� ã��
		findIndex = index - gTile_MaxNumX;

		// ������ �ڳ� üũ
		if (index % gTile_MaxNumX != gTile_MaxNumX - 1 && findIndex % gTile_MaxNumX != gTile_MaxNumX - 1)
		{
			// �ڳʷ� �Ǵ�
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
		// ���� �ڳ� üũ
		if (index % gTile_MaxNumX > 0 && findIndex % gTile_MaxNumX > 0)
		{
			// �ڳʷ� �Ǵ�
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
		// �������� ���̻� �� �� ����
		if (index % gTile_MaxNumX <= 0)
			return false;

		// �������� �̵�
		index = index - 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(index, tileList);

		// ������ �˻�
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


		// �������� ���̻� �� �� ����
		if (index % gTile_MaxNumX <= 0)
			continue;

		// �������� �ڳ� ã��
		findIndex = index - 1;

		// ���� �ڳ� üũ
		if (index >= gTile_MaxNumX && findIndex >= gTile_MaxNumX)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex // ���� üũ
				&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex) // ���� üũ
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
		// �Ʒ��� �ڳ� üũ
		if (index / gTile_MaxNumX < gTile_MaxNumY - 1 
			&& findIndex / gTile_MaxNumX < gTile_MaxNumY - 1)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex // �Ʒ��� üũ
				&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex) // �Ʒ��� üũ
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
		// ���� ���� ���̻� �� �� ����
		if (index % gTile_MaxNumX <= 0 || index < gTile_MaxNumX)
			return false;

		// ���� �� �밢�� �̵�
		index = (index - gTile_MaxNumX) - 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(index, tileList);

		// ������ �˻�
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// �ڳ� ã�� ��ĭ ���� üũ
		if (index >= gTile_MaxNumX)
		{
			findIndex = index - gTile_MaxNumX;

			// ���� �ڳ� üũ
			if (index % gTile_MaxNumX != gTile_MaxNumX - 1 &&  //������ üũ
				findIndex % gTile_MaxNumX != gTile_MaxNumX - 1) // ������ üũ
			{
				// �ڳʷ� �Ǵ�
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
		
		// �ڳ� ã�� ��ĭ ���� üũ
		if ((index % gTile_MaxNumX) > 0)
		{
			findIndex = index - 1;

			// �Ʒ��� �ڳ� üũ
			if (index / gTile_MaxNumX < gTile_MaxNumY - 1 && // �Ʒ��� üũ
				findIndex / gTile_MaxNumX < gTile_MaxNumY - 1) // �Ʒ��� üũ
			{
				// �ڳʷ� �Ǵ�
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
		
		// �밢�� �ڳʸ� ��ã���� ���� �ɼ� �˻�
		if (true == SearchLeft_CornerNode(parent, index, tileList, false)) // ���� ���⿡ �ڳʰ� �ִ��� üũ
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
		if (true == SearchUp_CornerNode(parent, index, tileList, false)) // ���� ���⿡ �ڳʰ� �ִ��� üũ
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
		// ���������� ���̻� �� �� ����
		if (index % gTile_MaxNumX == gTile_MaxNumX - 1)
			return false;

		// ���������� �̵�
		index = index + 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(index, tileList);

		// ������ �˻�
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

		// ���������� �ڳ� ã��
		if (index % gTile_MaxNumX == gTile_MaxNumX - 1) // ���������� ���̻� �� �� ����
			continue;

		findIndex = index + 1;
		// ���� �ڳ� üũ
		if (index >= gTile_MaxNumX && findIndex >= gTile_MaxNumX)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index - gTile_MaxNumX]->nodeIndex // ���� üũ
				&& NORMAL_INDEX >= tileList[findIndex - gTile_MaxNumX]->nodeIndex) // ���� üũ
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
		// �Ʒ��� �ڳ� üũ
		if (index / gTile_MaxNumX < gTile_MaxNumY - 1
			&& findIndex / gTile_MaxNumX < gTile_MaxNumY - 1)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index + gTile_MaxNumX]->nodeIndex // �Ʒ��� üũ
				&& NORMAL_INDEX >= tileList[findIndex + gTile_MaxNumX]->nodeIndex) // �Ʒ��� üũ
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
		// ������ ���� ���̻� �� �� ����
		if (index % gTile_MaxNumX == gTile_MaxNumX - 1 || index < gTile_MaxNumX)
			return false;

		// ������ �� �밢�� �̵�
		index = (index - gTile_MaxNumX) + 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(index, tileList);

		// ������ �˻�
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// �ڳ� ã�� ��ĭ ���� üũ
		if (index >= gTile_MaxNumX)
		{
			findIndex = index - gTile_MaxNumX;

			// ���� �ڳ� üũ
			if (index % gTile_MaxNumX > 0 &&  //���� üũ
				findIndex % gTile_MaxNumX > 0) // ���� üũ
			{
				// �ڳʷ� �Ǵ�
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
		
		// �ڳ� ã�� ��ĭ ������ üũ
		if (index % gTile_MaxNumX != gTile_MaxNumX - 1)
		{
			findIndex = index + 1;
			// �Ʒ��� �ڳ� üũ
			if (index / gTile_MaxNumX < gTile_MaxNumY - 1 && // �Ʒ��� üũ
				findIndex / gTile_MaxNumX < gTile_MaxNumY - 1) // �Ʒ��� üũ
			{
				// �ڳʷ� �Ǵ�
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
		

		// �밢�� �ڳʸ� ��ã���� ���� �ɼ� �˻�
		if (true == SearchRight_CornerNode(parent, index, tileList, false)) // ������ ���⿡ �ڳʰ� �ִ��� üũ
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
		if (true == SearchUp_CornerNode(parent, index, tileList, false)) // ���� ���⿡ �ڳʰ� �ִ��� üũ
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
		// �Ʒ������� ���̻� �� �� ����
		if ((index / gTile_MaxNumX) >= gTile_MaxNumY - 1)
			return false;

		// ���������� �̵�
		index = index + gTile_MaxNumX;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(index, tileList);

		// ������ �˻�
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

		// �Ʒ������� �ڳ� ã��
		if ((index / gTile_MaxNumX) >= gTile_MaxNumY - 1) // �Ʒ������� ���̻� �� �� ����
			continue;

		findIndex = index + gTile_MaxNumX;

		// ���� �ڳ� üũ
		if (index % gTile_MaxNumX > 0 
			&& findIndex % gTile_MaxNumX > 0)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index - 1]->nodeIndex // ���� üũ
				&& NORMAL_INDEX >= tileList[findIndex - 1]->nodeIndex) // ���� üũ
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
		// ������ �ڳ� üũ
		if (index % gTile_MaxNumX != gTile_MaxNumX - 1
			&& findIndex % gTile_MaxNumX != gTile_MaxNumX - 1)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index + 1]->nodeIndex // ������ üũ
				&& NORMAL_INDEX >= tileList[findIndex + 1]->nodeIndex) // ������ üũ
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
		// ���� �Ʒ��� ���̻� �� �� ����
		if ((index % gTile_MaxNumX) <= 0 || (index / gTile_MaxNumX) >= gTile_MaxNumY - 1)
			return false;

		// ���� �Ʒ� �밢�� �̵�
		index = (index + gTile_MaxNumX) - 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(index, tileList);

		// ������ �˻�
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// �ڳ� ã�� ��ĭ ���� üũ
		if ((index % gTile_MaxNumX) > 0)
		{
			findIndex = index - 1;

			// ���� �ڳ� üũ
			if (index >= gTile_MaxNumX && // ���� üũ
				findIndex >= gTile_MaxNumX) // ���� üũ
			{
				// �ڳʷ� �Ǵ�
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

		// �ڳ� ã�� �Ʒ��� üũ
		if (index / gTile_MaxNumX < gTile_MaxNumY - 1)
		{
			findIndex = index + gTile_MaxNumX;

			// ���� �ڳ� üũ
			if (index % gTile_MaxNumX != gTile_MaxNumX - 1 &&  //������ üũ
				findIndex % gTile_MaxNumX != gTile_MaxNumX - 1) // ������ üũ
			{
				// �ڳʷ� �Ǵ�
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

		// �밢�� �ڳʸ� ��ã���� ���� �ɼ� �˻�
		if (true == SearchLeft_CornerNode(parent, index, tileList, false)) // ���� ���⿡ �ڳʰ� �ִ��� üũ
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
		if (true == SearchDown_CornerNode(parent, index, tileList, false)) // �Ʒ��� ���⿡ �ڳʰ� �ִ��� üũ
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
		// ������ �Ʒ��� ���̻� �� �� ����
		if ((index % gTile_MaxNumX) == gTile_MaxNumX -1 || (index / gTile_MaxNumX) >= gTile_MaxNumY - 1)
			break;

		// ���� �Ʒ� �밢�� �̵�
		index = (index + gTile_MaxNumX) + 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			break;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(index, tileList);

		// ������ �˻�
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// �ڳ� ã�� ��ĭ ������ üũ
		if ((index % gTile_MaxNumX) != gTile_MaxNumX - 1)
		{
			findIndex = index + 1;

			// ���� �ڳ� üũ
			if (index >= gTile_MaxNumX && // ���� üũ
				findIndex >= gTile_MaxNumX) // ���� üũ
			{
				// �ڳʷ� �Ǵ�
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

		// �ڳ� ã�� �Ʒ��� üũ
		if (index / gTile_MaxNumX < gTile_MaxNumY - 1)
		{
			findIndex = index + gTile_MaxNumX;

			// ���� �ڳ� üũ
			if (index % gTile_MaxNumX > 0 &&  //���� üũ
				findIndex % gTile_MaxNumX > 0) //���� üũ
			{
				// �ڳʷ� �Ǵ�
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

		// �밢�� �ڳʸ� ��ã���� ���� �ɼ� �˻�
		if (true == SearchRight_CornerNode(parent, index, tileList, false)) // ������ ���⿡ �ڳʰ� �ִ��� üũ
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
		if (true == SearchDown_CornerNode(parent, index, tileList, false)) // �Ʒ��� ���⿡ �ڳʰ� �ִ��� üũ
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}
	}

	//while (true)
	//{
	//	// ������ �Ʒ��� ���̻� �� �� ����
	//	if ((originaIndex % gTile_MaxNumX) == gTile_MaxNumX - 1 || (originaIndex / gTile_MaxNumX) >= mTile_MaxNumY - 1)
	//		return false;

	//	// ���� �Ʒ� �밢�� �̵�
	//	originaIndex = (originaIndex + gTile_MaxNumX) + 1;

	//	if (tileList[originaIndex]->nodeIndex == BLOCK_INDEX)
	//		return false;

	//	// ������ �˻�
	//	if (mFinishIndex == originaIndex)
	//	{
	//		node = CreateNode(parent, originaIndex, tileList);
	//		if (node != nullptr)
	//			mOpenList.emplace_back(node);

	//		return true;
	//	}

	//	// �밢�� �ڳʸ� ��ã���� ���� �ɼ� �˻�
	//	if (true == SearchRight_CornerNode(parent, originaIndex, tileList, false)) // ������ ���⿡ �ڳʰ� �ִ��� üũ
	//	{
	//		node = CreateNode(parent, originaIndex, tileList);
	//		if (node != nullptr)
	//			mOpenList.emplace_back(node);

	//		return true;
	//	}
	//	if (true == SearchDown_CornerNode(parent, originaIndex, tileList, false)) // �Ʒ��� ���⿡ �ڳʰ� �ִ��� üũ
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
	// Ž���� ��ġ�� ����ε����� ǥ��
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
				// ��ֹ� ��尡 ������ �ٷ� ���ѷ����� �����ϰ� finishNode ���� �� ��� ����Ʈ ����
				if (index >= gTile_MaxNumX * gTile_MaxNumY)
				{
					wprintf(L"index:%d\n", index);
				}

				if (tileList[index]->nodeIndex == BLOCK_INDEX)
				{
					isBlock = true;
					if (tempNode != nullptr) // �������� ���� ������ �־����� �� ��带 finishNode �� ����
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

		// ���� �����̿� BlockNode�� ������ ���� ���� �����Ͽ� �̿� �������� Ȯ��, BlockNode�� ������ �ٷ� �� ������ ��� ����Ʈ ����.
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
