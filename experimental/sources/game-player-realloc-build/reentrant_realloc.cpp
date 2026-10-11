#include "reentrant_realloc.hpp"
using namespace norrath_realloc_detail;

extern "C" unsigned char* norrath_player_reallocate_reentrant(unsigned char* reent,unsigned char* allocation,Word requested) {
    if(allocation==nullptr)return norrath_player_malloc_reentrant(reent,requested);
    const Word original=address(allocation),chunk=original-8u;
    norrath_player_malloc_lock(reent);
    const Word initial_header=read_word(chunk+4u),old_size=initial_header&0xfffffffcu;
    const Word biased=requested+19u,required=biased<31u?16u:biased&0xfffffff0u;
    const Word bins=address(norrath_player_malloc_bins_view);
    auto finalize=[&](Word node,Word total,Word header,Word result)->unsigned char* {
        const Word remainder=total-required;
        if(remainder>=16u) {
            const Word rest=node+required;
            write_word(node+4u,(header&1u)|required);write_word(rest+4u,remainder|1u);
            const Word after=rest+remainder;
            const Word following_header=read_word(after+4u);
            write_word(after+4u,following_header|1u);
            norrath_player_free_reentrant(reent,bytes(rest+8u));
        } else {
            write_word(node+4u,(header&1u)|total);
            const Word after=node+total,following_header=read_word(after+4u);
            write_word(after+4u,following_header|1u);
        }
        norrath_player_malloc_unlock(reent);return bytes(result);
    };
    if(old_size>=required)return finalize(chunk,old_size,initial_header,chunk+8u);
    Word following=chunk+old_size,following_size=0u;
    bool following_available=true;
    const Word top=read_word(bins+8u);
    if(following==top)following_size=read_word(following+4u)&0xfffffffcu;
    else {
        const Word next_header=read_word(following+4u);
        const Word after=following+(next_header&0xfffffffeu);
        if(read_word(after+4u)&1u) {following=0u;following_available=false;}
        else following_size=next_header&0xfffffffcu;
    }
    if(following_available) {
        const Word combined=following_size+old_size;
        if(following==top) {
            if(combined>=required+16u) {
                const Word rest=chunk+required;
                write_word(bins+8u,rest);write_word(rest+4u,(combined-required)|1u);
                const Word fresh_header=read_word(chunk+4u);
                write_word(chunk+4u,(fresh_header&1u)|required);
                norrath_player_malloc_unlock(reent);return bytes(chunk+8u);
            }
        } else if(combined>=required) {
            const Word front=read_word(following+8u),back=read_word(following+12u);
            write_word(front+12u,back);write_word(back+8u,front);
            return finalize(chunk,combined,initial_header,chunk+8u);
        }
    }
    const Word fresh_header=read_word(chunk+4u);
    if((fresh_header&1u)==0u) {
        const Word previous=chunk-read_word(chunk);
        const Word previous_size=read_word(previous+4u)&0xfffffffcu;
        if(following!=0u) {
            const Word combined=following_size+previous_size+old_size;
            if(following==read_word(bins+8u)) {
                if(combined>=required+16u) {
                    const Word back=read_word(previous+12u),front=read_word(previous+8u);
                    write_word(front+12u,back);write_word(back+8u,front);
                    const Word destination=previous+8u;
                    copy_payload(destination,original,old_size-4u);
                    const Word rest=previous+required;
                    write_word(rest+4u,(combined-required)|1u);write_word(bins+8u,rest);
                    const Word header=read_word(previous+4u);
                    write_word(previous+4u,(header&1u)|required);
                    norrath_player_malloc_unlock(reent);return bytes(destination);
                }
            } else if(combined>=required) {
                const Word next_front=read_word(following+8u),next_back=read_word(following+12u);
                write_word(next_front+12u,next_back);write_word(next_back+8u,next_front);
                const Word back=read_word(previous+12u),front=read_word(previous+8u);
                write_word(front+12u,back);write_word(back+8u,front);
                const Word destination=previous+8u;
                copy_payload(destination,original,old_size-4u);
                return finalize(previous,combined,read_word(previous+4u),destination);
            }
        }
        const Word combined=previous_size+old_size;
        if(previous!=0u&&combined>=required) {
            const Word back=read_word(previous+12u),front=read_word(previous+8u);
            write_word(front+12u,back);write_word(back+8u,front);
            const Word destination=previous+8u;
            copy_payload(destination,original,old_size-4u);
            return finalize(previous,combined,read_word(previous+4u),destination);
        }
    }
    const Word result=address(norrath_player_malloc_reentrant(reent,requested));
    if(result==0u) {norrath_player_malloc_unlock(reent);return nullptr;}
    const Word header=read_word(chunk+4u),allocated=result-8u;
    if(allocated==chunk+(header&0xfffffffeu)) {
        const Word new_size=read_word(allocated+4u)&0xfffffffcu;
        return finalize(chunk,old_size+new_size,header,chunk+8u);
    }
    copy_payload(result,original,old_size-4u);
    norrath_player_free_reentrant(reent,allocation);
    norrath_player_malloc_unlock(reent);return bytes(result);
}
