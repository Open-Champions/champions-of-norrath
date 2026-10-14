// Host check only; not a build input. Stubs the font-height and colour callees, then checks every
// stored field, the computed height ((height - 4) * lines + 36), the forwarded font arguments and
// that no other byte of the window changes. The open flag at +36 is still untouched when the colour
// routine runs (it is stored afterwards).
#define NORRATH_HOST_ARENA 1
#include "init.cpp"
#include "../w16-wnd-a-common/host_world.hpp"

static std::string trace;
static int stub_height;
extern "C" int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, void* raster, void* dictionary) {
    trace += "h:" + std::to_string(hw::guest(font)) + "," + std::to_string(hw::guest(raster)) + "," +
             std::to_string(hw::guest(dictionary)) + ";";
    return stub_height;
}
extern "C" void SetUiColors__10VIUIWindow(void* self) {
    trace += "c:" + std::to_string(hw::guest(self)) + ";";
    trace += "s36=" + std::to_string(static_cast<int>(hw::w(self, 36))) + ";";
}

int main() {
    const unsigned window = hw::BASE + 0x100, ui = hw::BASE + 0x1000, raster = hw::BASE + 0x2000, dictionary = hw::BASE + 0x3000;
    const int heights[] = {4, 20, 31, 0};
    const int lines[] = {0, 1, 7};
    for (int height : heights) for (int b : lines) {
        hw::clear();
        std::memset(hw::at(window), 0x77, 400);
        hw::w(ui + 72) = raster;
        hw::w(ui + 80) = dictionary;
        int position[2] = {-12, 345}, size[2] = {640, 99};
        stub_height = height;
        trace.clear();
        const int result = Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(hw::at(window), hw::at(ui),
                                                                    reinterpret_cast<const Vect2I*>(position),
                                                                    reinterpret_cast<const Vect2I*>(size), 5, b, -9);
        hw::check(result == 0, "result");
        hw::check(hw::w(hw::at(window), 0) == static_cast<unsigned>(reinterpret_cast<uintptr_t>(hw::at(ui))), "ui word");
        hw::check(static_cast<int>(hw::w(window + 4)) == -12 && static_cast<int>(hw::w(window + 8)) == 345, "position");
        hw::check(hw::w(window + 12) == 640 && hw::w(window + 16) == 99, "size");
        hw::check(hw::w(window + 20) == 5, "a");
        hw::check(static_cast<int>(hw::w(window + 24)) == (height - 4) * b + 36, "pixel height", height, b);
        hw::check(hw::w(window + 28) == 18, "constant 18");
        hw::check(static_cast<int>(hw::w(window + 32)) == b, "b");
        hw::check(static_cast<int>(hw::w(window + 360)) == -9, "c");
        hw::check(hw::w(window + 36) == 1, "open flag");
        hw::check(trace == "h:" + std::to_string(ui + 56) + "," + std::to_string(raster) + "," + std::to_string(dictionary) +
                               ";c:" + std::to_string(window) + ";s36=" + std::to_string(0x77777777) + ";", "calls", height, b);
        for (unsigned off = 40; off < 400; ++off)
            if (off < 360 || off >= 364) hw::check(*hw::at(window + off) == 0x77, "untouched", off);
    }
    return hw::finish();
}
