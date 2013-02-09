#include "PathNode.h"
#include "tile.h"
#include "common.h"
#include <stdlib.h>

PathNode::PathNode(Tile* tile) :
		m_Tile(tile),m_ScoreG(0),
			m_ScoreH(0),m_ParentNode(NULL){}

PathNode::PathNode(Tile* tile,int scoreG,int scoreH) :
			m_Tile(tile),m_ScoreG(scoreG),
				m_ScoreH(scoreH),m_ParentNode(NULL){}

PathNode::PathNode(Tile* tile,int scoreG,int scoreH,
					PathNode* parentNode) : 
				m_Tile(tile),m_ScoreG(scoreG),
					m_ScoreH(m_ScoreH),m_ParentNode(parentNode){}

PathNode::PathNode(PathNode* pathNode) :			
				m_ScoreG(pathNode->getScoreG()),
				m_ScoreH(pathNode->getScoreH()){
		m_ParentNode = pathNode->getParentNode();
		m_Tile = pathNode->getTile();
}

PathNode::~PathNode(){
	m_Tile = NULL;
	m_ParentNode = NULL;
}

Tile* PathNode::getTile(){return m_Tile;}
void PathNode::setScoreG(int scoreG){m_ScoreG = scoreG;}
int PathNode::getScoreG(){return m_ScoreG;}
void PathNode::setScoreH(int scoreH){m_ScoreH = scoreH;}
int PathNode::getScoreH(){return m_ScoreH;}
int PathNode::getFinalScore(){return m_ScoreG + m_ScoreH;}
void PathNode::setParentNode(PathNode* parent){m_ParentNode = parent;}
PathNode* PathNode::getParentNode(){return m_ParentNode;}