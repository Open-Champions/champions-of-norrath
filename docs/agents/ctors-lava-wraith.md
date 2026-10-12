# Constructors from lava monster through wraith

Package `ctors-lava-wraith`, branch `agents/ctors-lava-wraith`. All 33 constructors in
the package are authored, one commit each (later correction commits are separate).
Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses are that
file's symbol values. No game bytes or listings are committed. Signatures come from
`game-object-factory-build-v2/factory.hpp` (the callers).

## Checks performed

- Every source compiles with the reviewed EE g++ and the `authored_cpp` flags.
- Every constructor has a host contract under its directory's `host/` (not a plan
  input). It stubs the callees, records the call order, and compares the whole object
  image with an expected image. `ctors-lava-wraith-detail/host_support.hpp` is the shared
  harness.
- Local differential check (private tooling, not committed): the original body and the
  freshly compiled object were both run in a Unicorn MIPS64 emulator over the same
  stubbed callees, random draws, tags, names, objects and tables. Call sequence and
  arguments, a hash of the object at every call, final object bytes, global writes and
  low V0 had to match. All 33 matched (4 to 216 scenarios each). The harness emulates the
  R5900 three-operand `mult` and `rsqrt.s` itself. It found and fixed 11 mistakes that the
  hand-written host contracts missed (flag masks, a doubleword mask, delay-slot store
  order, a duplicated light float). IEEE emulation is not R5900 arithmetic: the
  `1.0f/0.0f` in Savepoint and float rounding were compared only as IEEE.
- A private plan with the 33 entries added (private copy of snapshot and plan in `/tmp`)
  built: 941 units, 798 function bodies, 1105 unresolved imports, and all 33 constructor
  names left the unresolved list. `python3 -m unittest discover -s tools -p 'test_*.py'`
  passes (214 tests).

## Functions

