extern "C" void helpMessageFrame__Fv();
extern "C" void helpMessageInit__Fv();

static int calls;
static void (*got_callback)();
static int got_priority;
static const char* got_name;
static int got_flag;

extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag) {
    calls += 1;
    got_callback = callback;
    got_priority = priority;
    got_name = name;
    got_flag = flag;
}

extern "C" void helpMessageFrame__Fv() {}

int main() {
    helpMessageInit__Fv();
    if (calls != 1 || got_callback != helpMessageFrame__Fv) return 1;
    if (got_priority != 50 || got_flag != 1) return 2;
    const char* want = "HelpMessage";
    for (int i = 0; want[i] != 0; ++i) {
        if (got_name[i] != want[i]) return 3;
    }
    if (got_name[11] != 0) return 4;
    return 0;
}
