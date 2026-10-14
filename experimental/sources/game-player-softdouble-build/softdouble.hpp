#pragma once
#include "number_parser.hpp"
extern "C" {
void norrath_player_unpack_double(const unsigned char*,unsigned char*);
NorrathFormatUnsigned64 norrath_factory_mul64(NorrathFormatUnsigned64,NorrathFormatUnsigned64);
NorrathFormatUnsigned64 norrath_player_pack_double(const unsigned char*);
const unsigned char* norrath_player_double_add_parts(const unsigned char*,const unsigned char*,unsigned char*);
int norrath_player_double_compare_parts(const unsigned char*,const unsigned char*);
extern const unsigned char norrath_player_softdouble_nan_parts_view[]; // Actual __thenan_df47df08/24; no source owner.
}
namespace norrath_softdouble_detail {
using namespace norrath_player_number_detail;
inline Word pointer(const void* p){return address(p);}
inline const unsigned char* constant_bytes(Word a){return bytes(a);}
// Both raw incoming operands are snapshotted before the first real unpack call.
inline void unpack_pair(Wide left,Wide right,unsigned char* raw,unsigned char* parts){
    write_wide(address(raw),left);write_wide(address(raw)+8u,right);
    norrath_player_unpack_double(raw,parts);
    norrath_player_unpack_double(raw+8,parts+32);
}
inline Wide pack(Word a){return norrath_player_pack_double(bytes(a));}
inline Wide exceptional_product(Word chosen,Word left,Word right){
    Word lsign,rsign;
    if(chosen==right){rsign=read_word(right+4u);lsign=read_word(left+4u);}
    else{lsign=read_word(left+4u);rsign=read_word(right+4u);}
    write_word(chosen+4u,Word((lsign^rsign)!=0u));return pack(chosen);
}
}
