// Host contract for gameSetObjectVis__Fv. Not an EE plan input.
#include "set_object_vis.cpp"
extern "C" {
signed char netNetworkMode, cutMode;
int nmPlayers;
alignas(16) unsigned char player[4 * 23984];
unsigned char* gameCam;
unsigned char objectVisUseCamCenter = 9;
signed char objectVisCount = 77;
ObjectVisPoint objectVisPoints[4];
}
static unsigned char* dialog;
static int dialogCalls;
extern "C" unsigned char* DLG_GetDialogCreature__Fv() { ++dialogCalls; return dialog; }
alignas(16) static unsigned char cam[64], objs[4][32];
static void obj(int slot, float x, float y, float z, bool present = true) {
    *reinterpret_cast<unsigned char**>(player + slot * 23984 + 21872) = present ? objs[slot] : nullptr;
    float* f = reinterpret_cast<float*>(objs[slot] + 16); f[0] = x; f[1] = y; f[2] = z;
}
int main() {
    gameCam = cam;
    for (int i = 0; i < 4; ++i) obj(i, i + 1.f, i + 2.f, i + 3.f);
    nmPlayers = 3;
    // dialog creature present
    alignas(16) static unsigned char dc[32]; float* df = reinterpret_cast<float*>(dc + 16); df[0] = 7; df[1] = 8; df[2] = 9;
    dialog = dc; dialogCalls = 0;
    gameSetObjectVis__Fv();
    if (dialogCalls != 2 || objectVisUseCamCenter != 0 || objectVisCount != 1 ||
        objectVisPoints[0].x != 7 || objectVisPoints[0].y != 8 || objectVisPoints[0].z != 9) return 1;
    dialog = nullptr;
    // offline, camera word is 1 -> flag 0; points from players
    *reinterpret_cast<int*>(cam + 52) = 1; netNetworkMode = 0; cutMode = 5;
    obj(1, 0, 0, 0, false);
    gameSetObjectVis__Fv();
    if (objectVisUseCamCenter != 0 || objectVisCount != 2) return 2;
    if (objectVisPoints[0].x != 1 || objectVisPoints[1].x != 3 || objectVisPoints[1].z != 5) return 3;
    // camera word not 1 -> flag 1
    *reinterpret_cast<int*>(cam + 52) = 2;
    gameSetObjectVis__Fv();
    if (objectVisUseCamCenter != 1) return 4;
    // networked: depends on cutMode
    netNetworkMode = 1; cutMode = 0; objectVisUseCamCenter = 5;
    gameSetObjectVis__Fv();
    if (objectVisUseCamCenter != 0) return 5;
    cutMode = -3;
    gameSetObjectVis__Fv();
    if (objectVisUseCamCenter != 1) return 6;
    *reinterpret_cast<int*>(cam + 52) = 1;
    gameSetObjectVis__Fv();
    if (objectVisUseCamCenter != 0) return 7;
    // no players: count reset, nothing written
    nmPlayers = 0; objectVisCount = 5; objectVisPoints[0].x = -1;
    gameSetObjectVis__Fv();
    if (objectVisCount != 0 || objectVisPoints[0].x != -1) return 8;
    nmPlayers = -2; objectVisCount = 5;
    gameSetObjectVis__Fv();
    if (objectVisCount != 0) return 9;
    return 0;
}
