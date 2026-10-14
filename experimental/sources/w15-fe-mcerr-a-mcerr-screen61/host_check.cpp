// Host check only; not a build input. Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
#define NORRATH_HOST_CONTRACT 1
#include "screen61.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen61(Action = %d, Param1 = %d, Param2 = %d)\n]";
static const std::string TEXTS = "gs(ui,974,2);gs(ui,956,2);gs(ui,967,2);snp(2048,%S\\n\\n%S\\n\\n%S,t14,t60,t7);";

int main() {
    int bad = 0;
    int* w = hl_words();
    hl_status_count = 1;

    hl_reset(true, 0, 5);
    ProcessScreen61__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("draw", HEAD + "{0}{5}{6}" + TEXTS + "cl(self+40,L,510);init(self+40,ui,pos=0/0,size=self+420,550,5,0);begin(ui);draw(self+40);"
                                   "box(self+40,L,20,0,440,3,0,8);gs(ui,21,2);gs(ui,22,2);prompt(self+0,4,t21,t22);end(ui);");
    bad += w[104] != 5;
    for (int action : {1, 2, 4, -1}) {
        hl_reset(true, 0, 5);
        ProcessScreen61__10VIWndMcErriii(hl_self, action, 134, 6);
        bad += hl_check("ignored action", HEAD + "{" + std::to_string(action) + "}{134}{6}" + TEXTS);
    }
    hl_reset(true, 0, 5);
    w[102] = 1;
    ProcessScreen61__10VIWndMcErriii(hl_self, 3, 136, 6);
    bad += hl_check("left", HEAD + "{3}{136}{6}" + TEXTS);
    bad += w[102] != 0;
    ProcessScreen61__10VIWndMcErriii(hl_self, 3, 137, 6);
    bad += w[102] != 1;
    ProcessScreen61__10VIWndMcErriii(hl_self, 3, 138, 6);
    bad += w[102] != 1;

    hl_reset(true, 0, 9);
    w[102] = 1;
    ProcessScreen61__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("choice 1", HEAD + "{3}{134}{6}" + TEXTS + "P[VIWndMcErr::GotoScreen %d\n]{20}");
    bad += w[103] != 20 || w[102] != 0 || w[104] != 0;

    struct Go { int status, screen; };
    const Go go[] = {{0, 62}, {1, 30}, {2, 30}, {-1, 30}};
    for (const Go& g : go) {
        hl_reset(true, 0, 9);
        hl_status[0] = g.status;
        w[102] = 0;
        ProcessScreen61__10VIWndMcErriii(hl_self, 3, 134, 6);
        bad += hl_check("choice 0", HEAD + "{3}{134}{6}" + TEXTS + "status(0);P[VIWndMcErr::GotoScreen %d\n]{" + std::to_string(g.screen) + "}");
        bad += w[103] != g.screen;
    }
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