| source (under `experimental/sources/`) | sha256 | object | emitted | bytes |
|---|---|---|---|---|
| `ctors-lava-wraith-lava-monster/lava_monster.cpp` | `b41e77f5d27a7349a8f312a9ef60f2e198c335fd7db8c2d86c31b4eea5c07c88` | `lava_monster.o` | `norrath_construct_LavaMonster` | 1972 |
| `ctors-lava-wraith-male-dark-elf/male_dark_elf.cpp` | `e27724bb3a18a09ace7ff95a70159a66ec4fd91bb74b7afcebae229a2acf92c8` | `male_dark_elf.o` | `norrath_construct_MaleDarkElf` | 3016 |
| `ctors-lava-wraith-minion/minion.cpp` | `56d24398a264f08f63246b0b77fb6237f6b2cad790c7ecda9634674001b26704` | `minion.o` | `norrath_construct_Minion` | 1196 |
| `ctors-lava-wraith-mummy-king/mummy_king.cpp` | `1233e1972954aaed904cff0ca3a3a5c4f590009e2c249652f2bae155c729fac7` | `mummy_king.o` | `norrath_construct_MummyKing` | 1032 |
| `ctors-lava-wraith-mummy/mummy.cpp` | `5b21af871fcf52267443fd2d84028b6080365bf5188a2d581b3277761f8ad695` | `mummy.o` | `norrath_construct_Mummy` | 2112 |
| `ctors-lava-wraith-npc/npc.cpp` | `b210d73a1538b948946dd2afc77ac0269a8cb975ddf683e4646fc846ac4bb26b` | `npc.o` | `norrath_construct_NPC` | 1280 |
| `ctors-lava-wraith-orc/orc.cpp` | `fbb6c2cf7b2b1d67bb328016ac1e136a01c13817f93249357417f6b3bf18d228` | `orc.o` | `norrath_construct_Orc` | 3596 |
| `ctors-lava-wraith-push-physics-prop/push_physics_prop.cpp` | `a6f0b826ab6de4bd2cb28ea0b605f8a1e2b34ba8ec6444152c8b1511ae21dc1b` | `push_physics_prop.o` | `norrath_construct_PushPhysicsProp` | 968 |
| `ctors-lava-wraith-push-trigger/push_trigger.cpp` | `bbe5bc991fb96f927173c7e0c875ff462999d900dbc2cda205b0ccd81e195d6e` | `push_trigger.o` | `norrath_construct_PushTrigger` | 524 |
| `ctors-lava-wraith-roll-door/roll_door.cpp` | `4ba7433f7223595a8c05b6476e66d2fa81a22706e0ae2ce47b1ff970de25f98c` | `roll_door.o` | `norrath_construct_RollDoor` | 660 |
| `ctors-lava-wraith-rondo/rondo.cpp` | `05c459a69d04c258684b3612105c7dad7d31ae9903457d0d31ebfdba5af29bee` | `rondo.o` | `norrath_construct_Rondo` | 748 |
| `ctors-lava-wraith-savepoint/savepoint.cpp` | `23efdf1e465ad5b20cdf8cbbae36302e8023c186a1173bfd6106a6223b26a6c5` | `savepoint.o` | `norrath_construct_Savepoint` | 780 |
| `ctors-lava-wraith-scorpion/scorpion.cpp` | `0d43264a11ec2963d351dcd507f13a0d24a1d89fe56f00445a9e321e7027c0b2` | `scorpion.o` | `norrath_construct_Scorpion` | 1140 |
| `ctors-lava-wraith-sea-monster/sea_monster.cpp` | `740facaaa26cf735f98095473088db9942fa4d102c00046842770de11b50e544` | `sea_monster.o` | `norrath_construct_SeaMonster` | 856 |
| `ctors-lava-wraith-skeleton/skeleton.cpp` | `3dccf79354b645d97fe88b753da44e1f0eef314184731143b576fd9edc5f7f59` | `skeleton.o` | `norrath_construct_Skeleton` | 3872 |
| `ctors-lava-wraith-skelight/skelight.cpp` | `979db85950b71fe81945f8f8454775d132a02dae7cbc6d6434b33b46e96a1e2e` | `skelight.o` | `norrath_construct_Skelight` | 1072 |
| `ctors-lava-wraith-soul/soul.cpp` | `89c1169c105a5f16c968e873e65a1e2598cb470df71bc019fb77115b4e4babca` | `soul.o` | `norrath_construct_Soul` | 908 |
| `ctors-lava-wraith-spider-queen/spider_queen.cpp` | `87ad368965350666cf143ebc5a031ae6117eec8be4f96194a51a32ffc8c48745` | `spider_queen.o` | `norrath_construct_SpiderQueen` | 1344 |
| `ctors-lava-wraith-static-fire/static_fire.cpp` | `930c7e25e60651a4fbcc1ac0d08a230c2948bc225dc8f951c42b49ad7cb1fcdd` | `static_fire.o` | `norrath_construct_StaticFire` | 972 |
| `ctors-lava-wraith-super-orc/super_orc.cpp` | `7436538ead0c008132186223ce4a675bac87826cc8fa3a810835c78dbd1bd2e4` | `super_orc.o` | `norrath_construct_SuperOrc` | 1060 |
| `ctors-lava-wraith-switch/switch.cpp` | `8a2d96d0d7e38126b9672ffca52d5dbe67b625f44cb06ee9f6f8a1f3fc23281f` | `switch.o` | `norrath_construct_Switch` | 548 |
| `ctors-lava-wraith-torch/torch.cpp` | `4b87d48f30aebf132f3c0fb6d33b56dc89118933ef9604d2166b2024fd32f28d` | `torch.o` | `norrath_construct_Torch` | 692 |
| `ctors-lava-wraith-ulthork/ulthork.cpp` | `6d6bb760b6f24e57cce7915f8d3c8dba31eb6280f738e3fcbdef3bf81d894e65` | `ulthork.o` | `norrath_construct_Ulthork` | 1184 |
| `ctors-lava-wraith-undead-knight-clone/undead_knight_clone.cpp` | `229ef729af7a685d3ff2b12c2d22732053687b6ada4e360f446d476a1eb25682` | `undead_knight_clone.o` | `norrath_construct_UndeadKnightClone` | 1404 |
| `ctors-lava-wraith-undead-knight/undead_knight.cpp` | `e917d8ffa0ec111844f721460578cc524851e39dd912f1c7a3e85284434fba63` | `undead_knight.o` | `norrath_construct_UndeadKnight` | 1068 |
| `ctors-lava-wraith-vampire-lord/vampire_lord.cpp` | `56a9e21c95a6fbb38efdad51c702ea054d62efd8c77eb09166dbc4f3fa2b5af3` | `vampire_lord.o` | `norrath_construct_VampireLord` | 1592 |
| `ctors-lava-wraith-vampire/vampire.cpp` | `81d3b7e9a014d2860d71cd99049db61bdf11f3800f9be4a66d24bdeac6be349b` | `vampire.o` | `norrath_construct_Vampire` | 900 |
| `ctors-lava-wraith-water-spout/water_spout.cpp` | `95bba58977d4f7e85615a475ba890364b10c818d69d43b2b6a3dbb0c3743f66b` | `water_spout.o` | `norrath_construct_WaterSpout` | 416 |
| `ctors-lava-wraith-wave-maker/wave_maker.cpp` | `eda3a8883b70f08b0df40505d17ae4d741c94ef845270d5faab2390f45d15b8e` | `wave_maker.o` | `norrath_construct_WaveMaker` | 1956 |
| `ctors-lava-wraith-weapon-rack/weapon_rack.cpp` | `a27de1078600861ad8ce7ac69661f81f2bb11a8f7bdc0fe2eb5c196b4ae5e7e7` | `weapon_rack.o` | `norrath_construct_WeaponRack` | 568 |
| `ctors-lava-wraith-web-trap/web_trap.cpp` | `8df537450eb04938f98ba6a6259fd50e7e91f91dafcb5d5a420b14d86359a775` | `web_trap.o` | `norrath_construct_WebTrap` | 1108 |
| `ctors-lava-wraith-wood-elf-soldier/wood_elf_soldier.cpp` | `51564a2db35b75349591bf167457c07eda22f360949a9dbce29bcd7b91787251` | `wood_elf_soldier.o` | `norrath_construct_WoodElfSoldier` | 1560 |
| `ctors-lava-wraith-wraith/wraith.cpp` | `24fd491dc5d4ece4d2d3930cd5dcc773277b3d826318b573db6a9a3b373777bd` | `wraith.o` | `norrath_construct_Wraith` | 900 |

