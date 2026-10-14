# Math helpers and tables

Package work is on `agents/math`. It supplies the integer-angle polynomial coefficients, the float arctangent tables, the power constants read by the animation math, the megatile entity floor and float-to-double helpers, the formatter double classifiers, and the soft-double NaN parts record.

`floorf` and `ceilf` stay unresolved. They are the optional Newlib inputs. `norrath_entity_floorf` is a separate symbol: `megatileEntitize__FPci` calls `floorf` at `0x2f1390`, and the reconstruction names that call `norrath_entity_floorf`. The same routine calls `fptodp` at `0x2fb320`.

Host comparisons live beside the sources as `host_test.cpp`. They include host headers, so they are not EE build inputs. Each implementation file compiles with the reviewed EE g++ `authored_cpp` flags and has no unresolved symbols of its own.

A private copy of the plan, kept outside git, appended these eight units to the branch plan and compiled 304 authored inputs. The 20 names below were absent from that build's unresolved list. That link was not executed, and it is not an original-byte match.

## Evidence

Observed, inferred, and unknown are separated in the source comments. Short form:

- `isinf` at `0x30c470` (72 bytes) and `isnan` at `0x2f0db0` (60 bytes) are each called once, from `_vfprintf_r`, with the double bits in one 64-bit GPR. Infinity returns 1 for both signs. NaN returns 1 only for an all-ones exponent and a nonzero fraction.
- `floorf` at `0x2f1390` (216 bytes) is the body behind `norrath_entity_floorf`. Exponents at or above 23, including infinities and NaNs, return unchanged. Positive magnitudes below 1 become +0. Negative normals below 1 become -1. Negative zero and negative subnormals stay unchanged.
- `fptodp` stores F12 and calls `__unpack_f`. A zero float exponent becomes class word 2 and packs to a signed zero, so subnormals flush. Every nonzero float exponent, including 255, becomes class word 3 and packs as a finite double. Float infinities and NaNs therefore become finite values at power 128, not IEEE double infinities or NaNs.
- The eight `.lit4` words at `0x47fefc` are the cosine loads followed by the sine loads. The two groups hold the same four values and remain separate symbols. No ELF name covers that pool.
- Float `atanf` reads `atanhi` at `0x47caf0` (16 bytes), `atanlo` at `0x47cb00` (16 bytes), and `aT` at `0x47cb10` (44 bytes). The double-precision copies of those names are a different range and are not these symbols.
- `bp`, `dp_h`, and `dp_l` are the 8-byte objects at `0x47d3b0`, `0x47d3b8`, and `0x47d3c0`. Four unnamed bytes at `0x47d434` are `0x7f7fffff`. `two_over_pi` starts at `0x47d438`.
- `__thenan_df` at `0x47df08` is 24 zero bytes. The soft-double parts layout reads class, sign, exponent, and a fraction at offset 16. Class 0 is inside the NaN range those routines already treat as exceptional (`category < 2`).

## Imports closed

- `norrath_factory_double_isinf`
- `norrath_factory_double_isnan`
- `norrath_entity_floorf`
- `norrath_entity_fptodp`
- `norrath_icos_scale`
- `norrath_icos_cubic`
- `norrath_icos_quintic`
- `norrath_icos_septic`
- `norrath_isin_scale`
- `norrath_isin_cubic`
- `norrath_isin_quintic`
- `norrath_isin_septic`
- `norrath_animation_math_atan_high_view`
- `norrath_animation_math_atan_low_view`
- `norrath_animation_math_atan_coefficients_view`
- `norrath_animation_math_view_47d3b0`
- `norrath_animation_math_view_47d3b8`
- `norrath_animation_math_view_47d3c0`
- `norrath_animation_math_view_47d434`
- `norrath_player_softdouble_nan_parts_view`

## Imports opened

None. Each new object has an empty undefined-symbol list.

## Build-plan entries needed

