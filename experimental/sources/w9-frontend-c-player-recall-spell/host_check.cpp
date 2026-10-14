#include "player_recall_spell.cpp"
#include <cstring>
unsigned char player[4 * 23984];
int nmPlayers;
int nmPlayersAlive;
int playersAlive[4];
unsigned char recallOn;
signed char recallEnter;
signed char playerRecalled;
signed char cutMode;
int levelFlags;
char levelName[200];
Point3 recallPos[4];
int haloColor[4][4];
int castLightColor[4][4];
int lightIntensity[4];
float lightInc[4];

static int deactivated[8];
static int ndeact;
static int fullNameCalls[8];
static int nfull;
static int emptySpotCalls;
static unsigned char entityStore[4][1400];

void lightDeactivate__FP5Light(void* light) {
    for (int i = 0; i < 4; ++i)
        if (light == entityStore[i] + 1268) deactivated[ndeact++] = i;
}
void objectFindEmptySpot__FG6Point3iiPc(void* result, const void* point, int a, int b, char* c) {
    ++emptySpotCalls;
    if (a != 72 || b != 24 || c != 0) std::memset(result, 0x55, sizeof(Point3));
    Point3 p;
    std::memcpy(&p, point, sizeof p);
    p.x += 1000.0f;  // marker: result differs from the input
    std::memcpy(result, &p, sizeof p);
}
void gameGetFullLevelName__FPc(char* name) {
    for (int i = 0; i < 4; ++i)
        if (name == reinterpret_cast<char*>(player + i * 23984 + 36)) fullNameCalls[nfull++] = i;
}

