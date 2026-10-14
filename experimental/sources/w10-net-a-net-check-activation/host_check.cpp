// Host check for netCheckActivation. Not an EE plan input.
#define NORRATH_W10NA_HOST 1
#include "net_check_activation.cpp"
#include <cstdint>
#include <cstring>
#include <vector>
static std::vector<unsigned char*> table;
extern "C" const unsigned char* norrath_w10na_word_to_pointer(unsigned word) { return table[word]; }
int nmPlayers;
unsigned char player[4 * 23984];
static void put(int i, unsigned word) { std::memcpy(player + i * 23984 + 21872, &word, 4); }
int main() {
    alignas(8) static unsigned char objs[4][64];
    table = {0, objs[0], objs[1], objs[2], objs[3]};
    Point3 p = {10.0f, 20.0f, 30.0f};
    nmPlayers = 0;
    if (netCheckActivation__FG6Point3f(&p, 1000.0f) != 0) return 1;
    nmPlayers = 3;
    for (int i = 0; i < 4; ++i) put(i, 0);
    if (netCheckActivation__FG6Point3f(&p, 1000.0f) != 0) return 2;  // all null
    Point3 q = {13.0f, 24.0f, 30.0f};                                // distance 5
    std::memcpy(objs[1] + 16, &q, sizeof q);
    put(1, 2);
    if (netCheckActivation__FG6Point3f(&p, 5.1f) != 1) return 3;
    if (netCheckActivation__FG6Point3f(&p, 5.0f) != 0) return 4;      // strict compare
    if (netCheckActivation__FG6Point3f(&p, -5.1f) != 1) return 5;     // squared range
    put(1, 0);
    put(3, 4);  // index 3 is beyond nmPlayers
    std::memcpy(objs[3] + 16, &q, sizeof q);
    if (netCheckActivation__FG6Point3f(&p, 100.0f) != 0) return 6;
    nmPlayers = 4;
    if (netCheckActivation__FG6Point3f(&p, 100.0f) != 1) return 7;
    Point3 z = {10.0f, 20.0f, 33.0f};                                 // z contributes
    std::memcpy(objs[3] + 16, &z, sizeof z);
    if (netCheckActivation__FG6Point3f(&p, 3.5f) != 1 || netCheckActivation__FG6Point3f(&p, 2.5f) != 0) return 8;
    nmPlayers = -2;
    if (netCheckActivation__FG6Point3f(&p, 100.0f) != 0) return 9;
    return 0;
}
