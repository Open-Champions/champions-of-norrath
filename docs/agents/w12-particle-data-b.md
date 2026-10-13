# W12 particle data B

Package scope: particle-definition and file-local data imports owned by actor
message handlers, static initializers, and the RTTI/data-view callers. The
fresh reviewed-toolchain build before these additions compiled 5,113 source
inputs and reported 827 unresolved imports. It included all 33 names listed
below in its unresolved report. The expanded build, using a private copy of the
plan with the two new source entries, compiled 5,115 inputs and linked with 795
unresolved imports. It introduced no COMMON symbols and is not a game executable
or runtime verification. The worktree is isolated on
`agents/w12-particle-data-b`.

## Evidence and implementation

Observed: the RTTI views are address-backed four-byte words, except
`norrath_rtti_entities_a_view_4818ac`, which is nineteen consecutive words.
The exact bit patterns are recorded in `views.cpp` and its source notes from
the existing W9 data-view package. Five additional unresolved views in that
same family (`480bb8`, `480cb4`, `480cb8`, `480cbc`, `480cc0`) were in the
fresh report and have the same address/value evidence, so they are included.

Observed: the actor particle records and the named particle definitions have
80-byte extents. `norrath_w11_actor_c_view_4852ac` is a zero-initialized
four-byte flag, and `norrath_w11_actor_c_view_4852b0` points to the text
`Skeleton`. The atmosphere tables have extents of 2,880 bytes each for
`sDustCard` and `sFallEffect`, and 384 bytes for `sFogCard`; the existing
`GlobalInit__14VIEffectVolumeP7VIScene` source fills those tables.

Authored storage definitions are in `experimental/sources/w12-particle-data-b-objects/objects.cpp`.
The literal views are in `experimental/sources/w12-particle-data-b-views/views.cpp`.
No target function is part of this package. The data object host check passed
with AddressSanitizer and UndefinedBehaviorSanitizer; it checks the exact
literal words, all nineteen table entries, the Skeleton pointer text, and
the final byte of every 80-byte record and atmosphere table. Both target
objects compiled with the reviewed EE toolchain, and their ABI/object symbol
inventory was inspected. The repository tool suite passed: 214 tests.

Inferred: the particle records are constructor-backed `ParticleDef` objects,
based on the existing static-initializer call records. This package defines
the unresolved storage symbols; it does not prove that a separate reconstructed
initializer writes these exported objects. Existing initializer sources also
contain file-local storage, so matching storage identity and runtime
initialization still require integration review.

Unknown: `sRainParticleDist` is a 600-byte table, but its contents are expressly
not reproduced in the existing W11 atmosphere evidence. It remains unresolved
rather than being filled with guessed bytes. No execution or game runtime check
was performed.

## Handoff

### Imports closed

The private expanded build confirms these 32 names are defined and no longer
unresolved:
`norrath_rtti_entities_a_view_480bb8`, `norrath_rtti_entities_a_view_480cb4`,
`norrath_rtti_entities_a_view_480cb8`, `norrath_rtti_entities_a_view_480cbc`,
`norrath_rtti_entities_a_view_480cc0`, `norrath_rtti_entities_a_view_4810b4`,
`norrath_rtti_entities_a_view_4810b8`, `norrath_rtti_entities_a_view_481710`,
`norrath_rtti_entities_a_view_4818ac`, `norrath_rtti_entities_a_view_482424`,
`norrath_rtti_entities_a_view_482428`, `norrath_rtti_entities_a_view_48242c`,
`norrath_w11_actor_c_view_4852ac`, `norrath_w11_actor_c_view_4852b0`,
`norrath_w11_actor_c_view_7d7110`, `norrath_w11_actor_c_view_7d7540`,
`norrath_w11_actor_c_view_7d7590`, `norrath_w11_actor_c_view_7d7630`,
`norrath_w11_actor_c_view_7d7680`, `particleBloodDrop`, `particleEctoDrop`,
`particleSlimeDrop`, `particleWeaponBlood`, `particleWeaponEcto`,
`particleWeaponSlime`, `sDustCard`, `sFallEffect`, `sFogCard`, `snowParticle1`,
`sparkParticle`, `trailParticleFire`, and `trailParticleSmoke`.

### Imports opened

None. `sRainParticleDist` remains unresolved in the final 795-import report;
this package does not define it.

### Build-plan entries needed

The shared build plan and snapshot were not edited. The lead integrator needs
these two build-plan entries (profile `authored_cpp`, kind
`independently_authored_source`, and empty `header_closure`). Their corresponding
snapshot input records use the same order, source, and object name, with
`emitted_functions: []`.

```json
{
  "input_order": 5115,
  "kind": "independently_authored_source",
  "object_name": "w12_particle_data_b_views.o",
  "profile": "authored_cpp",
  "sha256": "c4ba2b4d9cf4acc247927348ab9fef49c6d3594e2c821644204f7271e6281323",
  "source": "sources/w12-particle-data-b-views/views.cpp",
  "header_closure": []
}
```

```json
{
  "input_order": 5116,
  "kind": "independently_authored_source",
  "object_name": "w12_particle_data_b_objects.o",
  "profile": "authored_cpp",
  "sha256": "89ab93a5ffd64704769f8d9d46d2e26a99464c8f9890b57fc9799d270fb4fbae",
  "source": "sources/w12-particle-data-b-objects/objects.cpp",
  "header_closure": []
}
```

### Authored but never executed

The 12 RTTI literal views, actor particle and flag/pointer storage, ten
80-byte particle records, and three atmosphere tables are authored data only.
No initializer or gameplay path was executed. The two data translation units
contain no target functions.

### Unknown

The contents of `sRainParticleDist` remain unknown. The post-construction
contents and runtime identity of same-named file-local particle records remain
unverified; the authored data closes external symbols, but does not establish
that the reconstructed constructor calls target these exported records. The
full partial integration remains incomplete and is not an executable or game
runtime verification. `python3 -m unittest discover -s tools -p 'test_*.py'`
passed all 214 tests.
