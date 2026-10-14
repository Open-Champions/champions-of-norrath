// Host contract for msg_hurt__10CloudGiantfP10GameObjecti; not an EE input. Includes the source.
#include "msg_hurt.cpp"
#include <cstdio>
#include <cstring>
int bossMeterCurrent;
static int calls; static void *seenSelf, *seenAttacker; static float seenAmount; static int seenFlags;
void msg_hurt__8CreaturefP10GameObjecti(void* s, float a, void* o, int f) { ++calls; seenSelf = s; seenAmount = a; seenAttacker = o; seenFlags = f; }
int main() {
    alignas(16) static unsigned char object[1100];
    int bad = 0;
    struct Case { int flag; float health; int want; } cases[] = {{1, 12.75f, 12}, {7, -3.5f, -3}, {0, 99.0f, 1234}, {1, 0.0f, 0}};
    for (const Case& c : cases) {
        std::memset(object, 0, sizeof object);
        std::memcpy(object + 976, &c.flag, 4); std::memcpy(object + 560, &c.health, 4);
        bossMeterCurrent = 1234; calls = 0;
        msg_hurt__10CloudGiantfP10GameObjecti(object, 2.5f, object + 4, 9);
        if (calls != 1 || seenSelf != object || seenAmount != 2.5f || seenAttacker != object + 4 || seenFlags != 9 || bossMeterCurrent != c.want) {
            std::printf("fail: flag=%d health=%f\n", c.flag, c.health); bad = 1;
        }
    }
    return bad;
}
