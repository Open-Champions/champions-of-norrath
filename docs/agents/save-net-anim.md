# save, network, and animation state

Package notes for `agents/save-net-anim`. These objects were compiled with the reviewed `authored_cpp` flags and checked by `host-tests/save-net-anim/`. They are not in `experimental/build-plan.json` or `experimental/snapshot.json`. `tools/check_source_snapshot.py` rejects every unrecorded file under `experimental/sources/` until the integrator adds the file hashes and keeps `input_order` contiguous. The tools unit tests do not run that checker.

`norrath_archive_map_animation_sounds` walks one animation header. A null sound-list word returns before the length call. Sound records are 8 bytes and stop on a null name. The mapped id is stored after the list pointer is reloaded. Event records are 28 bytes and stop on `0x7fffffff`. `*bodyfall*` stores half-index -3 and `*blank*` stores -4. Mode 0 clears event bytes 10 through 14. Mode below 2 rewrites indexes that are below -2 or past the sound count, to 0 when a sound exists and to -1 when it does not, so -3 and -4 remain only when the mode is at least 2. The length result times 60 is truncated toward zero and stored at header offset 64. The host test uses an address arena because this machine cannot map the EE pointer range. The EE object was compiled and its defined and undefined symbols were checked; the routine was not executed by the EE.

## Layout notes

The network mode byte, the HUD no-controller byte, and the memory-card startup-script flag are one-byte zeros. The mode byte is the same address as the still-unresolved `norrath_creature_view_483829`. This package defines only `norrath_network_mode_byte_view`. A second definition of the creature name would be a different byte.

String objects include the terminating NUL and stop there. `%s.lmp` is 7 bytes. `start` is 6. `script.scr` is 11. `preStart` is 9. `objects.ob` is 11. `anm` is 4. The padding before the next literal is not part of the symbol.

The atan2 scale word is the four bytes `83 f9 22 46`. The transition threshold is `88 88 88 3c`. The inventory transition rate is `99 99 99 3e`. These are pool slots, not ELF object symbols of their own. The animation math coefficient views are the math package.

The lump-name table is ten 16-byte slots, 160 bytes. The resource-name table is seven 16-byte slots, 112 bytes.

The never-save table is 19 words, 76 bytes: eighteen names and a null word. Seventeen entries are the existing `norrath_factory_text_*` objects at the original addresses. The two clearColor slots are the same symbol, `norrath_factory_text_462058`. The faytree slot is a string in this object because that address has no factory-text import. The filter compares names with `norrath_factory_strcasecmp`, so the table does not need pointer identity with a second copy of the text. Whether a later store replaces a slot is unknown.

Message callbacks are 64 zero bytes, sixteen pointer slots, ending where `pktHandler` begins. A handler index is a byte and is not bounds-checked. An index past 15 is outside this object.

The partner views are separate arrays, not one overlapping object. Each array is the contiguous span that makes a 17632-byte stride land on the next player's slot. Bytes between slots are inside that span, and another view may cover some of the same addresses as its own object. Four records come from the partners symbol running exactly four strides before the next symbol. Local flags and the connected-status byte are each 52897 bytes. Cursors are 52900 bytes. Rings are 61088 bytes because a 13-bit mask reaches 8192 bytes from each player's ring base. Indexes past the fourth player are outside these objects.

Inventory animation state is 1056 bytes, four records of 264, from that `animState` base through the byte before the next symbol. Saved map tiles are 93312 bytes, all initially zero. The original object is file-backed zero storage. Under `-G0` this zero definition is not promised to stay in that original section.

The saved-level index is 648 bytes, 27 records of 24. That is the gap from the index base to the payload base. The loader reads the first byte of record 26. No ELF symbol records a size. The payload is 618800 bytes, from its base through the byte before `slotInfoHeaders`, which is also the save routine's limit of `ramSlot + 0xb29c8`. The load buffer is 60032 bytes, the gap from `loadBuffer` to `tempBuffer`. All three are initially zero. Interior address calculations that looked like extra objects were not separate symbols.

The cosmetic animation table is 144 bytes, eighteen slots. Each slot is a zero word followed by a pointer. The last pointer is null. The other seventeen targets are unresolved object methods. Factory code that exists today only stores the address of the table.

`norrath_construct_Savepoint` is still unresolved. It is the factory constructor used by object creation, so it stays with object-infra. `norrath_network_partner_count_word_view` is already defined.

