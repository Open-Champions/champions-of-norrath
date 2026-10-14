// Host check only; not a build input. Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
#define NORRATH_HOST_CONTRACT 1
#include "screen30.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static int prepared_result;
extern "C" int VIMemCardIsPrepared__Fv() {
    trace += "prepared;";
    return prepared_result;
}
extern "C" int printf(const char* format, ...) {
    trace += std::string("printf[") + format + "]";
    return 0;
}

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen30(Action = %d, Param1 = %d, Param2 = %d)\n]";
static const std::string DRAW =
    "gs(ui,951,2);cl(self+40,t55,460);init(self+40,ui,pos=0/0,size=self+420,500,3,0);begin(ui);draw(self+40);gs(ui,951,2);"
    "box(self+40,t55,20,0,440,256,0,8);";

int main() {
    int bad = 0;
    int* w = hl_words();
    hl_status_count = 1;

    hl_reset(true, 0, 5);
    ProcessScreen30__10VIWndMcErriii(hl_self, 1, 5, 6);
    bad += hl_check("other action", HEAD + "{1}{5}{6}");
    hl_reset(true, 0, 5);
    ProcessScreen30__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("draw", HEAD + "{0}{5}{6}" + DRAW + "end(ui);");
    bad += w[104] != 6;
    for (int counter : {73, 75, 76}) {
        hl_reset(true, 0, counter);
        hl_status[0] = 0;
        ProcessScreen30__10VIWndMcErriii(hl_self, 0, 5, 6);
        bad += hl_check("counter", HEAD + "{0}{5}{6}" + DRAW + "end(ui);");
    }
    struct Go { int status, prepared, card, screen; bool unformatted; };
    const Go go[] = {{0, 1, 5000, 63, false}, {3, 1, 0, 63, false}, {0, 0, 2999, 60, false}, {3, 2, 3000, 61, false}, {0, 0, -7, 60, false},
                     {1, 0, 0, 20, false}, {5, 0, 0, 20, false}, {2, 0, 0, 50, true}, {4, 0, 0, 50, true}};
    for (const Go& g : go) {
        hl_reset(true, 0, 74);
        hl_status[0] = g.status;
        prepared_result = g.prepared;
        hl_card = g.card;
        ProcessScreen30__10VIWndMcErriii(hl_self, 0, 5, 6);
        std::string want = HEAD + "{0}{5}{6}" + DRAW + "status(ptr);";
        if (g.status == 0 || g.status == 3) want += "prepared;";
        if (g.unformatted) want += "printf[NOT FORMATTED\n]";
        want += "P[VIWndMcErr::GotoScreen %d\n]{" + std::to_string(g.screen) + "}end(ui);";
        bad += hl_check("status", want);
        bad += w[103] != g.screen || w[102] != 0 || w[104] != 0;
    }
    for (int status : {6, 7, -1}) {
        hl_reset(true, 0, 74);
        hl_status[0] = status;
        ProcessScreen30__10VIWndMcErriii(hl_self, 0, 5, 6);
        bad += hl_check("other status", HEAD + "{0}{5}{6}" + DRAW + "status(ptr);end(ui);");
        bad += w[104] != 75 || w[103] != 0;
    }
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