static void setEntity(int slot, int entityIndex) {
    unsigned char* e = entityIndex >= 0 ? entityStore[entityIndex] : 0;
    std::memcpy(player + slot * 23984 + 21872, &e, sizeof e);
}
static void setHealth(int slot, float h) { std::memcpy(player + slot * 23984 + 172, &h, 4); }
static void setPos(unsigned char* base, float x, float y, float z) {
    float v[3] = {x, y, z};
    std::memcpy(base, v, 12);
}
static Point3 getPos(const unsigned char* base) { Point3 p; std::memcpy(&p, base, 12); return p; }
static void reset() {
    std::memset(player, 0, sizeof player);
    std::memset(entityStore, 0, sizeof entityStore);
    std::memset(levelName, 0, sizeof levelName);
    std::memset(recallPos, 0, sizeof recallPos);
    std::memset(haloColor, 0, sizeof haloColor);
    std::memset(castLightColor, 0, sizeof castLightColor);
    ndeact = nfull = emptySpotCalls = 0;
    nmPlayers = 3; nmPlayersAlive = 2; playersAlive[0] = 1; playersAlive[1] = 2;
    recallOn = 0; recallEnter = 0; playerRecalled = 0; cutMode = 0; levelFlags = 0;
    for (int i = 0; i < 4; ++i) {
        lightIntensity[i] = 7; lightInc[i] = 7.0f;
        setEntity(i, i);
        setHealth(i, 5.0f);
        setPos(entityStore[i] + 16, 10.0f * (i + 1), 20.0f * (i + 1), 30.0f * (i + 1));
        setPos(player + i * 23984 + 68, -1.0f * (i + 1), -2.0f, -3.0f);
    }
}
#define CHECK(c, n) do { if (!(c)) return n; } while (0)
int main() {
    // off: no effect
    reset(); playerRecallSpell__Fv();
    CHECK(recallOn == 0 && lightIntensity[0] == 7 && ndeact == 0, 1);
    // frame 60: colors set for the first nmPlayers slots only
    reset(); recallOn = 60;
    playerRecallSpell__Fv();
    CHECK(recallOn == 59, 2);
    for (int i = 0; i < 3; ++i) {
        CHECK(haloColor[i][0] == 100 && haloColor[i][1] == 200 && haloColor[i][2] == 255 && haloColor[i][3] == 0, 3);
        CHECK(castLightColor[i][0] == 86 && castLightColor[i][1] == 126 && castLightColor[i][2] == 250 && castLightColor[i][3] == -96, 4);
        CHECK(lightIntensity[i] == 0 && lightInc[i] > 0.0499f && lightInc[i] < 0.0501f, 5);
    }
    CHECK(haloColor[3][0] == 0 && lightIntensity[3] == 7 && lightInc[3] == 7.0f, 6);
    CHECK(ndeact == 0 && nfull == 0, 7);
    // middle frame: only the counter moves
    reset(); recallOn = 30;
    playerRecallSpell__Fv();
    CHECK(recallOn == 29 && lightIntensity[0] == 7 && haloColor[0][0] == 0, 8);
    // final frame, cutMode set: lights off, nothing else
    reset(); recallOn = 1; cutMode = 1; setEntity(1, -1);
    std::strcpy(levelName + 81, "somewhere");
    playerRecallSpell__Fv();
    CHECK(recallOn == 0 && lightIntensity[0] == 0 && lightInc[2] == 0.0f && lightIntensity[3] == 7, 9);
    CHECK(ndeact == 2 && deactivated[0] == 0 && deactivated[1] == 2, 10);
    CHECK(nfull == 0 && levelName[80] == 0 && recallEnter == 0, 11);
    // final frame with nobody alive
    reset(); recallOn = 1; nmPlayersAlive = 0;
    playerRecallSpell__Fv();
    CHECK(nfull == 0 && levelName[80] == 0, 12);
    // hub: positions saved, level name taken from player 0 + 36
    reset(); recallOn = 1;
    std::strcpy(levelName + 81, "gnomehub");
    std::strcpy(reinterpret_cast<char*>(player + 36), "tavern");
    playerRecallSpell__Fv();
    CHECK(playerRecalled == 1 && getPos(reinterpret_cast<unsigned char*>(&recallPos[1])).x == -2.0f, 13);
    CHECK(getPos(reinterpret_cast<unsigned char*>(&recallPos[2])).x == -3.0f && recallPos[3].x == 0.0f, 14);
    CHECK(std::strcmp(levelName, "tavern") == 0 && levelName[80] == -3 && nfull == 0 && recallEnter == 0, 15);
    // each hub name is accepted
    static const char* hubs[] = {"kelethin", "gnomehub", "beachhub", "elfhub", "airhub"};
    for (const char* hub : hubs) {
        reset(); recallOn = 1;
        std::strcpy(levelName + 81, hub);
        std::strcpy(reinterpret_cast<char*>(player + 36), "x");
        playerRecallSpell__Fv();
        CHECK(playerRecalled == 1, 16);
    }
    // not a hub, "elfhubn" must not match
    reset(); recallOn = 1;
    std::strcpy(levelName + 81, "elfhubn");
    playerRecallSpell__Fv();
    CHECK(playerRecalled == 0 && recallEnter == 1, 17);
    // non-hub: slot 0 alive and healthy follows its entity, slot 1 dead and slot 2 without entity use the empty spot
    reset(); recallOn = 1;
    std::strcpy(levelName + 81, "dungeon");
    setHealth(1, 0.0f);
    setEntity(2, -1);
    levelFlags = 0x10000 | 0x200;  // gnomehub wins
    playerRecallSpell__Fv();
    CHECK(getPos(player + 68).x == 10.0f && getPos(player + 68).z == 30.0f, 18);
    // playersAlive[0] = 1 -> entity 1 position (20, 40, 60) + marker
    CHECK(emptySpotCalls == 2, 19);
    CHECK(getPos(player + 23984 + 68).x == 1020.0f && getPos(player + 23984 + 68).y == 40.0f, 20);
    CHECK(getPos(player + 2 * 23984 + 68).x == 1020.0f && getPos(player + 2 * 23984 + 68).z == 60.0f, 21);
    CHECK(nfull == 3 && fullNameCalls[0] == 0 && fullNameCalls[2] == 2, 22);
    CHECK(recallEnter == 1 && std::strcmp(levelName, "gnomehub") == 0 && levelName[80] == -3, 23);
    CHECK(std::strcmp(levelName + 81, "dungeon") == 0, 24);
    // flag priority, one test per name
    struct { int flags; const char* name; } cases[] = {
        {0x20000 | 0x10000, "kelethin"}, {0x8000 | 0x400, "beachhub"}, {0x400 | 0x200, "elfhub"}, {0x200, "airhub"}};
    for (auto& c : cases) {
        reset(); recallOn = 1;
        std::strcpy(levelName + 81, "dungeon");
        levelFlags = c.flags;
        playerRecallSpell__Fv();
        CHECK(std::strcmp(levelName, c.name) == 0 && levelName[80] == -3, 25);
    }
    // no flag: name unchanged but the marker byte is still written
    reset(); recallOn = 1;
    std::strcpy(levelName, "keep");
    std::strcpy(levelName + 81, "dungeon");
    playerRecallSpell__Fv();
    CHECK(std::strcmp(levelName, "keep") == 0 && levelName[80] == -3 && recallEnter == 1, 26);
    return 0;
}
