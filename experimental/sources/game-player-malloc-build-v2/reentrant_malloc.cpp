#include "reentrant_malloc.hpp"
using namespace norrath_malloc_detail;

extern "C" unsigned char* norrath_player_malloc_reentrant(unsigned char* reent,Word requested) {
    const Word biased=requested+19u;
    const Word required=biased<31u?16u:biased&0xfffffff0u;
    norrath_player_malloc_lock(reent);
    const Word bins=address(norrath_player_malloc_bins_view),remainder_head=bins+8u;
    auto finish=[&](Word chunk)->unsigned char* {
        norrath_player_malloc_unlock(reent);return bytes(chunk+8u);
    };
    auto take_whole=[&](Word chunk,Word size)->unsigned char* {
        const Word back=read_word(chunk+12u),after=chunk+size;
        const Word header=read_word(after+4u),front=read_word(chunk+8u);
        write_word(after+4u,header|1u);
        write_word(front+12u,back);write_word(back+8u,front);
        return finish(chunk);
    };
    auto split=[&](Word chunk,Word remainder,bool unlink)->unsigned char* {
        if(unlink) {
            const Word back=read_word(chunk+12u),front=read_word(chunk+8u);
            write_word(front+12u,back);write_word(back+8u,front);
        }
        const Word rest=chunk+required,footer=rest+remainder;
        write_word(chunk+4u,required|1u);
        write_word(remainder_head+12u,rest);write_word(remainder_head+8u,rest);
        write_word(rest+4u,remainder|1u);write_word(rest+8u,remainder_head);
        write_word(footer,remainder);write_word(rest+12u,remainder_head);
        return finish(chunk);
    };
    Word index;
    if(required<504u) {
        const Word head=bins+required;
        const Word chunk=read_word(head+12u);
        index=required>>3;
        if(chunk!=head) {
            const Word size=read_word(chunk+4u)&0xfffffffcu;
            const Word back=read_word(chunk+12u),front=read_word(chunk+8u),after=chunk+size;
            const Word header=read_word(after+4u);
            // The native small-bin path unlinks BEFORE publishing in-use.
            write_word(front+12u,back);write_word(back+8u,front);write_word(after+4u,header|1u);
            return finish(chunk);
        }
        index+=2u;
    } else {
        index=bin_index(required);
        const Word head=bins+index*8u;
        Word chunk=read_word(head+12u);
        for(;;) {
            if(chunk==head) {index+=1u;break;}
            const Word size=read_word(chunk+4u)&0xfffffffcu;
            const NorrathFormatSigned64 difference=size_difference(size,required);
            if(difference>=16)break;
            if(difference>=0)return take_whole(chunk,size);
            chunk=read_word(chunk+12u);
        }
    }
    const Word old_remainder=read_word(remainder_head+8u);
    if(old_remainder!=remainder_head) {
        const Word size=read_word(old_remainder+4u)&0xfffffffcu;
        const NorrathFormatSigned64 difference=size_difference(size,required);
        if(difference>=16)return split(old_remainder,Word(difference),false);
        write_word(remainder_head+12u,remainder_head);write_word(remainder_head+8u,remainder_head);
        if(difference>=0) {
            const Word after=old_remainder+size;
            const Word header=read_word(after+4u);
            write_word(after+4u,header|1u);return finish(old_remainder);
        }
        insert_old_remainder(bins,old_remainder,size);
    }

    Wide bit=1ull<<(signed_group_shift(index)&63u);
    Wide bitmap=Wide(read_word(bins+4u));
    if(bitmap>=bit) {
        if((bit&bitmap)==0u) {
            bit<<=1u;index=(index&0xfffffffcu)+4u;
            if((bit&bitmap)==0u) {
                do {bit<<=1u;index+=4u;}while((bit&bitmap)==0u);
            }
        }
        for(;;) {
            Word start_index=index,start_head=bins+index*8u,head=start_head;
            for(;;) {
                Word chunk=read_word(head+12u);
                while(chunk!=head) {
                    const Word size=read_word(chunk+4u)&0xfffffffcu;
                    const NorrathFormatSigned64 difference=size_difference(size,required);
                    if(difference>=16)return split(chunk,Word(difference),true);
                    if(difference>=0)return take_whole(chunk,size);
                    chunk=read_word(chunk+12u);
                }
                head+=8u;
                if(signed_word(index)<63) {head+=8u;index+=1u;}
                index+=1u;
                if((index&3u)==0u)break;
            }
            const Word inverse=~Word(bit);
            for(;;) {
                const Word phase=start_index&3u;start_index-=1u;
                if(phase==0u) {write_word(bins+4u,read_word(bins+4u)&inverse);break;}
                start_head-=8u;
                if(read_word(start_head+8u)!=start_head)break;
            }
            bitmap=Wide(read_word(bins+4u));bit<<=1u;
            if(bitmap<bit||bit==0u)break;
            if((bit&bitmap)!=0u)continue;
            bitmap=Wide(read_word(bins+4u));
            do {bit<<=1u;index+=4u;}while((bit&bitmap)==0u);
        }
    }

    Word top=read_word(bins+8u),top_size=read_word(top+4u)&0xfffffffcu;
    NorrathFormatSigned64 difference=size_difference(top_size,required);
    top=read_word(bins+8u);top_size=read_word(top+4u)&0xfffffffcu;
    if(top_size<required||difference<16) {
        norrath_player_malloc_extend_top(reent,required);
        top=read_word(bins+8u);top_size=read_word(top+4u)&0xfffffffcu;
        difference=size_difference(top_size,required);
        top=read_word(bins+8u);top_size=read_word(top+4u)&0xfffffffcu;
        if(top_size<required||difference<16) {
            norrath_player_malloc_unlock(reent);return nullptr;
        }
    }
    const Word chunk=read_word(bins+8u),rest=chunk+required;
    write_word(chunk+4u,required|1u);write_word(bins+8u,rest);
    write_word(rest+4u,Word(difference)|1u);
    return finish(chunk);
}
