// Host contract for __throw (source: w10-startup-a-throw/throw.cpp). Not an EE plan input.
// The five stack-walking builtins have no host equivalent, so they are replaced by recording
// stubs; the callees (get_eh_context pointer, __terminate, __frame_state_for, throw_helper) are
// stubs too. What this checks is the control flow and the arguments of the C++ part:
//   - a context whose info word is null ends in __terminate before anything else is asked;
//   - a null result from __frame_state_for ends in __terminate;
//   - otherwise word 0 of the frame record is set to the frame address, throw_helper gets the
//     context, the return address minus one, the record and an adjust slot, and the exception
//     return is made with that adjust slot's value and throw_helper's result.
// It does NOT check the original's exact register hand-over (the handler sees the context in v0
// in the original; the compiler's expansion cannot place it) or that the frame is saved with the
// original's instructions; see the notes of the package.
#include <cstdio>
#include <cstdlib>
#include <string>

static std::string trace;
static void* fake_cfa = reinterpret_cast<void*>(0x1234);
static long eh_adjust_value; static void* eh_handler_seen; static bool returned_from_eh;
#define __builtin_unwind_init() (trace += "unwind_init;")
#define __builtin_dwarf_cfa() (fake_cfa)
#define __builtin_return_address(n) (reinterpret_cast<void*>(0x4001))
#define __builtin_extract_return_addr(p) (p)
#define __builtin_eh_return(adjust, handler) do { eh_adjust_value = (adjust); eh_handler_seen = (handler); trace += "eh_return;"; returned_from_eh = true; } while (0)

#include "../w10-startup-a-throw/throw.cpp"

static W10EhContext context_storage;
static void* provider() { trace += "ctx;"; return &context_storage; }
extern "C" void* (*get_eh_context)() = provider;

struct Terminated {};
extern "C" void __terminate() { trace += "terminate;"; throw Terminated(); }
static W10FrameState record_storage;
static bool give_record = true;
static void* seen_pc; static W10FrameState* seen_state_in;
extern "C" W10FrameState* __frame_state_for(void* pc, W10FrameState* state) {
    trace += "frame_state;"; seen_pc = pc; seen_state_in = state;
    return give_record ? &record_storage : nullptr;
}
static W10EhContext* h_context; static void* h_pc; static W10FrameState* h_state; static long* h_adjust; static void* h_word0;
static int handler_target;
extern "C" void* throw_helper(W10EhContext* c, void* pc, W10FrameState* s, long* adjust) {
    trace += "helper;"; h_context = c; h_pc = pc; h_state = s; h_adjust = adjust;
    h_word0 = *reinterpret_cast<void**>(s);
    *adjust = 0x70;
    return &handler_target;
}

int main() {
    // null info word: terminate first (the stub throws, so the rest of __throw is skipped)
    context_storage.info = nullptr; trace.clear();
    try { __throw(); return 1; } catch (Terminated&) {}
    if (trace != "unwind_init;ctx;terminate;" && trace != "ctx;terminate;") return 2;
    // frame state missing
    context_storage.info = &context_storage; give_record = false; trace.clear(); returned_from_eh = false;
    try { __throw(); return 3; } catch (Terminated&) {}
    if (returned_from_eh) return 4;
    // normal run
    give_record = true; trace.clear();
    __throw();
    if (!returned_from_eh) return 5;
    if (h_context != &context_storage || h_state != &record_storage) return 6;
    if (h_pc != reinterpret_cast<void*>(0x4000)) return 7;                 // return address minus one
    if (h_word0 != fake_cfa) return 8;                                       // frame address stored before the helper runs
    if (eh_adjust_value != 0x70 || eh_handler_seen != &handler_target) return 9;
    const size_t a = trace.find("ctx;"), b = trace.find("frame_state;"), c = trace.find("helper;"), d = trace.find("eh_return;");
    if (a == std::string::npos || b == std::string::npos || c == std::string::npos || d == std::string::npos || !(a < b && b < c && c < d)) return 10;
    std::puts("__throw control flow ok");
    return 0;
}
