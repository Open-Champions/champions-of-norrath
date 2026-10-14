// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp lgaud_state.cpp
// The initial values must match the image: zero, zero, zero and -1 (0xffffffff). The sizes are the
// 4-byte words of the original; pointers are checked only for being null.
extern "C" {
extern int norrath_lgaud_size_limit_view;
extern void* norrath_lgaud_request_buffer_view;
extern void* norrath_lgaud_reply_words_view;
extern int norrath_lgaud_semaphore_view;
}
static_assert(sizeof(norrath_lgaud_size_limit_view) == 4 && sizeof(norrath_lgaud_semaphore_view) == 4, "word sizes");
static_assert(sizeof(void*) != 4 || sizeof(norrath_lgaud_request_buffer_view) == 4, "pointer words are 4 bytes on the target");
int main() {
    if (norrath_lgaud_size_limit_view != 0) return 1;
    if (norrath_lgaud_request_buffer_view != nullptr) return 2;
    if (norrath_lgaud_reply_words_view != nullptr) return 3;
    if (norrath_lgaud_semaphore_view != -1) return 4;
    if (static_cast<unsigned>(norrath_lgaud_semaphore_view) != 0xffffffffu) return 5;
    return 0;
}
