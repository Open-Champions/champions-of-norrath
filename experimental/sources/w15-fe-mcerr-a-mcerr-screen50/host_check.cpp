// Host check only; not a build input. Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
#define NORRATH_HOST_CONTRACT 1
#include "screen50.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen50(Action = %d, Param1 = %d, Param2 = %d)\n]";

int main() {
    int bad = 0;
    int* w = hl_words();

    hl_reset(true, 0, 0);
    w[102] = 7;
    ProcessScreen50__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("first draw", HEAD + "{0}{5}{6}gs(ui,964,2);cl(self+40,t4,310);init(self+40,ui,pos=0/0,size=self+420,350,5,0);begin(ui);draw(self+40);"
                                         "gs(ui,964,2);box(self+40,t4,20,0,440,3,0,8);gs(ui,786,2);prompt(self+0,4,t18,0);end(ui);");
    bad += w[104] != 1 || w[102] != 1;
    hl_reset(true, 0, 9);
    w[102] = 7;
    ProcessScreen50__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += w[104] != 10 || w[102] != 7;

    for (int action : {1, 2, 4, -1}) {
        hl_reset(true, 0, 5);
        ProcessScreen50__10VIWndMcErriii(hl_self, action, 134, 6);
        bad += hl_check("ignored action", HEAD + "{" + std::to_string(action) + "}{134}{6}");
    }
    hl_reset(true, 0, 5);
    ProcessScreen50__10VIWndMcErriii(hl_self, 3, 133, 6);
    bad += hl_check("other key", HEAD + "{3}{133}{6}");
    hl_reset(true, 0, 5);
    ProcessScreen50__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("confirm", HEAD + "{3}{134}{6}P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{0}msg:parent+16,self+0,4354,0,0;msg:parent+16,self+0,4358,0,0;");
    hl_reset(false, 0, 5);
    w[109] = 9;
    ProcessScreen50__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("confirm, no parent", HEAD + "{3}{134}{6}P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{0}");
    bad += w[109] != 0;
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
