extern "C" {
int norrath_vi_timer_handler_id __asm__("_7VITimer$HandlerID");
int norrath_vi_timer_ready __asm__("_7VITimer$Ready") = 1;
}
#include "timer_close.cpp"

static int step;
static int error;
static int disable_cause;
static int remove_cause;
static int remove_handler;
static int enable_cause;

static void fail(int code) {
    if (error == 0) error = code;
}

extern "C" void DisableIntc(int cause) {
    if (step != 0) fail(1);
    if (norrath_vi_timer_ready != 0) fail(2);
    norrath_vi_timer_handler_id = 41;
    disable_cause = cause;
    ++step;
}

extern "C" int RemoveIntcHandler(int cause, int handler) {
    if (step != 1) fail(3);
    remove_cause = cause;
    remove_handler = handler;
    ++step;
    return 0;
}

extern "C" void EnableIntc(int cause) {
    if (step != 2) fail(4);
    enable_cause = cause;
    ++step;
}

int main() {
    norrath_vi_timer_handler_id = 3;
    Close__7VITimer();
    if (error != 0) return error;
    if (step != 3) return 5;
    if (disable_cause != 9 || remove_cause != 9 || enable_cause != 9) return 6;
    if (remove_handler != 41) return 7;
    if (norrath_vi_timer_ready != 0) return 8;
    return 0;
}
