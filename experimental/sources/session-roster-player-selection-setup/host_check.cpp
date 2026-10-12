#include "player_selection_setup.cpp"
static int calls;
extern "C" void init__Fv() { ++calls; }
int main() {
    playerSelectionSetup__Fv();
    if (calls != 1) return 1;
    playerSelectionSetup__Fv();
    if (calls != 2) return 2;
    return 0;
}
