#include "dtor.cpp"

static int log_text[8];
static int log_count;
static void* freed_object;
static void* deleted_object;

extern "C" void Free__7VIFader(void* object) {
    freed_object = object;
    log_text[log_count++] = 1;
}
extern "C" void __builtin_delete(void* pointer) {
    deleted_object = pointer;
    log_text[log_count++] = 2;
}

int main() {
    int object;
    _$_7VIFader(&object, 0);
    if (log_count != 1 || log_text[0] != 1 || freed_object != &object || deleted_object) return 1;
    log_count = 0;
    _$_7VIFader(&object, 2);  // bit 0 clear
    if (log_count != 1 || deleted_object) return 2;
    log_count = 0;
    _$_7VIFader(&object, 1);
    if (log_count != 2 || log_text[0] != 1 || log_text[1] != 2 || deleted_object != &object) return 3;
    log_count = 0;
    deleted_object = nullptr;
    _$_7VIFader(&object, 3);
    if (log_count != 2 || deleted_object != &object) return 4;
    return 0;
}
