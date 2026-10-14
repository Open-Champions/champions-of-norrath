// Host check only; not a build input. Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
#define NORRATH_HOST_CONTRACT 1
#include "screen64.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen64(Action = %d, Param1 = %d, Param2 = %d)\n]";

int main() {
    int bad = 0;
    int* w = hl_words();
    hl_status_count = 1;

    hl_reset(true, 0, 5);
    ProcessScreen64__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("draw", HEAD + "{0}{5}{6}gs(ui,949,2);cl(self+40,t53,450);init(self+40,ui,pos=0/0,size=self+420,490,5,0);begin(ui);draw(self+40);"
                                   "gs(ui,949,2);box(self+40,t53,20,0,440,3,0,8);gs(ui,896,2);gs(ui,753,2);prompt(self+0,4,t0,t49);end(ui);");
    for (int action : {1, 2, 4, -1}) {
        hl_reset(true, 0, 5);
        ProcessScreen64__10VIWndMcErriii(hl_self, action, 134, 6);
        bad += hl_check("ignored action", HEAD + "{" + std::to_string(action) + "}{134}{6}");
    }
    hl_reset(true, 0, 5);
    w[102] = 1;
    ProcessScreen64__10VIWndMcErriii(hl_self, 3, 136, 6);
    bad += w[102] != 0;
    ProcessScreen64__10VIWndMcErriii(hl_self, 3, 137, 6);
    bad += w[102] != 1;
    ProcessScreen64__10VIWndMcErriii(hl_self, 3, 133, 6);
    bad += w[102] != 1;
    ProcessScreen64__10VIWndMcErriii(hl_self, 3, 138, 6);
    bad += w[102] != 1;

    hl_reset(true, 0, 9);
    w[102] = 1;
    ProcessScreen64__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("choice 1", HEAD + "{3}{134}{6}P[VIWndMcErr::GotoScreen %d\n]{20}");
    bad += w[103] != 20 || w[102] != 0 || w[104] != 0;

    struct Go { int status, screen; };
    const Go go[] = {{1, 20}, {0, 62}, {2, 30}, {3, 30}, {5, 30}, {6, 0}, {7, 0}, {-1, 0}};
    for (const Go& g : go) {
        hl_reset(true, 0, 9);
        hl_status[0] = g.status;
        ProcessScreen64__10VIWndMcErriii(hl_self, 3, 134, 6);
        std::string want = HEAD + "{3}{134}{6}status(0);";
        if (g.screen) want += "P[VIWndMcErr::GotoScreen %d\n]{" + std::to_string(g.screen) + "}";
        bad += hl_check("choice 0", want);
        bad += w[103] != g.screen || (g.screen && w[104] != 0) || (!g.screen && w[104] != 9);
    }
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
