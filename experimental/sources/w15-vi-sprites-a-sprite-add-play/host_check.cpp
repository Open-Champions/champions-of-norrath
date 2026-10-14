// Host check only; not a build input.
#include "add_play.cpp"
static void *sceneSeen, *sceneSeen2, *objSeen; static int idSeen, idSeen2, pbSeen; static float valSeen;
static unsigned char record[64];
extern "C" unsigned char* Animation__7VIScenei(void* scene, int id) { sceneSeen = scene; idSeen = id; return record; }
extern "C" int AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(void* o, int id, int pb, float v, void* scene) {
    objSeen = o; idSeen2 = id; pbSeen = pb; valSeen = v; sceneSeen2 = scene; return 4321;
}
int main() {
    char obj[4], scene[4];
    *reinterpret_cast<float*>(record + 28) = 2.5f; *reinterpret_cast<int*>(record + 32) = 6;
    if (AddPlay__9VIHSpriteiR7VIScene(obj, 9, scene) != 4321) return 1;
    if (sceneSeen != scene || idSeen != 9 || objSeen != obj || idSeen2 != 9 || pbSeen != 6 || valSeen != 2.5f || sceneSeen2 != scene) return 2;
    return 0;
}
