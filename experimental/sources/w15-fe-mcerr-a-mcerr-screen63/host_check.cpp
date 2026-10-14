// Host check only; not a build input. Build with -Wno-format (the image prints a 64-bit value with %ld).
// Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
// The host unsigned long is 64 bits wide, so the session id shows the full key here (32 bits on the EE).
#define NORRATH_HOST_CONTRACT 1
#include "screen63.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static long long time_value;
static int write_result;
extern "C" {
int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    trace += std::string("printf[") + format + "]{" + std::to_string(va_arg(args, long long)) + "}";
    va_end(args);
    return 0;
}
long long GetTime__7VITimer() { trace += "time;"; return time_value; }
void VISetMcSessionId__FUl(unsigned long id) { trace += "session(" + std::to_string(id) + ");"; }
int VIMemCardWriteFile__FPCcPCvi(const char* name, const void* data, int size) {
    long long key;
    std::memcpy(&key, data, 8);
    trace += std::string("write(") + name + "," + std::to_string(key) + "," + std::to_string(size) + ");";
    return write_result;
}
}

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen63(Action = %d, Param1 = %d, Param2 = %d)\n]";
static const std::string TEXTS = "gs(ui,973,2);gs(ui,965,2);snp(2048,%S\\n\\n%S,t13,t5);";
static const std::string TOP = "cl(self+40,L,510);init(self+40,ui,pos=0/0,size=self+420,550,5,0);begin(ui);";
static const std::string REST = "draw(self+40);box(self+40,L,20,0,440,3,0,8);gs(ui,786,2);prompt(self+0,4,t18,0);end(ui);";

int main() {
    int bad = 0;
    int* w = hl_words();
    hl_status_count = 1;

    hl_reset(true, 0, 3);
    ProcessScreen63__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("later frame", HEAD + "{0}{5}{6}" + TEXTS + TOP + REST);
    bad += w[104] != 4;

    // first frame, card ok: session key = time + card.
    hl_reset(true, 0, 0);
    hl_status[0] = 0;
    hl_card = -20;
    time_value = 0x100000064LL;
    write_result = 8;
    ProcessScreen63__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("first frame", HEAD + "{0}{5}{6}" + TEXTS + TOP + "status(ptr);time;session(4294967376);printf[Writing session key (%ld) to memory card.\n]{4294967376}"
                                         "write(session.id,4294967376,8);" + REST);
    bad += w[104] != 1 || w[103] != 0;

    // first frame, write fails.
    hl_reset(true, 0, 0);
    hl_status[0] = 0;
    hl_card = 3;
    time_value = 10;
    write_result = -1;
    w[102] = 7;
    ProcessScreen63__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("write failed", HEAD + "{0}{5}{6}" + TEXTS + TOP + "status(ptr);time;session(13);printf[Writing session key (%ld) to memory card.\n]{13}"
                                           "write(session.id,13,8);P[VIWndMcErr::GotoScreen %d\n]{64}" + REST);
    bad += w[103] != 64 || w[104] != 0 || w[102] != 0;

    // first frame, card busy.
    hl_reset(true, 0, 0);
    hl_status[0] = 2;
    ProcessScreen63__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("card busy", HEAD + "{0}{5}{6}" + TEXTS + TOP + "status(ptr);P[VIWndMcErr::GotoScreen %d\n]{64}" + REST);
    bad += w[103] != 64;

    // keys
    for (int action : {1, 2, -1}) {
        hl_reset(true, 0, 5);
        ProcessScreen63__10VIWndMcErriii(hl_self, action, 134, 6);
        bad += hl_check("ignored action", HEAD + "{" + std::to_string(action) + "}{134}{6}" + TEXTS);
    }
    hl_reset(true, 0, 1);
    ProcessScreen63__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("too early", HEAD + "{3}{134}{6}" + TEXTS);
    hl_reset(true, 0, 2);
    ProcessScreen63__10VIWndMcErriii(hl_self, 3, 133, 6);
    bad += hl_check("other key", HEAD + "{3}{133}{6}" + TEXTS);
    hl_reset(true, 0, 2);
    ProcessScreen63__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("confirm", HEAD + "{3}{134}{6}" + TEXTS + "P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{1}msg:parent+16,self+0,4354,0,0;msg:parent+16,self+0,4358,1,0;");
    hl_reset(false, 0, 9);
    ProcessScreen63__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("confirm, no parent", HEAD + "{3}{134}{6}" + TEXTS + "P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{1}");
    bad += w[109] != 1;
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
