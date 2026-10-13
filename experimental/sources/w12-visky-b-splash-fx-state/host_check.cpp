#include "splash_state.cpp"

int main() {
    if (sizeof(norrath_splash_last_anim_time) != 8) return 1;
    if (sizeof(norrath_splash_next_flock_delay) != 4 || sizeof(norrath_splash_glow_step) != 4) return 2;
    if (sizeof(norrath_splash_glow_dir) != 4 || sizeof(norrath_splash_next_bolt_delay) != 4) return 3;
    if (norrath_splash_next_flock_delay || norrath_splash_last_anim_time || norrath_splash_glow_step) return 4;
    if (norrath_splash_glow_dir || norrath_splash_next_bolt_delay) return 5;
    return 0;
}
