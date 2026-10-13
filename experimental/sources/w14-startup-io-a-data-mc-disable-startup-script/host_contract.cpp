#include "mc_disable_startup_script.cpp"
int main() {
    static_assert(sizeof(mcDisableStartupScript) == 1, "one byte");
    if (mcDisableStartupScript != 0) return 1;
    mcDisableStartupScript = -1;
    return mcDisableStartupScript < 0 ? 0 : 2;
}
