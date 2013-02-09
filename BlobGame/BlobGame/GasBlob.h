#ifndef GASBLOB_H
#define GASBLOB_H
#include "blob.h"		
class GasBlob : public Blob{
public:
	GasBlob();
	GasBlob(Tile* currentTile);
	GasBlob(int health,int damage,int attackSpeed,
			int moveSpeed,int armour);
	GasBlob(int health,int damage,int attackSpeed,
			int moveSpeed,int armour,Tile* currentTile);
	~GasBlob();

	void update();
	std::string getType();
	void useAbility(keyType key,int cursorWorldX,int cursorWorldY);
	void handleMessage(Message msg);
	void die();
protected:
	static int m_GasBlobCount;
};
#endif