## Imports closed

`norrath_archive_map_animation_sounds`, `norrath_network_mode_byte_view`, `norrath_game_hud_no_controller_byte_view`, `norrath_mc_disable_startup_script_bytes`, `norrath_mc_lump_format_bytes`, `norrath_mc_start_key_bytes`, `norrath_mc_script_section_key_bytes`, `norrath_mc_pre_start_key_bytes`, `norrath_mc_objects_section_key_bytes`, `norrath_archive_animation_extension_view`, `norrath_animation_dependency_view_47ff1c`, `norrath_animation_transition_threshold_view`, `norrath_inventory_animation_transition_rate_view`, `norrath_inventory_animation_lump_names_view`, `norrath_inventory_animation_resource_names_view`, `norrath_network_message_callbacks_view`, `norrath_network_player_local_flags_view`, `norrath_animation_dependency_view_4e6755`, `norrath_network_send_cursor_words_view`, `norrath_network_send_ring_bytes_view`, `norrath_inventory_animation_state_base_view`, `norrath_saved_map_tiles_bytes`, `norrath_mc_saved_level_index_bytes`, `norrath_mc_saved_level_payload_bytes`, `norrath_mc_load_buffer_bytes`, `norrath_game_never_save_table_view`, `norrath_factory_animation_vtable`.

## Imports opened

The sound mapper references `norrath_string_compare`, which `sources/lump-find-build/string_compare.cpp` already defines, plus `soundMapSound__FPci` and `soundComputeSequenceLength__FP15AnimationHeader`, which stay unresolved for audio.

The never-save table references these existing factory imports: `norrath_factory_text_483b38`, `norrath_factory_text_483b40`, `norrath_factory_text_483b48`, `norrath_factory_text_461fc0`, `norrath_factory_text_483b50`, `norrath_factory_text_461fd0`, `norrath_factory_text_461fe0`, `norrath_factory_text_483b60`, `norrath_factory_text_483b68`, `norrath_factory_text_461ff0`, `norrath_factory_text_462000`, `norrath_factory_text_462010`, `norrath_factory_text_462028`, `norrath_factory_text_462040`, `norrath_factory_text_462058`, `norrath_factory_text_462068`.

The animation table adds these unresolved method symbols: `__tf16CosmeticPropAnim`, `_$_16CosmeticPropAnim`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__16CosmeticPropAnim`, `msg_draw__16CosmeticPropAnim`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__16CosmeticPropAnimPc`, `msg_save__16CosmeticPropAnimPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__16CosmeticPropAnim`, `getModel__10GameObject`.

## Build-plan entries needed

Append these to `experimental/build-plan.json`. Orders 312 through 338 follow this worktree's current maximum of 311. Renumber them if another package takes those orders first. `snapshot.json` needs the same sources, the header, and a contiguous input list or `check_source_snapshot.py` fails.

