
#include"PathFinder.h"
#include "PathNode.h"
#include "Level.h"
#include "Tile.h"
#include "Constants.h"
#include "BlobGameConstants.h"
#include "common.h"

std::vector<PathNode*>* FindPath(PathNode* curNode,Tile* destTile,
			Level* level,unsigned int unWalkables,int (*calcBaseGScore)(Tile* tile))
{
	if(ContainsFlags(destTile->getTileTypes(),unWalkables)){
		return NULL;
	}
	static bool firstLoop = true;
	static std::vector<PathNode*>* openList;
	static std::vector<PathNode*>* closedList;

	if(firstLoop){
	openList = new std::vector<PathNode*>();
	closedList = new std::vector<PathNode*>();
	firstLoop = false;
	}

	//place node in closed list,erase from open.
	PathNode* currentNode = new PathNode(curNode);
	closedList->push_back(currentNode);
	if(openList->size() > 0)
	openList->erase(openList->begin());
	curNode = NULL;
	//check if node's tile index is same as destTile.
	//if yes copy node,then put the path into final path and return.
	if(currentNode->getTile() == destTile)
	{
		std::vector<PathNode*>* finalPath = 
			new std::vector<PathNode*>();
		finalPath->push_back(new PathNode(currentNode));
		for(int i = 0;i < finalPath->size();i++)
		{
			if((*finalPath)[i]->getParentNode() == NULL)
				break;

			Tile* tile = (*finalPath)[i]->getTile();
			finalPath->push_back(
				new PathNode((*finalPath)[i]->getParentNode()));
		}
		SafeVectorDelete<PathNode>(*openList);
		SafePtrRelease(openList);

		SafeVectorDelete<PathNode>(*closedList);
		SafePtrRelease(closedList);
		finalPath->pop_back();
		firstLoop = true;
		return finalPath;
	}
	//find adjacent walkable tiles,that arent in closed list.
	//check if in open list
	Tile* adTiles[4] = {AdjacentTileLeft(currentNode->getTile(),level),
						AdjacentTileTop(currentNode->getTile(),level),
						AdjacentTileRight(currentNode->getTile(),level),
						AdjacentTileBottom(currentNode->getTile(),level)};

	int adTilesSize = sizeof(adTiles)/sizeof(Tile*);
	bool inValid = false;
	for(int i = 0;i < adTilesSize;i++)
	{
		inValid = false;
		if(adTiles[i] == NULL)
		{
			continue;
		}

		if(!ContainsFlags(adTiles[i]->getTileTypes(),unWalkables))
		{
			for(int j = 0;j < closedList->size();j++)
			{
				if((*closedList)[j]->getTile() == adTiles[i])
				{
					inValid = true;
					break;
				}
			}
		}
		else
		{
			inValid = true;
		}

		if(!inValid)
		{
			if(openList->size() > 0)
			{
				for(int j = 0;j < openList->size();j++)
				{
					if((*openList)[j]->getTile() == adTiles[i] &&
						(*openList)[j]->getScoreG() < currentNode->getScoreG())
					{
						PathNode* adNode = new PathNode((*openList)[j]);
						closedList->push_back(adNode);
						openList->erase(openList->begin() + j);
						adNode = NULL;
						break;
					}
					else if(openList->at(openList->size() - 1) == (*openList)[j])
					{
						PathNode* adNode = CreateNewNode(
							currentNode,adTiles[i],destTile,level,calcBaseGScore);
						SortOpenList(adNode,openList);
						adNode = NULL;
						break;
					}
				}
			}
			else
			{
				PathNode* adNode = CreateNewNode(
					currentNode,adTiles[i],destTile,level,calcBaseGScore);
				SortOpenList(adNode,openList);
				adNode = NULL;
			}
		}
	}
	if(openList->size() < 1){
		SafeVectorDelete<PathNode>(*openList);
		SafePtrRelease(openList);
		SafeVectorDelete<PathNode>(*closedList);
		SafePtrRelease(closedList);
		firstLoop = true;
		return NULL;
	}
	else
	return FindPath((*openList)[0],destTile,level,unWalkables,calcBaseGScore);
}

int CalculateScoreH(Tile* startTile, Tile* destinationTile,Level* level)
{
		return abs(level->getTileCoordinateForPosition(destinationTile->getPositionX())
		- level->getTileCoordinateForPosition(startTile->getPositionX())) +
		abs(level->getTileCoordinateForPosition(destinationTile->getPositionY())
		- level->getTileCoordinateForPosition(startTile->getPositionY()));
}

