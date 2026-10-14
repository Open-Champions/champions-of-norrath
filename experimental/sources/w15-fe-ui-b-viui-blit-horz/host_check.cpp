#include "blit_horz.cpp"
#include <string>
#include <vector>

static int record[9] = {1, 1, 0, 0, 10, 6, 0, 0x3f800000, 0};
static const int* record_ptr = record;
struct Call { bool sized; int at[2], size[2], blend; float alpha; unsigned flags; };
static std::vector<Call> calls;
extern "C" const int* GetUITexture__F9VIUITexId(int) { return record_ptr; }
extern "C" int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(void*, int, VIVect2I at, int blend, float alpha, unsigned flags) {
    calls.push_back({false, {at.x, at.y}, {0, 0}, blend, alpha, flags});
    return 1;
}
extern "C" int BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(void*, int, VIVect2I at, VIVect2I size, int blend, float alpha, unsigned flags) {
    calls.push_back({true, {at.x, at.y}, {size.x, size.y}, blend, alpha, flags});
    return 1;
}
static VIVect2I make(int x, int y) { VIVect2I v; v.x = x; v.y = y; return v; }

int main() {
    char ui[8];
    // tiled: 35 wide with 10 wide tiles: tiles at 100, 110, 120 and a 5 wide remainder at 130
    if (BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(ui, make(100, 200), 5, 35, 3, 0.5f, 1u) != 0) return 1;
    if (calls.size() != 4) return 2;
    for (int i = 0; i < 3; ++i)
        if (calls[i].sized || calls[i].at[0] != 100 + 10 * i || calls[i].at[1] != 200 || calls[i].blend != 3 || calls[i].flags != 1u) return 3;
    if (!calls[3].sized || calls[3].at[0] != 130 || calls[3].at[1] != 200 || calls[3].size[0] != 5 || calls[3].size[1] != 6) return 4;
    // exactly one tile wide: the loop never runs (10 < 10 is false) and the remainder is the full tile
    calls.clear();
    BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(ui, make(0, 0), 5, 10, 0, 1.0f, 0u);
    if (calls.size() != 1 || !calls[0].sized || calls[0].size[0] != 10) return 5;
    // non-positive width: nothing drawn, result 0
    calls.clear();
    if (BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(ui, make(0, 0), 5, 0, 0, 1.0f, 0u) != 0 || !calls.empty()) return 6;
    if (BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(ui, make(0, 0), 5, -4, 0, 1.0f, 0u) != 0 || !calls.empty()) return 7;
    // other sheet: one stretched draw of the requested width
    record[1] = 2;
    if (BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(ui, make(7, 8), 5, 35, 2, 0.25f, 1u) != 0) return 8;
    if (calls.size() != 1 || !calls[0].sized || calls[0].at[0] != 7 || calls[0].at[1] != 8 || calls[0].size[0] != 35 || calls[0].size[1] != 6) return 9;
    // missing record
    calls.clear();
    record_ptr = nullptr;
    if (BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(ui, make(7, 8), 5, 35, 2, 0.25f, 1u) != -1 || !calls.empty()) return 10;
    return 0;
}
