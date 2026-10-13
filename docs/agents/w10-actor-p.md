# Spell, skill and actor virtuals: VampireLord to WoodElfSoldier

Package `w10-actor-p`, branch `agents/w10-actor-p`. Scope: the unresolved imports for the classes
VampireLord, WaterSpout, WaveMaker, WeaponRack, Weather (constructor only), WebTrap and WoodElfSoldier.
Every function is authored under its observed symbol, one per commit, with its own directory
`experimental/sources/w10-actor-p-<kind>-<class>/`. Private reference: `work/private/SLUS_205.65`,
SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses and byte counts are
that file's symbol values and extents (local evidence). Nothing here ran on an EE or as gameplay.

## Checks performed

- Every source compiles with the reviewed EE g++ and the `authored_cpp` flags (each unit alone).
- Every function has a host contract `host_check.cpp` next to it (not a plan input). It stubs the callees
  and compares call order, arguments and the memory image with expected values. Where objects hold 32-bit
  address words, the host contract keeps those words as offsets from a host base (the shared header
  `w10-actor-p-common/access.hpp`, which is not an EE input by itself).
- Private differential check (outside git): a small R5900 interpreter loads the original image, links the
  freshly compiled function with every undefined symbol bound to the original's address, then runs the
  original body and the authored body from identical memory snapshots with identical Python stub
  callees. Call sequence with arguments (pointer contents, not stack addresses), return value where
  meaningful, every changed byte outside the stack, and instruction coverage of the original body are
  compared. Scenario counts and coverage are in each commit message. For the larger functions
  (WaterSpout and WaveMaker run, VampireLord set-state) a deliberate edit of a constant was shown to be
  detected by the same run. The interpreter is IEEE (not the R5900's arithmetic), so float rounding was
  compared only as IEEE; `rsqrt.s` and `sqrt.s` are emulated in the interpreter.
- Observed, inferred and unknown are separated in each source header. Names of fields are not
  established anywhere in this package; offsets are used.

## Functions

| source (under `experimental/sources/`) | emitted symbol | bytes |
|---|---|---|
| `w10-actor-p-clone-10waterspout/clone.cpp` | `clone__10WaterSpout` | 272 |
| `w10-actor-p-clone-10weaponrack/clone.cpp` | `clone__10WeaponRack` | 272 |
| `w10-actor-p-clone-11vampirelord/clone.cpp` | `clone__11VampireLord` | 832 |
| `w10-actor-p-clone-7webtrap/clone.cpp` | `clone__7WebTrap` | 328 |
| `w10-actor-p-clone-9wavemaker/clone.cpp` | `clone__9WaveMaker` | 272 |
| `w10-actor-p-ctor-7weather/weather.cpp` | `__7Weatheri` | 80 |
| `w10-actor-p-dtor-10waterspout/dtor.cpp` | `_$_10WaterSpout` | 88 |
| `w10-actor-p-dtor-10weaponrack/dtor.cpp` | `_$_10WeaponRack` | 88 |
| `w10-actor-p-dtor-11vampirelord/dtor.cpp` | `_$_11VampireLord` | 112 |
| `w10-actor-p-dtor-14woodelfsoldier/dtor.cpp` | `_$_14WoodElfSoldier` | 112 |
| `w10-actor-p-dtor-7webtrap/dtor.cpp` | `_$_7WebTrap` | 112 |
| `w10-actor-p-dtor-9wavemaker/dtor.cpp` | `_$_9WaveMaker` | 88 |
| `w10-actor-p-get-model-11vampirelord/get_model.cpp` | `getModel__11VampireLord` | 44 |
| `w10-actor-p-get-poke-name-10weaponrack/get_poke_name.cpp` | `msg_getPokeName__10WeaponRackPUsi` | 60 |
| `w10-actor-p-msg-draw-10waterspout/msg_draw.cpp` | `msg_draw__10WaterSpout` | 64 |
| `w10-actor-p-msg-draw-10weaponrack/msg_draw.cpp` | `msg_draw__10WeaponRack` | 320 |
| `w10-actor-p-msg-draw-11vampirelord/msg_draw.cpp` | `msg_draw__11VampireLord` | 288 |
| `w10-actor-p-msg-draw-7webtrap/msg_draw.cpp` | `msg_draw__7WebTrap` | 412 |
| `w10-actor-p-msg-hurt-11vampirelord/msg_hurt.cpp` | `msg_hurt__11VampireLordfP10GameObjecti` | 200 |
| `w10-actor-p-msg-run-10waterspout/msg_run.cpp` | `msg_run__10WaterSpout` | 2096 |
| `w10-actor-p-msg-run-11vampirelord/msg_run.cpp` | `_ZN12_GLOBAL__N_1L10aim_targetEPhb` | 252 |
| `w10-actor-p-msg-run-11vampirelord/msg_run.cpp` | `_ZN12_GLOBAL__N_1L10spawn_batsEPh` | 428 |
| `w10-actor-p-msg-run-11vampirelord/msg_run.cpp` | `_ZN12_GLOBAL__N_1L11attack_waitEPhiiii` | 404 |
| `w10-actor-p-msg-run-11vampirelord/msg_run.cpp` | `_ZN12_GLOBAL__N_1L11choose_nextEPhii` | 396 |
| `w10-actor-p-msg-run-11vampirelord/msg_run.cpp` | `_ZN12_GLOBAL__N_1L12weapon_blockEPhjj.constprop.0` | 504 |
| `w10-actor-p-msg-run-11vampirelord/msg_run.cpp` | `_ZN12_GLOBAL__N_1L13approach_stepEPhS0_jjjj.constprop.0` | 488 |
| `w10-actor-p-msg-run-11vampirelord/msg_run.cpp` | `_ZN4w10p10call_entryEPhjm.isra.0` | 28 |
| `w10-actor-p-msg-run-11vampirelord/msg_run.cpp` | `msg_run__11VampireLord` | 4672 |
| `w10-actor-p-msg-run-7webtrap/msg_run.cpp` | `msg_run__7WebTrap` | 1804 |
| `w10-actor-p-msg-run-9wavemaker/msg_run.cpp` | `msg_run__9WaveMaker` | 2836 |
| `w10-actor-p-msg-trigger-11vampirelord/msg_trigger.cpp` | `msg_trigger__11VampireLordi` | 684 |
| `w10-actor-p-poke-10weaponrack/poke.cpp` | `msg_poke__10WeaponRacki` | 8 |
| `w10-actor-p-set-state-11vampirelord/set_state.cpp` | `_ZN12_GLOBAL__N_1L5chainEPhj` | 332 |
| `w10-actor-p-set-state-11vampirelord/set_state.cpp` | `setState__11VampireLordi` | 1964 |
| `w10-actor-p-tf-10waterspout/typeinfo.cpp` | `__tf10WaterSpout` | 92 |
| `w10-actor-p-tf-10weaponrack/typeinfo.cpp` | `__tf10WeaponRack` | 92 |
| `w10-actor-p-tf-11vampirelord/typeinfo.cpp` | `__tf11VampireLord` | 92 |
| `w10-actor-p-tf-14woodelfsoldier/typeinfo.cpp` | `__tf14WoodElfSoldier` | 92 |
| `w10-actor-p-tf-7webtrap/typeinfo.cpp` | `__tf7WebTrap` | 92 |
| `w10-actor-p-tf-9wavemaker/typeinfo.cpp` | `__tf9WaveMaker` | 92 |

## Imports closed

All 32 names of the package, each as an observed symbol:

`_$_11VampireLord`, `__tf11VampireLord`, `clone__11VampireLord`, `getModel__11VampireLord`,
`msg_draw__11VampireLord`, `msg_hurt__11VampireLordfP10GameObjecti`, `msg_run__11VampireLord`,
`msg_trigger__11VampireLordi`, `setState__11VampireLordi`, `_$_10WaterSpout`, `__tf10WaterSpout`,
`clone__10WaterSpout`, `msg_draw__10WaterSpout`, `msg_run__10WaterSpout`, `_$_9WaveMaker`, `__tf9WaveMaker`,
`clone__9WaveMaker`, `msg_run__9WaveMaker`, `_$_10WeaponRack`, `__tf10WeaponRack`, `clone__10WeaponRack`,
`msg_draw__10WeaponRack`, `msg_getPokeName__10WeaponRackPUsi`, `msg_poke__10WeaponRacki`, `__7Weatheri`,
`_$_7WebTrap`, `__tf7WebTrap`, `clone__7WebTrap`, `msg_draw__7WebTrap`, `msg_run__7WebTrap`,
`_$_14WoodElfSoldier`, `__tf14WoodElfSoldier`.

Defined data, not an import: `__ti11VampireLord`, `__ti14WoodElfSoldier`, `__ti7WebTrap`, `__ti9WaveMaker`,
`__ti10WaterSpout`, `__ti10WeaponRack` (12-byte zero-initialised typeinfo objects), and the private data of
WaterSpout's run function (three 80-byte, 16-byte-aligned particle definitions and three first-use flags).
A mini link of all 32 units (`ld -r`) had no duplicate definitions.

## Imports opened

Names that no other source on this branch's base mentions (other agents' branches may define some of them
under the same or an `norrath_` spelling): `DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState`,
`_vt$7Weather`, `batParams`, `convertToBat__11VampireLord`, `convertToLord__11VampireLord`,
`doWaveletCollision__9WaveMakerP6Point3i`, `doWaveletReflection__9WaveMakerP6Point3`,
`doWavelet__9WaveMakerP6Point3P8Matrix33i`, `dptoli`, `enemyInZone__8Creaturefs`,
`flee2__8CreatureP10GameObject`,
`fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`,
`litodp`, `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`,
`modelSetRelightAttachment__FG6Point3`, `produceMeleeDamage__8CreaturefiiiiPQ24item11description`,
`steerAndRouteToEnemy__8Creature`, `turnToFaceEnemy__8Creature`, `waveFroth`, `waveMainAlternate`,
`waveMainParticle`, `waveRandomFoam`, `waveSplashBig`, `waveSplashSmall`, `waveTrail`,
`worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii`.
Also used by name, as private (static) data of the original WaveMaker/VampireLord translation units:
`range`, `splashInfoBits`, `firstTime` (the symbol table has two; the flag read by WaveMaker's run function is
the initialised byte at the lower address, `0x483e40`), `smokeParticle` (seven local statics share the name; the
VampireLord run function uses the one at `0x7d7f20`) and `batParams` (an 8-byte table whose first entry is a
text pointer and second is null). The private paired runs bound these by address; the relocatable link cannot
tell the duplicates apart, so a data provider has to give each a distinct name. Every other callee is used under
its observed mangled name; where another package defines the same function under an authored `norrath_` name (for
example `norrath_game_object_construct`) both spellings stay unresolved until one provider is chosen. The Weather
constructor calls the GameObject constructor by its observed name
`__10GameObjectG6Point3PQ210GameObject11resourceMapPPc`.

