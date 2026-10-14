#include "mc_get_ram_player_ptr.cpp"

#include <cstdio>

unsigned char ramSlot[4];
Camera* gameCam;
static Camera* seen_camera;
static int camera_size = 1000;
static int jump_size = 20;
static int order;
static int camera_order, jump_order;

int GetSaveFootPrint__6Camera(Camera* self) {
    seen_camera = self;
    camera_order = ++order;
    return camera_size;
}
int jumpGateSaveFootPrint__Fv() {
    jump_order = ++order;
    return jump_size;
}

int main() {
    gameCam = reinterpret_cast<Camera*>(0x1234);
    for (int index = -2; index < 7; ++index) {
        unsigned char* got = MC_GetRamPlayerPtr__Fi(index);
        if (seen_camera != gameCam) return 1;
        if (index < 0 || index > 3) {
            if (got != nullptr) return 2;
            continue;
        }
        if (got != ramSlot + 112 + 1000 + 20 + 4104 + 14 + index * 23998) return 3;
    }
    camera_size = 8;
    jump_size = 0;
    if (MC_GetRamPlayerPtr__Fi(2) != ramSlot + 112 + 8 + 4104 + 14 + 2 * 23998) return 4;
    if (camera_order == 0 || jump_order == 0) return 5;
    std::puts("mc get ram player ptr passed");
    return 0;
}
