// Host check for scriptTask__Fv. Not an EE plan input.
#include <string>
extern "C" {
int nmDeferedCalls = 0;
int nextNmDeferedCalls = 0;
int scriptFrame = 0;
unsigned char deferedCall[2 * 5120];
}
static std::string trace;
static int mutate_frame_on_call = -1;
extern "C" int scriptStart__FPc(char* name) {
    trace += std::to_string((unsigned char*)name - deferedCall) + ",";
    if (mutate_frame_on_call >= 0 && (int)trace.size() > mutate_frame_on_call) scriptFrame += 10;
    return 99;
}
#include "script_task.cpp"
int main() {
    // Nothing queued: only the bookkeeping runs.
    nmDeferedCalls = 0; nextNmDeferedCalls = 3; scriptFrame = 4;
    scriptTask__Fv();
    if (!trace.empty() || nmDeferedCalls != 3 || nextNmDeferedCalls != 0 || scriptFrame != 5) return 1;
    // Frame is odd now: second bank (5120). Three records, stride 80.
    scriptTask__Fv();
    if (trace != "5120,5200,5280,") return 2;
    if (nmDeferedCalls != 0 || scriptFrame != 6) return 3;
    // Even frame: first bank.
    trace.clear(); nmDeferedCalls = 2; nextNmDeferedCalls = 1; scriptFrame = 8;
    scriptTask__Fv();
    if (trace != "0,80,") return 4;
    if (nmDeferedCalls != 1 || nextNmDeferedCalls != 0 || scriptFrame != 9) return 5;
    // A callee that moves scriptFrame is seen by the final increment.
    trace.clear(); nmDeferedCalls = 1; nextNmDeferedCalls = 0; scriptFrame = 0;
    mutate_frame_on_call = 0;
    scriptTask__Fv();
    if (scriptFrame != 11) return 6;
    return 0;
}
