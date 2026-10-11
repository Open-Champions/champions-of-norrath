#include "softdouble.hpp"
using namespace norrath_softdouble_detail;

extern "C" Wide norrath_player_double_add(Wide left,Wide right){
    alignas(16) unsigned char workspace[96];alignas(8) unsigned char raw[16];
    unpack_pair(left,right,raw,workspace);
    const unsigned char* chosen=norrath_player_double_add_parts(workspace,workspace+32,workspace+64);
    return norrath_player_pack_double(chosen);
}
extern "C" Wide norrath_factory_double_subtract(Wide left,Wide right){
    alignas(16) unsigned char workspace[96];alignas(8) unsigned char raw[16];
    unpack_pair(left,right,raw,workspace);
    const Word second=address(workspace)+32u;
    write_word(second+4u,read_word(second+4u)^1u);
    const unsigned char* chosen=norrath_player_double_add_parts(workspace,workspace+32,workspace+64);
    return norrath_player_pack_double(chosen);
}
extern "C" int norrath_factory_double_compare(Wide left,Wide right){
    alignas(16) unsigned char workspace[64];alignas(8) unsigned char raw[16];
    unpack_pair(left,right,raw,workspace);
    return norrath_player_double_compare_parts(workspace,workspace+32);
}

extern "C" Wide norrath_player_double_multiply(Wide left,Wide right){
    alignas(16) unsigned char workspace[96];alignas(8) unsigned char raw[16];
    unpack_pair(left,right,raw,workspace);
    const Word l=address(workspace),r=l+32u,out=l+64u;
    const Word lc=read_word(l);
    if(lc<2u)return exceptional_product(l,l,r);
    const Word rc=read_word(r);
    if(rc<2u)return exceptional_product(r,l,r);
    if(lc==4u){if(rc==2u)return norrath_player_pack_double(norrath_player_softdouble_nan_parts_view);return exceptional_product(l,l,r);}
    if(rc==4u){if(lc==2u)return norrath_player_pack_double(norrath_player_softdouble_nan_parts_view);return exceptional_product(r,l,r);}
    if(lc==2u)return exceptional_product(l,l,r);
    const Wide lfraction=read_wide(l+16u); // Actual delay-load occurs even if right category is zero.
    if(rc==2u)return exceptional_product(r,l,r);
    const Wide rfraction=read_wide(r+16u);
    const Wide llo=Word(lfraction),lhi=lfraction>>32,rlo=Word(rfraction),rhi=rfraction>>32;
    const Wide low_product=norrath_factory_mul64(rlo,llo);
    const Wide cross1=norrath_factory_mul64(rhi,llo);
    const Wide cross2=norrath_factory_mul64(rlo,lhi);
    const Wide cross=cross1+cross2;
    const Wide high_product=norrath_factory_mul64(rhi,lhi);
    const Wide cross_carry=Wide(cross<cross1)<<32;
    Wide low=low_product+(cross<<32);
    Wide high_product_sum=(cross>>32)+high_product+cross_carry+Wide(low<low_product);
    const Word lsign=read_word(l+4u),lexponent=read_word(l+8u),rexponent=read_word(r+8u),rsign=read_word(r+4u);
    Word exponent=lexponent+rexponent+4u;
    write_word(out+4u,Word((lsign^rsign)!=0u));write_word(out+8u,exponent);
    if(high_product_sum>0x1fffffffffffffffULL){
        do{
            const Wide discarded=high_product_sum&1u;
            high_product_sum>>=1;++exponent;
            // Genuine original odd low-product shift: when discarded bit is0,
            // the low product is left unchanged rather than shifted right.
            if(discarded!=0u)low=(low>>1)|0x8000000000000000ULL;
        }while(high_product_sum>0x1fffffffffffffffULL);
        write_word(out+8u,exponent);
    }
    if(high_product_sum<=0x0fffffffffffffffULL){
        exponent=read_word(out+8u);
        do{
            high_product_sum<<=1;
            if(low&0x8000000000000000ULL)high_product_sum|=1u;
            --exponent;low<<=1;
        }while(high_product_sum<=0x0fffffffffffffffULL);
        write_word(out+8u,exponent);
    }
    write_wide(out+16u,high_product_sum);write_word(out,3u);
    return pack(out);
}

extern "C" Wide norrath_player_double_divide(Wide left,Wide right){
    alignas(16) unsigned char workspace[64];alignas(8) unsigned char raw[16];
    unpack_pair(left,right,raw,workspace);
    const Word l=address(workspace),r=l+32u;
    const Word lc=read_word(l);if(lc<2u)return pack(l);
    const Word rc=read_word(r);if(rc<2u)return pack(r);
    const Word lsign=read_word(l+4u),rsign=read_word(r+4u);
    write_word(l+4u,lsign^rsign);
    if(lc==4u||lc==2u){
        if(lc==rc)return norrath_player_pack_double(norrath_player_softdouble_nan_parts_view);
        return pack(l);
    }
    if(rc==4u){write_wide(l+16u,0u);write_word(l+8u,0u);return pack(l);}
    if(rc==2u){write_word(l,4u);return pack(l);}
    const Word lexponent=read_word(l+8u),rexponent=read_word(r+8u);
    Wide numerator=read_wide(l+16u);
    const Wide denominator=read_wide(r+16u);
    Word exponent=lexponent-rexponent;write_word(l+8u,exponent);
    if(numerator<denominator){--exponent;numerator<<=1;write_word(l+8u,exponent);}
    Wide bit=0x1000000000000000ULL,quotient=0u;
    do{
        Wide shifted=numerator<<1;
        if(numerator>=denominator){numerator-=denominator;quotient|=bit;shifted=numerator<<1;}
        bit>>=1;numerator=shifted;
    }while(bit!=0u);
    write_wide(l+16u,quotient);return pack(l);
}
