// Host contract for msg_draw__7Critter; not an EE input.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_d_host_base;
typedef void (*HostFn)(void*, int);
HostFn w10_actor_d_host_functions[1];
#include "msg_draw.cpp"
static std::string calls; static unsigned char* objectUnderTest; static float zAtDraw;
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* x) {
    std::memcpy(&zAtDraw, objectUnderTest + 24, 4);
    calls += "draw(" + std::to_string(s == objectUnderTest) + "," + std::to_string(static_cast<unsigned char*>(m) - w10_actor_d_host_base) + "," + std::to_string(static_cast<unsigned char*>(t) - w10_actor_d_host_base) + "," + std::to_string(x == nullptr) + ");";
    return nullptr;
}
void modelSetFade__Fc(char v) { calls += "fade(" + std::to_string(int(v)) + ");"; }
void modelResetOptionalParams__Fv() { calls += "reset;"; }
int main() {
    alignas(16) static unsigned char arena[4096], object[1100];
    w10_actor_d_host_base = arena; objectUnderTest = object;
    int bad = 0;
    struct Case { short state; float lift; std::string want; } cases[] = {
        {0, 10.0f, "draw(1,1000,2000,1);"}, {13, 300.0f, "draw(1,1000,2000,1);"}, {15, 10.0f, "draw(1,1000,2000,1);"}, {-14, 10.0f, "draw(1,1000,2000,1);"},
        {14, 100.0f, "fade(50);draw(1,1000,2000,1);reset;"}, {16, 100.0f, "fade(50);draw(1,1000,2000,1);reset;"},
        {14, 195.0f, "fade(97);draw(1,1000,2000,1);reset;"}, {14, 196.0f, "fade(98);reset;"}, {16, 255.0f, "fade(127);reset;"},
        {16, 300.0f, "fade(-106);reset;"},
    };
    for (Case& c : cases) {
        std::memset(object, 0, sizeof object);
        unsigned m = 1000, t = 2000; std::memcpy(object + 1036, &m, 4); std::memcpy(object + 1040, &t, 4);
        float pos[3] = {1.25f, -2.5f, 40.0f}; std::memcpy(object + 16, pos, 12);
        std::memcpy(object + 572, &c.state, 2); std::memcpy(object + 1080, &c.lift, 4);
        calls.clear(); zAtDraw = 0;
        msg_draw__7Critter(object);
        float after[3]; std::memcpy(after, object + 16, 12);
        bool drew = c.want.find("draw") != std::string::npos;
        if (calls != c.want || std::memcmp(after, pos, 12) != 0 || (drew && zAtDraw != 40.0f + c.lift)) {
            std::printf("fail: state %d lift %f: %s\n", c.state, c.lift, calls.c_str()); bad = 1;
        }
    }
    return bad;
}
