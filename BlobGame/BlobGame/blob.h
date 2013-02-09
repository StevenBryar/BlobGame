#ifndef BLOB_H
#define BLOB_H

#include "unit.h"
#include <map>
#include "constants.h"

class Blob; 
struct Ability{
	bool onCoolDown;
	double coolDown;
	double coolDownDuration;
	std::string name;
	bool targetAbility;
	bool (*useAbility)(Blob* caster,int cursorWorldX,int cursorWorldY);
	
	Ability(std::string name,double cdDuration,bool tAbility,bool (*useAbility)(Blob*,int,int)){
		this->useAbility = useAbility;
		this->name = name;
		coolDownDuration = cdDuration;
		targetAbility = tAbility;
		coolDown = 0.0;
		onCoolDown = false;
	}
};

//map of abilities.
//abilities contain func pointer(actual ability)
//and cooldown timer,cooldown time,and cooldown bool.
//allegiance is ally.
//type is Blob.
//Target Ability/target attack steps.
//
class Blob : public Unit{
public:
	Blob();
	Blob(Tile* currentTile);
	Blob(int health,int damage,int attackSpeed,
			int moveSpeed,int armour);
	Blob(int health,int damage,int attackSpeed,
			int moveSpeed,int armour,Tile* currentTile);
	virtual ~Blob();
	void useAbility(keyType key,int cursorWorldX,int cursorWorldY);
	
	Ability* getAbilityFromKey(keyType key);
	Ability* getAbilityFromName(std::string name);
	std::map<keyType,Ability*>* getAbilities();
	void addAbilty(Ability* a,keyType key);
	void removeAbility(Ability* a);
	void removeAbility(keyType key);

	virtual void update();
	std::string getAllegiance();
	virtual std::string getType();
	virtual void handleMessage(Message msg);
	void die();
	int getEnergy();
	void setEnergy(int e);

protected:
	int m_EnergyResource;
	static int m_BlobCount;
	void updateAbilities();
	std::map<keyType,Ability*>* m_Abilities;
};

#endif