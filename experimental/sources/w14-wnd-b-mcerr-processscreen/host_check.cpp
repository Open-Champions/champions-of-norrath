// Host check only; not a build input.
#include "processscreen.cpp"
#include <cstdio>
#include <string>

static std::string trace;
extern "C" int scePrintf(const char* f, ...) { trace += "print;"; (void)f; return 0; }
extern "C" void ProcessScreen04__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h4:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen10__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h10:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen11__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h11:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen12__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h12:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen15__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h15:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen16__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h16:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen17__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h17:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen20__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h20:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen30__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h30:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen50__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h50:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen52__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h52:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen54__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h54:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen60__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h60:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen61__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h61:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen62__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h62:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen63__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h63:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
extern "C" void ProcessScreen64__10VIWndMcErriii(void*, int a, int b, int c) { trace += "h64:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }

int main() {
    static int w[200];
    bool ok = true;
    const int expected[] = {4,10,11,12,15,16,17,20,30,50,52,54,60,61,62,63,64};
    for (int screen = -3; screen < 80; ++screen) {
        w[103] = screen; trace.clear();
        const int r = ProcessScreen__10VIWndMcErriii(w, 7, 8, 9);
        std::string want = "print;";
        for (int e : expected) if (e == screen) want += "h" + std::to_string(e) + ":7,8,9;";
        ok = ok && r == 0 && trace == want;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
