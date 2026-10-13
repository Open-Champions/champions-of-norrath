// Host check for shopUnpackToGrid__Fiii. Not an EE plan input.
#include "shop_unpack_to_grid.cpp"
#include <cstring>
#include <vector>
unsigned char player[3 * 23984];
unsigned char shopList[3 * 4616];
static std::vector<unsigned char> invMem(2 * 145800 + 64), shopMem(2 * 145800 + 64);
unsigned char* invGrid = invMem.data();
unsigned char* shopGrid = shopMem.data();
static int calls;
static int sizes[16][2];
extern "C" void getPicSize__4itemiPQ24item11descriptionPiT3(int kind, unsigned char* e, int* w, int* h) {
    int i = (e[4]);
    *w = sizes[i][0]; *h = sizes[i][1]; calls += kind == 7;
}
static int& I(unsigned char* p) { return *reinterpret_cast<int*>(p); }
static unsigned short& H(unsigned char* p) { return *reinterpret_cast<unsigned short*>(p); }
static unsigned char* cell(unsigned char* g, int list, int c, int r) { return g + list * 48600 + c * 5400 + r * 72; }
int main() {
    unsigned char* l0 = player + 328;
    I(player + 84) = 7;
    I(l0 + 4608) = 3;
    for (int i = 0; i < 3; i++) { I(l0 + 72 * i) = 100 + i; l0[72 * i + 4] = i; }
    sizes[0][0] = 31; sizes[0][1] = 40;   // 1 column, 2 rows
    sizes[1][0] = 33; sizes[1][1] = 41;   // 2 columns, 2 rows
    sizes[2][0] = 0; sizes[2][1] = 0;     // 1 column, 1 row
    // dirty the grid so clearing is observable
    memset(invMem.data(), 0xee, invMem.size());
    shopUnpackToGrid__Fiii(0, 0, 0);
    if (calls != 3) return 1;
    unsigned char* g = invGrid;
    if (I(cell(g, 0, 0, 0)) != 100 || H(cell(g, 0, 0, 0) + 34) != 0) return 2;
    // item 1 is 2x2: the row-major scan finds row 0 / column 1 first (column 0 is taken)
    for (int r = 0; r <= 1; r++) for (int c = 1; c <= 2; c++)
        if (I(cell(g, 0, c, r)) != 101 || H(cell(g, 0, c, r) + 34) != 1) return 3;
    // item 2 (1x1): first free cell is row 0 column 3
    if (I(cell(g, 0, 3, 0)) != 102 || H(cell(g, 0, 3, 0) + 34) != 2) return 4;
    if (I(cell(g, 0, 4, 0)) != 0 || I(cell(g, 0, 8, 74)) != 0 || I(cell(g, 0, 0, 1)) != 100 || I(cell(g, 0, 0, 2)) != 0) return 5;  // item 0 is two rows tall
    if (H(l0 + 72 * 2 + 34) != 2) return 6;                   // source records get their index
    if (I(cell(g, 1, 0, 0)) != 0 || I(cell(g, 2, 3, 3)) != 0) return 7;  // empty lists clear their grids
    // a record too wide for the nine columns is skipped
    sizes[0][0] = 32 * 9; I(l0 + 4608) = 1;
    shopUnpackToGrid__Fiii(0, 0, 0);
    if (I(cell(g, 0, 0, 0)) != 0) return 8;
    // shop lists use shopGrid / shopList
    I(shopList + 4608) = 1; I(shopList) = 55; shopList[4] = 2;
    memset(shopMem.data(), 0x11, shopMem.size());
    shopUnpackToGrid__Fiii(0, 1, 1);
    if (I(cell(shopGrid + 145800, 0, 0, 0)) != 55) return 9;
    unsigned char before = invMem[5];
    shopUnpackToGrid__Fiii(-1, 0, 0);
    if (invMem[5] != before) return 10;
    return 0;
}
