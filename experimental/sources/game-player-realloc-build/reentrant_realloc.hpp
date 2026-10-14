#pragma once
#include "reentrant_malloc.hpp"
extern "C" {
unsigned char* norrath_player_reallocate_reentrant(unsigned char*,unsigned char*,unsigned);
void* norrath_copy_bytes(void*,const void*,__SIZE_TYPE__);
}
namespace norrath_realloc_detail {
using namespace norrath_malloc_detail;
inline void copy_payload(Word destination,Word source,Word count) {
    if(count>=37u) {norrath_copy_bytes(bytes(destination),bytes(source),count);return;}
    Word input=source,output=destination;
    if(count>=20u) {
        write_word(destination,read_word(source));input=source+8u;output=destination+8u;
        write_word(destination+4u,read_word(source+4u));
        if(count>=28u) {
            write_word(output,read_word(input));input=source+16u;output=destination+16u;
            write_word(destination+12u,read_word(source+12u));
            if(count>=36u) {
                write_word(output,read_word(input));input=source+24u;output=destination+24u;
                write_word(destination+20u,read_word(source+20u));
            }
        }
    }
    write_word(output,read_word(input));input+=4u;output+=4u;
    write_word(output,read_word(input));write_word(output+4u,read_word(input+4u));
}
}
