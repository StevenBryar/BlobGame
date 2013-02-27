#ifndef Unit_H
#define Unit_H

#include <vector>
#include "gameObject.h"
#include "MessageListener.h"

class Tile;
class PathNode;
class Level;
enum UnitStuff;

//health,move speed,damage,armour,target,path
//move,attack,patrol,die,take damage,
class Unit : public GameObject , public MessageListener{
public:
	Unit();
	Unit(Tile* currentTile);
	Unit(Tile* currentTile,Level* level);
	Unit(const int& health,const int& damage,const int& attackSpeed,
			const int& moveSpeed,const int& armour);
	Unit(const int& health,const int& damage,const int& attackSpeed,
			const int& moveSpeed,const int& armour,Tile* currentTile);
	Unit(const int& health,const int& damage,const int& attackSpeed,
			const int& moveSpeed,const int& armour,Tile* currentTile,Level* level);
	virtual ~Unit();

	virtual void attack(Unit* aUnit);
	virtual void hit(const int& damage);

	virtual void update();

	int (*calcBaseGScore)(Tile* tile);

	void setCurrentTile(Tile* aTile);
	void setDestinationTile(Tile* tile);
	void setLevel(Level* level);
	void stop();
	void fullStop();

	Tile* getCurrentTile() const;
	Tile* getDestinationTile() const;

	void setHealth(const int& hp);
	void setMoveSpeed(const int& moveSpeed);
	void setDamage(const int& damage);
	void setAttackSpeed(const int& attackSpeed);
	void setArmour(const int& armour);
	void setTarget(Unit* target);
	void setAttackTimer(const float& attackTimer);
	void setUnwalkables(const unsigned int& unWalkables);

	int getHealth() const;
	int getMoveSpeed() const;
	int getDamage() const;
	int getAttackSpeed() const;
	int getArmour() const;
	Unit* getTarget() const;
	float getAttackTimer() const;
	unsigned int getUnwalkables() const;

	void addStatus(const UnitStuff& status);
	void setStatus(const unsigned int& status);
	unsigned int getStatus() const;

	virtual std::string getAllegiance();
	virtual std::string getType();

	void addPassive(void (*passive)(Unit*));
	void removePassive(void (*passive)(Unit*));
	std::vector<void (*)(Unit*)>* getPassives();

	void changeDirection(const int& d);
	int getDirection();

	virtual void handleMessage(const Message& msg) = 0;
protected:
	virtual void die();
	void moveLerp();
	void updatePassives();
	void updateStatus();
	int m_Health;
	int m_MoveSpeed;
	int m_Damage;
	int m_Armour;
	int m_AttackSpeed;
	unsigned int m_UnitStatus;
	unsigned int m_Unwalkables;
	Tile* m_CurrentTile;
	Tile* m_DestinationTile;
	std::vector<PathNode*>* m_Path;
	Unit* m_Target;
	Level* m_Level;
	std::vector<void (*)(Unit*)>* m_Passives;
	std::vector<void (*)(Unit*)>* m_PassivesToDelete;
	float m_AttackTimer;

};
#endif