// Host check only; not a build input.
#include "set_hair.cpp"
#include <cstring>
#include <vector>
struct Call { int index, id, tex; };
static std::vector<Call> calls; static int hair_arg, hair_ret, last_entry;
int norrath_hair_null;
extern "C" {
int GetHairTexture__13VICSpriteCusti(int i) { hair_arg = i; return hair_ret; }
void* GetMaterialPalEntry__8VIRasterii(void*, int, int index) { last_entry = index; return nullptr; }
void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void*, int id, int tex, void*, void*) { calls.push_back({last_entry, id, tex}); }
}
static char sprite[8192], scene[20000];
static void setw(int off, int v) { *(int*)(sprite + off) = v; }
static void reset() {
    std::memset(sprite, 0, sizeof sprite); calls.clear();
    *(void**)(scene + 19356) = scene; *(void**)(scene + 19352) = scene + 8;
    for (int k = 0; k < 3; k++) for (int base : {4092, 4284, 4164}) { setw(base + 24 * k, 100 + base + k); setw(base + 4 + 24 * k, base + k); }
    setw(4236, 7); setw(4240, 8); setw(4260, 9); setw(4264, 10); setw(4420, -1);
}
int main() {
    reset(); norrath_hair_null = 5; hair_ret = 40;
    *(void**)(scene + 19356) = nullptr;
    if (SetHair__9VICSpriteiiiR7VIScene(sprite, 1, 2, 2, scene) != -1 || !calls.empty()) return 1;
    reset(); *(void**)(scene + 19352) = nullptr;
    if (SetHair__9VICSpriteiiiR7VIScene(sprite, 1, 2, 2, scene) != -1) return 2;
    reset(); setw(4212, -1);
    if (SetHair__9VICSpriteiiiR7VIScene(sprite, 1, 2, 2, scene) != 0 || !calls.empty() || *(int*)(sprite + 4352) != 0) return 3;
    reset(); hair_ret = -1; norrath_hair_null = -2;
    if (SetHair__9VICSpriteiiiR7VIScene(sprite, 1, 2, 2, scene) != 0 || !calls.empty() || *(int*)(sprite + 4352) != 1) return 4;
    reset(); hair_ret = 40; norrath_hair_null = 5;
    if (SetHair__9VICSpriteiiiR7VIScene(sprite, 12, 3, 1, scene) != 0 || hair_arg != 7) return 5;
    if (*(int*)(sprite + 4356) != 3 || *(int*)(sprite + 4360) != 1) return 6;
    if (calls.size() != 3 + 3 + 3 + 1 + 1) return 7;
    // first group: texture only for k < c (c = 1)
    if (calls[0].tex != 40 || calls[1].tex != 5 || calls[2].tex != 5) return 8;
    // second group needs this[4420] == -1 as well as k < c
    if (calls[3].tex != 40 || calls[4].tex != 5) return 9;
    // third group follows b = 3
    if (calls[6].tex != 40 || calls[7].tex != 40 || calls[8].tex != 40) return 10;
    // fourth slot needs c >= 2: not met here; the last slot always takes the texture
    if (calls[9].tex != 5 || calls[10].tex != 40 || calls[10].index != 9 || calls[10].id != 10) return 11;
    reset(); setw(4420, 0); setw(4092, -1); setw(4284, -1);
    SetHair__9VICSpriteiiiR7VIScene(sprite, -4, 3, 3, scene);
    if (hair_arg != 0 || calls.size() != 9) return 12;
    if (calls[0].index != 100 + 4092 + 1) return 13;
    if (calls[3].tex != 5 || calls[7].tex != 5) return 14;
    return 0;
}
