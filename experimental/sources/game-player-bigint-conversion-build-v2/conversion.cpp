#include "bigint_conversion.hpp"
using namespace norrath_bigint_conversion_detail;

extern "C" unsigned char* norrath_player_bigint_from_double(unsigned char* reent,Wide value,unsigned char* exponent_storage,unsigned char* bit_count_storage) {
    unsigned char* allocation=norrath_player_bigint_allocate(reent,1);
    const Word output=address(allocation),limbs=output+20u;
    const Wide magnitude=value&0x7fffffffffffffffull;
    const Word exponent=Word(magnitude>>52);
    unsigned char workspace[8];
    const Word scratch=address(workspace);
    Word high_fraction=high(value)&0x000fffffu;
    write_word(scratch+4u,high_fraction);
    if(exponent!=0u)write_word(scratch+4u,high_fraction|0x00100000u);
    Word shift,count;
    if(Word(magnitude)!=0u) {
        write_word(scratch,Word(magnitude));
        shift=Word(norrath_player_bigint_trailing_zeros(workspace));
        const Word captured_high=read_word(scratch+4u);
        if(shift!=0u) {
            const Word low=read_word(scratch)|(captured_high<<((0u-shift)&31u));
            write_word(limbs,low);
            const Word fresh_high=read_word(scratch+4u);
            write_word(scratch+4u,fresh_high>>(shift&31u));
        } else write_word(limbs,read_word(scratch));
        const Word remaining=read_word(scratch+4u);
        count=remaining==0u?1u:2u;
        write_word(limbs+4u,remaining);write_word(output+16u,count);
    } else {
        shift=Word(norrath_player_bigint_trailing_zeros(workspace+4u))+32u;count=1u;
        write_word(limbs,read_word(scratch+4u));write_word(output+16u,1u);
    }
    Word bits;
    if(exponent!=0u) {
        bits=53u-shift;
        write_word(address(exponent_storage),exponent+shift-1075u);
    } else {
        write_word(address(exponent_storage),shift-1074u);
        // The published exponent may alias a limb: this read is genuinely fresh.
        const Word last=read_word(limbs+count*4u-4u);
        const Word leading=Word(norrath_player_bigint_leading_zeros(last));
        bits=count*32u-leading;
    }
    write_word(address(bit_count_storage),bits);return allocation;
}

extern "C" Wide norrath_player_bigint_to_double(unsigned char* block,unsigned char* bit_count_storage) {
    const Word input=address(block),begin=input+20u;
    Word last=begin+read_word(input+16u)*4u-4u;
    const Word top=read_word(last);
    Word leading=Word(norrath_player_bigint_leading_zeros(top));
    write_word(address(bit_count_storage),32u-leading);
    Word upper,lower;
    if(signed_word(leading)<11) {
        const Word shift=11u-leading;
        upper=(top>>(shift&31u))|0x3ff00000u;
        const Word preceding=begin<last?read_word(last-4u):0u;
        lower=(top<<((leading+21u)&31u))|(preceding>>(shift&31u));
    } else {
        Word preceding=0u;
        if(begin<last){last-=4u;preceding=read_word(last);}
        leading-=11u;
        if(leading!=0u) {
            upper=(top<<(leading&31u))|(preceding>>((0u-leading)&31u))|0x3ff00000u;
            const Word next=begin<last?read_word(last-4u):0u;
            lower=(preceding<<(leading&31u))|(next>>((0u-leading)&31u));
        } else {upper=top|0x3ff00000u;lower=preceding;}
    }
    // Incoming native S1 residue is completely overwritten by both final words.
    return Wide(lower)|(Wide(upper)<<32);
}

extern "C" Wide norrath_player_bigint_ratio(unsigned char* left,unsigned char* right) {
    unsigned char workspace[8];
    const Word scratch=address(workspace);
    Wide numerator=norrath_player_bigint_to_double(left,workspace);
    Wide denominator=norrath_player_bigint_to_double(right,workspace+4u);
    const Word left_count=read_word(address(left)+16u),right_count=read_word(address(right)+16u);
    const Word right_bits=read_word(scratch+4u),left_bits=read_word(scratch);
    const Word difference=left_bits-right_bits+(left_count-right_count)*32u;
    const Word adjustment=difference<<20;
    if(signed_word(difference)>0)numerator=replace_high(numerator,high(numerator)+adjustment);
    else denominator=replace_high(denominator,high(denominator)-adjustment);
    return norrath_player_double_divide(numerator,denominator);
}
