#include "scene_ctor.cpp"

#include <cstring>
#include <string>

static std::string trace;
static unsigned char* object_bytes;

extern "C" void Clear__7VIScene(VIScene* s) {
    trace += reinterpret_cast<unsigned char*>(s) == object_bytes ? "clear;" : "clear?;";
    // The two stores before Clear have already happened.
    int a, b;
    std::memcpy(&a, object_bytes + 1348, 4);
    std::memcpy(&b, object_bytes + 19356, 4);
    if (a != 1 || b != 0) trace += "early;";
}
#define MEMBER(sym) \
    extern "C" void sym(Member* m) { \
        trace += std::string(#sym) + "@" + std::to_string(reinterpret_cast<unsigned char*>(m) - object_bytes) + ";"; \
    }
MEMBER(__12VIAtmosphere)
MEMBER(__14VIQueryTriList)
MEMBER(__16VIParticleSystem)
MEMBER(__19VIRadialFloraSystem)
MEMBER(__19VISpellEffectSystem)
MEMBER(__7VIWorld)
MEMBER(__t6VIList1Z7VIActor)
MEMBER(__t6VIList1ZQ27VIScene12VISceneOccup)
MEMBER(__t6VIPool1ZP13VIHSpriteAnim)
MEMBER(__t6VIPool1ZP16VIStaticLighting)
MEMBER(__t6VIPool1ZP8VIRefMap)
MEMBER(__t6VIPool1ZP8VISprite)
MEMBER(__t7VIArray1Z11VIClipEntry)
MEMBER(__t7VIArray1Z7VIPlane)
MEMBER(__t7VIArray1Z7VIVect3)
MEMBER(__t7VIArray1Zi)
MEMBER(__t8VIVector1Z18VISceneDisplayElem)

int main() {
    const int size = 19400;
    static unsigned char object[size];
    std::memset(object, 0xa5, size);
    object_bytes = object;
    __7VIScene(reinterpret_cast<VIScene*>(object));
    const std::string want_calls =
        "__7VIWorld@4;"
        "__t6VIList1Z7VIActor@972;"
        "__t6VIList1ZQ27VIScene12VISceneOccup@984;"
        "__t6VIPool1ZP8VISprite@996;"
        "__t6VIPool1ZP13VIHSpriteAnim@1020;"
        "__t6VIPool1ZP8VIRefMap@1044;"
        "__t6VIPool1ZP16VIStaticLighting@1068;"
        "__t7VIArray1Zi@1104;"
        "__t8VIVector1Z18VISceneDisplayElem@1112;"
        "__t7VIArray1Z11VIClipEntry@1300;"
        "__t7VIArray1Z7VIVect3@1312;"
        "__t7VIArray1Z7VIPlane@1324;"
        "__t7VIArray1Z7VIVect3@1332;"
        "__t7VIArray1Z7VIVect3@1340;"
        "__16VIParticleSystem@1352;"
        "__12VIAtmosphere@1600;"
        "__14VIQueryTriList@6128;"
        "__19VISpellEffectSystem@7024;"
        "__19VIRadialFloraSystem@10876;"
        "clear;";
    if (trace != want_calls) return 1;

    static unsigned char want[size];
    std::memset(want, 0xa5, size);
    auto zero = [&](int at, int n) { std::memset(want + at, 0, n); };
    auto set32 = [&](int at, int v) { std::memcpy(want + at, &v, 4); };
    zero(1168, 64);
    zero(1232, 64);
    zero(8164, 16);
    zero(8180, 24 * 108);
    zero(10792, 16);
    zero(10776, 12);
    set32(1348, 1);
    set32(19356, 0);
    for (int i = 0; i < size; ++i)
        if (object[i] != want[i]) return 2;
    return 0;
}
