#include <cstdio>
#include <cstring>
#include <vector>

#include "run.cpp"

extern "C" int world = 0;
extern "C" char levelName[200];
char levelName[200];

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static std::vector<int> order;
static int move_index, move_extra, removed_calls;
static void* move_world;
static Point3 move_point;
static void* removed_self;
static short position_at_move;
static unsigned char* current_object;
static const char* find_text(const char* haystack, const char* needle) {
    const unsigned n = static_cast<unsigned>(std::strlen(needle));
    for (; *haystack; ++haystack)
        if (std::memcmp(haystack, needle, n) == 0) return haystack;
    return nullptr;
}
extern "C" char* strstr(const char* haystack, const char* needle) {
    return const_cast<char*>(find_text(haystack, needle));
}
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* w, int index, Point3 point, short extra) {
    order.push_back(1);
    move_world = w;
    move_index = index;
    move_point = point;
    move_extra = extra;
    std::memcpy(&position_at_move, current_object + 80, 2);
    return 0;
}
extern "C" void objectRemoveFromRunList__FP10GameObject(void* object) {
    order.push_back(2);
    ++removed_calls;
    removed_self = object;
}

int main() {
    alignas(8) unsigned char object[128];
    struct Case { short position; float velocity; const char* level; signed char sign; short door; };
    const Case cases[] = {
        {100, 3.0f, "gothic", 1, 4},      {17000, 400.0f, "gothic", -1, 0},  {17385, 5.0f, "gothic", 1, 2},
        {17400, -5.0f, "gothic", 1, 2},   {17400, 0.0f, "gothic", 3, 2},     {20000, 5.0f, "cellar1", 1, 7},
        {25400, 100.0f, "cellar1", 1, 1}, {25469, 2.0f, "cellar1", -2, 1},   {25469, 2.0f, "gothic", 1, 1},
        {-500, -3.0f, "x", 1, 3},         {32000, 4000.0f, "", 1, 3},        {0, 0.5f, "xgothicx", 1, 6},
        {10, -0.9f, "cellar", 1, 8},      {10, 1.99f, "cellar", 1, 8},       {-30000, -2000.0f, "", -128, 9},
    };
    for (const Case& c : cases) {
        std::memset(object, 0x22, sizeof object);
        std::memcpy(object + 80, &c.position, 2);
        std::memcpy(object + 84, &c.velocity, 4);
        std::memcpy(object + 76, &c.door, 2);
        object[88] = static_cast<unsigned char>(c.sign);
        std::memset(levelName, 'X', sizeof levelName);
        std::strcpy(levelName + 81, c.level);
        current_object = object;
        world = 4660;
        order.clear();
        removed_calls = 0;
        msg_run__9DoorSwing(object);

        // independent model of the observed arithmetic
        const float damping = 0.98f;
        short position = static_cast<short>(static_cast<int>(static_cast<float>(c.position) + c.velocity));
        float velocity = c.velocity * damping;
        const bool gothic = find_text(c.level, "gothic") != nullptr;
        if (gothic) {
            if (position >= 17385 && velocity > 0.0f) velocity = -velocity * 0.1f;
        } else {
            if (position >= 25469 && velocity > 0.0f) velocity = -velocity * 0.74f;
        }
        short got_position;
        float got_velocity;
        std::memcpy(&got_position, object + 80, 2);
        std::memcpy(&got_velocity, object + 84, 4);
        expect(got_position == position, "position is the truncated sum");
        float expected_velocity = velocity;
        // the constants are bit patterns; compare to the tolerance of one part in a million
        float diff = got_velocity - expected_velocity;
        if (diff < 0) diff = -diff;
        float scale = expected_velocity < 0 ? -expected_velocity : expected_velocity;
        expect(diff <= scale * 1e-6f, "velocity after damping and bounce");
        expect(move_world == reinterpret_cast<void*>(4660) && move_index == c.door, "moves the door at +76");
        expect(move_point.x == 0.0f && move_point.y == 0.0f && move_point.z == 0.0f, "origin point");
        expect(move_extra == static_cast<short>(static_cast<unsigned short>(position) * c.sign), "step is position times the signed byte");
        expect(position_at_move == position, "position stored before the move");
        const bool slow = (got_velocity < 0 ? -got_velocity : got_velocity) < 1.0f;
        if (slow) {
            expect(order.size() == 2 && order[0] == 1 && order[1] == 2 && removed_self == object, "slow: removed from the run list after the move");
        } else {
            expect(order.size() == 1 && removed_calls == 0, "fast: stays on the run list");
        }
        expect(object[89] == 0x22 && object[79] == 0x22, "neighbouring bytes untouched");
    }
    return failures ? 1 : 0;
}