Shared header `ctors-lava-wraith-detail/detail.hpp` sha256 `7c3b1c97fb86dc90a580fcb4ad53328d2156e2c5a539ef17b78fb9ba000d0b7a`; `host_support.hpp` is
host-only.

## Build-plan entries needed

One entry per row above, profile `authored_cpp`, kind `independently_authored_source`,
`header_closure` `["sources/ctors-lava-wraith-detail/detail.hpp"]`, the source and
sha256 from the table, and the object name from the table. Add `detail.hpp` and the 33
sources to `files` and the snapshot, and 33 to `default_authored_inputs`. Only the
`.cpp` units outside `host/` are plan inputs; `host/` files and `host_support.hpp` are
not.

## Imports closed

`norrath_construct_LavaMonster`, `norrath_construct_MaleDarkElf`, `norrath_construct_Minion`, `norrath_construct_MummyKing`, `norrath_construct_Mummy`, `norrath_construct_NPC`, `norrath_construct_Orc`, `norrath_construct_PushPhysicsProp`, `norrath_construct_PushTrigger`, `norrath_construct_RollDoor`, `norrath_construct_Rondo`, `norrath_construct_Savepoint`, `norrath_construct_Scorpion`, `norrath_construct_SeaMonster`, `norrath_construct_Skeleton`, `norrath_construct_Skelight`, `norrath_construct_Soul`, `norrath_construct_SpiderQueen`, `norrath_construct_StaticFire`, `norrath_construct_SuperOrc`, `norrath_construct_Switch`, `norrath_construct_Torch`, `norrath_construct_Ulthork`, `norrath_construct_UndeadKnightClone`, `norrath_construct_UndeadKnight`, `norrath_construct_VampireLord`, `norrath_construct_Vampire`, `norrath_construct_WaterSpout`, `norrath_construct_WaveMaker`, `norrath_construct_WeaponRack`, `norrath_construct_WebTrap`, `norrath_construct_WoodElfSoldier`, `norrath_construct_Wraith`

## Imports opened (new unresolved names; 188 only this package references)

Callee names, in the original's terms: Creature `setParameters`, `setAnimEventFlags`,
`setAnimEventFlagsResMap`, `finishConstruction`; `objectERand`, `findTagFloat`,
`lightDeactivate`, `addIcon`, `strticmp`, `gameLineGet`, `gameLineGetPtr`,
`worldHitscan`, `matTransform`, `getWaveletMat`, `objectFindAnimation`, `createRandom`,
`item::create`, `Web::Web`, the soft-double `dpadd` (0x2fb5c0), `dpmul` (0x2fb680),
`dptofp` (0x2fbc20), and the per-class methods named below. Data views are named
`norrath_ctors_lava_wraith_view_<address>` and carry no payload.

