# w12-particle-data-a: particle constructors and local data

Package branch: `agents/w12-particle-data-a`. Private reference: `work/private/SLUS_205.65`, SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Findings below use this pinned image; none of its bytes, compiler listings, or extracted assets are committed.

The current shared plan builds 5,113 authored inputs and leaves 827 unresolved imports. All 29 names in the task's starting list remain unresolved in that baseline. This package closes four of them in three independent source units. The function and data objects were compiled with the reviewed EE toolchain. Their host checks passed. The objects are not integrated into the shared plan, and none ran in the game.

## Observed, inferred, unknown

### Observed

- `__12ParticlePropG6Point3i` is a 172-byte function at `0x00154960`. It copies a 12-byte point argument and calls `__10GameObjectG6Point3PQ210GameObject11resourceMapPPc` with two zero arguments. It stores `0x0045d018` at object offset 72 before calling `init__4Propi(this, argument)`, then stores `0x0044cf88` at offset 72, the float `12.0` at offset 28, and ORs `0x4002` into the word at offset 12. The host contract checks call order, argument words, the vtable visible to `init`, final fields, and all other object bytes.
- `batParams` is an 8-byte local data symbol at `0x004858e0`. Its first word points to the NUL-terminated text `oneshot=1`; its second word is null. The authored target object emits an 8-byte pointer table. Its host check checks the string and null terminator entry.
- `healColor` and `healPulse` are each 8-byte local data symbols at `0x00484528` and `0x00484530`. The image bytes are zero. The caller reads four halfwords from each array. Their host check checks the extent and every initial element.
- The fresh baseline unresolved list contains all four named symbols above, plus the 26 symbols listed under “Imports still open.”

### Inferred

- The two constructor vtable words belong to the intermediate `Prop` setup and final `ParticleProp` setup, based on their addresses and the class symbols. The source records the observed stores without depending on those names.
- `batParams` and the heal arrays are writable state: the original symbols are local data, and the call sites consume their pointer/halfword layouts. The authored objects preserve initial values and layouts; their linked addresses are not the original addresses.

### Unknown

- The semantic names for the constructor's flag bits and float field, and the runtime effects of either base initializer.
- Whether the heal arrays are read by any call site other than the reconstructed draw path.
- Game execution, EE runtime behavior, and placement of these objects in a final executable.

## Imports closed (names)

`__12ParticlePropG6Point3i`, `batParams`, `healColor`, `healPulse`.

The constructor source has object-level references to `__10GameObjectG6Point3PQ210GameObject11resourceMapPPc` and `init__4Propi`. Those names are already defined by the fresh baseline partial object, so the combined relocatable link introduces no new unresolved import for this function. The isolated three-unit build compiled one function and both data providers with the reviewed toolchain; its two unresolved names are those existing constructor callees.

## Imports still open

These 25 assigned names remain unresolved. They need an export from the translation unit or data owner identified below; a separate zero-filled copy would not share the storage initialized by the existing static initializer.

- Particle objects filled by existing static initializers: `disintoInnoruukParticle` (`w8-fx-particles-init-innoruuk`), `harm1`, `harm2` (`w8-spell-static-init-a-spell-harm-touch`), `hitParticleDust2`, `iceSwordParticle`, `newFlameSwordParticle1`, `newFlameSwordParticle2`, `newFlameSwordParticle3`, `newIceSwordParticle1`, `newIceSwordParticle2` (`w8-fx-particles-init-fx`), and `lampParticle` (`w8-spell-static-init-a-torch`). Export the initialized object from its owning unit.
- Particle objects with existing source providers whose plan entries or owning spelling still need integration: `lavaGoopParticle`, `lavaSteamParticle` (`w10-render-b-data-lava-particles`); `norrath_container_msg_run_fuseParticle_bytes`, `norrath_container_msg_run_smokeParticle_bytes` (`w9-render-math-c-switch-static-init`).
- Auto-chest particle objects `norrath_auto_chest_msg_run_fuseParticle_bytes` and `norrath_auto_chest_msg_run_smokeParticle_bytes` are the 80-byte objects initialized as local `fuseParticle` and `smokeParticle` in `w8-fx-particles-init-has-light-or-particle-number`; that initializer must export those actual objects under the names the callers import.
- `goblinWeaponResourceMap` is a 128-byte map also present as the address-named view `norrath_ctors_ant_lantern_view_42cfc8` in `w9-ctors-lantern-a-views`. It needs a shared owner spelling or a same-object alias.
- `norrath_ctors_ant_lantern_view_34b474` is an interior word at offset 52 in `skills`; `norrath_ctors_ant_lantern_view_34b6b1` is the string at offset 81 in `levelName`. They need exports or aliases from the `skills` and `levelName` owners, not standalone duplicate objects.
- `mStarParticleDist` is a 384-byte table read by `w11-vi-render-a-atmosphere-init`. The production table contents are not published in that unit; its host-only pattern is not an EE data source.
- `norrath_rtti_entities_a_view_480cb4`, `norrath_rtti_entities_a_view_480cb8`, `norrath_rtti_entities_a_view_480cbc`, and `norrath_rtti_entities_a_view_480cc0` are four observed single-precision literal views already authored in `w9-rtti-vtables-entities-a-view-480cb4`. That view source needs a build-plan entry or an equivalent owner selection.

## Imports opened

None in the combined partial-link check. The isolated constructor object references only the two existing constructor callees listed above.

## Build-plan entries needed

The host checks are not EE inputs. Append these three source inputs and file hashes; the lead integrator assigns `input_order` and updates the private-plan snapshot consistently. The isolated private three-input plan was built with the reviewed toolchain and emitted one function plus the two data objects. `compiled_body_bytes` for the constructor is 144 in the authored build; no original-byte match is claimed.

```json
{
  "emitted_functions": [{"compiled_body_bytes": 144, "name": "__12ParticlePropG6Point3i"}],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12_particle_prop_ctor.o",
  "profile": "authored_cpp",
  "sha256": "a8cb63b96ed6b53a678d7e9cbe0b175cbc23c1fb297d62c58b5d135c99052c3c",
  "source": "sources/w12-particle-data-a-ctor-particle-prop/particle_prop.cpp"
}
{
  "emitted_functions": [],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12_bat_params.o",
  "profile": "authored_cpp",
  "sha256": "dad9c657776c3c91848e00a7b7e7fff1d29e360dd69ba9a9500538ff96bc5763",
  "source": "sources/w12-particle-data-a-bat-params/bat_params.cpp"
}
{
  "emitted_functions": [],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12_heal_state.o",
  "profile": "authored_cpp",
  "sha256": "e9e5d1d7831df4c76ddebfb66c2e74dc4f60be9b38ac4346112c9650f7f09c12",
  "source": "sources/w12-particle-data-a-heal-pulse/heal_state.cpp"
}
```

The other names above need entries or aliasing in their owning packages. Do not select a second zero-filled particle object beside its hidden initialized counterpart. In particular, selecting the existing `w9-rtti-vtables-entities-a-view-480cb4` source defines all four RTTI literal imports.

## Authored but never executed

`__12ParticlePropG6Point3i` has a host contract for its calls and object writes and was freshly compiled by the reviewed EE compiler. `batParams`, `healColor`, and `healPulse` have host checks for their authored contents and fresh EE objects. None ran on an EE or inside the game. No runtime constructor execution or gameplay is claimed.

## Unknown

The remaining particle-object field meanings, final executable addresses, actual runtime initialization order, semantics of the constructor's stored values, and runtime behavior of all four authored definitions remain unknown. The list of unresolved owners above is intentionally not replaced by guessed duplicate definitions.
