#include <vector>

class Tile;
class Level;
class GameObject;

void tileUpdate(std::vector<GameObject*> objects);
int calcGScoreBlob(Tile* tile);
int calcGScoreBasicEnemy(Tile* tile);