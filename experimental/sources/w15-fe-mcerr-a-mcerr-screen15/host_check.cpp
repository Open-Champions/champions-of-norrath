// Host check only; not a build input. Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
#define NORRATH_HOST_CONTRACT 1
#include "screen15.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen15(Action = %d, Param1 = %d, Param2 = %d)\n]";

int main() {
    int bad = 0;
    int* w = hl_words();

    hl_reset(true, 0, 0);
    ProcessScreen15__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("draw", HEAD + "{0}{5}{6}gs(ui,955,2);cl(self+40,t59,510);init(self+40,ui,pos=0/0,size=self+420,550,5,0);open(self+40,1);begin(ui);"
                                   "draw(self+40);gs(ui,955,2);box(self+40,t59,20,0,440,3,0,8);gs(ui,969,2);gs(ui,896,2);prompt(self+0,4,t9,t0);end(ui);");
    bad += w[104] != 1 || w[102] != 1;

    hl_reset(true, 0, 6);
    ProcessScreen15__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += w[104] != 7 || w[102] != 0;

    for (int action : {1, 2, 4, -1}) {
        hl_reset(true, 0, 5);
        ProcessScreen15__10VIWndMcErriii(hl_self, action, 134, 6);
        bad += hl_check("ignored action", HEAD + "{" + std::to_string(action) + "}{134}{6}");
    }
    hl_reset(true, 0, 5);
    w[102] = 1;
    ProcessScreen15__10VIWndMcErriii(hl_self, 3, 136, 6);
    bad += hl_check("left", HEAD + "{3}{136}{6}");
    bad += w[102] != 0;
    ProcessScreen15__10VIWndMcErriii(hl_self, 3, 137, 6);
    bad += w[102] != 1;
    ProcessScreen15__10VIWndMcErriii(hl_self, 3, 135, 6);
    bad += w[102] != 1;

    hl_reset(true, 0, 17);
    w[102] = 1;
    ProcessScreen15__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("confirm choice 1", HEAD + "{3}{134}{6}P[VIWndMcErr::GotoScreen %d\n]{4}");
    bad += w[103] != 4 || w[102] != 0 || w[104] != 0;

    hl_reset(true, 0, 17);
    ProcessScreen15__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("confirm choice 0", HEAD + "{3}{134}{6}P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{3}msg:parent+16,self+0,4354,0,0;msg:parent+16,self+0,4358,3,0;");
    hl_reset(false, 0, 17);
    ProcessScreen15__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("confirm choice 0, no parent", HEAD + "{3}{134}{6}P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{3}");
    bad += w[109] != 3;
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
