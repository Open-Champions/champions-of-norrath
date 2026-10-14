// Host check only; not a build input. Uses ../w15-fe-mcerr-a-mcerr-host/hostlib.hpp (recording stubs).
#define NORRATH_HOST_CONTRACT 1
#include "screen62.cpp"
#include "../w15-fe-mcerr-a-mcerr-host/hostlib.hpp"

static int prepare_result, pad_result, idle_result, write_result, save_result;
extern "C" {
int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    trace += std::string("printf[") + format + "]{" + std::to_string(va_arg(args, int)) + "}";
    va_end(args);
    return 0;
}
int VIMemCardPrepare__Fi(int value) { trace += "prepare(" + std::to_string(value) + ");"; return prepare_result; }
int VIMemCardPadAsync__Fi(int value) { trace += "pad(" + std::to_string(value) + ");"; return pad_result; }
int VIMemCardAsyncIdle__Fv() { trace += "idle;"; return idle_result; }
static char product_id[4];
const char* VIMemCardProductId__Fv() { trace += "productid;"; return product_id; }
int VIMemCardWriteFile__FPCcPCvi(const char* name, const void* data, int size) {
    trace += std::string("write(") + (name == product_id ? "id" : "?") + "," + static_cast<const char*>(data) + "," + std::to_string(size) + ");";
    return write_result;
}
static int options_object;
void* VIGetGameOptions__Fv() { trace += "options;"; return &options_object; }
int Save__13VIGameOptions(void* options) { trace += std::string("save(") + (options == &options_object ? "opts" : "?") + ");"; return save_result; }
}

static const std::string HEAD = "P[VIWndMcErr::ProcessScreen62(Action = %d, Param1 = %d, Param2 = %d)\n]{0}{5}{6}";
static const std::string TOP = "gs(ui,968,2);cl(self+40,t8,510);init(self+40,ui,pos=0/0,size=self+420,550,4,0);begin(ui);draw(self+40);";
static const std::string TAIL_BOX = "gs(ui,968,2);box(self+40,t8,20,0,440,3,0,8);";
static std::string tail(const char* bits) { return TAIL_BOX + "progress(self+40,40,3,400," + bits + ");end(ui);"; }
// Independent statement of the progress rule: 0 below six frames, else clamp(count / 640, 0, 1) * 0.9 + 0.1
// with the float constants of the image (0x3acccccc, 0x3f666666, 0x3dcccccc).
static std::string bits_for(int count) {
    auto f = [](unsigned b) { float v; std::memcpy(&v, &b, 4); return v; };
    float p = 0.0f;
    if (count >= 6) {
        float x = static_cast<float>(count) * f(0x3acccccc);
        x = x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x);
        p = x * f(0x3f666666) + f(0x3dcccccc);
    }
    unsigned b;
    std::memcpy(&b, &p, 4);
    char buffer[16];
    std::snprintf(buffer, sizeof buffer, "0x%08x", b);
    return buffer;
}

int main() {
    int bad = 0;
    int* w = hl_words();
    hl_status_count = 1;
    hl_lines = 3;

    hl_reset(true, 0, 0);
    ProcessScreen62__10VIWndMcErriii(hl_self, 3, 5, 6);
    bad += hl_check("other action", "P[VIWndMcErr::ProcessScreen62(Action = %d, Param1 = %d, Param2 = %d)\n]{3}{5}{6}");

    // frames 1..4 before the card check: nothing but drawing and a zero progress.
    hl_reset(true, 0, 2);
    ProcessScreen62__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("early", HEAD + TOP + tail(bits_for(3).c_str()));
    bad += w[104] != 3;

    // frame 5: the card checks.
    hl_reset(true, 0, 4);
    hl_status[0] = 2;
    ProcessScreen62__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("busy card", HEAD + TOP + "status(0);P[VIWndMcErr::GotoScreen %d\n]{64}" + tail("0x00000000"));
    bad += w[103] != 64 || w[104] != 0;

    struct Chk { int card, prepare, pad; const char* trace; int screen; };
    const Chk chk[] = {
        {100, 0, 0, "status(0);status(ptr);P[VIWndMcErr::GotoScreen %d\n]{60}", 60},
        {2999, 0, 0, "status(0);status(ptr);P[VIWndMcErr::GotoScreen %d\n]{60}", 60},
        {3000, -1, 0, "status(0);status(ptr);prepare(-1);P[VIWndMcErr::GotoScreen %d\n]{64}", 64},
        {3000, 0, -1, "status(0);status(ptr);prepare(-1);pad(2996);P[VIWndMcErr::GotoScreen %d\n]{64}", 64},
        {3000, 0, 0, "status(0);status(ptr);prepare(-1);pad(2996);", 0},
    };
    for (const Chk& c : chk) {
        hl_reset(true, 0, 4);
        hl_status[0] = 0;
        hl_card = c.card;
        prepare_result = c.prepare;
        pad_result = c.pad;
        ProcessScreen62__10VIWndMcErriii(hl_self, 0, 5, 6);
        bad += hl_check("card check", HEAD + TOP + c.trace + tail(bits_for(c.screen ? 0 : 5).c_str()));
        bad += c.screen ? w[103] != c.screen : w[103] != 0;
    }

    // frame 6 and later: asynchronous work.
    for (int idle : {0, 2, -2}) {
        hl_reset(true, 0, 5);
        idle_result = idle;
        ProcessScreen62__10VIWndMcErriii(hl_self, 0, 5, 6);
        bad += hl_check("busy", HEAD + TOP + "idle;" + tail(bits_for(6).c_str()));
        bad += w[104] != 6;
    }
    hl_reset(true, 0, 5);
    idle_result = -1;
    ProcessScreen62__10VIWndMcErriii(hl_self, 0, 5, 6);
    bad += hl_check("failed", HEAD + TOP + "idle;P[VIWndMcErr::GotoScreen %d\n]{64}" + tail("0x00000000"));
    bad += w[103] != 64;

    struct Done { int write, save; int screen; };
    const Done done[] = {{8, 0, 63}, {-1, 0, 63}, {8, -3, 64}, {-1, -3, 64}};
    for (const Done& d : done) {
        hl_reset(true, 0, 5);
        idle_result = 1;
        write_result = d.write;
        save_result = d.save;
        ProcessScreen62__10VIWndMcErriii(hl_self, 0, 5, 6);
        std::string want = HEAD + TOP + "idle;printf[FINISHED: %d\n]{6}productid;write(id,12345678,8);P[VIWndMcErr::GotoScreen %d\n]{" +
                           std::to_string(d.write < 0 ? 64 : 63) + "}options;save(opts);P[VIWndMcErr::GotoScreen %d\n]{" + std::to_string(d.screen) + "}" +
                           tail(bits_for(0).c_str());
        bad += hl_check("done", want);
        bad += w[103] != d.screen || w[104] != 0 || w[102] != 0;
    }

    // progress values: the bar is drawn from the count after any screen change.
    for (int count : {5, 6, 7, 100, 320, 639, 640, 641, 5000, -9}) {
        hl_reset(true, 0, count - 1);
        idle_result = 0;
        hl_status[0] = 0;
        hl_card = 3000;
        prepare_result = 0;
        pad_result = 0;
        ProcessScreen62__10VIWndMcErriii(hl_self, 0, 5, 6);
        std::string want = HEAD + TOP;
        if (count == 5) want += "status(0);status(ptr);prepare(-1);pad(2996);";
        if (count >= 6) want += "idle;";
        bad += hl_check("progress", want + tail(bits_for(count).c_str()));
    }
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
