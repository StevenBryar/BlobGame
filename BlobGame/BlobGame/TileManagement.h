#include <vector>

class Tile;
class Level;
class GameObject;

void tileUpdate(std::vector<GameObject*> objects,std::vector<Tile*> tiles);
int calcGScoreBlob(Tile* tile);
int calcGScoreBasicEnemy(Tile* tile);