// Host check only; not a build input.
#include "query.cpp"
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> ev;
static float insKey; static unsigned insEntry[2]; static unsigned char* insMap; static int sizeRet, eraseIt, beginRet = 777, eraseCalls;
static unsigned char* eraseMap; static unsigned char* copyFrom; static unsigned dtorFlags;
extern "C" int Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(unsigned char* m, const float* k, const unsigned* e) { ev.push_back("insert"); insMap = m; insKey = *k; insEntry[0] = e[0]; insEntry[1] = e[1]; return 0; }
extern "C" int Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry(unsigned char*) { ev.push_back("size"); return sizeRet; }
extern "C" int Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(unsigned char*) { ev.push_back("begin"); return beginRet; }
extern "C" int Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(unsigned char* m, int it) { ev.push_back("erase"); eraseMap = m; eraseIt = it; ++eraseCalls; return 0; }
extern "C" void sceVu0CopyMatrix(unsigned char* to, const unsigned char* from) { ev.push_back("copy"); copyFrom = const_cast<unsigned char*>(from); std::memcpy(to, from, 64); }
extern "C" void _$_10VIMatrix44(unsigned char* m, int f) { ev.push_back("dtor"); dtorFlags = f; (void)m; }
int main() {
    alignas(16) unsigned char self[96]; alignas(16) float mat[16]; alignas(16) unsigned char query[96]; unsigned char scene[8];
    const float mats[][4] = {{3, 4, 12, 0.5f}, {0.25f, 1, 2, 1}, {5, 5, 5, 2}};
    for (int m = 0; m < 3; ++m) for (int match = 0; match < 2; ++match) for (int lim = 0; lim < 3; ++lim) {
        std::memset(self, 0, sizeof self); std::memset(query, 0, sizeof query);
        for (int i = 0; i < 16; ++i) mat[i] = 0.1f * (i + 1);
        mat[0] = mats[m][3]; mat[1] = 0.5f; mat[2] = 1.5f; mat[12] = mats[m][0]; mat[13] = mats[m][1]; mat[14] = mats[m][2];
        *reinterpret_cast<int*>(self + 52) = 9; *reinterpret_cast<float*>(self + 44) = 7.0f + m;
        *reinterpret_cast<int*>(query) = match ? 9 : 10;
        *reinterpret_cast<int*>(query + 4) = lim;       // limit
        *reinterpret_cast<unsigned*>(query + 8) = 0x1111u + m; *reinterpret_cast<unsigned*>(query + 12) = 0x2222u + lim;
        ev.clear(); sizeRet = 2; eraseCalls = 0; insKey = -1; insMap = nullptr; dtorFlags = 0;
        int got = QueryEmitters__8VISpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery(self, scene, reinterpret_cast<unsigned char*>(mat), query);
        if (got != 0) return 1;
        if (!match) { if (ev.size() != 1 || ev[0] != "dtor" || dtorFlags != 2) return 2; continue; }
        const float d2 = mat[12] * mat[12] + mat[13] * mat[13] + mat[14] * mat[14];
        const float r2 = mat[0] * mat[0] + mat[1] * mat[1] + mat[2] * mat[2];
        const float key = (7.0f + m) * __builtin_sqrtf(r2) * (1.0f / __builtin_sqrtf(d2));
        if (insKey != key) return 3;
        if (insEntry[0] != 0x1111u + m || insEntry[1] != 0x2222u + lim) return 4;
        if (insMap != query + 16) return 5;
        const bool erases = lim < 2;
        std::string order; for (auto& e : ev) order += e + ",";
        if (order != (erases ? "copy,insert,size,begin,erase,dtor," : "copy,insert,size,dtor,")) return 6;
        if (erases && (eraseIt != 777 || eraseMap != query + 16)) return 7;
        if (copyFrom != reinterpret_cast<unsigned char*>(mat) || dtorFlags != 2) return 8;
    }
    return 0;
}
