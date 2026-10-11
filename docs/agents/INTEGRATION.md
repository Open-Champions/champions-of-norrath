# Integration log

Official unresolved-import counts come from the default authored partial link
(`python3 tools/build_partial.py` with the reviewed EE toolchain, no Newlib
root). COMMON stayed zero. Nothing here was executed on the EE or as gameplay.

## Baseline

Before these packages, revision `6ec25da` linked **1029** unresolved imports,
434 function bodies, and 310 authored inputs.

## agents/math

Merged. The notes separate observed bytes from inferred reconstruction names,
the sources are authored C++ rather than a disassembly dump, and each function
has a host test that returns failure. No game binary was committed.

The worker's private trial was not the official count. This link reproduces
the claimed delta exactly.

| | |
|---|---|
| Before | 1029 |
| After | 1009 |
| Closed | 20 |
| Opened | 0 |

Closed: `norrath_factory_double_isinf`, `norrath_factory_double_isnan`,
`norrath_entity_floorf`, `norrath_entity_fptodp`, `norrath_icos_scale`,
`norrath_icos_cubic`, `norrath_icos_quintic`, `norrath_icos_septic`,
`norrath_isin_scale`, `norrath_isin_cubic`, `norrath_isin_quintic`,
`norrath_isin_septic`, `norrath_animation_math_atan_high_view`,
`norrath_animation_math_atan_low_view`,
`norrath_animation_math_atan_coefficients_view`,
`norrath_animation_math_view_47d3b0`, `norrath_animation_math_view_47d3b8`,
`norrath_animation_math_view_47d3c0`, `norrath_animation_math_view_47d434`,
`norrath_player_softdouble_nan_parts_view`.

Opened: none.

Compiled bodies matched the notes: infinity 60, NaN 44, entity floor 232,
float-to-double 104. The four table units emit no functions. `floorf` and
`ceilf` stay optional external inputs. Host checks are recorded in the
snapshot and are not plan inputs. 214 tool tests passed, and the host checks
for all eight units passed on this machine. No EE or game execution.
