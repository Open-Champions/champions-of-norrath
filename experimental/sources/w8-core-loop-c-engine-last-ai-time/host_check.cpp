// Host contract for engineLastAITime. Not an EE plan input.
#include "engine_last_ai_time.cpp"

int main() {
    if (engineLastAITime != 0) return 1;
    if (sizeof engineLastAITime != 4u) return 2;
    engineLastAITime = engineLastAITime + 1;
    if (engineLastAITime != 0 + 1) return 3;
    return 0;
}
