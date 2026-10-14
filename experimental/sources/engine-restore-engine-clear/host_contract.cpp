#include "engine_clear.cpp"

static int error;
static int step;
static int seen_cause[2];
static int seen_handler;

static void fail(int code) {
    if (error == 0) error = code;
}

int vblankHID;

extern "C" void DisableIntc(int cause) {
    if (step != 0) fail(1);
    seen_cause[0] = cause;
    vblankHID = 99;
    ++step;
}
extern "C" int RemoveIntcHandler(int cause, int handler) {
    if (step != 1) fail(2);
    seen_cause[1] = cause;
    seen_handler = handler;
    ++step;
    return 7;
}
extern "C" void drawRemoveInterrupts__Fv() {
    if (step != 2) fail(3);
    ++step;
}

int main() {
    vblankHID = 42;
    engineClear__Fv();
    if (error != 0) return error;
    if (step != 3) return 4;
    if (seen_cause[0] != 2 || seen_cause[1] != 2) return 5;
    if (seen_handler != 99) return 6;
    return 0;
}
