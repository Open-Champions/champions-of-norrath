// Host contract for GetSaveFootPrint__6Camera. Not an EE plan input.
#include "get_save_foot_print.cpp"

static void* seen_camera;
static char* seen_buffer;
static int calls;
extern "C" int Save__6CameraPc(void* camera, char* buffer) {
    seen_camera = camera; seen_buffer = buffer; ++calls;
    buffer[0] = 1; buffer[16383] = 2;  // the whole buffer must be addressable
    return 1234;
}

int main() {
    int camera = 0;
    int r = GetSaveFootPrint__6Camera(&camera);
    return (r == 1234 && calls == 1 && seen_camera == &camera && seen_buffer != nullptr) ? 0 : 1;
}
