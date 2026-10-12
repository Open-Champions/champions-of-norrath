#include <cstdio>
#include <cstring>

#include "msg_run.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(8) static unsigned char object[320];
static int order[8];
static int steps;
static int set_flag_in_run;
static int active_result;
static int seen_flag, seen_value;
static void* seen_object;

extern "C" void msg_run__8AutoProp(void* o) {
    order[steps++] = 1;
    seen_object = o;
    if (set_flag_in_run >= 0) *reinterpret_cast<int*>(object + 280) = set_flag_in_run;
}
extern "C" int objectActiveProcessing__FP10GameObjectii(void* o, int flag, int value) {
    order[steps++] = 2;
    expect(o == object, "active processing gets the object");
    seen_flag = flag;
    seen_value = value;
    return active_result;
}
extern "C" void objectAlwaysDraw__FP10GameObject(void* o) {
    order[steps++] = 3;
    expect(o == object, "always draw gets the object");
}
extern "C" void objectCancelAllAlwaysDraw__Fv() { order[steps++] = 4; }

static int flag_word() { return *reinterpret_cast<int*>(object + 280); }

static void run(int before, int in_run, int result, int expect_tail, int expect_word) {
    std::memset(object, 0x33, sizeof object);
    *reinterpret_cast<int*>(object + 280) = before;
    steps = 0;
    set_flag_in_run = in_run;
    active_result = result;
    msg_run__10HateBridge(object);
    expect(order[0] == 1 && order[1] == 2, "run method then active processing");
    expect(seen_value == 1000, "passes 1000");
    expect(seen_flag == (in_run >= 0 ? in_run : before), "re-reads the word after the run method");
    expect(flag_word() == expect_word, "stores 0 or 1");
    if (expect_tail) {
        expect(steps == 3 && order[2] == expect_tail, "expected follow-up call");
    } else {
        expect(steps == 2, "no follow-up call");
    }
    expect(object[279] == 0x33 && object[284] == 0x33, "neighbours untouched");
}

int main() {
    run(0, -1, 0, 0, 0);
    run(0, -1, 5, 3, 1);
    run(0, -1, -1, 3, 1);
    run(7, -1, 0, 4, 0);
    run(7, -1, 3, 0, 1);
    run(1, 0, 9, 0, 1);   // the pre-run word decides the branch, not the re-read one
    run(0, 6, 0, 0, 0);
    return failures ? 1 : 0;
}
