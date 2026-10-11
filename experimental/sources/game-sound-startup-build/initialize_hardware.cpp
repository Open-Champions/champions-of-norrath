#include "sound_startup.hpp"
using namespace norrath_sound_startup_detail;

extern "C" void norrath_sound_initialize_hardware() {
    (void)norrath_sound_remote(1,0x8000,0);
    (void)norrath_sound_remote(1,0x8010,0x0980,0x3fff);
    const float captured_scale=read_float_fresh(address(norrath_sound_master_scale_view));
    (void)norrath_sound_remote(1,0x8010,0x0a80,0x3fff);
    const Address master=address(norrath_sound_master_volume_view);
    const float first_master=read_float_fresh(master);
    const unsigned first_word=native_convert_word(native_multiply_ordered(first_master,captured_scale));
    (void)norrath_sound_remote(1,0x8010,0x0981,signed_word(first_word));
    const float second_master=read_float_fresh(master);
    const unsigned second_word=native_convert_word(native_multiply_ordered(second_master,captured_scale));
    (void)norrath_sound_remote(1,0x8010,0x0a81,signed_word(second_word));
    for(int core=0;core<2;++core){
        (void)norrath_sound_remote(1,0x8010,core|0x0b80,0);
        (void)norrath_sound_remote(1,0x8010,core|0x0c80,0);
        (void)norrath_sound_remote(1,0x8010,core|0x0f80,0);
        (void)norrath_sound_remote(1,0x8010,core|0x1080,0);
        if(core==1){
            (void)norrath_sound_remote(1,0x8010,0x0d81,0x7fff);
            (void)norrath_sound_remote(1,0x8010,0x0e81,0x7fff);
        }
        (void)norrath_sound_remote(1,0x8030,core|0x1300,0);
        (void)norrath_sound_remote(1,0x8030,core|0x1400,0);
    }
    (void)norrath_sound_remote(1,0x8010,0x0800,0x0c00);
    (void)norrath_sound_remote(1,0x8010,0x0801,0x0c0c);
}
