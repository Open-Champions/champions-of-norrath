#include "sound_cache_writeback.hpp"
using namespace norrath_sound_cache_writeback_detail;
extern "C" void norrath_sound_write_back_data_cache(void* address,int byte_count) {
    const Word first=address_word(address);
    // The original nonpositive-count branch delay performs the wrapped end
    // addition. It has no cache/memory effect or promised incidental return.
    const Word end=first+static_cast<Word>(byte_count);
    if(byte_count<=0)return;
    Word cursor=first&0xffffffc0u;
    const Word aligned_last=(end-1u)&0xffffffc0u;
    const Word span=aligned_last-cursor;
    const Word line_count=(span>>6)+1u;
    Word remainder=line_count&7u;
    Word blocks=line_count>>3;

    if(remainder!=0u) {
        do {
            const Raw64 native_base=canonical_line_operand(cursor);
            native_sync();
            native_cache_writeback<0>(native_base);
            native_sync();
            native_remainder_nop();
            --remainder;
            cursor+=64u;
        } while(remainder!=0u);
    }
    // Original zero-block branch also decrements its private loop register in
    // the delay slot; no cache effect or void-return promise depends on it.
    if(blocks!=0u) {
        do {
            --blocks;
            const Raw64 native_base=canonical_line_operand(cursor);
            native_sync();
            native_cache_writeback<0>(native_base);
            native_sync();
            native_cache_writeback<64>(native_base);
            native_sync();
            native_cache_writeback<128>(native_base);
            native_sync();
            native_cache_writeback<192>(native_base);
            native_sync();
            native_cache_writeback<256>(native_base);
            native_sync();
            native_cache_writeback<320>(native_base);
            native_sync();
            native_cache_writeback<384>(native_base);
            native_sync();
            native_cache_writeback<448>(native_base);
            native_sync();
            cursor+=512u;
        } while(blocks!=0u);
    }
    // The extra original trailing JR after reachable JR/NOP has no rooted
    // predecessor. Do not insert opaque source merely to manufacture bytes.
}
