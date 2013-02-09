#ifndef GRUNTBLOB_H
#define GRUNTBLOB_H
#include "blob.h"

//Abilities:surge:the blob speeds up for 1.5 seconds,armour is reduced.
//			
class GruntBlob : public Blob{
public:
	GruntBlob();
		GruntBlob(Tile* currentTile);
	GruntBlob(int health,int damage,int attackSpeed,
			int moveSpeed,int armour);
	GruntBlob(int health,int damage,int attackSpeed,
			int moveSpeed,int armour,Tile* currentTile);
	~GruntBlob();

	void update();
	std::string getType();
	void useAbility(keyType key,int cursorWorldX,int cursorWorldY);
	void handleMessage(Message msg);
	void die();
protected:
	static int m_GruntCount;
};
#endif