# Assignment_C_Advanced_01
Assignment_C_Advanced_01
# Measurement Struct — Memory Layout Explained

This document explains 'sizeof' , 'offsetof', 'alignment', and the 'observed padding'
for the `Measurement` struct, verified on a 64-bit system (x86_64, MSYS2 UCRT64).

```c
typedef struct {
    uint16_t id;          // 2 bytes
    uint8_t  status;      // 1 byte
    uint32_t timestamp;   // 4 bytes
    size_t   sample_count;// 8 bytes (on 64-bit)
    int16_t  samples[];   // flexible array member (0 bytes in sizeof)
} Measurement;
```
## SIZEOF 
sizeof(Measurement) returns the total number of bytes including any padding the compiler inserts.
The flexible array member `samples[]` contributes 0 bytes to `sizeof`.

=> Measured result: `sizeof(Measurement) == 16`

## ALIGNMENT

**Alignment** is a hardware rule: a value of size *N* usually must sit at a memory
address that is a **multiple of its alignment** (often equal to its size). CPUs read
memory in aligned blocks, so a misaligned value would cost extra reads — or crash on
strict architectures.

Alignment requirement of each member on this platform:

| Type     | Alignment |
|----------|----------:|
| uint8_t  | 1         |
| uint16_t | 2         |
| uint32_t | 4         |
| size_t   | 8         |
| int16_t  | 2         |

**The struct's own alignment = the largest member alignment = 8** (from `size_t`).
Confirmed: `_Alignof(Measurement) == 8`.

This is why `sample_count` must land on an offset that is a multiple of 8
(it lands on offset 8), and why the whole struct size must be a multiple of 8.

---
## OBSERVED PADDING

**Padding** = the invisible filler bytes the compiler inserts so every field lands on
a properly aligned address. Walking through the layout:

```
byte:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
      [ id  ] [st] [XX] [  timestamp  ] [       sample_count       ]
                    ^pad
```

- `id` occupies bytes **0–1**.
- `status` occupies byte **2**.
- `timestamp` needs a **4-byte-aligned** address. The next free byte is 3, which is
  **not** a multiple of 4, so the compiler inserts **1 byte of padding at offset 3**.
  `timestamp` then starts at offset 4.
- `sample_count` needs an **8-byte-aligned** address. `timestamp` ends at offset 8,
  which is already a multiple of 8 → **no padding needed** here.
- The struct ends at offset 16, already a multiple of the struct alignment (8),
  so there is **no trailing padding**.

**Total observed padding: 1 byte** (at offset 3).

That single padding byte is exactly the difference between the raw field sum (15)
and `sizeof(Measurement)` (16).

---

## OFFSETOF

`offsetof(type, member)` (from `<stddef.h>`) returns the **byte distance from the
start of the struct to a given field**. It reveals exactly where each field sits.

| Field          | Type       | `offsetof` | Bytes occupied |
|----------------|------------|-----------:|----------------|
| `id`           | uint16_t   | **0**      | 0 – 1          |
| `status`       | uint8_t    | **2**      | 2              |
| *(padding)*    | —          | *3*        | *3 (1 byte)*   |
| `timestamp`    | uint32_t   | **4**      | 4 – 7          |
| `sample_count` | size_t     | **8**      | 8 – 15         |
| `samples`      | int16_t[]  | **16**     | starts at 16   |

`offsetof(Measurement, samples) == 16` confirms the flexible array begins right
after the last real field — this is where the dynamically allocated samples live.

---

## Summary

| Concept          | Result for `Measurement` |
|------------------|--------------------------|
| `sizeof`         | 16 bytes                 |
| `_Alignof`       | 8 bytes                  |
| Largest field    | `size_t sample_count` (8 bytes) |
| Padding inserted | 1 byte (offset 3)        |
| Flexible array   | 0 bytes in `sizeof`; allocated separately |

**Key takeaway:** field order matters. Because `status` (1 byte) sits before the
4-byte `timestamp`, a padding byte is required. Reordering fields from largest to
smallest alignment can sometimes eliminate padding and shrink the struct.
