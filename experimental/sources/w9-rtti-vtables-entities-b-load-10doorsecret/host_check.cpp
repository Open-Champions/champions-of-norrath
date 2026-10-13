#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <vector>

#include "load.cpp"

extern "C" int world = 0;
static std::vector<int> order;
static void* collision_world;
static int collision_index, collision_flag, move_index, move_extra;
static void* move_world;
static Point3 move_point;
static unsigned char* current_object;
static unsigned flags_at_move;
extern "C" void worldSetDoorCollision__FP12_worldHeaderii(void* w, int index, int enable) {
    order.push_back(1);
    collision_world = w;
    collision_index = index;
    collision_flag = enable;
}
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* w, int index, Point3 point, short extra) {
    order.push_back(2);
    move_world = w;
    move_index = index;
    move_point = point;
    move_extra = extra;
    std::memcpy(&flags_at_move, current_object + 12, 4);
    return 0;
}

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    alignas(8) unsigned char object[128];
    for (int value : {0, 1, 5, 0x7f, 0x80, 0xff, -1}) {
        for (short door : {short(0), short(3), short(-2)}) {
            std::memset(object, 0x22, sizeof object);
            std::memcpy(object + 76, &door, 2);
            unsigned flags = 0xffffffffu;
            std::memcpy(object + 12, &flags, 4);
            current_object = object;
            world = 4660;
            char stream[4] = {static_cast<char>(value), 0x55, 0x66, 0x77};
            order.clear();
            int used = msg_load__10DoorSecretPc(object, stream);
            expect(used == 1, "consumes one byte");
            expect(object[88] == static_cast<unsigned char>(value), "stores the byte at +88");
            std::memcpy(&flags, object + 12, 4);
            if (static_cast<signed char>(value) != 0) {
                expect(order.size() == 2 && order[0] == 1 && order[1] == 2, "collision off, then move");
                expect(collision_world == reinterpret_cast<void*>(4660) && move_world == collision_world, "world pointer is passed");
                expect(collision_index == door && move_index == door && collision_flag == 0, "door index from +76, collision flag 0");
                expect(move_point.x == 0.0f && move_point.y == 0.0f && move_point.z == -300.0f && move_extra == 0, "moves by (0, 0, -300) with extra 0");
                expect(flags == 0xffefffffu && flags_at_move == 0xffffffffu, "bit 20 is cleared after the move");
            } else {
                expect(order.empty() && flags == 0xffffffffu, "zero byte: nothing happens");
            }
            expect(object[87] == 0x22 && object[89] == 0x22, "neighbours untouched");
        }
    }
    return failures ? 1 : 0;
}
