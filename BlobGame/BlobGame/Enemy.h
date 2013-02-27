#ifndef ENEMY_H
#define ENEMY_H

#include "unit.h"
#include <vector>

//enemy is made up of states(search,hold,attack,re-position,
//states will be made up of function pointers and an enum.
//search:if there is no blob within site, go to a prevoiusly undiscovered location.
//if no undiscovered location,pick any location(tile).
//enemy can be in squads with one squad leader(enemies will follow leader around,but act independtly in battle).
//Hold:enemy will stay at his current position.
//
class Level;
class Tile;

enum EnemyStates{
	Hold,
	Attack,
	Search,
	rePosition,
};

class Enemy : public Unit{
public:
	Enemy(std::vector<Enemy*>* mates,const std::string& spriteFileName,
		std::vector<Tile*> (*getAttackRange)(Tile* currentTile,const int& dirFacing,Level* level),
		void (*fire)(Tile* currentTile,const int& dirFacing,Level* level,Enemy* attacker),const int& aRange);

	Enemy(std::vector<Enemy*>* mates,Tile* currentTile,const std::string& spriteFileName,
		std::vector<Tile*> (*getAttackRange)(Tile* currentTile,const int& dirFacing,Level* level),
		void (*fire)(Tile* currentTile,const int& dirFacing,Level* level,Enemy* attacker),const int& aRange);

	Enemy(std::vector<Enemy*>* mates,const int& health,const int& damage,const int& attackSpeed,
			const int& moveSpeed,const int& armour,const std::string& spriteFileName,
			std::vector<Tile*> (*getAttackRange)(Tile* currentTile,const int& dirFacing,Level* level),
			void (*fire)(Tile* currentTile,const int& dirFacing,Level* level,Enemy* attacker),const int& aRange);

	Enemy(std::vector<Enemy*>* mates, const int& health,const int& damage,const int& attackSpeed,
			const int& moveSpeed,const int& armour,Tile* currentTile,const std::string& spriteFileName,
			std::vector<Tile*> (*getAttackRange)(Tile* currentTile,const int& dirFacing,Level* level),
			void (*fire)(Tile* currentTile,const int& dirFacing,Level* level,Enemy* attacker),const int& aRange);
	~Enemy();
	void search();
	void hold();
	void idle();
	std::vector<Enemy*>* getSquadMates() const;
	void setSquadMates(std::vector<Enemy*>* mates);
	Enemy* getSquadLeader();
	EnemyStates getState() const;
	bool isSquadLeader() const;
	void setIsSquadLeader(const bool& isLeader);
	void update();
	void handleMessage(const Message& msg);
	std::string getAllegiance();
	std::string getType();
protected:
	void die();
	void (Enemy::*EnemyUpdate)();
	std::vector<Tile*> (*getAttackRange)(Tile* currentTile,const int& dirFacing,Level* level);
	void (*fire)(Tile* currentTile,const int& dirFacing,Level* level,Enemy* attacker);
	void changeState(EnemyStates state);
	void searchUpdate();
	void holdUpdate();
	void attackUpdate();
	void rePosUpdate();
	bool m_SquadLeader;
	std::vector<Enemy*>* m_SquadMates;
	EnemyStates m_State;
	static int m_EnemyCount;
	int m_AttackRange;
};

#endif