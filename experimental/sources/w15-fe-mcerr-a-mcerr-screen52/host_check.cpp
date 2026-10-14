// Host check only; not a build input. Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
#define NORRATH_HOST_CONTRACT 1
#include "screen52.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static int format_result;
extern "C" int VIMemCardFormat__Fv() {
    trace += "format;";
    return format_result;
}

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen52(Action = %d, Param1 = %d, Param2 = %d)\n]";
static const std::string DRAW =
    "gs(ui,959,2);cl(self+40,t63,540);init(self+40,ui,pos=0/0,size=self+420,580,3,0);begin(ui);draw(self+40);gs(ui,959,2);"
    "box(self+40,t63,20,0,440,3,0,8);";

int main() {
    int bad = 0;
    int* w = hl_words();

    hl_reset(true, 0, 5);
    ProcessScreen52__10VIWndMcErriii(hl_self, 3, 134, 6);
    bad += hl_check("other action", HEAD + "{3}{134}{6}");
    hl_reset(true, 0, 5);
    ProcessScreen52__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("draw", HEAD + "{0}{5}{6}" + DRAW + "end(ui);");
    bad += w[104] != 6;
    for (int counter : {7, 10, 11}) {
        hl_reset(true, 0, counter);
        ProcessScreen52__10VIWndMcErriii(hl_self, 0, 5, 6);
        bad += hl_check("not 10", HEAD + "{0}{5}{6}" + DRAW + "end(ui);");
    }
    for (int result : {-1, -9}) {
        hl_reset(true, 0, 9);
        format_result = result;
        ProcessScreen52__10VIWndMcErriii(hl_self, 0, 5, 6);
        bad += hl_check("failed", HEAD + "{0}{5}{6}" + DRAW + "format;P[VIWndMcErr::GotoScreen %d\n]{54}end(ui);");
        bad += w[103] != 54 || w[102] != 0 || w[104] != 0;
    }
    for (int result : {0, 5}) {
        hl_reset(true, 0, 9);
        format_result = result;
        ProcessScreen52__10VIWndMcErriii(hl_self, 0, 5, 6);
        bad += hl_check("formatted", HEAD + "{0}{5}{6}" + DRAW + "format;P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{1}msg:parent+16,self+0,4354,0,0;"
                                                                 "msg:parent+16,self+0,4358,1,0;end(ui);");
        hl_reset(false, 0, 9);
        format_result = result;
        ProcessScreen52__10VIWndMcErriii(hl_self, 0, 5, 6);
        bad += hl_check("formatted, no parent", HEAD + "{0}{5}{6}" + DRAW + "format;P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{1}end(ui);");
        bad += w[109] != 1;
    }
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