## Authored but never executed

All of it. No function ran on an EE or in the game. The differential checks above are interpreter runs against
the original image with stubbed callees; they do not exercise the callees' own behaviour, interrupts, the R5900's
non-IEEE float arithmetic (`mul.s`, `add.s`, rounding of `rsqrt.s` and `sqrt.s`) or hardware timing. NaN payloads
differ with operand order; the runs avoided NaN inputs. WaveMaker's two short nop delay loops are reproduced as
`asm volatile` loops with no effect other than time. WaveMaker's run function, with the closing flag at the line
block's offset 8 clear, reads stack storage that the original never writes; the paired run compared that case
only where the storage is zero. The function-local statics of WaterSpout's run function are manual flags and
byte arrays, not C++ guarded statics.

## Unknown

- What any field, flag or timer in these classes means; no field names are claimed. The state numbers of
  VampireLord's state functions are unnamed; the state summaries in the source headers are inferred from calls.
- The meaning of the float and integer constants in the run and draw functions (all read as literals, several
  from the original's small-data pool, which has no symbols).
- Whether `batParams`, `range`, `splashInfoBits`, the particle definitions and `firstTime` are owned by one
  WaveMaker/VampireLord data unit, and their layouts beyond what the code reads (the ParticleDef size 80 and the
  16-byte alignment are read from the addresses of the three WaterSpout statics only).
- The size of the strike description structure used by VampireLord's weapon block (at most 80 bytes by the stack
  layout; the compared fields are at offsets 16..33 and 36..63).
