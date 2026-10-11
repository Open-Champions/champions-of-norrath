#include "sound_memory_lifecycle.hpp"
using namespace norrath_sound_memory_lifecycle_detail;

extern "C" void norrath_sound_assert(const char* file,unsigned line,const char* condition,int captured_ra) {
    const char* format=text_at(address(norrath_assert_format_view));
    for(unsigned repetition=0;repetition<3u;++repetition) {
        const int context=signed_word(read_word_fresh(address(norrath_assert_context_word_view)));
        norrath_sce_printf(format,condition,file,line,context,captured_ra);
    }
    native_store_zero_word(0u);
}

