// Host check only; not a build input. Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
#define NORRATH_HOST_CONTRACT 1
#include "screen20.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen20(Action = %d, Param1 = %d, Param2 = %d)\n]";
static const std::string TEXTS = "gs(ui,1626,2);gs(ui,957,2);snp(2048,%S\\n\\n%S,t26,t61);";

int main() {
    int bad = 0;
    int* w = hl_words();

    hl_reset(true, 0, 3);
    ProcessScreen20__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("draw", HEAD + "{0}{5}{6}" + TEXTS + "cl(self+40,L,430);init(self+40,ui,pos=0/0,size=self+420,470,5,0);begin(ui);draw(self+40);"
                                   "box(self+40,L,20,0,440,3,0,8);gs(ui,786,2);prompt(self+0,4,t18,0);end(ui);");
    bad += w[104] != 3 || w[103] != 0;
    for (int action : {1, 2, 4, -1}) {
        hl_reset(true, 0, 5);
        ProcessScreen20__10VIWndMcErriii(hl_self, action, 134, 6);
        bad += hl_check("ignored action", HEAD + "{" + std::to_string(action) + "}{134}{6}" + TEXTS);
    }
    hl_reset(true, 0, 5);
    ProcessScreen20__10VIWndMcErriii(hl_self, 3, 133, 6);
    bad += hl_check("other key", HEAD + "{3}{133}{6}" + TEXTS);
    hl_reset(true, 1, 17);
    w[102] = 4;
    ProcessScreen20__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("confirm", HEAD + "{3}{134}{6}" + TEXTS + "P[VIWndMcErr::GotoScreen %d\n]{30}");
    bad += w[103] != 30 || w[102] != 0 || w[104] != 0;
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
