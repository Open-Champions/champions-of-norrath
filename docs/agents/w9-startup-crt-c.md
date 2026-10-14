# w9-startup-crt-c

Startup and runtime support (c): closing the unresolved imports of the partial build that belong to
the SIF/file-system RPC layer, a few C runtime pieces, font width/print helpers and the type-info and
virtual-table data of six classes. Every unit was compiled alone with the plan's `authored_cpp` flags
and the reviewed EE g++ (all compiled), and its host check was built with the host C++ compiler and
exited 0. Sizes of compiled bodies differ from the original (no byte-match claim). Nothing here was
executed on the EE or in game. No full partial build was run (private plan not built; the shared
plan was not edited).

## Imports closed (names)

`_sceRpcGetFPacket`, `_sceRpcGetFPacket2`, `_search_svdata`, `_alarm_bind`, `_alarm_rdata`,
`_sceFsWaitS`, `_sceFsSigSema`, `_fs_version` (with `_fswildcard`), `_exit`, `_sceSifSendCmd`
(a local queueing routine, unit `sif_send_cmd_internal`), `_fontStringSize__FP4FontPci`,
`_fontStringSizeu__FP4FontPUsi`, `_fontPrintu__FiiPUsi`, `_vt$7Vehicle`, `_vt$8SkulBoat`,
`_vt$11LavaVehicle`, `_vt$11AddParticle`, `_vt$11LightEffect`, `_vt$19ParticleMorphEffect`,
`__tf9MummyKing`, `__tf9Savepoint`, `__tf11LavaVehicle` (each with its own `__ti*` object:
`__ti9MummyKing`, `__ti9Savepoint`, `__ti11LavaVehicle`), `__throw_bad_typeid` (normal path only,
see Unknown), `__udivdi3`, `__umoddi3`, `_impure_ptr` (with the local `impure_data` object).
Also authored, not in the list: `_sceFsSemInit` and `_fs_semid`, which `_sceFsWaitS`/`_sceFsSigSema` need.

Checks added for other packages' units that had none: `w9-startup-crt-c-ctype-check` (host check of
`_ctype_` from `w8-startup-sce-a-ctype-table`, compared with host C-locale classification; it reads the
other unit's source by relative path) and `w9-startup-crt-c-k-exit-tlb-check` (assembles
`_kExitTLBHandler` from `w8-startup-sce-b-k-exit-tlb-handler` and compares its 61-instruction sequence
and relocation targets with the described sequence; takes the toolchain root and source root as
arguments). Those two units were left unselected by the integrator for lack of a check; this gives
them one. They define nothing themselves.

## Imports opened (new unresolved symbols the authored units reference)

`isceSifSendCmd` (from the two alarm units), `Exit`, `CreateSema`, `SignalSema`, `WaitSema`,
`__ps2_klibinfo__`, `_fsversion`, `memcmp` (already authored elsewhere), `isceSifSetDma`, `sceSifSetDma`,
`sceSifWriteBackDCache`, `norrath_sound_command_dma_destination_word_view`, `GLISTSIZE`,
`engineFullFrameMode`, `glistp`, `gstart`, `nmChars`, `nmDirectQWords`, `p`, `reglistLoopCount`,
`scanMask`, `subfontPrintu__FiiPUsi`, `supersampleMode`, `__rtti_si`, `__tf8Creature`, `__ti8Creature`,
`__tf7Vehicle`, `__ti7Vehicle`, `__tf10GameObject`, `__ti10GameObject`, `__eh_alloc`,
`__tf10bad_typeid`, `__cp_push_exception`, `__throw`, `_$_10bad_typeid`, `_vt$10bad_typeid`, plus the
slot functions named in each virtual table unit (GameObject, Vehicle, class-specific `msg_*`, `event*`,
destructors `_$_*`, `__tf*`, `__pure_virtual`, `doMyUniqueForces__*`, `debugReport__11AddParticle`,
`draw__11AddParticleG6Point3P6Point3i`, ...). Run the partial build to get the exact list.

