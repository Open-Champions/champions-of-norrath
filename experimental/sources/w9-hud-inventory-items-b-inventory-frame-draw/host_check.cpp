#include <cstring>
#include <cstdio>
struct Point3 { float x, y, z; };
struct Matrix34 { float m[12]; };
extern "C" {
void* inventoryLump;
int inventoryDrawDelay;
int localNmPlayersAlive;
signed char interfaceOn;
unsigned char inventory[464] __attribute__((aligned(4)));
unsigned char animState[264 * 4];
void* invHeader = (void*)0x11;
void* marble = (void*)0x22;
void* stage = (void*)0x33;
void* bevel = (void*)0x44;
void inventoryFrameDraw__Fv();
}

static char out[2048];
static void ev(const char* f, long a = 0, long b = 0, long c = 0, long d = 0, long e = 0) {
    char t[160]; std::snprintf(t, sizeof t, f, a, b, c, d, e); std::strcat(out, t); std::strcat(out, ";");
}
static int matrixOps;
static Matrix34* lastMatrix;

extern "C" int inventoryLoad__Fv() { ev("LOAD"); return 0; }
extern "C" void lightSetSceneLights__FG6Point3iiiiiiiii(Point3 p, int a, int b, int c, int d, int e, int f, int g, int h, int i) {
    char t[200]; std::snprintf(t, sizeof t, "LIGHT(%g,%g,%g,%d,%d,%d,%d,%d,%d,%d,%d,%d);", p.x, p.y, p.z, a, b, c, d, e, f, g, h, i); std::strcat(out, t);
}
extern "C" void matIdent__FP8Matrix34(Matrix34* m) { for (float& f : m->m) f = 0; m->m[0] = m->m[5] = m->m[10] = 1; lastMatrix = m; matrixOps = 0; ev("IDENT"); }
extern "C" void matRot__FP8Matrix34is(Matrix34* m, int axis, short ang) { if (m != lastMatrix) ev("BADM"); ++matrixOps; ev("ROT(%d,%d)", axis, ang); }
extern "C" void worldSetViewMatricies__FP6Point3P8Matrix34(Point3* p, Matrix34* m) {
    char t[100]; std::snprintf(t, sizeof t, "VIEW(%g,%g,%g,%d,%d);", p->x, p->y, p->z, m == lastMatrix, matrixOps); std::strcat(out, t);
}
extern "C" void inventoryDrawPlayer__FiiP14AnimationStatei(int p, int a, void* s, int b) { ev("PLAYER(%d,%d,%d,%d)", p, a, (long)((unsigned char*)s - animState), b); }
extern "C" void inventoryDrawPlayerColor__Fiii(int p, int a, int b) { ev("PCOLOR(%d,%d,%d)", p, a, b); }
extern "C" void textureDrawColor__FP8_textureiiiUi(void* t, int a, int b, int c, unsigned col) { ev("TEX(%d,%d,%d,%d,%x)", (long)t, a, b, c, col); }
extern "C" void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* t, int a, int b, int c, unsigned col, int d, int e, int f, int g, char h, char i) {
    char s[160]; std::snprintf(s, sizeof s, "SUPER(%ld,%d,%d,%d,%x,%d,%d,%d,%d,%d,%d);", (long)t, a, b, c, col, d, e, f, g, h, i); std::strcat(out, s);
}
extern "C" void inventoryDraw__Fi(int i) { ev("DRAW(%d)", i); }

static int& word(int r, int off) { return *reinterpret_cast<int*>(inventory + r * 116 + off); }
#define VIEW "IDENT;ROT(0,16384);ROT(2,0);VIEW(0,-108,0,1,2);"
#define LIGHT "LIGHT(-0.5,-0.5,0.5,101,107,78,0,38,40,48,0,128);"

static bool is(const char* expect) { if (std::strcmp(out, expect) == 0) return true; std::printf("got  %s\nwant %s\n", out, expect); return false; }

int main() {
    int lump;
    word(0, 52) = 3; word(1, 52) = 1;
    // lump not loaded
    out[0] = 0; inventoryLump = nullptr; inventoryDrawDelay = 5; localNmPlayersAlive = 1;
    inventoryFrameDraw__Fv();
    if (!is("LOAD;") || inventoryDrawDelay != 5) return 1;
    // delay running down
    out[0] = 0; inventoryLump = &lump;
    inventoryFrameDraw__Fv();
    if (!is("") || inventoryDrawDelay != 4) return 2;
    inventoryDrawDelay = 0;
    // one player
    out[0] = 0; localNmPlayersAlive = 1; word(0, 76) = 2; word(1, 76) = 2;
    inventoryFrameDraw__Fv();
    if (!is(LIGHT VIEW "PLAYER(3,1,792,0);TEX(17,0,0,0,80808080);TEX(34,0,105,0,80808080);TEX(51,319,0,0,80808080);DRAW(0);")) return 3;
    // two players, neither in mode 2
    out[0] = 0; localNmPlayersAlive = 2; word(0, 76) = 0; word(1, 76) = 1; interfaceOn = 0;
    inventoryFrameDraw__Fv();
    if (!is(LIGHT "PCOLOR(3,0,0);PCOLOR(1,1,0);TEX(17,320,0,0,80808080);TEX(34,320,105,0,80808080);"
                  "TEX(17,0,0,0,80808080);TEX(34,0,105,0,80808080);TEX(68,314,0,6,80808080);" VIEW "DRAW(1);")) return 4;
    // two players, interface on
    out[0] = 0; interfaceOn = 1; localNmPlayersAlive = 0;
    inventoryFrameDraw__Fv();
    if (std::strstr(out, "DRAW(1);DRAW(0);") == nullptr || std::strstr(out, "PCOLOR(1,1,0)") == nullptr) return 5;
    // record 1 in mode 2
    out[0] = 0; localNmPlayersAlive = 3; word(0, 76) = 0; word(1, 76) = 2; interfaceOn = 0;
    inventoryFrameDraw__Fv();
    if (!is(LIGHT VIEW "PCOLOR(3,0,0);PLAYER(3,1,792,0);TEX(51,319,0,0,80808080);"
                  "TEX(17,0,0,0,80808080);TEX(34,0,105,0,80808080);" VIEW "DRAW(1);")) return 6;
    // record 0 in mode 2
    out[0] = 0; word(0, 76) = 2; word(1, 76) = 0;
    inventoryFrameDraw__Fv();
    if (!is(LIGHT "PCOLOR(3,0,0);PCOLOR(1,1,0);TEX(17,320,0,0,80808080);TEX(34,320,105,0,80808080);"
                  VIEW "PLAYER(1,2,264,1);SUPER(51,0,1,0,80808080,0,0,640,512,1,0);" VIEW "DRAW(1);")) return 7;
    // both in mode 2
    out[0] = 0; word(0, 76) = 2; word(1, 76) = 2;
    inventoryFrameDraw__Fv();
    if (!is(LIGHT VIEW "PCOLOR(3,0,0);PLAYER(3,1,792,0);TEX(51,319,0,0,80808080);"
                  VIEW "PLAYER(1,2,264,1);SUPER(51,0,1,0,80808080,0,0,640,512,1,0);" VIEW "DRAW(1);")) return 8;
    return 0;
}