```
norrath_body_init
norrath_creature_finish_construction
norrath_creature_set_anim_event_flags
norrath_creature_set_anim_event_flags_res_map
norrath_creature_set_parameters
norrath_ctors_lava_wraith_view_34b474
norrath_ctors_lava_wraith_view_42b010
norrath_ctors_lava_wraith_view_42b330
norrath_ctors_lava_wraith_view_42b3d8
norrath_ctors_lava_wraith_view_42b478
norrath_ctors_lava_wraith_view_42b7e8
norrath_ctors_lava_wraith_view_42bcb8
norrath_ctors_lava_wraith_view_42bd18
norrath_ctors_lava_wraith_view_42c228
norrath_ctors_lava_wraith_view_42c570
norrath_ctors_lava_wraith_view_42c630
norrath_ctors_lava_wraith_view_42c8e0
norrath_ctors_lava_wraith_view_42ca58
norrath_ctors_lava_wraith_view_42cbd0
norrath_ctors_lava_wraith_view_42ccf0
norrath_ctors_lava_wraith_view_42ce60
norrath_ctors_lava_wraith_view_42d048
norrath_ctors_lava_wraith_view_42d138
norrath_ctors_lava_wraith_view_42d2b0
norrath_ctors_lava_wraith_view_42d480
norrath_ctors_lava_wraith_view_42d578
norrath_ctors_lava_wraith_view_42d650
norrath_ctors_lava_wraith_view_42d700
norrath_ctors_lava_wraith_view_42d7d0
norrath_ctors_lava_wraith_view_42d8d0
norrath_ctors_lava_wraith_view_42dbf0
norrath_ctors_lava_wraith_view_42dc40
norrath_ctors_lava_wraith_view_438c70
norrath_ctors_lava_wraith_view_448fb0
norrath_ctors_lava_wraith_view_449310
norrath_ctors_lava_wraith_view_449550
norrath_ctors_lava_wraith_view_449820
norrath_ctors_lava_wraith_view_449940
norrath_ctors_lava_wraith_view_449c20
norrath_ctors_lava_wraith_view_44a390
norrath_ctors_lava_wraith_view_44ab70
norrath_ctors_lava_wraith_view_44adb0
norrath_ctors_lava_wraith_view_44aea0
norrath_ctors_lava_wraith_view_44af90
norrath_ctors_lava_wraith_view_44b080
norrath_ctors_lava_wraith_view_44b260
norrath_ctors_lava_wraith_view_44b350
norrath_ctors_lava_wraith_view_44b630
norrath_ctors_lava_wraith_view_44b720
norrath_ctors_lava_wraith_view_44b810
norrath_ctors_lava_wraith_view_44c710
norrath_ctors_lava_wraith_view_44c7a0
norrath_ctors_lava_wraith_view_44def0
norrath_ctors_lava_wraith_view_44dfe0
norrath_ctors_lava_wraith_view_44e2b0
norrath_ctors_lava_wraith_view_44f3d8
norrath_ctors_lava_wraith_view_44f5b8
norrath_ctors_lava_wraith_view_44f798
norrath_ctors_lava_wraith_view_44fa38
norrath_ctors_lava_wraith_view_44fb28
norrath_ctors_lava_wraith_view_44fc18
norrath_ctors_lava_wraith_view_44fd08
norrath_ctors_lava_wraith_view_44fdf8
norrath_ctors_lava_wraith_view_44fee8
norrath_ctors_lava_wraith_view_450440
norrath_ctors_lava_wraith_view_46cd20
norrath_ctors_lava_wraith_view_46cd30
norrath_ctors_lava_wraith_view_46cd40
norrath_ctors_lava_wraith_view_46cd50
norrath_ctors_lava_wraith_view_46cd60
norrath_ctors_lava_wraith_view_46cd70
norrath_ctors_lava_wraith_view_46d7b8
norrath_ctors_lava_wraith_view_470358
norrath_ctors_lava_wraith_view_4718e8
norrath_ctors_lava_wraith_view_4718f8
norrath_ctors_lava_wraith_view_471908
norrath_ctors_lava_wraith_view_471918
norrath_ctors_lava_wraith_view_471928
norrath_ctors_lava_wraith_view_471938
norrath_ctors_lava_wraith_view_471948
norrath_ctors_lava_wraith_view_471958
norrath_ctors_lava_wraith_view_471968
norrath_ctors_lava_wraith_view_471978
norrath_ctors_lava_wraith_view_471988
norrath_ctors_lava_wraith_view_471998
norrath_ctors_lava_wraith_view_472040
norrath_ctors_lava_wraith_view_472050
norrath_ctors_lava_wraith_view_472060
norrath_ctors_lava_wraith_view_472c10
norrath_ctors_lava_wraith_view_472c28
norrath_ctors_lava_wraith_view_472c38
norrath_ctors_lava_wraith_view_472c50
norrath_ctors_lava_wraith_view_472c60
norrath_ctors_lava_wraith_view_472c78
norrath_ctors_lava_wraith_view_472c88
norrath_ctors_lava_wraith_view_472ca0
norrath_ctors_lava_wraith_view_472cb0
norrath_ctors_lava_wraith_view_472cc8
norrath_ctors_lava_wraith_view_473f20
norrath_ctors_lava_wraith_view_473f30
norrath_ctors_lava_wraith_view_473f40
norrath_ctors_lava_wraith_view_473f58
norrath_ctors_lava_wraith_view_473f70
norrath_ctors_lava_wraith_view_473f88
norrath_ctors_lava_wraith_view_473fa0
norrath_ctors_lava_wraith_view_473fb0
norrath_ctors_lava_wraith_view_473fc0
norrath_ctors_lava_wraith_view_473fd8
norrath_ctors_lava_wraith_view_473ff0
norrath_ctors_lava_wraith_view_474000
norrath_ctors_lava_wraith_view_474010
norrath_ctors_lava_wraith_view_474028
norrath_ctors_lava_wraith_view_474040
norrath_ctors_lava_wraith_view_474050
norrath_ctors_lava_wraith_view_474060
norrath_ctors_lava_wraith_view_474540
norrath_ctors_lava_wraith_view_474550
norrath_ctors_lava_wraith_view_474560
norrath_ctors_lava_wraith_view_474570
norrath_ctors_lava_wraith_view_477568
norrath_ctors_lava_wraith_view_477580
norrath_ctors_lava_wraith_view_477598
norrath_ctors_lava_wraith_view_4775b0
norrath_ctors_lava_wraith_view_4775c8
norrath_ctors_lava_wraith_view_4775d8
norrath_ctors_lava_wraith_view_4778c8
norrath_ctors_lava_wraith_view_4778d8
norrath_ctors_lava_wraith_view_483e28
norrath_ctors_lava_wraith_view_483f88
norrath_ctors_lava_wraith_view_484fa0
norrath_ctors_lava_wraith_view_484fa8
norrath_ctors_lava_wraith_view_485240
norrath_ctors_lava_wraith_view_4852a8
norrath_ctors_lava_wraith_view_485330
norrath_ctors_lava_wraith_view_4853b8
norrath_ctors_lava_wraith_view_4853d0
norrath_ctors_lava_wraith_view_4853d8
norrath_ctors_lava_wraith_view_4857e8
norrath_ctors_lava_wraith_view_4857f0
norrath_ctors_lava_wraith_view_4857f8
norrath_ctors_lava_wraith_view_485800
norrath_ctors_lava_wraith_view_485808
norrath_ctors_lava_wraith_view_485810
norrath_ctors_lava_wraith_view_485818
norrath_ctors_lava_wraith_view_485820
norrath_ctors_lava_wraith_view_485828
norrath_ctors_lava_wraith_view_4858b8
norrath_ctors_lava_wraith_view_4858c0
norrath_ctors_lava_wraith_view_4858c8
norrath_ctors_lava_wraith_view_486118
norrath_ctors_lava_wraith_view_486120
norrath_ctors_lava_wraith_view_486128
norrath_ctors_lava_wraith_view_486130
norrath_ctors_lava_wraith_view_601030
norrath_ctors_lava_wraith_view_601fd0
norrath_game_line_get
norrath_game_line_get_ptr
norrath_game_object_e_rand
norrath_game_object_find_tag_float
norrath_item_create
norrath_item_create_random
norrath_lava_monster_get_model
norrath_light_deactivate
norrath_male_dark_elf_init_type
norrath_male_dark_elf_set_state
norrath_map_add_icon
norrath_mat_transform
norrath_mummy_init_type
norrath_mummy_set_state
norrath_mummy_set_weapon
norrath_object_find_animation
norrath_orc_init_type
norrath_orc_set_weapon
norrath_skeleton_init_type
norrath_skeleton_set_state
norrath_skelight_init_weapon
norrath_skelight_set_state
norrath_softdouble_add_2fb5c0
norrath_softdouble_multiply_2fb680
norrath_softdouble_to_float_2fbc20
norrath_soul_set_state
norrath_static_fire_init
norrath_strticmp
norrath_wave_maker_get_wavelet_mat
norrath_web_construct
norrath_wood_elf_soldier_init_type
norrath_wood_elf_soldier_set_state
norrath_world_hitscan
```

