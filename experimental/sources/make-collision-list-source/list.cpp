#include "list.hpp"
namespace {
using Word=unsigned;
Word address(const unsigned char* p) { return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
unsigned char* bytes(Word a) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a)); }
Word read_half(Word a) { const unsigned char* p=bytes(a);return Word(p[0])|(Word(p[1])<<8); }
Word read_word(Word a) { const unsigned char* p=bytes(a);return Word(p[0])|(Word(p[1])<<8)|(Word(p[2])<<16)|(Word(p[3])<<24); }
void write_word(Word a,Word value) {
 unsigned char* p=bytes(a);p[0]=static_cast<unsigned char>(value);p[1]=static_cast<unsigned char>(value>>8);
 p[2]=static_cast<unsigned char>(value>>16);p[3]=static_cast<unsigned char>(value>>24);
}
float read_float(Word a) {
 float f;unsigned char* representation=reinterpret_cast<unsigned char*>(&f);
 for(unsigned i=0;i<4;++i) representation[i]=bytes(a)[i];
 return f;
}
int signed_half(Word w) {return w<32768u?static_cast<int>(w):static_cast<int>(w)-65536;}
int signed_word(Word w) {return w<0x80000000u?static_cast<int>(w):-1-static_cast<int>(~w);}
}
extern "C" unsigned char* norrath_make_collision_list(unsigned char* world_pointer,
 float low_x,float low_y,float high_x,float high_y,unsigned char* output,int capacity) {
 const Word world=address(world_pointer);
 const float grid_scale=read_float(address(norrath_collision_grid_scale_bytes));
 const Word low_x_cell=ee_cvt_w_s_word(floorf(low_x*grid_scale));
 const Word low_y_cell=ee_cvt_w_s_word(floorf(low_y*grid_scale));
 const Word high_x_cell=ee_cvt_w_s_word(floorf(high_x*grid_scale));
 const float high_y_floor=floorf(high_y*grid_scale);
 const Word origin_x=read_word(world+8u);
 const Word high_y_cell=ee_cvt_w_s_word(high_y_floor);
 const Word height=read_word(world+20u);
 const Word origin_y=read_word(world+12u);
 const Word width=read_word(world+16u);
 int first_row=signed_word(low_y_cell-origin_y);
 int last_row=signed_word(high_y_cell-origin_y);
 int first_column=signed_word(low_x_cell-origin_x);
 int last_column=signed_word(high_x_cell-origin_x);
 if(first_row<0) first_row=0;
 if(last_row>signed_word(height-1u)) last_row=signed_word(height-1u);
 if(first_column<0) first_column=0;
 if(last_column>signed_word(width-1u)) last_column=signed_word(width-1u);
 Word count=0;
 for(int row=first_row;row<=last_row;++row) {
  if(first_column>last_column) continue;
  const Word cells=read_word(world+24u);
  for(int column=first_column;column<=last_column;++column) {
   const Word row_width=read_word(world+16u);
   Word list=read_word(cells+4u*(static_cast<Word>(row)*row_width+static_cast<Word>(column)));
   while(read_half(list)!=65535u) {
    const int index=signed_half(read_half(list));
    const Word parts=read_word(world+32u);
    const Word frame=read_word(address(norrath_collision_frame_bytes));
    const Word part=parts+static_cast<Word>(index)*36u;
    const Word previous_frame=read_word(part+8u);
    list+=2u;
    if(previous_frame!=frame && (read_half(part+16u)&2u)==0u) {
     bool accepted=true;
     if(high_x<static_cast<float>(signed_half(read_half(part)))) accepted=false;
     if(accepted && high_y<static_cast<float>(signed_half(read_half(part+2u)))) accepted=false;
     if(accepted && static_cast<float>(signed_half(read_half(part+4u)))<low_x) accepted=false;
     if(accepted && static_cast<float>(signed_half(read_half(part+6u)))<low_y) accepted=false;
     if(accepted) {
      const Word slot=address(output)+count*4u;
      ++count;
      write_word(part+8u,frame);
      write_word(slot,part);
      if(count==static_cast<Word>(capacity)-1u) {
       const Word end=address(output)+count*4u;
       write_word(end,0u);
       return bytes(end);
      }
     }
    }
   }
  }
 }
 const Word end=address(output)+count*4u;
 write_word(end,0u);
 return bytes(end);
}
