// Host check only; not a build input. Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
#define NORRATH_HOST_CONTRACT 1
#include "screen04.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen04(Action = %d, Param1 = %d, Param2 = %d)\n]{0}{5}{6}";
static const std::string DRAW =
    "gs(ui,951,2);cl(self+40,t55,460);init(self+40,ui,pos=0/0,size=self+420,500,3,0);open(self+40,1);begin(ui);draw(self+40);"
    "gs(ui,951,2);box(self+40,t55,20,0,440,3,0,8);";

int main() {
    int bad = 0;
    int* w = hl_words();
    hl_status_count = 1;

    hl_reset(true, 0, 0);
    ProcessScreen04__10VIWndMcErriii(hl_self, 1, 2, 3);
    bad += hl_check("other action", "P[VIWndMcErr::ProcessScreen04(Action = %d, Param1 = %d, Param2 = %d)\n]{1}{2}{3}");
    bad += w[104] != 0;

    hl_reset(true, 0, 5);
    ProcessScreen04__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("draw only", HEAD + DRAW + "end(ui);");
    bad += w[104] != 6;

    for (int counter : {38, 40, 41}) {
        hl_reset(true, 1, counter);
        hl_status[0] = 0;
        ProcessScreen04__10VIWndMcErriii(hl_self, 0, 5, 6);
        bad += hl_check("counter not 40", HEAD + DRAW + "end(ui);");
        bad += w[104] != counter + 1;
    }

    hl_reset(true, 0, 39);
    hl_status[0] = 3;
    ProcessScreen04__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("exit with parent", HEAD + DRAW + "status(0);P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{1}msg:parent+16,self+0,4354,0,0;"
                                                      "msg:parent+16,self+0,4358,1,0;end(ui);");
    bad += w[104] != 40 || w[109] != 0;

    hl_reset(false, 0, 39);
    hl_status[0] = 0;
    ProcessScreen04__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("exit no parent", HEAD + DRAW + "status(0);P[VIWndMcErr::ExitMcErr(Rval = %d)\n]{1}end(ui);");
    bad += w[109] != 1;

    struct Go { int status, mode, screen; };
    const Go go[] = {{1, 1, 12}, {1, 0, 10}, {2, 1, 17}, {2, 0, 11}, {4, 1, 17}, {4, 2, 11}, {5, 1, 12}, {5, 0, 10}};
    for (const Go& g : go) {
        hl_reset(true, g.mode, 39);
        hl_status[0] = g.status;
        ProcessScreen04__10VIWndMcErriii(hl_self, 0, 5, 6);
        bad += hl_check("goto", HEAD + DRAW + "status(0);P[VIWndMcErr::GotoScreen %d\n]{" + std::to_string(g.screen) + "}end(ui);");
        bad += w[103] != g.screen || w[102] != 0 || w[104] != 0;
    }
    for (int status : {6, 7, -1}) {
        hl_reset(true, 1, 39);
        hl_status[0] = status;
        ProcessScreen04__10VIWndMcErriii(hl_self, 0, 5, 6);
        bad += hl_check("other status", HEAD + DRAW + "status(0);end(ui);");
        bad += w[104] != 40 || w[103] != 0;
    }
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
