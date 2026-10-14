#pragma once
#include "constructor.hpp"
#include "actors.hpp"
#include "formatting.hpp"
#include "dispatch.hpp"
extern "C" {
unsigned norrath_construct_UserParamProp(void*,const void*,short,char**);
unsigned norrath_construct_Gold(void*,const void*,char**);
unsigned norrath_construct_Player1(void*,const void*,short,char**);
unsigned norrath_construct_ItemProp(void*,const void*,short,char**);
void norrath_object_update_in_grid(void*);
void norrath_matrix_make_translation_yaw(void*,const void*,short);
NorrathPoint3* norrath_object_find_empty_spot(NorrathPoint3* destination,NorrathPoint3 origin,int distance,int clearance,char* result_flag);
// Actual effect-bearing services; no host/parser/item/resource answers.
float norrath_player_find_tag_float(void*,char*,int);
float norrath_player_treasure_ramp();
void* norrath_player_resource_map_get(void*);
void norrath_animation_set_to_end(void*);
void norrath_player_remove_quotations(char*);
int norrath_player_sscanf(const char*,const char*,...);
void norrath_player_create_item(char*,void*,int,int);
int norrath_player_proxy_mesh_id(void*);
char* norrath_factory_strcpy(char*,const char*);
int norrath_string_compare(const char*,const char*);
extern unsigned char norrath_player_view_44c9e8[];
extern unsigned char norrath_player_view_44cd48[];
extern unsigned char norrath_player_view_44d2e8[];
extern unsigned char norrath_player_view_44d018[];
extern unsigned char norrath_player_view_44cef8[];
extern unsigned char norrath_player_view_483ba0[];
extern unsigned char norrath_player_view_483ba8[];
extern unsigned char norrath_player_view_483bb0[];
extern unsigned char norrath_player_view_483bb8[];
extern unsigned char norrath_player_view_483bc0[];
extern unsigned char norrath_player_view_483bc8[];
extern unsigned char norrath_player_view_483bd0[];
extern unsigned char norrath_player_view_483bd8[];
extern unsigned char norrath_player_view_483be0[];
extern unsigned char norrath_player_view_483be8[];
extern unsigned char norrath_player_view_483bf0[];
extern unsigned char norrath_player_view_483c08[];
extern unsigned char norrath_player_view_483c10[];
extern unsigned char norrath_player_view_483c18[];
extern unsigned char norrath_player_view_483c20[];
extern unsigned char norrath_player_view_483c28[];
extern unsigned char norrath_player_view_462350[];
extern unsigned char norrath_player_view_462360[];
extern unsigned char norrath_player_view_4623a0[];
extern unsigned char norrath_player_view_4623b0[];
extern unsigned char norrath_player_view_34b752[];
extern unsigned char norrath_player_view_42a3f8[];
extern unsigned char norrath_player_view_484ca8[];
extern unsigned char norrath_player_view_483f3e[];
extern unsigned char norrath_player_view_484f38[];
extern unsigned char norrath_player_view_429410[];
extern unsigned char norrath_player_view_600d20[];
extern unsigned char norrath_player_view_31f540[];
extern unsigned char norrath_player_view_47fed4[];
extern unsigned char norrath_player_view_47fed8[];
extern unsigned char norrath_player_view_47fedc[];
extern unsigned char norrath_player_view_47fee0[];
}
namespace norrath_player_detail {
using namespace norrath_constructor_detail;
struct RawPoint { unsigned x,y,z; };
static_assert(sizeof(RawPoint)==12 && alignof(RawPoint)==4,"observed point byte extent");
inline RawPoint snapshot(Address a){return {read_word(a),read_word(a+4u),read_word(a+8u)};}
inline void publish(Address a,RawPoint p){write_word(a,p.x);write_word(a+4u,p.y);write_word(a+8u,p.z);}
inline void base(void* object,const void* point,char** args){RawPoint p=snapshot(address(point));(void)norrath_game_object_construct(object,&p,nullptr,args);}
inline unsigned half(Address a){return unsigned(bytes(a)[0])|unsigned(bytes(a)[1])<<8;}
inline void put_half(Address a,unsigned v){bytes(a)[0]=static_cast<unsigned char>(v);bytes(a)[1]=static_cast<unsigned char>(v>>8);}
inline unsigned long long wide(Address a){return read_word(a)|(static_cast<unsigned long long>(read_word(a+4u))<<32);}
inline void put_wide(Address a,unsigned long long v){write_word(a,unsigned(v));write_word(a+4u,unsigned(v>>32));}
inline short short_bits(unsigned v){v&=65535u;return static_cast<short>(v<32768u?int(v):int(v)-65536);}
inline unsigned step(unsigned v){return v*1664525u+1013904223u;}
inline void write_float(Address a,float value){write_word(a,read_word(address(&value)));}
inline NorrathPoint3 point(Address a){return {read_float(a),read_float(a+4u),read_float(a+8u)};}
inline void publish_point(Address a,NorrathPoint3 p){write_float(a,p.x);write_float(a+4u,p.y);write_float(a+8u,p.z);}
}
