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
	Unit(int health,int damage,int attackSpeed,
			int moveSpeed,int armour);
	Unit(int health,int damage,int attackSpeed,
			int moveSpeed,int armour,Tile* currentTile);
	Unit(int health,int damage,int attackSpeed,
			int moveSpeed,int armour,Tile* currentTile,Level* level);
	virtual ~Unit();

	virtual void attack(Unit* aUnit);
	virtual void hit(int damage);

	virtual void update();

	int (*calcBaseGScore)(Tile* tile);

	void setCurrentTile(Tile* aTile);
	void setDestinationTile(Tile* tile);
	void setLevel(Level* level);
	void stop();
	void fullStop();

	Tile* getCurrentTile();
	Tile* getDestinationTile();

	void setHealth(int hp);
	void setMoveSpeed(int moveSpeed);
	void setDamage(int damage);
	void setAttackSpeed(int attackSpeed);
	void setArmour(int armour);
	void setTarget(Unit* target);
	void setAttackTimer(float attackTimer);
	void setUnwalkables(unsigned int unWalkables);

	int getHealth();
	int getMoveSpeed();
	int getDamage();
	int getAttackSpeed();
	int getArmour();
	Unit* getTarget();
	float getAttackTimer();
	unsigned int getUnwalkables();

	void addStatus(UnitStuff status);
	void setStatus(unsigned int status);
	unsigned int getStatus();

	virtual std::string getAllegiance();
	virtual std::string getType();

	void addPassive(void (*passive)(Unit*));
	void removePassive(void (*passive)(Unit*));
	std::vector<void (*)(Unit*)>* getPassives();

	void changeDirection(int d);
	int getDirection();

	virtual void handleMessage(Message msg) = 0;
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