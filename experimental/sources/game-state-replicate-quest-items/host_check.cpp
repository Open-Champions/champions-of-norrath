#include "replicate_quest_items.cpp"
#include <cstdio>
static_assert(sizeof(gameNetReplicateQuestItems) == 4, "gameNetReplicateQuestItems is one word");
int main() {
    if (gameNetReplicateQuestItems != 0) return 1;
    gameNetReplicateQuestItems = 8;
    if (gameNetReplicateQuestItems != 8) return 2;
    std::puts("gameNetReplicateQuestItems contract passed");
    return 0;
}