- `DrawWebSplay` (1864 bytes) is called by WebTrap's draw function and is not part of this package.

## Build-plan entries needed

Profile `authored_cpp`, kind `independently_authored_source`; the integrator assigns `input_order`. `host_check.cpp` files are not plan inputs. `sources/w10-actor-p-common/access.hpp` must be added to `files` (and the snapshot) and is the `header_closure` of every unit that includes it. Object names are private-plan names; the integrator may change them.

```json
{"source":"sources/w10-actor-p-clone-10waterspout/clone.cpp","object_name":"w10p_clone-10waterspout.o","sha256":"941a5dbd4c00bfa50a37355728ba633343d3c1c131767869e0d0b80212370a7a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__10WaterSpout","compiled_body_bytes":272}]}
{"source":"sources/w10-actor-p-clone-10weaponrack/clone.cpp","object_name":"w10p_clone-10weaponrack.o","sha256":"f392705da0d596956e198c80ca98eb600016f08582014e77dafd66df55bb1a6a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__10WeaponRack","compiled_body_bytes":272}]}
{"source":"sources/w10-actor-p-clone-11vampirelord/clone.cpp","object_name":"w10p_clone-11vampirelord.o","sha256":"390c120c5faf539995c3591caafe5565b6bcae14c53443589dd822ca90a622e7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__11VampireLord","compiled_body_bytes":832}]}
{"source":"sources/w10-actor-p-clone-7webtrap/clone.cpp","object_name":"w10p_clone-7webtrap.o","sha256":"bc1d633900fffb0be6e253dc3436a2b88139aa87619720211ea02861333d565a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__7WebTrap","compiled_body_bytes":328}]}
{"source":"sources/w10-actor-p-clone-9wavemaker/clone.cpp","object_name":"w10p_clone-9wavemaker.o","sha256":"b1cbb1a620fa70342b85897be87badc26a64e7851ec0e39994b44db3353ebf6c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__9WaveMaker","compiled_body_bytes":272}]}
{"source":"sources/w10-actor-p-ctor-7weather/weather.cpp","object_name":"w10p_ctor-7weather.o","sha256":"a0a88675601517536c404b80c51b2bb223326dffd8ff1b36bc91db0da4e83d9f","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__7Weatheri","compiled_body_bytes":80}]}
{"source":"sources/w10-actor-p-dtor-10waterspout/dtor.cpp","object_name":"w10p_dtor-10waterspout.o","sha256":"9ff476efb3d12c1c3e7fe28df02e8c82adbff76a36dfad72ec624141d502a27e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_10WaterSpout","compiled_body_bytes":88}]}
{"source":"sources/w10-actor-p-dtor-10weaponrack/dtor.cpp","object_name":"w10p_dtor-10weaponrack.o","sha256":"6c7477b57f4b70dec9e15ca1e1f9c23c4366f0ab326916d665f749782a973cfd","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_10WeaponRack","compiled_body_bytes":88}]}
{"source":"sources/w10-actor-p-dtor-11vampirelord/dtor.cpp","object_name":"w10p_dtor-11vampirelord.o","sha256":"fbc1c6b548663e190b1fd70d5918625f52b8a8cc2b4f50a4d02b46eff264a2c6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_11VampireLord","compiled_body_bytes":112}]}
{"source":"sources/w10-actor-p-dtor-14woodelfsoldier/dtor.cpp","object_name":"w10p_dtor-14woodelfsoldier.o","sha256":"5f34e8321e0f59cfc5cced398c447b9f9eae5d067f74dc410d1abc49ff67616f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_14WoodElfSoldier","compiled_body_bytes":112}]}
{"source":"sources/w10-actor-p-dtor-7webtrap/dtor.cpp","object_name":"w10p_dtor-7webtrap.o","sha256":"aad168f76beeb5a07d3b2853bf4cfeb6a4d22d8037f1313b0aebe1ad5bfcf194","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_7WebTrap","compiled_body_bytes":112}]}
{"source":"sources/w10-actor-p-dtor-9wavemaker/dtor.cpp","object_name":"w10p_dtor-9wavemaker.o","sha256":"d2bf001fde0c17d9de2bdc0d83cbcbf1fde7aa50c20498a15690dd955e2555c1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_9WaveMaker","compiled_body_bytes":88}]}
{"source":"sources/w10-actor-p-get-model-11vampirelord/get_model.cpp","object_name":"w10p_get-model-11vampirelord.o","sha256":"1ed210763a1b92e63ed49534873c931627d8b23de8a5c3e8abc1004e693fd94c","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"getModel__11VampireLord","compiled_body_bytes":44}]}
{"source":"sources/w10-actor-p-get-poke-name-10weaponrack/get_poke_name.cpp","object_name":"w10p_get-poke-name-10weaponrack.o","sha256":"1163bd6581a2fd698e6c29dce5dbaf3599686a878f28fdd1f87f643e2e9d7230","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_getPokeName__10WeaponRackPUsi","compiled_body_bytes":60}]}
{"source":"sources/w10-actor-p-msg-draw-10waterspout/msg_draw.cpp","object_name":"w10p_msg-draw-10waterspout.o","sha256":"8862b03d1d5f1da3d3aef3d1de73b693be861501fec3d2c346203ce0e83effa9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__10WaterSpout","compiled_body_bytes":64}]}
{"source":"sources/w10-actor-p-msg-draw-10weaponrack/msg_draw.cpp","object_name":"w10p_msg-draw-10weaponrack.o","sha256":"14aeda13d4d5da7fd1f2305dfe405b3531b4a5c56d7fda1d96c62264ccf27d03","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__10WeaponRack","compiled_body_bytes":320}]}
{"source":"sources/w10-actor-p-msg-draw-11vampirelord/msg_draw.cpp","object_name":"w10p_msg-draw-11vampirelord.o","sha256":"e254620b46bc3e80397cf04549625d7640b9979cdb60aa16fc90621b03ab5b23","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__11VampireLord","compiled_body_bytes":288}]}
{"source":"sources/w10-actor-p-msg-draw-7webtrap/msg_draw.cpp","object_name":"w10p_msg-draw-7webtrap.o","sha256":"aace0e42ffd9bdfda8d47ec22c08cfb2ff26077c51255585a3dd7a6dbc279dd0","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__7WebTrap","compiled_body_bytes":412}]}
{"source":"sources/w10-actor-p-msg-hurt-11vampirelord/msg_hurt.cpp","object_name":"w10p_msg-hurt-11vampirelord.o","sha256":"5eca7b5a98b856aae1cad488e7cafa2f5013976c3112a1f8b92f3a790792240f","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_hurt__11VampireLordfP10GameObjecti","compiled_body_bytes":200}]}
{"source":"sources/w10-actor-p-msg-run-10waterspout/msg_run.cpp","object_name":"w10p_msg-run-10waterspout.o","sha256":"962412cd46eb754ff5dd6a0d7c58b60932eb1808954e4f809a1f589888d6aafc","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_run__10WaterSpout","compiled_body_bytes":2096}]}
{"source":"sources/w10-actor-p-msg-run-11vampirelord/msg_run.cpp","object_name":"w10p_msg-run-11vampirelord.o","sha256":"d5067a97c0ce400e6f432c449f8b12efb35b8caf421fb8ca2581f786b3d18a94","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_ZN12_GLOBAL__N_1L10aim_targetEPhb","compiled_body_bytes":252},{"name":"_ZN12_GLOBAL__N_1L10spawn_batsEPh","compiled_body_bytes":428},{"name":"_ZN12_GLOBAL__N_1L11attack_waitEPhiiii","compiled_body_bytes":404},{"name":"_ZN12_GLOBAL__N_1L11choose_nextEPhii","compiled_body_bytes":396},{"name":"_ZN12_GLOBAL__N_1L12weapon_blockEPhjj.constprop.0","compiled_body_bytes":504},{"name":"_ZN12_GLOBAL__N_1L13approach_stepEPhS0_jjjj.constprop.0","compiled_body_bytes":488},{"name":"_ZN4w10p10call_entryEPhjm.isra.0","compiled_body_bytes":28},{"name":"msg_run__11VampireLord","compiled_body_bytes":4672}]}
{"source":"sources/w10-actor-p-msg-run-7webtrap/msg_run.cpp","object_name":"w10p_msg-run-7webtrap.o","sha256":"2685807417a1ee5c62f52028c91608184b9d733b2b9c2decba9bf800ca699c30","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_run__7WebTrap","compiled_body_bytes":1804}]}
{"source":"sources/w10-actor-p-msg-run-9wavemaker/msg_run.cpp","object_name":"w10p_msg-run-9wavemaker.o","sha256":"c95694d6f9adf3b7a6ead9c299a8fcd0d5ad70ab9f27778077be44fcb95618dd","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_run__9WaveMaker","compiled_body_bytes":2836}]}
{"source":"sources/w10-actor-p-msg-trigger-11vampirelord/msg_trigger.cpp","object_name":"w10p_msg-trigger-11vampirelord.o","sha256":"29ad774bb16f88754b97bc6d959e1970fe3b7c4239997912c3b038da66abdc71","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_trigger__11VampireLordi","compiled_body_bytes":684}]}
{"source":"sources/w10-actor-p-poke-10weaponrack/poke.cpp","object_name":"w10p_poke-10weaponrack.o","sha256":"71cb18a4c056bc4d9975d13a59d9448de8ab2c59a82ac20414336b8ed11bfd35","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_poke__10WeaponRacki","compiled_body_bytes":8}]}
{"source":"sources/w10-actor-p-set-state-11vampirelord/set_state.cpp","object_name":"w10p_set-state-11vampirelord.o","sha256":"567d140cfa4d04dd5c1e03fd21e220353dfdc7ac3f63feb112033f0690bfea13","header_closure":["sources/w10-actor-p-common/access.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_ZN12_GLOBAL__N_1L5chainEPhj","compiled_body_bytes":332},{"name":"setState__11VampireLordi","compiled_body_bytes":1964}]}
{"source":"sources/w10-actor-p-tf-10waterspout/typeinfo.cpp","object_name":"w10p_tf-10waterspout.o","sha256":"3986db32d3fb24d2451cbeddb2c09ce99a93952bec7115e740370d3e6e875538","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf10WaterSpout","compiled_body_bytes":92}]}
{"source":"sources/w10-actor-p-tf-10weaponrack/typeinfo.cpp","object_name":"w10p_tf-10weaponrack.o","sha256":"344ce8df3a9506193ddaaef1022583aaac23e1b9639d8c17b7d63380c66717c3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf10WeaponRack","compiled_body_bytes":92}]}
{"source":"sources/w10-actor-p-tf-11vampirelord/typeinfo.cpp","object_name":"w10p_tf-11vampirelord.o","sha256":"a4973821e5474ad6b7ed968fb78127f3a45d2ee5698a0221988a0f84fd61a146","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf11VampireLord","compiled_body_bytes":92}]}
{"source":"sources/w10-actor-p-tf-14woodelfsoldier/typeinfo.cpp","object_name":"w10p_tf-14woodelfsoldier.o","sha256":"272879934037da4e20eb65e156678802d374ae8e386db6383e2a63189d043985","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf14WoodElfSoldier","compiled_body_bytes":92}]}
{"source":"sources/w10-actor-p-tf-7webtrap/typeinfo.cpp","object_name":"w10p_tf-7webtrap.o","sha256":"3d6953029187c45eae2d34d407ed9f07ce46b4f7c8b458f98f21d7c5065c3752","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf7WebTrap","compiled_body_bytes":92}]}
{"source":"sources/w10-actor-p-tf-9wavemaker/typeinfo.cpp","object_name":"w10p_tf-9wavemaker.o","sha256":"2cf0d36aa4a268469e105688d64151fd4859e8428ba1d80c6e43fd9ce86d3db5","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf9WaveMaker","compiled_body_bytes":92}]}
```
