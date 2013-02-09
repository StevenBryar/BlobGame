#ifndef PSIBLOB_H
#define PSIBLOB_H
#include "blob.h"		
class PsiBlob : public Blob{
public:
	PsiBlob();
		PsiBlob(Tile* currentTile);
	PsiBlob(int health,int damage,int attackSpeed,
			int moveSpeed,int armour);
	PsiBlob(int health,int damage,int attackSpeed,
			int moveSpeed,int armour,Tile* currentTile);
	~PsiBlob();

	void update();
	std::string getType();
	void useAbility(keyType key,int cursorWorldX,int cursorWorldY);
	void handleMessage(Message msg);
	void die();
protected:
	static int m_PsiCount;
};
#endif