Append these authored inputs and assign fresh `input_order` values. The private trial used orders 298 through 305 against revision `12237bf`, whose plan had 298 inputs. The published tip already has more startup inputs, so those trial orders are not stable. Leave `floorf` and `ceilf` as the optional external inputs. Do not add any `host_test.cpp` file: each includes host headers. There is no private integration object yet, so no `private_object_sha256` is claimed. Body sizes are from the reviewed EE g++ `authored_cpp` profile, not from the original executable.

`default_authored_inputs` increases by 8. `header_closure` is empty for every unit. `profile` is `authored_cpp`.

```json
{"source":"sources/math-double-isinf/double_isinf.cpp","object_name":"double_isinf.o","sha256":"a74f52effb9ea3126f6f29e3ba237829f11996be0593a2ab0fdf4eb91c70e0bb","emitted_functions":[{"name":"norrath_factory_double_isinf","compiled_body_bytes":60}]}
{"source":"sources/math-double-isnan/double_isnan.cpp","object_name":"double_isnan.o","sha256":"34db1e4bb097a46110efc159098d0937c0f7177b06ca6fd40bf1d39f9b66e5fb","emitted_functions":[{"name":"norrath_factory_double_isnan","compiled_body_bytes":44}]}
{"source":"sources/math-entity-floor/entity_floor.cpp","object_name":"entity_floor.o","sha256":"17a95f18fca30e6c58a498f999d8dc315c96f62e444fcbe31ef782a387094c07","emitted_functions":[{"name":"norrath_entity_floorf","compiled_body_bytes":232}]}
{"source":"sources/math-entity-fptodp/entity_fptodp.cpp","object_name":"entity_fptodp.o","sha256":"4dc3d52dacda1d11b5552aec5d535854446a74ab7365a0b784cafead141700a3","emitted_functions":[{"name":"norrath_entity_fptodp","compiled_body_bytes":104}]}
{"source":"sources/math-integer-trig-coefficients/coefficients.cpp","object_name":"coefficients.o","sha256":"64d2b076423092833c4f5c7f49ae0c3a05c88ce39c3d1c4040b451dce0d4523c","emitted_functions":[]}
{"source":"sources/math-atan-tables/atan_tables.cpp","object_name":"atan_tables.o","sha256":"5ccbd9af257fb289ad79fecb9a9c57956a70d37a9ee8b9e4fee7d0402b79b5d6","emitted_functions":[]}
{"source":"sources/math-pow-constants/pow_constants.cpp","object_name":"pow_constants.o","sha256":"d7f05b71a48437646814998e928b2d34b994f110fdc1413faab32ff6c9b8324f","emitted_functions":[]}
{"source":"sources/math-softdouble-nan/nan_parts.cpp","object_name":"nan_parts.o","sha256":"559525bb8e9ae5ba0aba3e1d082c1f18dfe60c78c5619c2ac6458844916b2293","emitted_functions":[]}
```

## Authored but never executed

All four functions and every table in this package. Host comparisons ran on this Mac, and the reviewed EE compiler produced relocatable objects. No routine was executed on the EE, under the original process, or against gameplay. The floor helper's large add is the original inexact probe; that flag was not sampled. The float-to-double helper's unread stack slots on a zero exponent were not sampled, because the class-2 pack does not read them. The private 304-input link only checked symbol closure.

## Unknown

- The original identifier of the four unnamed bytes at `0x47d434`. The reconstruction name is the existing view symbol. Nothing was found that reads past those four bytes, and that absence is not a proof.
- Why the sine and cosine coefficient pools repeat the same four values as separate objects.
- These definitions close imports. They do not place the objects at the original addresses, and they do not claim byte-identical code or section identity with `.lit4` and `.rodata`.
- The double-precision `atanhi`, `atanlo`, and `aT` objects are not part of this float table.
- Exception flags, denormal hardware mode, interrupts, and game runtime remain untested.
- `norrath_formatter_nan_text_view` and `norrath_dtoa_nan_text_view` are text imports, not this NaN parts record. They were left open.
