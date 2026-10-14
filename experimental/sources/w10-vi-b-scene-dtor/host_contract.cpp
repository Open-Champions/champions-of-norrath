#include "scene_dtor.cpp"

#include <string>

static std::string trace;
static unsigned char* object_bytes;

static long off(Member* m) { return reinterpret_cast<unsigned char*>(m) - object_bytes; }

extern "C" void Clear__7VIScene(VIScene* s) {
    trace += reinterpret_cast<unsigned char*>(s) == object_bytes ? "clear;" : "clear?;";
}
#define MEMBER(sym) \
    extern "C" void sym(Member* m, int flags) { trace += std::string(#sym) + "@" + std::to_string(off(m)) + "," + std::to_string(flags) + ";"; }
MEMBER(_$_10VIMatrix44)
MEMBER(_$_12VIAtmosphere)
MEMBER(_$_14VIQueryTriList)
MEMBER(_$_16VIParticleSystem)
MEMBER(_$_19VISpellEffectSystem)
MEMBER(_$_7VIWorld)
MEMBER(_$_t6VIList1Z7VIActor)
MEMBER(_$_t6VIList1ZQ27VIScene12VISceneOccup)
MEMBER(_$_t6VIPool1ZP13VIHSpriteAnim)
MEMBER(_$_t6VIPool1ZP16VIStaticLighting)
MEMBER(_$_t6VIPool1ZP8VIRefMap)
MEMBER(_$_t6VIPool1ZP8VISprite)
MEMBER(_$_t7VIArray1Z11VIClipEntry)
MEMBER(_$_t7VIArray1Z7VIPlane)
MEMBER(_$_t7VIArray1Z7VIVect3)
MEMBER(_$_t7VIArray1Zi)
MEMBER(_$_t8VIVector1Z18VISceneDisplayElem)
extern "C" void __builtin_delete(void* p) { trace += p == object_bytes ? "delete;" : "delete?;"; }

int main() {
    static unsigned char object[12000];
    object_bytes = object;
    const std::string members =
        "clear;"
        "_$_19VISpellEffectSystem@7024,2;"
        "_$_14VIQueryTriList@6128,2;"
        "_$_12VIAtmosphere@1600,2;"
        "_$_16VIParticleSystem@1352,2;"
        "_$_t7VIArray1Z7VIVect3@1340,2;"
        "_$_t7VIArray1Z7VIVect3@1332,2;"
        "_$_t7VIArray1Z7VIPlane@1324,2;"
        "_$_t7VIArray1Z7VIVect3@1312,2;"
        "_$_t7VIArray1Z11VIClipEntry@1300,2;"
        "_$_10VIMatrix44@1232,2;"
        "_$_10VIMatrix44@1168,2;"
        "_$_t8VIVector1Z18VISceneDisplayElem@1112,2;"
        "_$_t7VIArray1Zi@1104,2;"
        "_$_t6VIPool1ZP16VIStaticLighting@1068,2;"
        "_$_t6VIPool1ZP8VIRefMap@1044,2;"
        "_$_t6VIPool1ZP13VIHSpriteAnim@1020,2;"
        "_$_t6VIPool1ZP8VISprite@996,2;"
        "_$_t6VIList1ZQ27VIScene12VISceneOccup@984,2;"
        "_$_t6VIList1Z7VIActor@972,2;"
        "_$_7VIWorld@4,2;"
        ;
    _$_7VIScene(reinterpret_cast<VIScene*>(object), 0);
    if (trace != members) return 1;
    trace.clear();
    _$_7VIScene(reinterpret_cast<VIScene*>(object), 1);
    if (trace != members + "delete;") return 2;
    trace.clear();
    _$_7VIScene(reinterpret_cast<VIScene*>(object), 2);
    if (trace != members) return 3;
    return 0;
}
