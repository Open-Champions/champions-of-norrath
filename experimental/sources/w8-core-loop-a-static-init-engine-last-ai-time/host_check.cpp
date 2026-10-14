// Host check for _GLOBAL_$I$engineLastAITime. Not an EE plan input.
#include "static_init_engine_last_ai_time.cpp"

int main() {
    // The original has no memory effect, so the check calls it repeatedly and confirms control returns.
    _GLOBAL_$I$engineLastAITime();
    _GLOBAL_$I$engineLastAITime();
    return 0;
}
