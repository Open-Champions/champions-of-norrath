#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

#define NORRATH_W8AVB_HOST
extern "C" unsigned long long norrath_w8avb_host_base = 0;
static int type_push, type_other;
static void* push_type_function() { return &type_push; }
static void* other_type_function() { return &type_other; }
extern "C" void* norrath_w8avb_host_function(unsigned word) {
    return word == 21 ? reinterpret_cast<void*>(&push_type_function)
                      : word == 22 ? reinterpret_cast<void*>(&other_type_function) : nullptr;
}
#include "msg_collision.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(16) static unsigned char arena[8192];
static const unsigned kTable = 6144;  // table: word 21 at +4 (push type) ; second table at +64 (other)
static const unsigned kSelf = 0, kOther = 1536, kListed = 3072, kListedOther = 4608;

static float low_seen[3], high_seen[3];
static int capacity_seen, find_calls;
static std::vector<void*> to_find;

extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** found, int capacity) {
    ++find_calls;
    std::memcpy(low_seen, low, 12);
    std::memcpy(high_seen, high, 12);
    capacity_seen = capacity;
    unsigned i = 0;
    for (void* p : to_find) found[i++] = p;
    found[i] = nullptr;
}
extern "C" void* __tf15PushPhysicsProp() { return &type_push; }
extern "C" int __eq__C9type_infoRC9type_info(const void* l, const void* r) { return l == r; }
extern "C" void __throw_bad_typeid() { throw 1; }

static unsigned char* at(unsigned off) { return arena + off; }
static float& f(unsigned base, unsigned off) { return *reinterpret_cast<float*>(arena + base + off); }
static int& w(unsigned base, unsigned off) { return *reinterpret_cast<int*>(arena + base + off); }

static void make_object(unsigned base, bool push) {
    std::memset(arena + base, 0, 1400);
    w(base, 72) = static_cast<int>(kTable + (push ? 0 : 64));
}

static void setup() {
    std::memset(arena, 0, sizeof arena);
    norrath_w8avb_host_base = reinterpret_cast<unsigned long long>(arena);
    // table for push type: function word 21 at +4 ; table for other type at +64: word 22
    *reinterpret_cast<unsigned*>(arena + kTable + 4) = 21;
    *reinterpret_cast<unsigned*>(arena + kTable + 64 + 4) = 22;
    make_object(kSelf, true);
    make_object(kOther, true);
    make_object(kListed, true);
    make_object(kListedOther, false);
    find_calls = 0;
    to_find.clear();
}
static bool near(float a, double b) { return std::fabs(a - b) <= 1e-3 + 1e-5 * std::fabs(b); }

int main() {
    // flag set, pushed (dot well below 1), box search, two listed objects
    setup();
    w(kOther, 12) = 0x40;
    f(kOther, 1196) = 3.0f;
    f(kOther, 1200) = 4.0f;
    f(kSelf, 124) = -2.0f;
    f(kSelf, 128) = -1.0f;
    f(kSelf, 16) = 100.0f;
    f(kSelf, 20) = 200.0f;
    f(kSelf, 28) = 8.0f;
    w(kOther, 252) = 50;
    w(kSelf, 252) = 30;
    w(kListed, 252) = 7;
    w(kListedOther, 252) = 7;
    to_find = {at(kListed), at(kListedOther)};
    msg_collision__15PushPhysicsPropP10GameObject(at(kSelf), at(kOther));
    expect(near(f(kSelf, 124), -2.0 + 3.0 * 0.01) && near(f(kSelf, 128), -1.0 + 4.0 * 0.01), "pushed by 0.01 times the pair");
    expect(find_calls == 1 && capacity_seen == 16, "one search for up to 16");
    expect(low_seen[0] == 92.0f && low_seen[1] == 192.0f && low_seen[2] == -10000.0f, "box low corner");
    expect(high_seen[0] == 108.0f && high_seen[1] == 208.0f && high_seen[2] == 10000.0f, "box high corner");
    expect(w(kListed, 252) == 190, "listed push prop gets 190");
    expect(w(kListedOther, 252) == 7, "listed object of another type is untouched");
    expect(w(kSelf, 252) == 50, "self takes the larger word (other = 50)");

    // dot product at or above 1: no push, search still happens; other in list gets 190 first
    setup();
    w(kOther, 12) = 0x40;
    f(kOther, 1196) = 3.0f;
    f(kOther, 1200) = 4.0f;
    f(kSelf, 124) = 6.0f;
    f(kSelf, 128) = 8.0f;  // dot = 10
    w(kOther, 252) = 5;
    w(kSelf, 252) = 30;
    to_find = {at(kOther)};
    msg_collision__15PushPhysicsPropP10GameObject(at(kSelf), at(kOther));
    expect(f(kSelf, 124) == 6.0f && f(kSelf, 128) == 8.0f, "dot >= 1: not pushed");
    expect(w(kOther, 252) == 190 && w(kSelf, 252) == 190, "other was listed, set to 190, self takes 190");

    // flag clear: no push, no search; type test still runs; signed max with negatives
    setup();
    w(kOther, 12) = 0x3f;
    w(kOther, 252) = -5;
    w(kSelf, 252) = -9;
    f(kSelf, 124) = 1.0f;
    msg_collision__15PushPhysicsPropP10GameObject(at(kSelf), at(kOther));
    expect(find_calls == 0 && f(kSelf, 124) == 1.0f, "flag clear: no search, no push");
    expect(w(kSelf, 252) == -5, "signed maximum");
    w(kOther, 252) = -20;
    msg_collision__15PushPhysicsPropP10GameObject(at(kSelf), at(kOther));
    expect(w(kSelf, 252) == -5, "smaller other word does not lower self");

    // other of another type: part 2 does nothing
    setup();
    make_object(kOther, false);
    w(kOther, 12) = 0;
    w(kOther, 252) = 99;
    w(kSelf, 252) = 1;
    msg_collision__15PushPhysicsPropP10GameObject(at(kSelf), at(kOther));
    expect(w(kSelf, 252) == 1, "other type: unchanged");

    // empty list
    setup();
    w(kOther, 12) = 0x40;
    f(kOther, 1196) = 1.0f;
    f(kOther, 1200) = 0.0f;
    f(kSelf, 124) = 5.0f;
    to_find = {};
    msg_collision__15PushPhysicsPropP10GameObject(at(kSelf), at(kOther));
    expect(find_calls == 1, "empty list searched");

    // a null object reaches __throw_bad_typeid
    bool threw = false;
    try {
        (void)run_time_type(nullptr);
    } catch (int) {
        threw = true;
    }
    expect(threw, "null object throws bad_typeid");
    return failures ? 1 : 0;
}
