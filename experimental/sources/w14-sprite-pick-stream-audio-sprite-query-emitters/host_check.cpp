// Host check only; not a build input.
#include "query.cpp"
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> ev;
static float insKey; static unsigned insEntry[2]; static unsigned char* insMap; static int sizeRet, beginRet = 777, eraseIt, eraseCalls;
static unsigned char* eraseMap; static unsigned char* containsBox; static float containsPoint[3]; static int containsRet;
static float magSeen[3]; static unsigned char* invIn; static unsigned char* copyFrom; static int dtors;
extern "C" int Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(unsigned char* m, const float* k, const unsigned* e) { ev.push_back("insert"); insMap = m; insKey = *k; insEntry[0] = e[0]; insEntry[1] = e[1]; return 0; }
extern "C" int Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry(unsigned char*) { ev.push_back("size"); return sizeRet; }
extern "C" int Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(unsigned char*) { ev.push_back("begin"); return beginRet; }
extern "C" int Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(unsigned char* m, int it) { ev.push_back("erase"); eraseMap = m; eraseIt = it; ++eraseCalls; return 0; }
extern "C" void InvertAffine__10VIMatrix44RC10VIMatrix44(unsigned char* out, unsigned char* in) {
    ev.push_back("invert"); invIn = in;
    float* o = reinterpret_cast<float*>(out); const float* s = reinterpret_cast<const float*>(in);
    for (int i = 0; i < 16; ++i) o[i] = s[i] * 0.5f + (i >= 12 ? static_cast<float>(i) : 0.0f);
}
extern "C" int Contains__C6VIBBoxRC7VIVect3(unsigned char* box, unsigned char* p) {
    ev.push_back("contains"); containsBox = box; std::memcpy(containsPoint, p, 12); return containsRet;
}
extern "C" float Magnitude__C7VIVect3(unsigned char* v) {
    ev.push_back("magnitude"); std::memcpy(magSeen, v, 12);
    return __builtin_sqrtf(magSeen[0] * magSeen[0] + magSeen[1] * magSeen[1] + magSeen[2] * magSeen[2]);
}
extern "C" void sceVu0CopyMatrix(unsigned char* to, const unsigned char* from) { ev.push_back("copy"); copyFrom = const_cast<unsigned char*>(from); std::memcpy(to, from, 64); }
extern "C" void _$_10VIMatrix44(unsigned char*, int f) { ev.push_back("dtor"); if (f == 2) ++dtors; }
int main() {
    alignas(16) unsigned char self[128]; alignas(16) float mat[16]; alignas(16) unsigned char query[96]; unsigned char scene[8];
    for (int match = 0; match < 2; ++match) for (int cont = 0; cont < 2; ++cont) for (int mode = -1; mode <= 4; ++mode) for (int lim = 0; lim < 3; ++lim) {
        std::memset(self, 0, sizeof self); std::memset(query, 0, sizeof query);
        for (int i = 0; i < 16; ++i) mat[i] = 0.25f * (i + 1);
        float* box = reinterpret_cast<float*>(self + 8);
        const float boxv[6] = {-2, -1, 0, 4, 5, 8};
        for (int i = 0; i < 6; ++i) box[i] = boxv[i];
        *reinterpret_cast<int*>(self + 52) = 9; *reinterpret_cast<int*>(self + 68) = mode;
        *reinterpret_cast<int*>(query) = match ? 9 : 10; *reinterpret_cast<int*>(query + 4) = lim;
        *reinterpret_cast<unsigned*>(query + 8) = 0xaaa0u + lim; *reinterpret_cast<unsigned*>(query + 12) = 0xbbb0u + mode;
        ev.clear(); sizeRet = 2; eraseCalls = 0; dtors = 0; containsRet = cont; insKey = -99; insMap = nullptr;
        int got = QueryEmitters__19VIStreamAudioSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery(self, scene, reinterpret_cast<unsigned char*>(mat), query);
        if (!match) { if (got != 0 || !ev.empty()) return 1; continue; }
        // the inverse stub halves the matrix and adds the index to floats 12..14 of the output
        const float px = mat[12] * 0.5f + 12, py = mat[13] * 0.5f + 13, pz = mat[14] * 0.5f + 14;
        if (invIn != reinterpret_cast<unsigned char*>(mat)) return 2;
        if (!cont) {
            if (got != 0 || ev.size() != 4 || ev[0] != "invert" || ev[1] != "contains" || ev[2] != "dtor" || ev[3] != "dtor" || dtors != 2) return 3;
            if (containsBox != self + 8 || containsPoint[0] != px || containsPoint[1] != py || containsPoint[2] != pz) return 4;
            continue;
        }
        if (mode < 0 || mode > 2) {
            if (got != -1 || ev.size() != 4 || ev[2] != "dtor" || ev[3] != "dtor" || dtors != 2) return 5;
            continue;
        }
        if (got != 0) return 6;
        const float scale = mode == 0 ? 1.0f : mode == 1 ? 2.0f : 3.0f;
        const float qx = px - (boxv[0] + boxv[3]) * 0.5f, qy = py - (boxv[1] + boxv[4]) * 0.5f, qz = pz - (boxv[2] + boxv[5]) * 0.5f;
        if (magSeen[0] != qx || magSeen[1] != qy || magSeen[2] != qz) return 7;
        const float mag = __builtin_sqrtf(qx * qx + qy * qy + qz * qz);
        if (insKey != scale - mag * 0.0001f) return 8;
        if (insEntry[0] != 0xaaa0u + lim || insEntry[1] != 0xbbb0u + static_cast<unsigned>(mode)) return 9;
        if (insMap != query + 16 || copyFrom != reinterpret_cast<unsigned char*>(mat)) return 10;
        const bool erases = lim < 2;
        std::string order; for (auto& e : ev) order += e + ",";
        if (order != std::string("invert,contains,magnitude,copy,insert,size,") + (erases ? "begin,erase," : "") + "dtor,dtor,") return 11;
        if (erases && (eraseIt != 777 || eraseMap != query + 16)) return 12;
        if (dtors != 2) return 13;
    }
    return 0;
}
