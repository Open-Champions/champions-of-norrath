#include <csetjmp>
#include <cstring>
#include <string>

#define NORRATH_HOST_CONTRACT 1
#include "deregister_frame_info.cpp"

extern "C" {
FrameObject* objects;
volatile int __sce_eh_sema_id;
}

static std::string trace;
static void* freed;
static std::jmp_buf aborted;

extern "C" int WaitSema(int id) {
    trace += "wait:" + std::to_string(id) + ";";
    __sce_eh_sema_id = 77;
    return 0;
}
extern "C" int SignalSema(int id) {
    trace += "signal:" + std::to_string(id) + ";";
    return 0;
}
extern "C" void __wrap_free(void* pointer) {
    freed = pointer;
    trace += "free;";
}
extern "C" __attribute__((noreturn)) void abort() {
    trace += "abort;";
    std::longjmp(aborted, 1);
}

static int fail(int code) { return code; }

static void reset(FrameObject* head) {
    trace.clear();
    freed = nullptr;
    objects = head;
    __sce_eh_sema_id = 41;
}

int main() {
    char frame_a = 0;
    char frame_b = 0;
    char frame_c = 0;
    char extra = 0;
    FrameObject node_a{};
    FrameObject node_b{};
    FrameObject node_c{};
    node_a.begin = &frame_a;
    node_b.begin = &frame_b;
    node_b.word3 = &extra;
    node_b.next = &node_a;
    node_c.begin = &frame_c;
    node_c.word3 = &frame_c;
    node_c.next = &node_b;

    reset(&node_c);
    FrameObject* removed = __deregister_frame_info(&frame_b);
    if (removed != &node_b) return fail(1);
    if (objects != &node_c || node_c.next != &node_a) return fail(2);
    if (freed != &extra) return fail(3);
    if (trace != "wait:41;free;signal:77;") return fail(4);

    reset(&node_c);
    node_c.next = &node_a;
    removed = __deregister_frame_info(&frame_c);
    if (removed != &node_c || objects != &node_a || node_a.next != nullptr) return fail(5);
    if (freed != nullptr) return fail(6);
    if (trace != "wait:41;signal:77;") return fail(7);

    reset(&node_a);
    node_a.word3 = nullptr;
    node_a.next = nullptr;
    removed = __deregister_frame_info(&frame_a);
    if (removed != &node_a || objects != nullptr || freed != nullptr) return fail(8);
    if (trace != "wait:41;signal:77;") return fail(9);

    reset(nullptr);
    if (setjmp(aborted) == 0) {
        __deregister_frame_info(&frame_a);
        return fail(10);
    }
    if (objects != nullptr || trace != "wait:41;signal:77;abort;") return fail(11);

    reset(&node_a);
    node_a.begin = &frame_a;
    node_a.next = nullptr;
    if (setjmp(aborted) == 0) {
        __deregister_frame_info(&frame_b);
        return fail(12);
    }
    if (objects != &node_a || trace != "wait:41;signal:77;abort;") return fail(13);
    return 0;
}
