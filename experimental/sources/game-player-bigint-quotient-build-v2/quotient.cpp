#include "quotient.hpp"
using namespace norrath_bigint_quotient_detail;

extern "C" int norrath_player_bigint_quotient(unsigned char* numerator,unsigned char* divisor) {
    const Word n=address(numerator),d=address(divisor);
    Word last_index=read_word(d+16u);
    const Word numerator_count=read_word(n+16u);
    if(signed_word(numerator_count)<signed_word(last_index))return 0;
    --last_index;
    const Word divisor_begin=d+20u,numerator_begin=n+20u;
    const Word divisor_last=divisor_begin+last_index*4u;
    const Word numerator_last=numerator_begin+last_index*4u;
    const Word divisor_top=read_word(divisor_last),captured_numerator_top=read_word(numerator_last);
    const Word initial_quotient=native_divide_or_break(captured_numerator_top,divisor_top+1u);
    Word quotient=initial_quotient;
    auto trim=[&](Word last) {
        last-=4u;
        while(numerator_begin<last) {
            if(read_word(last)!=0u)break;
            last-=4u;--last_index;
        }
        write_word(n+16u,last_index);
    };
    if(quotient!=0u) {
        Word input=divisor_begin,output=numerator_begin,borrow=0u,carry=0u;
        do {
            const Word a=read_word(input);input+=4u;
            const Word b=read_word(output);
            const Word product_low=(a&0xffffu)*quotient+carry;
            const Word product_high=(a>>16)*quotient+(product_low>>16);
            const Word lower=(b&0xffffu)-(product_low&0xffffu)+borrow;
            const Word upper=(b>>16)-(product_high&0xffffu)+arithmetic_right16(lower);
            write_half(output,lower);write_half(output+2u,upper);
            output+=4u;carry=product_high>>16;borrow=arithmetic_right16(upper);
        } while(!(divisor_last<input));
        // The native first trim tests original T5, NOT a reloaded result limb.
        if(captured_numerator_top==0u)trim(numerator_last);
    }
    if(norrath_player_bigint_compare(numerator,divisor)<0)return signed_word(quotient);
    quotient=initial_quotient+1u;
    Word input=divisor_begin,output=numerator_begin,borrow=0u,carry=0u;
    do {
        const Word a=read_word(input);input+=4u;
        const Word b=read_word(output);
        const Word product_low=(a&0xffffu)+carry;
        const Word product_high=(a>>16)+(product_low>>16);
        const Word lower=(b&0xffffu)-(product_low&0xffffu)+borrow;
        const Word upper=(b>>16)-(product_high&0xffffu)+arithmetic_right16(lower);
        write_half(output,lower);write_half(output+2u,upper);
        output+=4u;carry=product_high>>16;borrow=arithmetic_right16(upper);
    } while(!(divisor_last<input));
    const Word fresh_last=numerator_begin+last_index*4u;
    if(read_word(fresh_last)==0u)trim(fresh_last);
    return signed_word(quotient);
}
