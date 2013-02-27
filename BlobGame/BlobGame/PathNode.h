#ifndef PATH_NODE_H
#define PATH_NODE_H

class Tile;

class PathNode{
public:
	PathNode(Tile* tile);
	PathNode(Tile* tile,const int& scoreG,const int& scoreH);
	PathNode(Tile* tile,const int& scoreG,const int& scoreH,PathNode* parentNode);
	PathNode(PathNode* pathNode);
	~PathNode();

	Tile* getTile() const;

	void setScoreG(const int& scoreG);
	int getScoreG() const;

	void setScoreH(const int& scoreH);
	int getScoreH() const;

	int getFinalScore() const;

	void setParentNode(PathNode* parent);
	PathNode* getParentNode() const;

private:
	Tile* m_Tile;
	int m_ScoreG;
	int m_ScoreH;
    PathNode* m_ParentNode;
};

#endif