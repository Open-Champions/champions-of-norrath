// Host check only; not a build input. Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
#define NORRATH_HOST_CONTRACT 1
#include "screen11.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static int same_result;
extern "C" int VIIsMcSameSession__Fv() {
    trace += "same;";
    return same_result;
}

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen11(Action = %d, Param1 = %d, Param2 = %d)\n]";
static const std::string EXIT_PARENT = "P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{1}msg:parent+16,self+0,4354,0,0;msg:parent+16,self+0,4358,1,0;";

int main() {
    int bad = 0;
    int* w = hl_words();
    hl_status_count = 1;

    hl_reset(true, 0, 5);
    ProcessScreen11__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("draw", HEAD + "{0}{5}{6}gs(ui,963,2);cl(self+40,t3,510);init(self+40,ui,pos=0/0,size=self+420,550,5,0);open(self+40,1);begin(ui);"
                                   "draw(self+40);gs(ui,963,2);box(self+40,t3,20,0,440,3,0,8);gs(ui,786,2);prompt(self+0,4,t18,0);end(ui);");
    for (int action : {1, 2, 4}) {
        hl_reset(true, 0, 5);
        ProcessScreen11__10VIWndMcErriii(hl_self, action, 134, 6);
        bad += hl_check("ignored action", HEAD + "{" + std::to_string(action) + "}{134}{6}");
    }
    hl_reset(true, 0, 5);
    ProcessScreen11__10VIWndMcErriii(hl_self, 3, 0, 6);
    bad += hl_check("other key", HEAD + "{3}{0}{6}");

    struct Go { int status, screen; };
    const Go go[] = {{1, 4}, {2, 16}, {4, 16}, {5, 4}, {6, 4}, {-1, 4}};
    for (const Go& g : go) {
        hl_reset(true, 1, 17);
        hl_status[0] = g.status;
        ProcessScreen11__10VIWndMcErriii(hl_self, 3, 134, 6);
        bad += hl_check("goto", HEAD + "{3}{134}{6}status(0);P[VIWndMcErr::GotoScreen %d\n]{" + std::to_string(g.screen) + "}");
        bad += w[103] != g.screen || w[104] != 0;
    }
    for (int status : {0, 3}) {
        for (int same : {0, 2}) {
            hl_reset(true, 1, 17);
            hl_status[0] = status;
            same_result = same;
            ProcessScreen11__10VIWndMcErriii(hl_self, 3, 134, 6);
            bad += hl_check("not same session", HEAD + "{3}{134}{6}status(0);same;P[VIWndMcErr::GotoScreen %d\n]{16}");
            bad += w[103] != 16;
        }
        hl_reset(true, 1, 17);
        hl_status[0] = status;
        same_result = 1;
        ProcessScreen11__10VIWndMcErriii(hl_self, 3, 134, 6);
        bad += hl_check("exit with parent", HEAD + "{3}{134}{6}status(0);same;" + EXIT_PARENT);
        bad += w[109] != 0 || w[103] != 0;
        hl_reset(false, 1, 17);
        hl_status[0] = status;
        same_result = 1;
        ProcessScreen11__10VIWndMcErriii(hl_self, 3, 134, 6);
        bad += hl_check("exit no parent", HEAD + "{3}{134}{6}status(0);same;P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{1}");
        bad += w[109] != 1;
    }
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
