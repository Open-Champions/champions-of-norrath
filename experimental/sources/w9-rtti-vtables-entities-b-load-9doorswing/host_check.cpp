#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <vector>

#include "load.cpp"

extern "C" int world = 0;
static std::vector<int> order;
static int collision_index, collision_flag, move_index, move_extra;
static void* collision_world;
static void* move_world;
static Point3 move_point;
static unsigned char* current_object;
static unsigned flags_at_collision, flags_at_run;
extern "C" void worldSetDoorCollision__FP12_worldHeaderii(void* w, int index, int enable) {
    order.push_back(1);
    collision_world = w;
    collision_index = index;
    collision_flag = enable;
    std::memcpy(&flags_at_collision, current_object + 12, 4);
}
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    order.push_back(3);
    std::memcpy(&flags_at_run, static_cast<unsigned char*>(object) + 12, 4);
    world = 999;  // a later world read must see the new value
}
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* w, int index, Point3 point, short extra) {
    order.push_back(2);
    move_world = w;
    move_index = index;
    move_point = point;
    move_extra = extra;
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
    struct Case { short moved; short second; float velocity; short door; };
    const Case cases[] = {{0, 0, 1.5f, 2}, {1, 1, -2.25f, 5}, {300, 3, 0.0f, -1}, {-7, 0x1f4, 8.0f, 0}, {-32768, -1, 1.0f, 9},
                          {200, 0x80, 3.5f, 4}, {0, 0x7f, 0.5f, 1}};
    for (const Case& c : cases) {
        std::memset(object, 0x22, sizeof object);
        std::memcpy(object + 76, &c.door, 2);
        unsigned flags = 0xfff0u;
        std::memcpy(object + 12, &flags, 4);
        current_object = object;
        world = 4660;
        alignas(4) char stream[12];
        std::memset(stream, 0x66, sizeof stream);
        std::memcpy(stream, &c.moved, 2);
        std::memcpy(stream + 2, &c.second, 2);
        std::memcpy(stream + 4, &c.velocity, 4);
        order.clear();
        int used = msg_load__9DoorSwingPc(object, stream);
        expect(used == 8, "consumes eight bytes");
        short moved;
        float velocity;
        std::memcpy(&moved, object + 80, 2);
        std::memcpy(&velocity, object + 84, 4);
        expect(moved == c.moved, "halfword stored at +80");
        expect(object[88] == static_cast<unsigned char>(c.second), "low byte of the second halfword stored at +88");
        expect(velocity == c.velocity, "float stored at +84");
        expect(object[82] == 0x22 && object[83] == 0x22 && object[89] == 0x22, "neighbouring bytes untouched");
        std::memcpy(&flags, object + 12, 4);
        const int step = static_cast<short>(static_cast<unsigned short>(c.moved) * static_cast<signed char>(c.second & 0xff));
        if (c.moved != 0) {
            expect(order.size() == 3 && order[0] == 1 && order[1] == 3 && order[2] == 2, "collision off, run list, then move");
            expect(collision_world == reinterpret_cast<void*>(4660), "collision call sees the world pointer");
            expect(collision_flag == 0 && collision_index == c.door, "door index from +76, collision flag 0");
            expect(flags_at_collision == 0xfff0u && flags_at_run == 0xfff1u && flags == 0xfff1u, "bit 20 cleared, bit 0 set before the run-list call");
            expect(move_world == reinterpret_cast<void*>(999), "the move call re-reads the world word");
        } else {
            expect(order.size() == 1 && order[0] == 2 && flags == 0xfff0u, "moved is zero: only the move");
            expect(move_world == reinterpret_cast<void*>(4660), "world pointer passed to the move");
        }
        expect(move_index == c.door && move_point.x == 0.0f && move_point.y == 0.0f && move_point.z == 0.0f, "origin point and door index");
        expect(move_extra == step, "step is the +80 halfword times the signed +88 byte");
    }
    return failures ? 1 : 0;
}
