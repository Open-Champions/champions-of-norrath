#pragma once
#include "dispatch.hpp"
static_assert(__FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 129, "native EE single format");
extern "C" {
unsigned char* norrath_game_create_by_name(char*, NorrathPoint3, short, char**);
unsigned char* norrath_block_alloc(int);
int norrath_factory_strcasecmp(const char*, const char*);
int norrath_factory_strncmp(const char*, const char*, unsigned);
int norrath_object_find_tag_int(char**, char*, int);
char* norrath_object_find_tag_string(char**, char*, int);
int norrath_has_light_or_particle(char**);
void* norrath_lump_find_resource(char*, char*);
void norrath_route_add_node(const unsigned char*);
// Genuine unimplemented effect-bearing callees; no response bodies are provided.
void norrath_monster_spawn(char**, const void*, short, bool);
void norrath_custom_variables_process(char**);
void norrath_game_line_add(const void*, short, char**);
void norrath_object_update_in_grid(void*);
void norrath_object_add_to_slow_run_list(void*);
void norrath_matrix_make_translation_yaw(void*, const void*, short);
void norrath_actor_animation_add(void*, void*, int);
void norrath_animation_set_to_end(void*);
extern const unsigned char norrath_factory_ctype_minimum_view[];
extern unsigned char norrath_factory_marker_data_view[]; // 0x37f960, minimum36
extern unsigned char norrath_factory_clear_color_word[]; // 0x48667c, minimum4
extern unsigned char norrath_factory_world_pointer_word[]; // 0x4844a0, minimum4
extern unsigned char norrath_factory_world_flags_word[]; // 0x4844c0, minimum4
extern const unsigned char norrath_factory_ambient_vtable[]; // 0x44d258 address only
extern const unsigned char norrath_factory_directional_vtable[]; // 0x44d1c8 address only
extern const unsigned char norrath_factory_directional_d_vtable[]; // 0x44d138 address only
extern const unsigned char norrath_factory_cosmetic_vtable[]; // 0x44ca78 address only
extern const unsigned char norrath_factory_prop_vtable[]; // 0x44d018 address only
extern const unsigned char norrath_factory_animation_vtable[]; // 0x44cb08 address only
// Unsized genuine original string views; some are mutable tag operands.
// No original payloads or manufactured allocations are emitted here.
extern unsigned char norrath_factory_text_461fc0[];
extern unsigned char norrath_factory_text_461fd0[];
extern unsigned char norrath_factory_text_461fe0[];
extern unsigned char norrath_factory_text_461ff0[];
extern unsigned char norrath_factory_text_462000[];
extern unsigned char norrath_factory_text_462010[];
extern unsigned char norrath_factory_text_462028[];
extern unsigned char norrath_factory_text_462040[];
extern unsigned char norrath_factory_text_462058[];
extern unsigned char norrath_factory_text_462068[];
extern unsigned char norrath_factory_text_4623c0[];
extern unsigned char norrath_factory_text_4623d0[];
extern unsigned char norrath_factory_text_4623e0[];
extern unsigned char norrath_factory_text_4623f0[];
extern unsigned char norrath_factory_text_462400[];
extern unsigned char norrath_factory_text_462410[];
extern unsigned char norrath_factory_text_462420[];
extern unsigned char norrath_factory_text_462430[];
extern unsigned char norrath_factory_text_462440[];
extern unsigned char norrath_factory_text_462450[];
extern unsigned char norrath_factory_text_462460[];
extern unsigned char norrath_factory_text_462470[];
extern unsigned char norrath_factory_text_462480[];
extern unsigned char norrath_factory_text_462490[];
extern unsigned char norrath_factory_text_4624a0[];
extern unsigned char norrath_factory_text_4624b0[];
extern unsigned char norrath_factory_text_4624c0[];
extern unsigned char norrath_factory_text_4624d0[];
extern unsigned char norrath_factory_text_4624e0[];
extern unsigned char norrath_factory_text_4624f0[];
extern unsigned char norrath_factory_text_462500[];
extern unsigned char norrath_factory_text_462510[];
extern unsigned char norrath_factory_text_462520[];
extern unsigned char norrath_factory_text_462530[];
extern unsigned char norrath_factory_text_462540[];
extern unsigned char norrath_factory_text_462550[];
extern unsigned char norrath_factory_text_462560[];
extern unsigned char norrath_factory_text_462570[];
extern unsigned char norrath_factory_text_462580[];
extern unsigned char norrath_factory_text_462590[];
extern unsigned char norrath_factory_text_4625a0[];
extern unsigned char norrath_factory_text_4625b0[];
extern unsigned char norrath_factory_text_4625c0[];
extern unsigned char norrath_factory_text_4625d0[];
extern unsigned char norrath_factory_text_4625e0[];
extern unsigned char norrath_factory_text_4625f0[];
extern unsigned char norrath_factory_text_462600[];
extern unsigned char norrath_factory_text_462610[];
extern unsigned char norrath_factory_text_462620[];
extern unsigned char norrath_factory_text_462630[];
extern unsigned char norrath_factory_text_462640[];
extern unsigned char norrath_factory_text_462650[];
extern unsigned char norrath_factory_text_462660[];
extern unsigned char norrath_factory_text_462670[];
extern unsigned char norrath_factory_text_462680[];
extern unsigned char norrath_factory_text_462690[];
extern unsigned char norrath_factory_text_4626a8[];
extern unsigned char norrath_factory_text_4626c0[];
extern unsigned char norrath_factory_text_4626d0[];
extern unsigned char norrath_factory_text_4626e0[];
extern unsigned char norrath_factory_text_4626f0[];
extern unsigned char norrath_factory_text_462700[];
extern unsigned char norrath_factory_text_462718[];
extern unsigned char norrath_factory_text_462728[];
extern unsigned char norrath_factory_text_462738[];
extern unsigned char norrath_factory_text_462748[];
extern unsigned char norrath_factory_text_462758[];
extern unsigned char norrath_factory_text_462770[];
extern unsigned char norrath_factory_text_462788[];
extern unsigned char norrath_factory_text_462798[];
extern unsigned char norrath_factory_text_4627a8[];
extern unsigned char norrath_factory_text_4627b8[];
extern unsigned char norrath_factory_text_4627d0[];
extern unsigned char norrath_factory_text_4627e8[];
extern unsigned char norrath_factory_text_4627f8[];
extern unsigned char norrath_factory_text_462808[];
extern unsigned char norrath_factory_text_462818[];
extern unsigned char norrath_factory_text_462828[];
extern unsigned char norrath_factory_text_462840[];
extern unsigned char norrath_factory_text_462858[];
extern unsigned char norrath_factory_text_462868[];
extern unsigned char norrath_factory_text_462878[];
extern unsigned char norrath_factory_text_462888[];
extern unsigned char norrath_factory_text_462898[];
extern unsigned char norrath_factory_text_4628a8[];
extern unsigned char norrath_factory_text_4628c0[];
extern unsigned char norrath_factory_text_4628d8[];
extern unsigned char norrath_factory_text_4628e8[];
extern unsigned char norrath_factory_text_4628f8[];
extern unsigned char norrath_factory_text_462908[];
extern unsigned char norrath_factory_text_462918[];
extern unsigned char norrath_factory_text_462930[];
extern unsigned char norrath_factory_text_462948[];
extern unsigned char norrath_factory_text_462958[];
extern unsigned char norrath_factory_text_462968[];
extern unsigned char norrath_factory_text_462978[];
extern unsigned char norrath_factory_text_462988[];
extern unsigned char norrath_factory_text_4629a0[];
extern unsigned char norrath_factory_text_4629b8[];
extern unsigned char norrath_factory_text_4629c8[];
extern unsigned char norrath_factory_text_4629d8[];
extern unsigned char norrath_factory_text_4629e8[];
extern unsigned char norrath_factory_text_4629f8[];
extern unsigned char norrath_factory_text_462a10[];
extern unsigned char norrath_factory_text_462a28[];
extern unsigned char norrath_factory_text_462a38[];
extern unsigned char norrath_factory_text_462a48[];
extern unsigned char norrath_factory_text_462a60[];
extern unsigned char norrath_factory_text_462a78[];
extern unsigned char norrath_factory_text_462a88[];
extern unsigned char norrath_factory_text_462a98[];
extern unsigned char norrath_factory_text_462ab0[];
extern unsigned char norrath_factory_text_462ac0[];
extern unsigned char norrath_factory_text_462ad0[];
extern unsigned char norrath_factory_text_462ae8[];
extern unsigned char norrath_factory_text_462af8[];
extern unsigned char norrath_factory_text_462b08[];
extern unsigned char norrath_factory_text_462b20[];
extern unsigned char norrath_factory_text_462b30[];
extern unsigned char norrath_factory_text_462b40[];
extern unsigned char norrath_factory_text_462b50[];
extern unsigned char norrath_factory_text_462b68[];
extern unsigned char norrath_factory_text_462b78[];
extern unsigned char norrath_factory_text_462b88[];
extern unsigned char norrath_factory_text_462b98[];
extern unsigned char norrath_factory_text_462bb0[];
extern unsigned char norrath_factory_text_462bc8[];
extern unsigned char norrath_factory_text_462bd8[];
extern unsigned char norrath_factory_text_462be8[];
extern unsigned char norrath_factory_text_462bf8[];
extern unsigned char norrath_factory_text_462c10[];
extern unsigned char norrath_factory_text_462c28[];
extern unsigned char norrath_factory_text_462c38[];
extern unsigned char norrath_factory_text_462c48[];
extern unsigned char norrath_factory_text_462c58[];
extern unsigned char norrath_factory_text_462c68[];
extern unsigned char norrath_factory_text_462c80[];
extern unsigned char norrath_factory_text_462c98[];
extern unsigned char norrath_factory_text_462ca8[];
extern unsigned char norrath_factory_text_462cb8[];
extern unsigned char norrath_factory_text_462cc8[];
extern unsigned char norrath_factory_text_462ce0[];
extern unsigned char norrath_factory_text_462cf8[];
extern unsigned char norrath_factory_text_462d08[];
extern unsigned char norrath_factory_text_462d18[];
extern unsigned char norrath_factory_text_462d28[];
extern unsigned char norrath_factory_text_462d38[];
extern unsigned char norrath_factory_text_462d50[];
extern unsigned char norrath_factory_text_462d68[];
extern unsigned char norrath_factory_text_462d78[];
extern unsigned char norrath_factory_text_462d88[];
extern unsigned char norrath_factory_text_462d98[];
extern unsigned char norrath_factory_text_462db0[];
extern unsigned char norrath_factory_text_462dc8[];
extern unsigned char norrath_factory_text_462dd8[];
extern unsigned char norrath_factory_text_462de8[];
extern unsigned char norrath_factory_text_462df8[];
extern unsigned char norrath_factory_text_462e08[];
extern unsigned char norrath_factory_text_462e18[];
extern unsigned char norrath_factory_text_462e28[];
extern unsigned char norrath_factory_text_462e38[];
extern unsigned char norrath_factory_text_462e48[];
extern unsigned char norrath_factory_text_462e58[];
extern unsigned char norrath_factory_text_462e68[];
extern unsigned char norrath_factory_text_462e78[];
extern unsigned char norrath_factory_text_462e88[];
extern unsigned char norrath_factory_text_462e98[];
extern unsigned char norrath_factory_text_462ea8[];
extern unsigned char norrath_factory_text_462eb8[];
extern unsigned char norrath_factory_text_462ec8[];
extern unsigned char norrath_factory_text_462ed8[];
extern unsigned char norrath_factory_text_462ef8[];
extern unsigned char norrath_factory_text_462f18[];
extern unsigned char norrath_factory_text_462f28[];
extern unsigned char norrath_factory_text_462f48[];
extern unsigned char norrath_factory_text_462f68[];
extern unsigned char norrath_factory_text_462f78[];
extern unsigned char norrath_factory_text_462f88[];
extern unsigned char norrath_factory_text_462f98[];
extern unsigned char norrath_factory_text_462fb0[];
extern unsigned char norrath_factory_text_462fc0[];
extern unsigned char norrath_factory_text_462fd0[];
extern unsigned char norrath_factory_text_462fe0[];
extern unsigned char norrath_factory_text_462ff0[];
extern unsigned char norrath_factory_text_463000[];
extern unsigned char norrath_factory_text_463010[];
extern unsigned char norrath_factory_text_463020[];
extern unsigned char norrath_factory_text_463030[];
extern unsigned char norrath_factory_text_463040[];
extern unsigned char norrath_factory_text_463050[];
extern unsigned char norrath_factory_text_463060[];
extern unsigned char norrath_factory_text_483b38[];
extern unsigned char norrath_factory_text_483b40[];
extern unsigned char norrath_factory_text_483b48[];
extern unsigned char norrath_factory_text_483b50[];
extern unsigned char norrath_factory_text_483b60[];
extern unsigned char norrath_factory_text_483b68[];
extern unsigned char norrath_factory_text_483ba8[];
extern unsigned char norrath_factory_text_483c38[];
extern unsigned char norrath_factory_text_483c40[];
extern unsigned char norrath_factory_text_483c48[];
extern unsigned char norrath_factory_text_483c50[];
extern unsigned char norrath_factory_text_483c58[];
extern unsigned char norrath_factory_text_483c60[];
extern unsigned char norrath_factory_text_483c68[];
extern unsigned char norrath_factory_text_483c70[];
extern unsigned char norrath_factory_text_483c78[];
extern unsigned char norrath_factory_text_483c80[];
extern unsigned char norrath_factory_text_483c88[];
extern unsigned char norrath_factory_text_483c90[];
extern unsigned char norrath_factory_text_483c98[];
extern unsigned char norrath_factory_text_483ca0[];
extern unsigned char norrath_factory_text_483ca8[];
extern unsigned char norrath_factory_text_483cb0[];
extern unsigned char norrath_factory_text_483cb8[];
extern unsigned char norrath_factory_text_483cc0[];
extern unsigned char norrath_factory_text_483cc8[];
extern unsigned char norrath_factory_text_483cd0[];
extern unsigned char norrath_factory_text_483cd8[];
extern unsigned char norrath_factory_text_483ce0[];
extern unsigned char norrath_factory_text_483ce8[];
extern unsigned char norrath_factory_text_483cf0[];
extern unsigned char norrath_factory_text_483cf8[];
extern unsigned char norrath_factory_text_483d00[];
extern unsigned char norrath_factory_text_483d08[];
extern unsigned char norrath_factory_text_483d10[];
extern unsigned char norrath_factory_text_483d18[];
extern unsigned char norrath_factory_text_483d20[];
extern unsigned char norrath_factory_text_483d28[];
extern unsigned char norrath_factory_text_483d30[];
extern unsigned char norrath_factory_text_483d38[];
extern unsigned char norrath_factory_text_483d40[];
extern unsigned char norrath_factory_text_483d48[];
extern unsigned char norrath_factory_text_483d50[];
extern unsigned char norrath_factory_text_483d58[];
extern unsigned char norrath_factory_text_483d60[];
extern unsigned char norrath_factory_text_483d68[];
extern unsigned char norrath_factory_text_483d70[];
extern unsigned char norrath_factory_text_483d78[];
extern unsigned char norrath_factory_text_483d80[];
extern unsigned char norrath_factory_text_483d88[];
extern unsigned char norrath_factory_text_483d90[];
extern unsigned char norrath_factory_text_483d98[];
extern unsigned char norrath_factory_text_483da0[];
extern unsigned char norrath_factory_text_483da8[];
extern unsigned char norrath_factory_text_483db0[];
extern unsigned char norrath_factory_text_483db8[];
extern unsigned char norrath_factory_text_483dc0[];
extern unsigned char norrath_factory_text_483dc8[];
extern unsigned char norrath_factory_text_483dd0[];
extern unsigned char norrath_factory_text_483dd8[];
extern unsigned char norrath_factory_text_483de0[];
extern unsigned char norrath_factory_text_483de8[];
extern unsigned char norrath_factory_text_483df0[];
extern unsigned char norrath_factory_text_483df8[];
extern unsigned char norrath_factory_text_483e00[];
extern unsigned char norrath_factory_text_483e08[];
extern unsigned char norrath_factory_text_483e10[];
extern unsigned char norrath_factory_text_483e18[];
extern unsigned char norrath_factory_text_483e20[];
extern unsigned char norrath_factory_text_483e28[];
extern unsigned char norrath_factory_text_483e30[];
extern unsigned char norrath_factory_text_483e38[];
// 0x1411f0 __10GameObjectG6Point3PQ210GameObject11resourceMapPPc; returned machine word is ignored.
unsigned norrath_game_object_construct(void*, const void*, void*, char**);
// 0x14a610 __13UserParamPropG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_UserParamProp(void*, const void*, short, char**);
// 0x14aa38 __4GoldG6Point3PPc; returned machine word is ignored.
unsigned norrath_construct_Gold(void*, const void*, char**);
// 0x14b400 __7Player1G6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Player1(void*, const void*, short, char**);
// 0x14bee8 __8ItemPropG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_ItemProp(void*, const void*, short, char**);
// 0x150280 __15PushPhysicsPropG6Point3sPcN23f; returned machine word is ignored.
unsigned norrath_construct_PushPhysicsProp(void*, const void*, short, char*, char*, char*, float);
// 0x151080 __9WaveMakerG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_WaveMaker(void*, const void*, short, char**);
// 0x155ce8 __15PushPhysicsPropG6Point3; returned machine word is ignored.
unsigned norrath_construct_PushPhysicsPropSimple(void*, const void*);
// 0x1d12d8 __11PushTriggerG6Point3iPPc; returned machine word is ignored.
unsigned norrath_construct_PushTrigger(void*, const void*, int, char**);
// 0x1d1470 __7TriggerG6Point3PPc; returned machine word is ignored.
unsigned norrath_construct_Trigger(void*, const void*, char**);
// 0x1d19f8 __13TriggerParamsG6Point3PPc; returned machine word is ignored.
unsigned norrath_construct_TriggerParams(void*, const void*, char**);
// 0x1e5700 __10WaterSpoutG6Point3i; returned machine word is ignored.
unsigned norrath_construct_WaterSpout(void*, const void*, int);
// 0x1e5bc8 __11MissileTrapG6Point3iPPc; returned machine word is ignored.
unsigned norrath_construct_MissileTrap(void*, const void*, int, char**);
// 0x1e5ca8 __7ShooterG6Point3iPPc; returned machine word is ignored.
unsigned norrath_construct_Shooter(void*, const void*, int, char**);
// 0x1e61f0 __3IceG6Point3sPcN43; returned machine word is ignored.
unsigned norrath_construct_Ice(void*, const void*, short, char*, char*, char*, char*, char*);
// 0x1e6b30 __10CloudGiantG6Point3sPPcPc; returned machine word is ignored.
unsigned norrath_construct_CloudGiant(void*, const void*, short, char**, char*);
// 0x1e9bc0 __5DemonG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Demon(void*, const void*, short, char**);
// 0x1eb4e0 __11LavaMonsterG6Point3sPPcPc; returned machine word is ignored.
unsigned norrath_construct_LavaMonster(void*, const void*, short, char**, char*);
// 0x1ee390 __3CatG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Cat(void*, const void*, short, char**);
// 0x1eef40 __6BadgerG6Point3sPPci; returned machine word is ignored.
unsigned norrath_construct_Badger(void*, const void*, short, char**, int);
// 0x1efeb0 __7CritterPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Critter(void*, char*, const void*, short, char**);
// 0x1f19c0 __9GeneratorG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Generator(void*, const void*, short, char**);
// 0x1f2148 __8ScorpionG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Scorpion(void*, const void*, short, char**);
// 0x1f2f08 __4SoulG6Point3sPPcP6Minion; returned machine word is ignored.
unsigned norrath_construct_Soul(void*, const void*, short, char**, void*);
// 0x1f5050 __7UlthorkG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Ulthork(void*, const void*, short, char**);
// 0x1f62e8 __5GhoulG6Point3siPPc; returned machine word is ignored.
unsigned norrath_construct_Ghoul(void*, const void*, short, int, char**);
// 0x1f7c60 __7MermaidG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Mermaid(void*, const void*, short, char**);
// 0x1f8088 __10SeaMonsterG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_SeaMonster(void*, const void*, short, char**);
// 0x1f8c08 __8InnoruukG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Innoruuk(void*, const void*, short, char**);
// 0x1ff820 __8FroglockG6Point3sPPcPc; returned machine word is ignored.
unsigned norrath_construct_Froglock(void*, const void*, short, char**, char*);
// 0x2011f8 __5GnomeG6Point3sPPcPc; returned machine word is ignored.
unsigned norrath_construct_Gnome(void*, const void*, short, char**, char*);
// 0x201ca0 __14GnomeNavigatorG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_GnomeNavigator(void*, const void*, short, char**);
// 0x202678 __7CyclopsG6Point3sPPci; returned machine word is ignored.
unsigned norrath_construct_Cyclops(void*, const void*, short, char**, int);
// 0x203cf8 __6WraithG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Wraith(void*, const void*, short, char**);
// 0x205368 __3AntG6Point3sPPcPcc; returned machine word is ignored.
unsigned norrath_construct_Ant(void*, const void*, short, char**, char*, signed char);
// 0x206c08 __8AntQueenG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_AntQueen(void*, const void*, short, char**);
// 0x2097b0 __11SmallSpiderG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_SmallSpider(void*, const void*, short, char**);
// 0x209c20 __10BlackWidowG6Point3sPPcPc; returned machine word is ignored.
unsigned norrath_construct_BlackWidow(void*, const void*, short, char**, char*);
// 0x20b2f0 __11SpiderQueenG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_SpiderQueen(void*, const void*, short, char**);
// 0x20e490 __7WebTrapG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_WebTrap(void*, const void*, short, char**);
// 0x20ff48 __10FireBeetleG6Point3sPPci; returned machine word is ignored.
unsigned norrath_construct_FireBeetle(void*, const void*, short, char**, int);
// 0x2118e8 __7FireFlyG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_FireFly(void*, const void*, short, char**);
// 0x2126c0 __6SwitchG6Point3PPc; returned machine word is ignored.
unsigned norrath_construct_Switch(void*, const void*, char**);
// 0x2127f0 __9DoorSwingG6Point3PPc; returned machine word is ignored.
unsigned norrath_construct_DoorSwing(void*, const void*, char**);
// 0x212f08 __10DoorSecretG6Point3PPc; returned machine word is ignored.
unsigned norrath_construct_DoorSecret(void*, const void*, char**);
// 0x2132c0 __11FloorSwitchG6Point3PPc; returned machine word is ignored.
unsigned norrath_construct_FloorSwitch(void*, const void*, char**);
// 0x214a48 __9SavepointG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Savepoint(void*, const void*, short, char**);
// 0x215a78 __8RollDoorG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_RollDoor(void*, const void*, short, char**);
// 0x215f58 __10WeaponRackG6Point3iPPc; returned machine word is ignored.
unsigned norrath_construct_WeaponRack(void*, const void*, int, char**);
// 0x216600 __7BlockerG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Blocker(void*, const void*, short, char**);
// 0x218790 __5ChestG6Point3sPPci; returned machine word is ignored.
unsigned norrath_construct_Chest(void*, const void*, short, char**, int);
// 0x21c768 __13PokeReflectorG6Point3PPc; returned machine word is ignored.
unsigned norrath_construct_PokeReflector(void*, const void*, char**);
// 0x21c920 __5LeverG6Point3iPPc; returned machine word is ignored.
unsigned norrath_construct_Lever(void*, const void*, int, char**);
// 0x21cc10 __5TimerG6Point3PPc; returned machine word is ignored.
unsigned norrath_construct_Timer(void*, const void*, char**);
// 0x21cd08 __7CounterG6Point3PPc; returned machine word is ignored.
unsigned norrath_construct_Counter(void*, const void*, char**);
// 0x21ce00 __10TeleporterG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Teleporter(void*, const void*, short, char**);
// 0x21d0f8 __9CdEyeballG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_CdEyeball(void*, const void*, short, char**);
// 0x21d578 __9ContainerG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Container(void*, const void*, short, char**);
// 0x21d6f0 __9ContainerG6Point3sPcN43iiiPPc; returned machine word is ignored.
unsigned norrath_construct_ContainerWithResources(void*, const void*, short, char*, char*, char*, char*, char*, int, int, int, char**);
// 0x21de50 __4TrapG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Trap(void*, const void*, short, char**);
// 0x21eee8 __9AutoChestPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_AutoChest(void*, char*, const void*, short, char**);
// 0x220278 __8AutoPropPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_AutoProp(void*, char*, const void*, short, char**);
// 0x221118 __13AutoContainerPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_AutoContainer(void*, char*, const void*, short, char**);
// 0x223bd8 __10AutoPropLPPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_AutoPropLP(void*, char*, const void*, short, char**);
// 0x225250 __11AutoChestLPPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_AutoChestLP(void*, char*, const void*, short, char**);
// 0x225518 __15AutoContainerLPPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_AutoContainerLP(void*, char*, const void*, short, char**);
// 0x225fe8 __7AntTrapPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_AntTrap(void*, char*, const void*, short, char**);
// 0x2260b8 __7EggSackPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_EggSack(void*, char*, const void*, short, char**);
// 0x226140 __8HateIntoPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_HateInto(void*, char*, const void*, short, char**);
// 0x2261b8 __8HateGatePcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_HateGate(void*, char*, const void*, short, char**);
// 0x226218 __8GothTrapPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_GothTrap(void*, char*, const void*, short, char**);
// 0x226298 __8HateHolePcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_HateHole(void*, char*, const void*, short, char**);
// 0x2263e8 __10HateBridgePcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_HateBridge(void*, char*, const void*, short, char**);
// 0x2264e8 __8HackDoorPcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_HackDoor(void*, char*, const void*, short, char**);
// 0x226598 __3OrcG6Point3sPPcPcc; returned machine word is ignored.
unsigned norrath_construct_Orc(void*, const void*, short, char**, char*, signed char);
// 0x229cf8 __8SuperOrcG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_SuperOrc(void*, const void*, short, char**);
// 0x22bb10 __6GoblinG6Point3sPPcPc; returned machine word is ignored.
unsigned norrath_construct_Goblin(void*, const void*, short, char**, char*);
// 0x22f590 __5TorchG6Point3PPci; returned machine word is ignored.
unsigned norrath_construct_Torch(void*, const void*, char**, int);
// 0x2301a8 __6CandleG6Point3i; returned machine word is ignored.
unsigned norrath_construct_Candle(void*, const void*, int);
// 0x230890 __7LanternG6Point3; returned machine word is ignored.
unsigned norrath_construct_Lantern(void*, const void*);
// 0x2309e8 __4LampG6Point3; returned machine word is ignored.
unsigned norrath_construct_Lamp(void*, const void*);
// 0x2310a8 __7Candle2G6Point3; returned machine word is ignored.
unsigned norrath_construct_Candle2(void*, const void*);
// 0x2315f8 __3NPCG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_NPC(void*, const void*, short, char**);
// 0x232818 __5RondoG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Rondo(void*, const void*, short, char**);
// 0x2332a0 __7NataslaG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Natasla(void*, const void*, short, char**);
// 0x233878 __6MinionG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Minion(void*, const void*, short, char**);
// 0x235478 __10ArenabeastG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Arenabeast(void*, const void*, short, char**);
// 0x236b68 __5MummyG6Point3sPPcPc; returned machine word is ignored.
unsigned norrath_construct_Mummy(void*, const void*, short, char**, char*);
// 0x239a88 __9MummyKingG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_MummyKing(void*, const void*, short, char**);
// 0x23c330 __12UndeadKnightG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_UndeadKnight(void*, const void*, short, char**);
// 0x23d798 __17UndeadKnightCloneG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_UndeadKnightClone(void*, const void*, short, char**);
// 0x23fa78 __7VampireG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Vampire(void*, const void*, short, char**);
// 0x2406c0 __11VampireLordG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_VampireLord(void*, const void*, short, char**);
// 0x243868 __8SkeletonG6Point3sPPcPci; returned machine word is ignored.
unsigned norrath_construct_Skeleton(void*, const void*, short, char**, char*, int);
// 0x246310 __8SkelightG6Point3sPPcci; returned machine word is ignored.
unsigned norrath_construct_Skelight(void*, const void*, short, char**, signed char, int);
// 0x247400 __14WoodElfSoldierG6Point3sPPcPc; returned machine word is ignored.
unsigned norrath_construct_WoodElfSoldier(void*, const void*, short, char**, char*);
// 0x280a88 __6CthuluG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Cthulu(void*, const void*, short, char**);
// 0x282128 __8JumpGateG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_JumpGate(void*, const void*, short, char**);
// 0x284b88 __11LavaTractorG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_LavaTractor(void*, const void*, short, char**);
// 0x284f20 __13FemaleDarkElfG6Point3sPPcPc; returned machine word is ignored.
unsigned norrath_construct_FemaleDarkElf(void*, const void*, short, char**, char*);
// 0x286920 __11MaleDarkElfG6Point3sPPcPc; returned machine word is ignored.
unsigned norrath_construct_MaleDarkElf(void*, const void*, short, char**, char*);
// 0x2898b0 __8FireBombG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_FireBomb(void*, const void*, short, char**);
// 0x28b858 __8CatapultG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Catapult(void*, const void*, short, char**);
// 0x28dd00 __10StaticFireG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_StaticFire(void*, const void*, short, char**);
// 0x2bfa20 __4BoatG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_Boat(void*, const void*, short, char**);
// 0x2bfaf0 __8SkulBoatG6Point3sPPc; returned machine word is ignored.
unsigned norrath_construct_SkulBoat(void*, const void*, short, char**);
}
