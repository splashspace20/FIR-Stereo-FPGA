# AXI-Lite Address Map

The base address is assigned in the Vivado Address Editor and exposed to
software via `xparameters.h`.

This AXI-Lite interface is used for:
- FIR core control (enable / clear state)
- Runtime coefficient updates

---

## Register Layout

| Offset | Name | Description |
|------:|------|-------------|
| 0x00 | CTRL | Control register |
| 0x10 | COEF[0] | FIR coefficient tap 0 (Q1.15) |
| 0x14 | COEF[1] | FIR coefficient tap 1 (Q1.15) |
| ...  | ... | ... |
| 0x10 + 4·(N−1) | COEF[N−1] | FIR coefficient tap N−1 (Q1.15) |

---

## Control Register (0x00)

| Bit | Name | Description |
|----:|------|-------------|
| 0 | ENABLE | Enables FIR processing |
| 1 | CLEAR_STATE | Clears FIR internal state (delay line & accumulators) |

- `CLEAR_STATE` is edge-triggered (write 1 → write 0).
- Coefficient memory is **not** affected by `CLEAR_STATE`.

---

## Coefficient Memory

- Each coefficient is mapped to a 32-bit AXI-Lite word.
- Only the **lower 16 bits** are used by hardware.
- Coefficients are interpreted as **signed Q1.15** values.
- Coefficients are shared between Left and Right channels.

---

## Notes

- All coefficients use **signed 16-bit fixed-point (Q1.15)** format.
- Upper 16 bits of AXI-Lite writes are ignored.
- Writes outside the valid tap range are safely ignored.
- Only a single outstanding AXI-Lite transaction is supported.
