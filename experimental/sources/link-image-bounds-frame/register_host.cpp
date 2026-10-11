#include <cstring>

#define NORRATH_HOST_CONTRACT 1
#include "register_frame_info.cpp"

extern "C" {
volatile int __sce_eh_sema_id;
}

static int waits;
static int signals;
static int wait_id;
static int signal_id;
static FrameObject* head_during_wait;
static FrameObject* head_during_signal;
static const void* begin_during_wait;
static void* next_during_wait;
static FrameObject* watched;

extern "C" int WaitSema(int id) {
    waits += 1;
    wait_id = id;
    head_during_wait = objects;
    begin_during_wait = watched->begin;
    next_during_wait = watched->next;
    __sce_eh_sema_id = 77;
    return 0;
}

extern "C" int SignalSema(int id) {
    signals += 1;
    signal_id = id;
    head_during_signal = objects;
    return 0;
}

static int fail(int code) { return code; }

int main() {
    __sce_eh_sema_id = 41;
    objects = nullptr;
    FrameObject first;
    FrameObject second;
    std::memset(&first, 0xa5, sizeof first);
    std::memset(&second, 0xa5, sizeof second);
    char frame_a = 0;
    char frame_b = 0;
    watched = &first;
    __register_frame_info(&frame_a, &first);
    if (waits != 1 || signals != 1) return fail(1);
    if (wait_id != 41 || signal_id != 77) return fail(2);
    if (head_during_wait != nullptr || head_during_signal != &first) return fail(3);
    if (begin_during_wait != &frame_a || next_during_wait == nullptr) return fail(4);
    if (first.word0 != nullptr || first.word1 != nullptr) return fail(5);
    if (first.begin != &frame_a || first.word3 != nullptr || first.word4 != nullptr) return fail(6);
    if (first.next != nullptr || objects != &first) return fail(7);

    __sce_eh_sema_id = 42;
    watched = &second;
    __register_frame_info(&frame_b, &second);
    if (waits != 2 || signals != 2) return fail(8);
    if (wait_id != 42 || signal_id != 77) return fail(9);
    if (head_during_wait != &first || head_during_signal != &second) return fail(10);
    if (second.begin != &frame_b || second.next != &first) return fail(11);
    if (second.word0 != nullptr || second.word1 != nullptr || second.word3 != nullptr || second.word4 != nullptr) return fail(12);
    if (first.next != nullptr || objects != &second) return fail(13);
    return 0;
}
