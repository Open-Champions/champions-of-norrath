extern "C" { void* _14VIEffectVolume$mScene; }
#include "global_free.cpp"

int main() {
    int marker;
    _14VIEffectVolume$mScene = &marker;
    GlobalFree__14VIEffectVolume();
    if (_14VIEffectVolume$mScene != 0) return 1;
    GlobalFree__14VIEffectVolume();
    return _14VIEffectVolume$mScene != 0;
}
