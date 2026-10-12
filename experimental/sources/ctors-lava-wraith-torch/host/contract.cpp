#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../torch.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_ctors_lava_wraith_view_481970[4], norrath_ctors_lava_wraith_view_44ab70[4];
static unsigned char* obj;
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** arguments) {
    event("GameObject map=%u args=%d", map != nullptr, arguments != nullptr);
    obj = (unsigned char*)object;
    std::memcpy(obj + 16, point, 12);
    return 1;
}
extern "C" void norrath_light_add(void* light) {
    event("lightAdd at %d", (int)((unsigned char*)light - obj));
    // the light record is complete when lightAdd is called
    expect(word_at(obj + 112) == fbits(240.0f) && word_at(obj + 116) == 0x3e99999au && word_at(obj + 120) == fbits(1.0f), "light floats");
    expect(obj[85] == 0 && obj[84] == 0, "light bytes");
    expect((obj[94] | obj[95] << 8) == 200 && (obj[96] | obj[97] << 8) == 150 && (obj[98] | obj[99] << 8) == 50, "light halves");
}
static void run_case(int kind) {
    calls.clear();
    put(norrath_ctors_lava_wraith_view_481970, 0x3e99999au, 4);
    Image<160> o, want;
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    o.w(12, 0x40); want.w(12, 0x40);
    char* args[] = {nullptr};
    const unsigned r = norrath_construct_Torch(o.b, point, args, kind);
    expect_log({"GameObject map=0 args=1", "lightAdd at 76"}, "torch");
    for (int i = 0; i < 12; ++i) { want.b[16 + i] = point[i]; want.b[124 + i] = point[i]; }
    want.w(72, low(norrath_ctors_lava_wraith_view_44ab70));
    want.w(112, fbits(240.0f)); want.w(116, 0x3e99999au); want.w(120, fbits(1.0f));
    want.w(104, fbits(240.0f)); want.w(108, 0x3e99999au);
    want.w(86, 200, 2); want.w(88, 150, 2); want.w(90, 50, 2); want.w(92, 0, 2);
    want.w(94, 51, 2); want.w(96, 39, 2); want.w(98, 18, 2); want.w(100, 0, 2);
    want.w(84, 0, 1); want.w(85, 1, 1); want.w(40, 0xf0, 1);
    want.w(138, (unsigned)kind & 0xff, 1); want.w(136, 0, 2);
    want.w(12, 0x40 | ((kind & 0xff) ? 0x200u : 0x1u) | 0x10008u);
    expect_image(o, want, "torch image");
    expect(r == 39u, "torch return");
}
int main() {
    run_case(0); run_case(1); run_case(5); run_case(0x100); run_case(-1);
    return finish("torch constructor");
}
