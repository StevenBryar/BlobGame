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
	bool (*useAbility)(Blob* caster,const int& cursorWorldX,const int& cursorWorldY);
	
	Ability(const std::string& name,const double& cdDuration,const bool& tAbility,bool (*useAbility)(Blob*,const int&,const int&)){
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
	Blob(const int& health,const int& damage,const int& attackSpeed,
			const int& moveSpeed,const int& armour);
	Blob(const int& health,const int& damage,const int& attackSpeed,
			const int& moveSpeed,const int& armour,Tile* currentTile);
	virtual ~Blob();
	void useAbility(const keyType& key,const int& cursorWorldX,const int& cursorWorldY);
	
	Ability* getAbilityFromKey(const keyType& key);
	Ability* getAbilityFromName(const std::string& name);
	std::map<keyType,Ability*>* getAbilities() const;
	void addAbilty(Ability* a,const keyType& key);
	void removeAbility(Ability* a);
	void removeAbility(const keyType& key);

	virtual void update();
	std::string getAllegiance() const;
	virtual std::string getType();
	virtual void handleMessage(const Message& msg);
	void die();
	int getEnergy() const;
	void setEnergy(const int& e);

protected:
	int m_EnergyResource;
	static int m_BlobCount;
	void updateAbilities();
	std::map<keyType,Ability*>* m_Abilities;
};

#endif