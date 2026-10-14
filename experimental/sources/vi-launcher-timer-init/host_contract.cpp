#define NORRATH_HOST_CONTRACT
#include "timer_init.cpp"

static int step;
static int error;
static int disable_cause;
static int enable_cause;
static int add_cause;
static int add_next;
static void (*add_handler)(int);
static unsigned store_address[2];
static unsigned store_value[2];
static int store_count;
static int handler_result = 77;

static void fail(int code) {
    if (error == 0) error = code;
}

extern "C" void norrath_host_store32(unsigned address, unsigned value) {
    if (store_count < 2) {
        store_address[store_count] = address;
        store_value[store_count] = value;
    }
    ++store_count;
}

extern "C" void DisableIntc(int cause) {
    if (step != 0) fail(1);
    if (norrath_vi_timer_system_time != 0) fail(2);
    disable_cause = cause;
    ++step;
}

extern "C" int AddIntcHandler(int cause, void (*handler)(int), int next) {
    if (step != 1) fail(3);
    if (norrath_vi_timer_mode != 1473) fail(4);
    if (store_count != 2) fail(5);
    add_cause = cause;
    add_handler = handler;
    add_next = next;
    ++step;
    return handler_result;
}

extern "C" void EnableIntc(int cause) {
    if (step != 2) fail(6);
    if (norrath_vi_timer_handler_id != handler_result) fail(7);
    if (norrath_vi_timer_ready != 0) fail(8);
    enable_cause = cause;
    ++step;
}

extern "C" void Handler__7VITimeri(int) {}

int main() {
    norrath_vi_timer_system_time = 9;
    norrath_vi_timer_ready = 0;
    norrath_vi_timer_mode = 0;
    norrath_vi_timer_handler_id = 0;
    Init__7VITimer();
    if (error != 0) return error;
    if (step != 3) return 9;
    if (disable_cause != 9 || enable_cause != 9 || add_cause != 9 || add_next != -1) return 10;
    if (add_handler != Handler__7VITimeri) return 11;
    if (store_address[0] != 0x10000010u || store_value[0] != 1473u) return 12;
    if (store_address[1] != 0x10000020u || store_value[1] != 18750u) return 13;
    if (norrath_vi_timer_ready != 1 || norrath_vi_timer_system_time != 0) return 14;
    return 0;
}
