// Host check for inventoryRun__Fi. Not an EE plan input.
#include "inventory_run.cpp"
#include <string>
unsigned char player[3 * 23984];
unsigned char inventory[232] __attribute__((aligned(4)));
unsigned char netLocalPad[352];
signed char multiHelpMessageOn[4];
unsigned padBackButton = 0x120;
int padAcceptButton = 0x40;
int localNmPlayersAlive = 2, nmPlayers = 3;
static std::string ev;
static int found = -1;
extern "C" {
void invShutDown__Fi(int a) { ev += "S" + std::to_string(a) + ";"; }
int inventoryPadFindButton__Fi(int a) { ev += "F" + std::to_string(a) + ";"; return found; }
void msgInventoryExit__Fiii(int a, int b, int c) { ev += "X" + std::to_string(a) + std::to_string(b) + std::to_string(c) + ";"; }
void displayItems__FiiP12screenStructi(int a, int b, unsigned char* s, int d) { ev += "D" + std::to_string(a) + std::to_string(b) + std::to_string(s == inventory + 116 * a) + std::to_string(d) + ";"; }
void scriptStart__FPc(char* n) { ev += std::string("N") + n + ";"; }
}
static int& W(int screen, int off) { return *reinterpret_cast<int*>(inventory + 116 * screen + off); }
int main() {
    W(0, 76) = 2; W(1, 76) = 2;
    inventoryRun__Fi(1);
    if (ev != "S1;") return 1;
    ev.clear(); W(1, 76) = 0; localNmPlayersAlive = 1; W(0, 76) = 2;
    inventoryRun__Fi(0);
    if (ev != "S0;") return 2;
    // mode 2: accept button picks player 2
    ev.clear(); localNmPlayersAlive = 2; W(0, 52) = 0; W(1, 52) = 1; found = 2;
    player[2 * 23984 + 88] = 0xfb; player[2 * 23984 + 89] = 1;
    inventoryRun__Fi(0);
    if (ev != "F64;X021;") return 3;
    ev.clear(); player[2 * 23984 + 88] = 5; inventoryRun__Fi(0);
    if (ev != "F64;X020;") return 4;
    ev.clear(); found = 1; inventoryRun__Fi(0);
    if (ev != "F64;") return 5;
    // mode 0 with no back button: draw only
    ev.clear(); W(1, 76) = 0; W(0, 76) = 2; W(1, 52) = 1; W(1, 88) = 1;
    inventoryRun__Fi(1);
    if (ev != "D1113;") return 6;
    ev.clear(); *reinterpret_cast<unsigned*>(netLocalPad + 88 + 12) = 0x100;
    inventoryRun__Fi(1);
    if (ev != "D1113;" || W(1, 88) != 0 || W(1, 52) != 1) return 7;
    ev.clear(); W(1, 48) = 5; W(1, 88) = 2; multiHelpMessageOn[1] = 1;
    inventoryRun__Fi(1);
    if (ev != "D1112;" || W(1, 48) != 5) return 8;
    multiHelpMessageOn[1] = 0; W(1, 88) = 0; ev.clear();
    inventoryRun__Fi(1);
    if (ev != "D1112;" || W(1, 48) != 0) return 9;
    *reinterpret_cast<int*>(player + 4936) = 2; *reinterpret_cast<int*>(player + 23984 + 4936) = 1;
    *reinterpret_cast<int*>(player + 2 * 23984 + 4936) = 7;
    ev.clear(); inventoryRun__Fi(1);
    if (ev != "D1112;N*exitInventoryNotify;N*exitInventoryNotify;" || W(1, 52) != -1 || W(1, 76) != 2) return 10;
    W(1, 76) = 9; ev.clear(); inventoryRun__Fi(1);
    if (!ev.empty()) return 11;
    return 0;
}