```json
[
  {
    "header_closure": [
      "sources/save-net-anim-map-sounds/map_sounds.hpp"
    ],
    "input_order": 312,
    "kind": "independently_authored_source",
    "object_name": "map_sounds.o",
    "profile": "authored_cpp",
    "sha256": "d766e2f9bf1401ecc6173b5f3d7e66caac0cf88ee5d7368c2698e7dffed3b907",
    "source": "sources/save-net-anim-map-sounds/map_sounds.cpp"
  },
  {
    "header_closure": [],
    "input_order": 313,
    "kind": "independently_authored_source",
    "object_name": "norrath_animation_dependency_view_47ff1c.o",
    "profile": "authored_cpp",
    "sha256": "fb51ec35f1aee30590e49dfaded7ce009603e9b2ea6062855f3aede3ee1e7ba4",
    "source": "sources/save-net-anim-views/norrath_animation_dependency_view_47ff1c.cpp"
  },
  {
    "header_closure": [],
    "input_order": 314,
    "kind": "independently_authored_source",
    "object_name": "norrath_animation_dependency_view_4e6755.o",
    "profile": "authored_cpp",
    "sha256": "e4766f33ab0a1e538ce2bf8cd1789b1dea37048162258163df6ca7c70a63e068",
    "source": "sources/save-net-anim-views/norrath_animation_dependency_view_4e6755.cpp"
  },
  {
    "header_closure": [],
    "input_order": 315,
    "kind": "independently_authored_source",
    "object_name": "norrath_animation_transition_threshold_view.o",
    "profile": "authored_cpp",
    "sha256": "ae3c530a7510083304db10104dfa65d1d1f3da65f550ec9a6a3cc17762c97974",
    "source": "sources/save-net-anim-views/norrath_animation_transition_threshold_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 316,
    "kind": "independently_authored_source",
    "object_name": "norrath_archive_animation_extension_view.o",
    "profile": "authored_cpp",
    "sha256": "9846ebcf223ec7fe81671d64ee245cdfadaba5f3d1bfd77e69b8a13049a2006e",
    "source": "sources/save-net-anim-views/norrath_archive_animation_extension_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 317,
    "kind": "independently_authored_source",
    "object_name": "norrath_factory_animation_vtable.o",
    "profile": "authored_cpp",
    "sha256": "09471cd3f13abe4903de0b294e219ccb7c43fe89cf15e16eba4b60f11299814b",
    "source": "sources/save-net-anim-views/norrath_factory_animation_vtable.cpp"
  },
  {
    "header_closure": [],
    "input_order": 318,
    "kind": "independently_authored_source",
    "object_name": "norrath_game_hud_no_controller_byte_view.o",
    "profile": "authored_cpp",
    "sha256": "74f55f74946f74e8289fb9d78630e9f692b9fadc640cf39a89fd51bc609942d9",
    "source": "sources/save-net-anim-views/norrath_game_hud_no_controller_byte_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 319,
    "kind": "independently_authored_source",
    "object_name": "norrath_game_never_save_table_view.o",
    "profile": "authored_cpp",
    "sha256": "4fc926f1081fa12ebbe94ec78d18804973fa0cac301bcc25f630a29331646cca",
    "source": "sources/save-net-anim-views/norrath_game_never_save_table_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 320,
    "kind": "independently_authored_source",
    "object_name": "norrath_inventory_animation_lump_names_view.o",
    "profile": "authored_cpp",
    "sha256": "7b4d5a9c26f8d37f77e64a9bfa34be2a274dd6f7219f555b0a8d89a2d054260d",
    "source": "sources/save-net-anim-views/norrath_inventory_animation_lump_names_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 321,
    "kind": "independently_authored_source",
    "object_name": "norrath_inventory_animation_resource_names_view.o",
    "profile": "authored_cpp",
    "sha256": "5c584f22809f4e06621023b6921ded82afa830f4df93a74f9939331fcd0b5b07",
    "source": "sources/save-net-anim-views/norrath_inventory_animation_resource_names_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 322,
    "kind": "independently_authored_source",
    "object_name": "norrath_inventory_animation_state_base_view.o",
    "profile": "authored_cpp",
    "sha256": "c7eea382cb7ada206bd7c42e52fc70d1226cfd30ba9a01aa4937eb6ec5174a69",
    "source": "sources/save-net-anim-views/norrath_inventory_animation_state_base_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 323,
    "kind": "independently_authored_source",
    "object_name": "norrath_inventory_animation_transition_rate_view.o",
    "profile": "authored_cpp",
    "sha256": "7c30f5131916e78969e9e11c3236a3d52e9b0ee42fe5738d0551fd7c70a908e0",
    "source": "sources/save-net-anim-views/norrath_inventory_animation_transition_rate_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 324,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_disable_startup_script_bytes.o",
    "profile": "authored_cpp",
    "sha256": "084bb3d5da86807db14f8790ce6c07817abc57038ba377e7f28e356ce71f3ab1",
    "source": "sources/save-net-anim-views/norrath_mc_disable_startup_script_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 325,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_load_buffer_bytes.o",
    "profile": "authored_cpp",
    "sha256": "e8e9249d39c4daeabb65d952ea03b89fb97c9d13f956c46876a8e4a4ec22156c",
    "source": "sources/save-net-anim-views/norrath_mc_load_buffer_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 326,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_lump_format_bytes.o",
    "profile": "authored_cpp",
    "sha256": "3184b41051fcf9fe7a152f80083bc45b6e2d8e42ca4ad75e89cd80731c4d385e",
    "source": "sources/save-net-anim-views/norrath_mc_lump_format_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 327,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_objects_section_key_bytes.o",
    "profile": "authored_cpp",
    "sha256": "86ba14a734ab660e9d8d15f0c0be755793dfba37f41a04940c9c598a78131c6d",
    "source": "sources/save-net-anim-views/norrath_mc_objects_section_key_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 328,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_pre_start_key_bytes.o",
    "profile": "authored_cpp",
    "sha256": "3de5197c45b266ad2cda194224297dea5ee57a52f45b9f9bbbd69825dc7af632",
    "source": "sources/save-net-anim-views/norrath_mc_pre_start_key_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 329,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_saved_level_index_bytes.o",
    "profile": "authored_cpp",
    "sha256": "887132f1508265499d1f5068eb27df5b1445c42cfd1ca8e9cdf2a67c611892e9",
    "source": "sources/save-net-anim-views/norrath_mc_saved_level_index_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 330,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_saved_level_payload_bytes.o",
    "profile": "authored_cpp",
    "sha256": "5afecb9c48d71a26db59177b7e32e7be22e445d3206857b71d65bc111e978816",
    "source": "sources/save-net-anim-views/norrath_mc_saved_level_payload_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 331,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_script_section_key_bytes.o",
    "profile": "authored_cpp",
    "sha256": "b977c790dfff058f3d0dd8e6fe080a69b294a78e6b849cbe022fa5a7f85ac33f",
    "source": "sources/save-net-anim-views/norrath_mc_script_section_key_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 332,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_start_key_bytes.o",
    "profile": "authored_cpp",
    "sha256": "e90b6b0422e61655552c71addab8f15bdf6690ccc65e2015c757b2c5b663bad1",
    "source": "sources/save-net-anim-views/norrath_mc_start_key_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 333,
    "kind": "independently_authored_source",
    "object_name": "norrath_network_message_callbacks_view.o",
    "profile": "authored_cpp",
    "sha256": "ca276e8f9dcdade5231be42d44b7fbefe96db338f3867fe8f9c9f48794368529",
    "source": "sources/save-net-anim-views/norrath_network_message_callbacks_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 334,
    "kind": "independently_authored_source",
    "object_name": "norrath_network_mode_byte_view.o",
    "profile": "authored_cpp",
    "sha256": "138e2f020a200eba859ea2c10695523b01100186560f190021de6729ba94d7e6",
    "source": "sources/save-net-anim-views/norrath_network_mode_byte_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 335,
    "kind": "independently_authored_source",
    "object_name": "norrath_network_player_local_flags_view.o",
    "profile": "authored_cpp",
    "sha256": "b9918825c8bd0fd70302d9f182f25f4e979f59b07d4bee6e737ea913f55e7862",
    "source": "sources/save-net-anim-views/norrath_network_player_local_flags_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 336,
    "kind": "independently_authored_source",
    "object_name": "norrath_network_send_cursor_words_view.o",
    "profile": "authored_cpp",
    "sha256": "fcfd75408266ea715505f5894172bac0c9e34644e9fc96484e6dc19762fa0b4a",
    "source": "sources/save-net-anim-views/norrath_network_send_cursor_words_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 337,
    "kind": "independently_authored_source",
    "object_name": "norrath_network_send_ring_bytes_view.o",
    "profile": "authored_cpp",
    "sha256": "d28fa422afe7d50327f4790cfc743de815701b2e8aa78a273b4cc99dca157195",
    "source": "sources/save-net-anim-views/norrath_network_send_ring_bytes_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 338,
    "kind": "independently_authored_source",
    "object_name": "norrath_saved_map_tiles_bytes.o",
    "profile": "authored_cpp",
    "sha256": "0109d172252921f11bf417b5e87e41ec7a1a1fb42775b670de87981610161f0e",
    "source": "sources/save-net-anim-views/norrath_saved_map_tiles_bytes.cpp"
  }
]
```

## Authored but never executed

`norrath_archive_map_animation_sounds` runs in the host test. The EE compile checks the symbol only. It is not linked into the partial image and was not executed by the EE. The views and the vtable are data. Nothing in this branch calls the vtable slots.

## Unknown

Which startup store first writes the zero views. Whether a later store replaces a never-save pointer. The original section of each `-G0` zero object. The bytes between partner slots, which sit inside these spans and also inside neighboring views that this package does not alias. Handler indexes past 15, and player indexes past 3. The animation math views, `norrath_construct_Savepoint`, and `norrath_creature_view_483829`.
