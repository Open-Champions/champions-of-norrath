// Host check only; not a build input. Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
#define NORRATH_HOST_CONTRACT 1
#include "screen10.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen10(Action = %d, Param1 = %d, Param2 = %d)\n]";

int main() {
    int bad = 0;
    int* w = hl_words();
    hl_status_count = 1;

    hl_reset(true, 0, 5);
    ProcessScreen10__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("draw", HEAD + "{0}{5}{6}gs(ui,962,2);cl(self+40,t2,510);init(self+40,ui,pos=0/0,size=self+420,550,5,0);open(self+40,1);begin(ui);"
                                   "draw(self+40);gs(ui,962,2);box(self+40,t2,20,0,440,3,0,8);gs(ui,786,2);prompt(self+0,4,t18,0);end(ui);");
    bad += w[104] != 5 || w[103] != 0;

    for (int action : {1, 2, 4, -1}) {
        hl_reset(true, 0, 5);
        ProcessScreen10__10VIWndMcErriii(hl_self, action, 134, 6);
        bad += hl_check("ignored action", HEAD + "{" + std::to_string(action) + "}{134}{6}");
    }
    hl_reset(true, 0, 5);
    ProcessScreen10__10VIWndMcErriii(hl_self, 3, 133, 6);
    bad += hl_check("other key", HEAD + "{3}{133}{6}");

    struct Go { int status, screen; };
    const Go go[] = {{0, 4}, {1, 15}, {2, 4}, {3, 4}, {4, 4}, {5, 15}, {6, 4}, {-1, 4}};
    for (const Go& g : go) {
        hl_reset(true, 1, 17);
        hl_status[0] = g.status;
        w[102] = 9;
        ProcessScreen10__10VIWndMcErriii(hl_self, 3, 134, 6);
        bad += hl_check("key", HEAD + "{3}{134}{6}status(0);P[VIWndMcErr::GotoScreen %d\n]{" + std::to_string(g.screen) + "}");
        bad += w[103] != g.screen || w[102] != 0 || w[104] != 0;
    }
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
