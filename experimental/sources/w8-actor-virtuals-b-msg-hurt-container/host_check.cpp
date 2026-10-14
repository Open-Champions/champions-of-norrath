#include <cstdio>
#include <cstring>

#include "msg_hurt.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(8) static unsigned char object[160];
static int added;
static int state_at_add;
extern "C" void objectAddToRunList__FP10GameObject(void* o) {
    ++added;
    if (o != object) std::fprintf(stderr, "wrong object\n"), ++failures;
    state_at_add = *reinterpret_cast<int*>(object + 128);
}
static int& word(unsigned off) { return *reinterpret_cast<int*>(object + off); }

static void fresh(int options, int state) {
    std::memset(object, 0x11, sizeof object);
    word(104) = options;
    word(128) = state;
    word(116) = 99;
    added = 0;
}

int main() {
    fresh(4, 0);
    msg_hurt__9ContainerfP10GameObjecti(object, 1.5f, nullptr, 0);
    expect(added == 1 && state_at_add == 0, "bit 2: run list added before state is stored");
    expect(word(128) == 1 && word(116) == 0, "bit 2 without 0x800: counter 0");

    fresh(4, 2);
    msg_hurt__9ContainerfP10GameObjecti(object, 0.0f, nullptr, 0x800);
    expect(added == 1 && word(128) == 1 && word(116) == 7, "bit 2 with 0x800: counter 7");

    fresh(4, 0);
    msg_hurt__9ContainerfP10GameObjecti(object, 0.0f, nullptr, 0x7ff);
    expect(word(116) == 0, "other flag bits do not select 7");

    fresh(1, 0);
    msg_hurt__9ContainerfP10GameObjecti(object, 0.0f, nullptr, 0x800);
    expect(added == 1 && word(128) == 1 && word(116) == 0, "bit 0 only: counter 0 even with 0x800");

    fresh(5, 0);
    msg_hurt__9ContainerfP10GameObjecti(object, 0.0f, nullptr, 0);
    expect(added == 1 && word(128) == 1, "both bits take the bit 2 arm only once");

    fresh(2, 0);
    msg_hurt__9ContainerfP10GameObjecti(object, 0.0f, nullptr, 0x800);
    expect(added == 0 && word(128) == 0 && word(116) == 99, "neither bit: nothing happens");

    fresh(5, 1);
    msg_hurt__9ContainerfP10GameObjecti(object, 0.0f, nullptr, 0x800);
    expect(added == 0 && word(128) == 1 && word(116) == 99, "state already 1: nothing happens");
    return failures ? 1 : 0;
}