Tile* AdjacentTileLeft(Tile* tile,Level* level)
{
	Tile* adTile = 
		level->getTileForPosition(
		tile->getPositionX() - TILE_SIZE,tile->getPositionY());

	return adTile;
}
Tile* AdjacentTileRight(Tile* tile,Level* level)
{
	Tile* adTile = 
		level->getTileForPosition(
		tile->getPositionX() + TILE_SIZE,tile->getPositionY());

	return adTile;
}
Tile* AdjacentTileTopLeft(Tile* tile,Level* level){
	Tile* adTile = 
		level->getTileForPosition(
		tile->getPositionX() - TILE_SIZE,
		tile->getPositionY() - TILE_SIZE);

	return adTile;
}
Tile* AdjacentTileTopRight(Tile* tile,Level* level){
	Tile* adTile = 
		level->getTileForPosition(
		tile->getPositionX() + TILE_SIZE,
		tile->getPositionY() - TILE_SIZE);

	return adTile;
}

Tile* AdjacentTileTop(Tile* tile,Level* level)
{
	Tile* adTile = 
		level->getTileForPosition(
		tile->getPositionX(),tile->getPositionY() - TILE_SIZE);

	return adTile;
}

Tile* AdjacentTileBottomLeft(Tile* tile,Level* level){
	Tile* adTile = 
		level->getTileForPosition(
		tile->getPositionX() - TILE_SIZE,
		tile->getPositionY() + TILE_SIZE);

	return adTile;
}
Tile* AdjacentTileBottomRight(Tile* tile,Level* level){
	Tile* adTile = 
		level->getTileForPosition(
		tile->getPositionX() + TILE_SIZE,
		tile->getPositionY() + TILE_SIZE);

	return adTile;
}
Tile* AdjacentTileBottom(Tile* tile,Level* level)
{
	Tile* adTile = 
		level->getTileForPosition(
		tile->getPositionX(),tile->getPositionY() + TILE_SIZE);

	return adTile;
}

PathNode* CreateNewNode(PathNode* parentNode,Tile* tile,Tile* destTile,Level* level,int (*calcBaseGScore)(Tile* tile))
{
	PathNode* newNode;
	if(!calcBaseGScore){
		newNode = new PathNode(tile,parentNode->getScoreG() + 1,
					CalculateScoreH(tile,destTile,level));
	}
	else{
		newNode = new PathNode(tile,calcBaseGScore(tile) + parentNode->getScoreG() + 1,
					CalculateScoreH(tile,destTile,level));
	}
	newNode->setParentNode(parentNode);

	return newNode;
}

void SortOpenList(PathNode* node,std::vector<PathNode*>* openList)
{
	if(openList->size() == 0)
	{
		openList->push_back(node);
	}
	else
	{
		for(int i = 0;i < openList->size();i++)
		{
			if((*openList)[0]->getFinalScore() > node->getFinalScore())
			{
				openList->insert(openList->begin()+0,node);
				return;
			}
			if(openList->at(openList->size() - 1) == (*openList)[i])
			{
				if((*openList)[i]->getFinalScore() <= 
				node->getFinalScore())
				{
					openList->push_back(node);
					return;
				}
				else if((*openList)[i]->getFinalScore() > 
				node->getFinalScore())
				{
					openList->insert(openList->begin()+i,node);
					return;
				}
			}
			if((*openList)[i]->getFinalScore() <
				node->getFinalScore() && 
				(*openList)[i+1]->getFinalScore() > 
				node->getFinalScore())
			{
				openList->insert(openList->begin()+i,node);
				return;
			}
		}
	}
}

std::vector<Tile*> getSquareVision(Level* level,const int& visionRange,Tile* centerTile){
	//sides
	std::vector<Tile*> tiles;
	int tileXCoor = level->getTileCoordinateForPosition(centerTile->getPositionX());
	int tileYCoor = level->getTileCoordinateForPosition(centerTile->getPositionY());
	int startVisionCoorX = tileXCoor - visionRange;
	int startVisionCoorY = tileYCoor - visionRange;
	int endVisionCoorX = tileXCoor + visionRange+1;
	int endVisionCoorY = tileYCoor + visionRange+1;
	Tile* tile = NULL;
	for(tileYCoor = startVisionCoorY;tileYCoor < endVisionCoorY;tileYCoor++){
		for(tileXCoor = startVisionCoorX;tileXCoor < endVisionCoorX;tileXCoor++){
			if((tile = level->getTileForCoordinates(tileXCoor,tileYCoor))){
				tiles.push_back(tile);
			}
		}
	}
	return tiles;
}