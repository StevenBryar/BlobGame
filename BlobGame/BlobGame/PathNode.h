#ifndef PATH_NODE_H
#define PATH_NODE_H

class Tile;

class PathNode{
public:
	PathNode(Tile* tile);
	PathNode(Tile* tile,int scoreG,int scoreH);
	PathNode(Tile* tile,int scoreG,int scoreH,PathNode* parentNode);
	PathNode(PathNode* pathNode);
	~PathNode();

	Tile* getTile();

	void setScoreG(int scoreG);
	int getScoreG();

	void setScoreH(int scoreH);
	int getScoreH();

	int getFinalScore();

	void setParentNode(PathNode* parent);
	PathNode* getParentNode();

private:
	Tile* m_Tile;
	int m_ScoreG;
	int m_ScoreH;
    PathNode* m_ParentNode;
};

#endif