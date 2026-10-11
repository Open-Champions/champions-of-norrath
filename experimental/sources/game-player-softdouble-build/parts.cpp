#include "softdouble.hpp"
using namespace norrath_softdouble_detail;

extern "C" Wide norrath_player_pack_double(const unsigned char* parts){
    const Word source=address(parts),category=read_word(source);
    Wide fraction=read_wide(source+16u); // Actual unconditional native LD, even for zero category.
    const Word sign=read_word(source+4u);Word exponent=0u;
    if(category<2u){fraction|=0x0008000000000000ULL;exponent=2047u;}
    else if(category==4u){exponent=2047u;fraction=0u;}
    else if(category==2u)fraction=0u;
    else if(fraction!=0u){
        const int power=signed_word(read_word(source+8u));
        if(power < -1022)fraction=0u;
        else if(power>=1024){exponent=2047u;fraction=0u;}
        else{exponent=Word(power)+1023u;fraction>>=8u;}
    }
    // Original incoming A2 residue cancels: all52 fraction bits,11 exponent
    // bits and the sign bit are replaced. No initialized seed is invented.
    return (fraction&0x000fffffffffffffULL)|(Wide(exponent&2047u)<<52)|(Wide(sign&1u)<<63);
}

extern "C" const unsigned char* norrath_player_double_add_parts(const unsigned char* left,const unsigned char* right,unsigned char* output){
    const Word l=address(left),r=address(right),out=address(output);
    const Word lc=read_word(l);if(lc<2u)return left;
    const Word rc=read_word(r);if(rc<2u)return right;
    if(lc==4u){
        if(rc==4u){const Word rsign=read_word(r+4u),lsign=read_word(l+4u);if(lsign!=rsign)return norrath_player_softdouble_nan_parts_view;}
        return left;
    }
    if(rc==4u)return right;
    if(rc==2u){
        if(lc!=2u)return left;
        // Actual ordered three8-byte copies include original unwritten padding.
        Wide value=read_wide(l);write_wide(out,value);
        value=read_wide(l+8u);write_wide(out+8u,value);
        value=read_wide(l+16u);write_wide(out+16u,value);
        const Word lsign=read_word(l+4u),rsign=read_word(r+4u);
        write_word(out+4u,lsign&rsign);return output;
    }
    if(lc==2u)return right;
    Word lexponent=read_word(l+8u),rexponent=read_word(r+8u);
    Wide lfraction=read_wide(l+16u),rfraction;
    const Word difference=lexponent-rexponent;
    const Word absolute=signed_word(difference)<0?0u-difference:difference;
    rfraction=read_wide(r+16u);
    if(signed_word(absolute)<64){
        if(signed_word(rexponent)<signed_word(lexponent)){
            Word left_bits=lexponent-rexponent;
            do{rfraction=(rfraction>>1)|(rfraction&1u);--left_bits;}while(left_bits!=0u);
            rexponent=lexponent;
        }
        if(signed_word(lexponent)<signed_word(rexponent)){
            do{++lexponent;lfraction=(lfraction>>1)|(lfraction&1u);}while(signed_word(lexponent)<signed_word(rexponent));
        }
    }else{
        if(signed_word(rexponent)<signed_word(lexponent))rfraction=0u;
        else{lexponent=rexponent;lfraction=0u;}
    }
    const Word lsign=read_word(l+4u),rsign=read_word(r+4u);
    Wide mantissa;
    if(lsign!=rsign){
        const Wide delta=lsign==0u?lfraction-rfraction:rfraction-lfraction;
        mantissa=delta;
        if((delta>>63)!=0u){
            mantissa=0u-delta;write_word(out+4u,1u);write_word(out+8u,lexponent);write_wide(out+16u,mantissa);
        }else{
            write_word(out+8u,lexponent);write_wide(out+16u,mantissa);write_word(out+4u,0u);
        }
        mantissa=read_wide(out+16u);
        while(mantissa-1u<=0x0ffffffffffffffeULL){
            const Wide shifted=mantissa<<1;
            const Word exponent=read_word(out+8u)-1u;
            write_word(out+8u,exponent);mantissa=shifted;write_wide(out+16u,shifted);
        }
    }else{
        mantissa=lfraction+rfraction;
        write_word(out+4u,lsign);write_word(out+8u,lexponent);write_wide(out+16u,mantissa);
    }
    write_word(out,3u);
    if(mantissa>0x1fffffffffffffffULL){
        const Word exponent=read_word(out+8u)+1u;
        const Wide shifted=(mantissa>>1)|(mantissa&1u);
        write_wide(out+16u,shifted);write_word(out+8u,exponent);
    }
    return output;
}

extern "C" int norrath_player_double_compare_parts(const unsigned char* left,const unsigned char* right){
    const Word l=address(left),r=address(right);
    const Word lc=read_word(l);if(lc<2u)return 1;
    const Word rc=read_word(r);if(rc<2u)return 1;
    if(lc==4u){
        if(rc==4u){const Word rsign=read_word(r+4u),lsign=read_word(l+4u);return signed_word(rsign-lsign);}
        return read_word(l+4u)==0u?1:-1;
    }
    if(rc==4u)return read_word(r+4u)==0u?-1:1;
    if(lc==2u){if(rc==2u)return 0;return read_word(r+4u)==0u?-1:1;}
    if(rc==2u)return read_word(l+4u)==0u?1:-1;
    const Word sign=read_word(l+4u),other_sign=read_word(r+4u);
    if(sign!=other_sign)return sign==0u?1:-1;
    const int exponent=signed_word(read_word(l+8u)),other_exponent=signed_word(read_word(r+8u));
    if(other_exponent<exponent)return sign==0u?1:-1;
    if(exponent<other_exponent)return sign==0u?-1:1;
    const Wide fraction=read_wide(l+16u),other_fraction=read_wide(r+16u);
    if(other_fraction<fraction)return sign==0u?1:-1;
    if(fraction<other_fraction)return sign==0u?-1:1;
    return 0;
}
