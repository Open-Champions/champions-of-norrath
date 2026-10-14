#pragma once
static_assert(__CHAR_BIT__==8 && sizeof(unsigned)==4 && sizeof(int)==4 && sizeof(short)==2 &&
              sizeof(void*)==4 && sizeof(__UINTPTR_TYPE__)==4 && sizeof(unsigned long long)==8 &&
              sizeof(float)==4 && __FLT_MANT_DIG__==24 && __FLT_MAX_EXP__==129 &&
              __BYTE_ORDER__==__ORDER_LITTLE_ENDIAN__, "fixed target32 representation");
extern "C" {
unsigned norrath_actor_set_parameters(void*, float, float, float, float, float, int);
unsigned norrath_actor_finish_construction(void*);
void norrath_actor_animation_randomize_time(void*);
// Whole genuine code dependencies; no responses or original target bindings.
unsigned norrath_actor_random(void*);
int norrath_game_object_find_tag_int(void*, char*, int);
char* norrath_actor_find_tag_string(void*, char*, int);
int norrath_network_player_connected(int);
float norrath_native_powf(float, float);
void norrath_model_bounding_box(void*, int, int*, int*, int*, int*, int*, int*);
unsigned norrath_animation_set_to_end_fully_blended(void*);
void* norrath_animation_frame_state_acquire();
int norrath_integer_atan2(float, float);
extern unsigned char norrath_actor_view_4837b8[];
extern unsigned char norrath_creature_view_4837e0[];
extern unsigned char norrath_creature_view_47fb58[];
extern unsigned char norrath_creature_view_47fb5c[];
extern unsigned char norrath_creature_view_47fb60[];
extern unsigned char norrath_creature_view_47fb64[];
extern unsigned char norrath_creature_view_47fb68[];
extern unsigned char norrath_creature_view_483829[];
extern unsigned char norrath_creature_view_484538[];
extern unsigned char norrath_creature_view_484cac[];
extern unsigned char norrath_creature_view_484cd8[];
extern unsigned char norrath_creature_view_483e58[];
extern unsigned char norrath_creature_view_48690c[];
extern unsigned char norrath_creature_view_486910[];
}
namespace creature_detail {
using Address=unsigned; using Wide=unsigned long long;
inline Address address(const void* p){return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(p));}
inline unsigned char* bytes(Address a){return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));}
inline void* pointer(Address a){return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(a));}
inline unsigned word(Address a){return unsigned(bytes(a)[0])|(unsigned(bytes(a)[1])<<8)|(unsigned(bytes(a)[2])<<16)|(unsigned(bytes(a)[3])<<24);}
inline unsigned half(Address a){return unsigned(bytes(a)[0])|(unsigned(bytes(a)[1])<<8);}
inline Wide wide(Address a){return Wide(word(a))|(Wide(word(a+4u))<<32);}
inline void put_word(Address a,unsigned v){for(unsigned i=0;i<4u;++i)bytes(a)[i]=static_cast<unsigned char>(v>>(i*8u));}
inline void put_half(Address a,unsigned v){bytes(a)[0]=static_cast<unsigned char>(v);bytes(a)[1]=static_cast<unsigned char>(v>>8);}
inline void put_wide(Address a,Wide v){put_word(a,unsigned(v));put_word(a+4u,unsigned(v>>32));}
inline int signed_word(unsigned v){return v<=0x7fffffffu?int(v):-1-int(0xffffffffu-v);}
inline int signed_half(unsigned v){return v<0x8000u?int(v):int(v)-65536;}
inline int signed_byte(unsigned v){return v<128u?int(v):int(v)-256;}
inline unsigned sra(unsigned v,unsigned n){n&=31u;return n==0u?v:(v>>n)|((v&0x80000000u)?(~0u<<(32u-n)):0u);}
inline float scalar(Address a){float f;put_word(address(&f),word(a));return f;}
inline float bits(unsigned v){float f;put_word(address(&f),v);return f;}
inline void put_scalar(Address a,float f){put_word(a,word(address(&f)));}
inline float view(unsigned char* p){return scalar(address(p));}
inline int chop(float f){return static_cast<int>(f);} // finite/in-range diagnostic policy; no clamp/FCSR promise
inline char* tag(unsigned char* p){return reinterpret_cast<char*>(p);}
inline Address adjusted_this(Address object,unsigned offset){const Address entry=word(object+72u)+offset;return object+unsigned(signed_half(half(entry)));}
inline Address virtual_target(Address object,unsigned offset){return word(word(object+72u)+offset+4u);}
}