Names also referenced by other packages and still unresolved: `norrath_actors_a_view_44d0a8`, `norrath_actors_a_view_485358`, `norrath_factory_world_pointer_word`, `norrath_light_add`, `norrath_object_add_to_delete_list`, `norrath_world_check_for_water`, `norrath_world_find_closest_door`, `norrath_world_get_door_height`, `norrath_world_set_door_collision`, `strcpy`.

Names the package declares but that other packages already define (not counted above):
`norrath_creature_construct`, `norrath_game_object_construct`, `norrath_actor_find_tag_string`,
`norrath_game_object_find_tag_int`, `norrath_actor_add_run_list`,
`norrath_actor_add_slow_run_list`, `norrath_actor_animation_initialize`,
`norrath_actor_animation_add`, `norrath_actor_model_active_chunks`, `norrath_light_add`,
`norrath_lump_find_resource`, `norrath_lump_load`, `norrath_player_treasure_ramp`,
`norrath_integer_sine`, `norrath_integer_cosine`, `norrath_entity_fptodp`,
`norrath_list_oneway_link`, `norrath_world_*`, `norrath_factory_strcasecmp`, `strcmp`,
`strcpy`, `sprintf`, `memset`, `levelName`, and views `norrath_actor_view_4844a4`,
`norrath_creature_view_484cac`, `norrath_actors_b_view_448a70`,
`norrath_actors_a_view_44d0a8`, `norrath_actors_a_view_485358`,
`norrath_factory_world_pointer_word`.

