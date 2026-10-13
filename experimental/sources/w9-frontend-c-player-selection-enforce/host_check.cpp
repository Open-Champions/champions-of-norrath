#include "player_selection_enforce.cpp"
#include <cstring>
int needEnforce;
int nmPlayers;
signed char playerSelectInfo[4 * 41];
signed char targets[10 * 636];
struct Call { int index, second; char* name; int b0, b1, b2, b3; char* tint; };
static Call calls[8];
static int ncalls;
void playerInit__FiiPcScScScScT2(int index, int second, char* name, signed char b0, signed char b1,
                                 signed char b2, signed char b3, char* tint) {
    calls[ncalls++] = Call{index, second, name, b0, b1, b2, b3, tint};
}
#define CHECK(c, n) do { if (!(c)) return n; } while (0)
int main() {
    for (int t = 0; t < 10; ++t) {
        targets[t * 636 + 16] = static_cast<signed char>(40 + t);
        targets[t * 636 + 18] = 1;
        targets[t * 636 + 25] = (t % 2) ? 1 : 2;  // odd: equal, even: different
    }
    for (int i = 0; i < 4; ++i) {
        signed char* r = playerSelectInfo + i * 41;
        r[32] = static_cast<signed char>(i + 1);  // targets 1..4
        r[33] = static_cast<signed char>(10 + i);
        r[34] = static_cast<signed char>(20 + i);
        r[35] = static_cast<signed char>(30 + i);
        r[36] = static_cast<signed char>(-5 - i);
    }
    nmPlayers = 3;
    needEnforce = 0;
    playerSelectionEnforce__Fv();
    CHECK(ncalls == 0, 1);
    needEnforce = 1;
    playerSelectionEnforce__Fv();
    CHECK(needEnforce == 0 && ncalls == 3, 2);
    for (int i = 0; i < 3; ++i) {
        const Call& c = calls[i];
        char* rec = reinterpret_cast<char*>(playerSelectInfo + i * 41);
        CHECK(c.index == i && c.second == 40 + i + 1 && c.name == rec && c.tint == rec + 37, 3);
        CHECK(c.b1 == 20 + i && c.b2 == -5 - i && c.b3 == 30 + i, 4);
        // target i+1: odd targets (i == 0, 2) have equal bytes -> -1, even targets keep rec[33]
        CHECK(c.b0 == ((i + 1) % 2 ? -1 : 10 + i), 5);
    }
    ncalls = 0; needEnforce = 1; nmPlayers = 0;
    playerSelectionEnforce__Fv();
    CHECK(ncalls == 0 && needEnforce == 0, 6);
    return 0;
}
