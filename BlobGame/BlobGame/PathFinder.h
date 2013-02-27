class Tile;
class Level;
class PathNode;
#include <vector>

int CalculateScoreH(Tile* startTile, Tile* destinationTile,Level* level);
std::vector<PathNode*>* FindPath(PathNode* curNode,Tile* destTile,Level* level,
									unsigned int unWalkables,int (*calcBaseGScore)(Tile* tile));
Tile* AdjacentTileLeft(Tile* tile,Level* level);
Tile* AdjacentTileTopLeft(Tile* tile,Level* level);
Tile* AdjacentTileTopRight(Tile* tile,Level* level);
Tile* AdjacentTileTop(Tile* tile,Level* level);
Tile* AdjacentTileRight(Tile* tile,Level* level);
Tile* AdjacentTileBottom(Tile* tile,Level* level);
Tile* AdjacentTileBottomLeft(Tile* tile,Level* level);
Tile* AdjacentTileBottomRight(Tile* tile,Level* level);
std::vector<Tile*> getSquareVision(Level* level,const int& visionRange,Tile* centerTile);
PathNode* CreateNewNode(PathNode* parentNode,Tile* tile,Tile* destTile,Level* level,int (*calcBaseGScore)(Tile* tile));
void SortOpenList(PathNode* node,std::vector<PathNode*>* openList);