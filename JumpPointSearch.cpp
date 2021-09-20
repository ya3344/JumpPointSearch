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
	short index = 0;
	WORD finishCount = 0;
	AStarNodeInfo* parent = new AStarNodeInfo(0, 0, mStartIndex, nullptr);
	mCloseList.emplace_back(parent);
	
	while (true)
	{
		// �θ� ���� ��� ��ü 8���� �˻� ����
		if (nullptr == parent->parent)
		{
			RandomColorSetting();
			index = parent->index;
			// ����
			SearchUp_CornerNode(parent, index, tileList, true);
			// ����
			RandomColorSetting();
			SearchLeft_CornerNode(parent, index, tileList, true);
			// ���� ��
			RandomColorSetting();
			SearchLeftUp_CornerNode(parent, index, tileList);
			// ������
			RandomColorSetting();
			SearchRight_CornerNode(parent, index, tileList, true);
			// ������ ��
			RandomColorSetting();
			SearchRightUp_CornerNode(parent, index, tileList);
			// �Ʒ���
			RandomColorSetting();
			SearchDown_CornerNode(parent, index, tileList, true);
			// ���� �Ʒ�
			RandomColorSetting();
			SearchLeftDown_CornerNode(parent, index, tileList);
			// ������ �Ʒ�
			RandomColorSetting();
			SearchRightDown_CornerNode(parent, index, tileList);

		}
		// �θ� ���� ���
		else
		{
			index = parent->index;
			// ��
			//SearchCornerNode(UP, parent, index, tileList);
		}
		// ��
		//index = parent->index - mTile_MaxNumX;

		//if (parent->index >= mTile_MaxNumX && tileList[index]->nodeIndex != BLOCK_INDEX
		//	&& CheckList(index))
		//{
		//	node = CreateNode(parent, index, tileList);
		//	if(node != nullptr)
		//		mOpenList.emplace_back(node);
		//}

		//// ������ ��
		//index = (parent->index - mTile_MaxNumX) + 1;

		//if (parent->index >= mTile_MaxNumX && (parent->index % mTile_MaxNumX) != mTile_MaxNumX - 1
		//	&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		//{
		//	node = CreateNode(parent, index, tileList);
		//	if (node != nullptr)
		//		mOpenList.emplace_back(node);
		//}

		//// ������
		//index = parent->index + 1;

		//if ((parent->index % mTile_MaxNumX) != mTile_MaxNumX - 1
		//	&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		//{
		//	node = CreateNode(parent, index, tileList);
		//	if (node != nullptr)
		//		mOpenList.emplace_back(node);
		//}

		//// ������ �Ʒ� 
		//index = (parent->index + mTile_MaxNumX) + 1;

		//if ((parent->index / mTile_MaxNumX) < mTile_MaxNumY - 1  && (parent->index % mTile_MaxNumX) != mTile_MaxNumX - 1
		//	&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		//{
		//	node = CreateNode(parent, index, tileList);
		//	if (node != nullptr)
		//		mOpenList.emplace_back(node);
		//}

		//// �Ʒ�
		//index = parent->index + mTile_MaxNumX;

		//if ((parent->index / mTile_MaxNumX) < mTile_MaxNumY - 1
		//	&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		//{
		//	node = CreateNode(parent, index, tileList);
		//	if (node != nullptr)
		//		mOpenList.emplace_back(node);
		//}

		//// ���� �Ʒ�
		//index = (parent->index + mTile_MaxNumX) - 1;

		//if ((parent->index / mTile_MaxNumX) < mTile_MaxNumY - 1 && (parent->index % mTile_MaxNumX) > 0
		//	&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		//{
		//	node = CreateNode(parent, index, tileList);
		//	if (node != nullptr)
		//		mOpenList.emplace_back(node);
		//}

		//// ����
		//index = parent->index - 1;

		//if ((parent->index % mTile_MaxNumX) > 0
		//	&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		//{
		//	node = CreateNode(parent, index, tileList);
		//	if (node != nullptr)
		//		mOpenList.emplace_back(node);
		//}

		//// ���� �� 
		//index = (parent->index - mTile_MaxNumX) - 1;
		//if ((parent->index % mTile_MaxNumX) > 0 && parent->index >= mTile_MaxNumX
		//	&& tileList[index]->nodeIndex != BLOCK_INDEX && CheckList(index))
		//{
		//	node = CreateNode(parent, index, tileList);
		//	if (node != nullptr)
		//		mOpenList.emplace_back(node);
		//}

		if (mOpenList.empty())
		{
			return false;
		}
			
		// ����ġ �������� �����Ѵ�.
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
				//Visualization AStar ��ã�� ��� ǥ��
				mVisualization->DrawFinishLine(tileList[parent->index]->point);
				// ��θ� �������ش�.
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

void JumpPointSearch::SearchCornerNode(const BYTE direction, AStarNodeInfo* parent, WORD index, vector<RectInfo*>& tileList)
{
	AStarNodeInfo* node = nullptr;
	short findIndex = 0;
	bool isCorner = false;

	switch (direction)
	{
		case UP:
			{
				while (true)
				{
					// �������� �̵�
					index = index - mTile_MaxNumX;
					// �������� ���̻� �� �� ����
					if (index < 0)
						break;

					if (tileList[index]->nodeIndex == BLOCK_INDEX)
						break;

					// Ž���� ��ġ�� ����ε����� ǥ��
					SearchIndexRender(SEARCH_INDEX, index, tileList);

					// ������ �˻�
					if(mFinishIndex == index)
					{
						node = CreateNode(parent, index, tileList);
						if (node != nullptr)
							mOpenList.emplace_back(node);

						break;
					}

					// �ڳ� ã��
					findIndex = index - mTile_MaxNumX;
					// �������� ���̻� �� �� ����
					if (findIndex < 0)
						break;

					if (tileList[findIndex]->nodeIndex == BLOCK_INDEX)
						break;

					// ������ �ڳ� üũ
					if (index % mTile_MaxNumX != mTile_MaxNumX - 1 && findIndex % mTile_MaxNumX != mTile_MaxNumX - 1)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + 1]->nodeIndex && NORMAL_INDEX == tileList[findIndex + 1]->nodeIndex)
						{
							isCorner = true;

						}
					}
					// ���� �ڳ� üũ
					if (index % mTile_MaxNumX > 0 && findIndex % mTile_MaxNumX > 0)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index - 1]->nodeIndex && NORMAL_INDEX == tileList[findIndex - 1]->nodeIndex)
						{
							isCorner = true;
						}
					}

					if (true == isCorner)
					{
						node = CreateNode(parent, index, tileList);
						if (node != nullptr)
							mOpenList.emplace_back(node);

						break;
					}
				}			
			}
			break;
		case LEFT:
			{
				while (true)
				{
					// �������� �̵�
					index = index - 1;
					// �������� ���̻� �� �� ����
					if (index % mTile_MaxNumX < 0)
						break;

					if (tileList[index]->nodeIndex == BLOCK_INDEX)
						break;

					// Ž���� ��ġ�� ����ε����� ǥ��
					SearchIndexRender(SEARCH_INDEX, index, tileList);

					// ������ �˻�
					if (mFinishIndex == index)
					{
						node = CreateNode(parent, index, tileList);
						if (node != nullptr)
							mOpenList.emplace_back(node);

						break;
					}

					// �ڳ� ã��
					findIndex = index - 1;
					// �������� ���̻� �� �� ����
					if (findIndex % mTile_MaxNumX < 0)
						break;

					if (tileList[findIndex]->nodeIndex == BLOCK_INDEX)
						break;

					// ���� �ڳ� üũ
					if (index >= mTile_MaxNumX && findIndex >= mTile_MaxNumX)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index - mTile_MaxNumX]->nodeIndex 
							&& NORMAL_INDEX == tileList[findIndex - mTile_MaxNumX]->nodeIndex)
						{
							isCorner = true;
						}
					}
					// �Ʒ��� �ڳ� üũ
					if (index / mTile_MaxNumX < mTile_MaxNumY - 1 && findIndex / mTile_MaxNumX < mTile_MaxNumY - 1)
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + mTile_MaxNumX]->nodeIndex 
							&& NORMAL_INDEX == tileList[findIndex + mTile_MaxNumX]->nodeIndex)
						{
							isCorner = true;
						}
					}

					if (true == isCorner)
					{
						node = CreateNode(parent, index, tileList);
						if (node != nullptr)
							mOpenList.emplace_back(node);

						break;
					}
				}
			}
			break;
		case RIGHT:
			{

			}
			break;
		case LEFT_UP:
			{
				while (true)
				{
					// ���� �� �밢�� �̵�
					index = (index - mTile_MaxNumX) -1;
					// ���� ���� ���̻� �� �� ����
					if (index < 0)
						break;

					if (tileList[index]->nodeIndex == BLOCK_INDEX)
						break;

					// Ž���� ��ġ�� ����ε����� ǥ��
					SearchIndexRender(SEARCH_INDEX, index, tileList);

					// ������ �˻�
					if (mFinishIndex == index)
					{
						node = CreateNode(parent, index, tileList);
						if (node != nullptr)
							mOpenList.emplace_back(node);

						break;
					}

					// �ڳ� ã�� ��ĭ ���� üũ
					findIndex = index - mTile_MaxNumX;
					// �������� ���̻� �� �� ����
					if (findIndex % mTile_MaxNumX < 0)
						break;

					// ���� �ڳ� üũ
					if (index % mTile_MaxNumX != mTile_MaxNumX - 1 //������ üũ
						&& findIndex % mTile_MaxNumX != mTile_MaxNumX - 1) // ������ üũ
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + 1]->nodeIndex
							&& NORMAL_INDEX == tileList[findIndex + 1]->nodeIndex)
						{
							isCorner = true;
						}
					}

					// �ڳ� ã�� ��ĭ ���� üũ
					findIndex = index - 1;
					// ���� ���� ���̻� �� �� ����
					if (findIndex % mTile_MaxNumX < 0)
						break;

					// �Ʒ��� �ڳ� üũ
					if (index / mTile_MaxNumX < mTile_MaxNumY - 1 // �Ʒ��� üũ
						&& findIndex / mTile_MaxNumX < mTile_MaxNumY - 1) // �Ʒ��� üũ
					{
						// �ڳʷ� �Ǵ�
						if (BLOCK_INDEX == tileList[index + mTile_MaxNumX]->nodeIndex
							&& NORMAL_INDEX == tileList[findIndex + mTile_MaxNumX]->nodeIndex)
						{
							isCorner = true;
						}
					}

					if (true == isCorner)
					{
						node = CreateNode(parent, index, tileList);
						if (node != nullptr)
							mOpenList.emplace_back(node);

						break;
					}
				}
			}
			break;
		case RIGHT_UP:
			{

			}
			break;
		case DOWN:
			{

			}
			break;
		case LEFT_DOWN:
			{

			}
			break;
		case RIGHTR_DOWN:
			{

			}
			break;
		default:
			break;
	}
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
	G = (dx + dy);

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
		if (index < mTile_MaxNumX)
			return false;

		// �������� �̵�
		index = index - mTile_MaxNumX;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(SEARCH_INDEX, index, tileList);

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

		// �������� �ڳ� ã��
		findIndex = index - mTile_MaxNumX;
		// �������� ���̻� �� �� ����
		if (findIndex < 0)
			continue;

		// ������ �ڳ� üũ
		if (index % mTile_MaxNumX != mTile_MaxNumX - 1 && findIndex % mTile_MaxNumX != mTile_MaxNumX - 1)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index + 1]->nodeIndex && NORMAL_INDEX == tileList[findIndex + 1]->nodeIndex)
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
		if (index % mTile_MaxNumX > 0 && findIndex % mTile_MaxNumX > 0)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index - 1]->nodeIndex && NORMAL_INDEX == tileList[findIndex - 1]->nodeIndex)
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
		if (index % mTile_MaxNumX <= 0)
			return false;

		// �������� �̵�
		index = index - 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(SEARCH_INDEX, index, tileList);

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

		// �������� �ڳ� ã��
		findIndex = index - 1;
		// �������� ���̻� �� �� ����
		if (findIndex % mTile_MaxNumX < 0)
			continue;

		// ���� �ڳ� üũ
		if (index >= mTile_MaxNumX && findIndex >= mTile_MaxNumX)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index - mTile_MaxNumX]->nodeIndex // ���� üũ
				&& NORMAL_INDEX == tileList[findIndex - mTile_MaxNumX]->nodeIndex) // ���� üũ
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
		if (index / mTile_MaxNumX < mTile_MaxNumY - 1 
			&& findIndex / mTile_MaxNumX < mTile_MaxNumY - 1)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index + mTile_MaxNumX]->nodeIndex // �Ʒ��� üũ
				&& NORMAL_INDEX == tileList[findIndex + mTile_MaxNumX]->nodeIndex) // �Ʒ��� üũ
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
		if (index % mTile_MaxNumX <= 0 || index < mTile_MaxNumX)
			return false;

		// ���� �� �밢�� �̵�
		index = (index - mTile_MaxNumX) - 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(SEARCH_INDEX, index, tileList);

		// ������ �˻�
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// �ڳ� ã�� ��ĭ ���� üũ
		if (index >= mTile_MaxNumX)
		{
			findIndex = index - mTile_MaxNumX;

			// ���� �ڳ� üũ
			if (index % mTile_MaxNumX != mTile_MaxNumX - 1 &&  //������ üũ
				findIndex % mTile_MaxNumX != mTile_MaxNumX - 1) // ������ üũ
			{
				// �ڳʷ� �Ǵ�
				if (BLOCK_INDEX == tileList[index + 1]->nodeIndex
					&& NORMAL_INDEX == tileList[findIndex + 1]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}
		
		// �ڳ� ã�� ��ĭ ���� üũ
		if ((index % mTile_MaxNumX) > 0)
		{
			findIndex = index - 1;

			// �Ʒ��� �ڳ� üũ
			if (index / mTile_MaxNumX < mTile_MaxNumY - 1 && // �Ʒ��� üũ
				findIndex / mTile_MaxNumX < mTile_MaxNumY - 1) // �Ʒ��� üũ
			{
				// �ڳʷ� �Ǵ�
				if (BLOCK_INDEX == tileList[index + mTile_MaxNumX]->nodeIndex
					&& NORMAL_INDEX == tileList[findIndex + mTile_MaxNumX]->nodeIndex)
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
		if (index % mTile_MaxNumX == mTile_MaxNumX - 1)
			return false;

		// ���������� �̵�
		index = index + 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(SEARCH_INDEX, index, tileList);

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
		if (index % mTile_MaxNumX == mTile_MaxNumX - 1) // ���������� ���̻� �� �� ����
			continue;

		findIndex = index + 1;
		// ���� �ڳ� üũ
		if (index >= mTile_MaxNumX && findIndex >= mTile_MaxNumX)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index - mTile_MaxNumX]->nodeIndex // ���� üũ
				&& NORMAL_INDEX == tileList[findIndex - mTile_MaxNumX]->nodeIndex) // ���� üũ
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
		if (index / mTile_MaxNumX < mTile_MaxNumY - 1
			&& findIndex / mTile_MaxNumX < mTile_MaxNumY - 1)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index + mTile_MaxNumX]->nodeIndex // �Ʒ��� üũ
				&& NORMAL_INDEX == tileList[findIndex + mTile_MaxNumX]->nodeIndex) // �Ʒ��� üũ
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
		if (index % mTile_MaxNumX == mTile_MaxNumX - 1 || index < mTile_MaxNumX)
			return false;

		// ������ �� �밢�� �̵�
		index = (index - mTile_MaxNumX) + 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(SEARCH_INDEX, index, tileList);

		// ������ �˻�
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// �ڳ� ã�� ��ĭ ���� üũ
		if (index >= mTile_MaxNumX)
		{
			findIndex = index - mTile_MaxNumX;

			// ���� �ڳ� üũ
			if (index % mTile_MaxNumX > 0 &&  //���� üũ
				findIndex % mTile_MaxNumX > 0) // ���� üũ
			{
				// �ڳʷ� �Ǵ�
				if (BLOCK_INDEX == tileList[index - 1]->nodeIndex
					&& NORMAL_INDEX == tileList[findIndex - 1]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}
		
		// �ڳ� ã�� ��ĭ ������ üũ
		if (index % mTile_MaxNumX != mTile_MaxNumX - 1)
		{
			findIndex = index + 1;
			// �Ʒ��� �ڳ� üũ
			if (index / mTile_MaxNumX < mTile_MaxNumY - 1 && // �Ʒ��� üũ
				findIndex / mTile_MaxNumX < mTile_MaxNumY - 1) // �Ʒ��� üũ
			{
				// �ڳʷ� �Ǵ�
				if (BLOCK_INDEX == tileList[index + mTile_MaxNumX]->nodeIndex
					&& NORMAL_INDEX == tileList[findIndex + mTile_MaxNumX]->nodeIndex)
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
		if ((index / mTile_MaxNumX) >= mTile_MaxNumY - 1)
			return false;

		// ���������� �̵�
		index = index + mTile_MaxNumX;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(SEARCH_INDEX, index, tileList);

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
		if ((index / mTile_MaxNumX) >= mTile_MaxNumY - 1) // �Ʒ������� ���̻� �� �� ����
			continue;

		findIndex = index + mTile_MaxNumX;

		// ���� �ڳ� üũ
		if (index % mTile_MaxNumX > 0 
			&& findIndex % mTile_MaxNumX > 0)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index - 1]->nodeIndex // ���� üũ
				&& NORMAL_INDEX == tileList[findIndex - 1]->nodeIndex) // ���� üũ
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
		if (index % mTile_MaxNumX != mTile_MaxNumX - 1
			&& findIndex % mTile_MaxNumX != mTile_MaxNumX - 1)
		{
			// �ڳʷ� �Ǵ�
			if (BLOCK_INDEX == tileList[index + 1]->nodeIndex // ������ üũ
				&& NORMAL_INDEX == tileList[findIndex + 1]->nodeIndex) // ������ üũ
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
		if ((index % mTile_MaxNumX) <= 0 || (index / mTile_MaxNumX) >= mTile_MaxNumY - 1)
			return false;

		// ���� �Ʒ� �밢�� �̵�
		index = (index + mTile_MaxNumX) - 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(SEARCH_INDEX, index, tileList);

		// ������ �˻�
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// �ڳ� ã�� ��ĭ ���� üũ
		if ((index % mTile_MaxNumX) > 0)
		{
			findIndex = index - 1;

			// ���� �ڳ� üũ
			if (index >= mTile_MaxNumX && // ���� üũ
				findIndex >= mTile_MaxNumX) // ���� üũ
			{
				// �ڳʷ� �Ǵ�
				if (BLOCK_INDEX == tileList[index - mTile_MaxNumX]->nodeIndex
					&& NORMAL_INDEX == tileList[findIndex - mTile_MaxNumX]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}

		// �ڳ� ã�� �Ʒ��� üũ
		if (index / mTile_MaxNumX < mTile_MaxNumY - 1)
		{
			findIndex = index + mTile_MaxNumX;

			// ���� �ڳ� üũ
			if (index % mTile_MaxNumX != mTile_MaxNumX - 1 &&  //������ üũ
				findIndex % mTile_MaxNumX != mTile_MaxNumX - 1) // ������ üũ
			{
				// �ڳʷ� �Ǵ�
				if (BLOCK_INDEX == tileList[index + 1]->nodeIndex
					&& NORMAL_INDEX == tileList[findIndex + 1]->nodeIndex)
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

	while (true)
	{
		// ������ �Ʒ��� ���̻� �� �� ����
		if ((index % mTile_MaxNumX) == mTile_MaxNumX -1 || (index / mTile_MaxNumX) >= mTile_MaxNumY - 1)
			return false;

		// ���� �Ʒ� �밢�� �̵�
		index = (index + mTile_MaxNumX) + 1;

		if (tileList[index]->nodeIndex == BLOCK_INDEX)
			return false;

		// Ž���� ��ġ�� ����ε����� ǥ��
		SearchIndexRender(SEARCH_INDEX, index, tileList);

		// ������ �˻�
		if (mFinishIndex == index)
		{
			node = CreateNode(parent, index, tileList);
			if (node != nullptr)
				mOpenList.emplace_back(node);

			return true;
		}

		// �ڳ� ã�� ��ĭ ������ üũ
		if ((index % mTile_MaxNumX) != mTile_MaxNumX - 1)
		{
			findIndex = index + 1;

			// ���� �ڳ� üũ
			if (index >= mTile_MaxNumX && // ���� üũ
				findIndex >= mTile_MaxNumX) // ���� üũ
			{
				// �ڳʷ� �Ǵ�
				if (BLOCK_INDEX == tileList[index - mTile_MaxNumX]->nodeIndex
					&& NORMAL_INDEX == tileList[findIndex - mTile_MaxNumX]->nodeIndex)
				{
					node = CreateNode(parent, index, tileList);
					if (node != nullptr)
						mOpenList.emplace_back(node);

					return true;
				}
			}
		}

		// �ڳ� ã�� �Ʒ��� üũ
		if (index / mTile_MaxNumX < mTile_MaxNumY - 1)
		{
			findIndex = index + mTile_MaxNumX;

			// ���� �ڳ� üũ
			if (index % mTile_MaxNumX > 0 &&  //���� üũ
				findIndex % mTile_MaxNumX > 0) //���� üũ
			{
				// �ڳʷ� �Ǵ�
				if (BLOCK_INDEX == tileList[index - 1]->nodeIndex
					&& NORMAL_INDEX == tileList[findIndex - 1]->nodeIndex)
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

	return false;
}

void JumpPointSearch::SearchIndexRender(const BYTE nodeIndex, const WORD index, vector<RectInfo*>& tileList)
{
	// Ž���� ��ġ�� ����ε����� ǥ��
	if (index != mStartIndex && index != mFinishIndex)
	{
		tileList[index]->nodeIndex = nodeIndex;
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
	static uniform_int_distribution<__int16> randDist(0, 255);

	mRed = (BYTE)randDist(rand);
	mGreen = (BYTE)randDist(rand);
	mBlue = (BYTE)randDist(rand);
}


void JumpPointSearch::Release(void)
{
	for_each(mOpenList.begin(), mOpenList.end(), SafeDelete<AStarNodeInfo*>);
	mOpenList.clear();

	for_each(mCloseList.begin(), mCloseList.end(), SafeDelete<AStarNodeInfo*>);
	mCloseList.clear();
}