## Authored but never executed

All 33 were executed only as host contracts and in the local emulator against the
original body. None ran on the EE, in a game or against real callees. Nothing here is an
original-byte match; compiled bodies are larger than the originals.

## Observed, inferred, unknown

Observed: every store offset, constant, branch and call order above, read from the
original and confirmed by the differential check. The `0x484cac` float and the `0x4852a8`
counter are data words, not literals; the other floats are literal-pool values.

Inferred: the callee names (all taken from the original's mangled names, but the C
signatures, e.g. which results are `void`, are assumptions); the three soft-double helpers
are named by address because no verified authored counterpart is known (`dpadd`,
`dpmul`, `dptofp`); `levelName` is assumed at `0x34b660` from `levelName + 81` in other
packages.

Unknown: what most fields mean (offsets are named by address only); the tag texts are
seen in the binary but not given meaning; the vtable and resource-map payloads; the owner
object at `0x34b474` and its method (LavaMonster) ; the contents and writers of the
resource tables at `0x42cbd0`, `0x42ccf0`, `0x42d8d0` and the load stamp `0x483f88`
(Orc, Skeleton); the buffers `0x601fd0` and `0x601030` and the line records (WaveMaker);
whether any caller uses these constructors' V0 (several return incidental words:
Torch 39, Wraith/Scorpion/etc. the finish result, SeaMonster, StaticFire, WaveMaker 1192);
R5900 results of `div.s` by zero (Savepoint) and of `rsqrt.s` (WaveMaker uses the
instruction through inline assembly on the EE and a divide/sqrt on hosts); two empty
counting loops in UndeadKnightClone that store nothing. Mummy, MaleDarkElf, Skeleton and
Orc switch on a table; the tables were read and their six or five targets are described
in each file's header comment. The vehicle, line and A-F constructors stay imports.