## Build-plan entries needed (for the lead integrator)

The object names below are made unique (the bare file names `typeinfo.o` x3 and `font_string_sizeu.o`
collide with existing plan entries). `input_order` is left to the integrator. Two units include a
private header, listed in `header_closure`; add those headers to `files` too. The two `*-check`
directories and every `host_check.cpp` must stay out of the plan (host checks use the host C library).
The `files` map follows this list. Hashes are SHA-256 of the committed files.

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_alarm_bind.o", "profile": "authored_cpp", "sha256": "501c56a874c1663329c08e3c6a7657656ee9f4ae4bc7753fb1fc5d2b5fff3796", "source": "sources/w9-startup-crt-c-alarm-bind/alarm_bind.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_alarm_rdata.o", "profile": "authored_cpp", "sha256": "f013b9e56e8fa00be7eda9ed95c6d0a47730a12e12b1964e4da74ff5fae1a653", "source": "sources/w9-startup-crt-c-alarm-rdata/alarm_rdata.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_exit.o", "profile": "authored_cpp", "sha256": "90c677ce3aa460b36c8ff9554608fe01c592f35da00b35bdeff8f3ca725f01a0", "source": "sources/w9-startup-crt-c-exit/exit_stub.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_font_printu_inner.o", "profile": "authored_cpp", "sha256": "e76b6e41d97fa018442a30882e1e7c9b7e599a6472edc527a61073d0518f1b9b", "source": "sources/w9-startup-crt-c-font-printu-inner/font_printu_inner.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_font_string_size.o", "profile": "authored_cpp", "sha256": "2d5f9411c0e75f220a9ba81f674a7c12226f68f54a29555d338f3aed9500f3d8", "source": "sources/w9-startup-crt-c-font-string-size/font_string_size.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_font_string_sizeu.o", "profile": "authored_cpp", "sha256": "e92e09c0ceac1157013e54658f295e0e4f0ad62d392851797df7e60fb5a0d5cb", "source": "sources/w9-startup-crt-c-font-string-sizeu/font_string_sizeu.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_fs_sem_init.o", "profile": "authored_cpp", "sha256": "b42d4b1debd4c75f282acafca8ec649972189b288db8379fc8dd6c8eb601a054", "source": "sources/w9-startup-crt-c-fs-sem-init/fs_sem_init.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_fs_sig_sema.o", "profile": "authored_cpp", "sha256": "8ac79b2f2f4bd3c58f1433f91abc8f1fd737ed1777e3e9e0acc83c57b641e738", "source": "sources/w9-startup-crt-c-fs-sig-sema/fs_sig_sema.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_fs_version.o", "profile": "authored_cpp", "sha256": "1beba06ea3664b9053105913b5077da07c96c2151e82681915b8550e4d3a6408", "source": "sources/w9-startup-crt-c-fs-version/fs_version.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_fs_wait_s.o", "profile": "authored_cpp", "sha256": "a50b09848a1d45b599e90d4e99d9df0e8430c122c6ec020e37ce7ac2d8303f2e", "source": "sources/w9-startup-crt-c-fs-wait-s/fs_wait_s.cpp"},
{"header_closure": ["sources/w9-startup-crt-c-impure-data/impure_data.hpp"], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_impure_data.o", "profile": "authored_cpp", "sha256": "b7f08527676cdd1bcd905d8de762fb40b3e5d80ab1c1fa20aa8599dbc107fec8", "source": "sources/w9-startup-crt-c-impure-data/impure_data.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_rpc_get_fpacket.o", "profile": "authored_cpp", "sha256": "bb0c60a7d7d3df31a929b72ee3f8493c4921f57b37eb573fc98c5b01326ebdd7", "source": "sources/w9-startup-crt-c-rpc-get-fpacket/rpc_get_fpacket.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_rpc_get_fpacket2.o", "profile": "authored_cpp", "sha256": "9ac4a5138f35b4284160c21f5eb9dc8b18e4151246d7f3450f93f8a1fba5229b", "source": "sources/w9-startup-crt-c-rpc-get-fpacket2/rpc_get_fpacket2.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_search_svdata.o", "profile": "authored_cpp", "sha256": "276c0a4930e42cc02f226f48bed75102a8b67c49f639ef8ef092fcd3f32f5528", "source": "sources/w9-startup-crt-c-search-svdata/search_svdata.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_sif_send_cmd_internal.o", "profile": "authored_cpp", "sha256": "56b5abfcc559372fd3baed471dd009ac9e51f75b1c3cf3245d59e2bd084c9ccd", "source": "sources/w9-startup-crt-c-sif-send-cmd-internal/sif_send_cmd_internal.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_tf_lavavehicle.o", "profile": "authored_cpp", "sha256": "c5fece91de45abbafb2c350d9f140f0f7db62320f273998b0da4ca2c9bf52096", "source": "sources/w9-startup-crt-c-tf-lavavehicle/typeinfo.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_tf_mummyking.o", "profile": "authored_cpp", "sha256": "02b819ad674e3caeb6ea6ed0e9495d99f436681a8917b7bb6274d46a62e8c30d", "source": "sources/w9-startup-crt-c-tf-mummyking/typeinfo.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_tf_savepoint.o", "profile": "authored_cpp", "sha256": "c4d98f654ac3d5fbbf2adee983b065ed981aa188188f058ff5698307398ef6bc", "source": "sources/w9-startup-crt-c-tf-savepoint/typeinfo.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_throw_bad_typeid.o", "profile": "authored_cpp", "sha256": "fd47727dac54fb47a46be0c2c2eb7873fa03e6713cb86fe1d4c21db34ef6e2fd", "source": "sources/w9-startup-crt-c-throw-bad-typeid/throw_bad_typeid.cpp"},
{"header_closure": ["sources/w9-startup-crt-c-libgcc-div-common/udivmod.hpp"], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_udivdi3.o", "profile": "authored_cpp", "sha256": "9dba28ac38723aa400664cedf9cd3e02536a65a54b9881f31353db79243f3770", "source": "sources/w9-startup-crt-c-udivdi3/udivdi3.cpp"},
{"header_closure": ["sources/w9-startup-crt-c-libgcc-div-common/udivmod.hpp"], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_umoddi3.o", "profile": "authored_cpp", "sha256": "241414232c8e31c49ef9a269d7d8d67a8bb47952e336c1a73c9ae1586d82a9ab", "source": "sources/w9-startup-crt-c-umoddi3/umoddi3.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_vt_addparticle.o", "profile": "authored_cpp", "sha256": "b976586e2474c522aa8adf025ee0d5d824154ac0c1c120b3ab256b06e47870d6", "source": "sources/w9-startup-crt-c-vt-addparticle/vt_addparticle.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_vt_lavavehicle.o", "profile": "authored_cpp", "sha256": "febfd82e985cc8425dcec94fbfc95f2dac95f220503e4fd559392f54b2ec022a", "source": "sources/w9-startup-crt-c-vt-lavavehicle/vt_lavavehicle.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_vt_lighteffect.o", "profile": "authored_cpp", "sha256": "2e0969a6a2fd11397f34deb827c34b949f15b40d0d40a86e58e4070dfa45a5c0", "source": "sources/w9-startup-crt-c-vt-lighteffect/vt_lighteffect.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_vt_particlemorpheffect.o", "profile": "authored_cpp", "sha256": "d545659c3a4b7a574bd857b9dad07470a21b0cd0c60c9a77c5adb45182fe287d", "source": "sources/w9-startup-crt-c-vt-particlemorpheffect/vt_particlemorpheffect.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_vt_skulboat.o", "profile": "authored_cpp", "sha256": "ea6b751f56571a3d60cdef5cfe69c5b892936ac82ba8fa1c53aa2c38f5cf5879", "source": "sources/w9-startup-crt-c-vt-skulboat/vt_skulboat.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w9_startup_crt_c_vt_vehicle.o", "profile": "authored_cpp", "sha256": "a72a5024537ed7437c8774fb2368f76de4a71b600f8dfbce49c90ef38f7af779", "source": "sources/w9-startup-crt-c-vt-vehicle/vt_vehicle.cpp"}
```

Files map additions:

```json
{
 "sources/w9-startup-crt-c-alarm-bind/alarm_bind.cpp": "501c56a874c1663329c08e3c6a7657656ee9f4ae4bc7753fb1fc5d2b5fff3796",
 "sources/w9-startup-crt-c-alarm-rdata/alarm_rdata.cpp": "f013b9e56e8fa00be7eda9ed95c6d0a47730a12e12b1964e4da74ff5fae1a653",
 "sources/w9-startup-crt-c-exit/exit_stub.cpp": "90c677ce3aa460b36c8ff9554608fe01c592f35da00b35bdeff8f3ca725f01a0",
 "sources/w9-startup-crt-c-font-printu-inner/font_printu_inner.cpp": "e76b6e41d97fa018442a30882e1e7c9b7e599a6472edc527a61073d0518f1b9b",
 "sources/w9-startup-crt-c-font-string-size/font_string_size.cpp": "2d5f9411c0e75f220a9ba81f674a7c12226f68f54a29555d338f3aed9500f3d8",
 "sources/w9-startup-crt-c-font-string-sizeu/font_string_sizeu.cpp": "e92e09c0ceac1157013e54658f295e0e4f0ad62d392851797df7e60fb5a0d5cb",
 "sources/w9-startup-crt-c-fs-sem-init/fs_sem_init.cpp": "b42d4b1debd4c75f282acafca8ec649972189b288db8379fc8dd6c8eb601a054",
 "sources/w9-startup-crt-c-fs-sig-sema/fs_sig_sema.cpp": "8ac79b2f2f4bd3c58f1433f91abc8f1fd737ed1777e3e9e0acc83c57b641e738",
 "sources/w9-startup-crt-c-fs-version/fs_version.cpp": "1beba06ea3664b9053105913b5077da07c96c2151e82681915b8550e4d3a6408",
 "sources/w9-startup-crt-c-fs-wait-s/fs_wait_s.cpp": "a50b09848a1d45b599e90d4e99d9df0e8430c122c6ec020e37ce7ac2d8303f2e",
 "sources/w9-startup-crt-c-impure-data/impure_data.cpp": "b7f08527676cdd1bcd905d8de762fb40b3e5d80ab1c1fa20aa8599dbc107fec8",
 "sources/w9-startup-crt-c-impure-data/impure_data.hpp": "74603d1ba12cb6c300289719aacecee5d8cc224d09405e70d004cb71a0e1367b",
 "sources/w9-startup-crt-c-libgcc-div-common/udivmod.hpp": "7f7a654a633565ca8c74457d0435eb6d22b5b77bca7b89aa5077a107510d04a7",
 "sources/w9-startup-crt-c-rpc-get-fpacket/rpc_get_fpacket.cpp": "bb0c60a7d7d3df31a929b72ee3f8493c4921f57b37eb573fc98c5b01326ebdd7",
 "sources/w9-startup-crt-c-rpc-get-fpacket2/rpc_get_fpacket2.cpp": "9ac4a5138f35b4284160c21f5eb9dc8b18e4151246d7f3450f93f8a1fba5229b",
 "sources/w9-startup-crt-c-search-svdata/search_svdata.cpp": "276c0a4930e42cc02f226f48bed75102a8b67c49f639ef8ef092fcd3f32f5528",
 "sources/w9-startup-crt-c-sif-send-cmd-internal/sif_send_cmd_internal.cpp": "56b5abfcc559372fd3baed471dd009ac9e51f75b1c3cf3245d59e2bd084c9ccd",
 "sources/w9-startup-crt-c-tf-lavavehicle/typeinfo.cpp": "c5fece91de45abbafb2c350d9f140f0f7db62320f273998b0da4ca2c9bf52096",
 "sources/w9-startup-crt-c-tf-mummyking/typeinfo.cpp": "02b819ad674e3caeb6ea6ed0e9495d99f436681a8917b7bb6274d46a62e8c30d",
 "sources/w9-startup-crt-c-tf-savepoint/typeinfo.cpp": "c4d98f654ac3d5fbbf2adee983b065ed981aa188188f058ff5698307398ef6bc",
 "sources/w9-startup-crt-c-throw-bad-typeid/throw_bad_typeid.cpp": "fd47727dac54fb47a46be0c2c2eb7873fa03e6713cb86fe1d4c21db34ef6e2fd",
 "sources/w9-startup-crt-c-udivdi3/udivdi3.cpp": "9dba28ac38723aa400664cedf9cd3e02536a65a54b9881f31353db79243f3770",
 "sources/w9-startup-crt-c-umoddi3/umoddi3.cpp": "241414232c8e31c49ef9a269d7d8d67a8bb47952e336c1a73c9ae1586d82a9ab",
 "sources/w9-startup-crt-c-vt-addparticle/vt_addparticle.cpp": "b976586e2474c522aa8adf025ee0d5d824154ac0c1c120b3ab256b06e47870d6",
 "sources/w9-startup-crt-c-vt-lavavehicle/vt_lavavehicle.cpp": "febfd82e985cc8425dcec94fbfc95f2dac95f220503e4fd559392f54b2ec022a",
 "sources/w9-startup-crt-c-vt-lighteffect/vt_lighteffect.cpp": "2e0969a6a2fd11397f34deb827c34b949f15b40d0d40a86e58e4070dfa45a5c0",
 "sources/w9-startup-crt-c-vt-particlemorpheffect/vt_particlemorpheffect.cpp": "d545659c3a4b7a574bd857b9dad07470a21b0cd0c60c9a77c5adb45182fe287d",
 "sources/w9-startup-crt-c-vt-skulboat/vt_skulboat.cpp": "ea6b751f56571a3d60cdef5cfe69c5b892936ac82ba8fa1c53aa2c38f5cf5879",
 "sources/w9-startup-crt-c-vt-vehicle/vt_vehicle.cpp": "a72a5024537ed7437c8774fb2368f76de4a71b600f8dfbce49c90ef38f7af779"
}
```

## Observed

- `__tf9MummyKing` at `0x00316040` (80 bytes), `__tf9Savepoint` at `0x00312060` (76) and `__tf11LavaVehicle`
  at `0x003167c8` (80): each tests the first word of its 12-byte type-info object (`__ti9MummyKing` at
  `0x008eb618`, `__ti9Savepoint` at `0x008eafc8`, `__ti11LavaVehicle` at `0x008eb6c8`, all `.bss` in the
  image); when zero it calls the base's `__tf` function (`__tf8Creature`, `__tf10GameObject`,
  `__tf7Vehicle`) and then `__rtti_si(object, class-name text, base typeinfo)`; the object is returned
  either way. Class-name text and base type-info address were read from the call arguments.
- `__throw_bad_typeid` at `0x002ff3c8` (100 bytes): `__eh_alloc(4)`; store `_vt$10bad_typeid`
  (`0x00453810`) in the block's first word; `__cp_push_exception(block, __tf10bad_typeid(), _$_10bad_typeid)`
  (`0x0031aac8`); `__throw()`. A second region at `0x002ff414` stores `_vt$9exception` (`0x00453930`)
  on the stack and calls `__rethrow` with the data at `0x0047f910`.
- `__udivdi3` at `0x002f8f60` (1404 bytes) and `__umoddi3` at `0x002f94e0` (1384 bytes): 32-bit word
  algorithms with a range-test-plus-256-byte-table leading-zero count (local table copies at
  `0x0047db08`..`0x0047de08`, each equal to the bit length of the index), a 16-bit-digit two-word division
  step with a two-stage correction, and a full 32x32 product correction. A zero divisor ends in a divide
  instruction with a zero-check trap. `__umoddi3` stores its remainder to a stack slot behind a null test
  on the slot's address and reloads it. Compiled: `__udivdi3` 1464, `__umoddi3` 1368.
- `_impure_ptr` at `0x00448480` holds `0x00448190`, the 752-byte local `impure_data`; nonzero words only at
  +4/+8/+12 (addresses of three 88-byte zero records at +484/+572/+660), +52 (address of text `"C"`) and
  the 64-bit value 1 at +168. The compiled data object has those offsets (enforced by static assertions)
  and the same word values in an EE-compiled object (checked by reading the section bytes).
- `_ctype_` at `0x0047e1f0`: the digit entries carry only the digit bit (0x04), the hex bit 0x40 is on A-F
  and a-f only; the authored table (other package) equals the image bytes (private comparison) and the
  new host check encodes that rule.
- Earlier commits of this package (SIF/FS RPC, fonts, vtables) carry their own comment headers with
  addresses and sizes; not repeated here.

## Inferred

- The first word in the `impure_data` initialiser being a stream-record pointer array, the `"C"` text a
  locale name and the +168 value a seed is inferred from callers (see libc-format notes); the authored
  field names state offsets only.
- `_$_10bad_typeid` being a destructor and `_vt$10bad_typeid` a vtable pointer: from symbol names.
- `__udivdi3`/`__umoddi3` follow the widely published generic word-by-word 64-bit divide; the equivalence
  with the original paths was read from the branch structure, not proven instruction-for-instruction.

## Authored but never executed

Everything. Host checks ran the authored C++ on the host with stubbed callees (call-order and argument
checks for RPC, font, typeinfo and `__throw_bad_typeid` units; 2,000,000-case randomized plus edge sweeps
against native 64-bit division and remainder for `__udivdi3`/`__umoddi3`; slot-order checks for the six
vtables; layout/pointer/value checks for `_impure_ptr`). No EE execution of any of them. The R5900
paired-run harness was not used in this session.

## Unknown / not closed

- `__throw_bad_typeid`: only the straight-line path is authored. The exception-landing region at
  `0x002ff414` (which sets a stack slot to `_vt$9exception` and calls `__rethrow` with `0x0047f910`) is
  not reproduced; the profile is `-fno-exceptions` and no exception-table evidence was used. This is not
  a complete function by the repository's definition; treat it as partial.
- `__udivdi3`/`__umoddi3` zero-divisor behaviour: the compiled code traps on a zero check; the original
  uses a `break 7`; which instruction form the original source produced is not known.
- `_args` (`0x005192c0`): left unresolved by design (`docs/STARTUP_ENTRY.md` says `_root` and `_args` are
  deliberately unresolved). Image evidence gives only a gap of 384 bytes to the next object (`dataBlock`
  at `0x00519440`); its real size and layout (argc word at +0, argument data from +4) are not established.
- `_end`, `_bssend`, `_fbss`, `_gp`, `_srsctorsstart`, `_srsctorsend`: linker-layout symbols
  (`_gp=0x004879f0`, `_fbss=0x00486800`, `_bssend=0x008eab1c`, `_srsctorsstart=0x008eab1c`,
  `_srsctorsend=0x008eab58`, `_end=0x008ec91c`) owned by the link layout/launchSRS work, not by source
  files; not authored.
- `amx_Exec`: already authored by the script-amx package (`script-amx-exec`); not touched. It stays
  unresolved only if that unit is not in the plan.
- `__ti8AutoProp`, `__ti10GameObject`: defined by the units that author `__tf8AutoProp` (prop-poke-clone) and
  `__tf10GameObject` (gameobject-base). Not duplicated here; `host_check.cpp` of `tf-savepoint` defines a
  stand-in only for the host test.
- `_ctype_` and `_kExitTLBHandler`: the sources are other packages'; only checks were added here.
- Whether `__ti11LavaVehicle` etc. need to be separate from the `__tf` functions in the original link
  order (they are `.bss` objects in the image; here they are defined in the same unit).
