#include "load_thread_param.cpp"
int main() {
    static_assert(sizeof(loadThreadParam) == 48, "gap to modulePresent");
    for (unsigned word : loadThreadParam) if (word != 0) return 1;
    return 0;
}
