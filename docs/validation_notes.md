# Validation Notes

This document summarizes the verification and validation activities
performed for the Stereo FIR AXI Filter.

---

## RTL Simulation

- FIR core verified using:
  - Impulse response
  - Step and sine wave inputs
  - Saturation and clipping behavior
  - Dynamic coefficient switching during streaming

- AXI wrapper verified using:
  - AXI-Lite control register access
  - Runtime coefficient updates via memory-mapped interface
  - AXI-Stream handshake and backpressure
  - Stereo interleaved data flow

Simulation results are generated from self-checking
SystemVerilog testbenches.

---

## Hardware Validation

- Bare-metal test application performs:
  - FIR driver initialization
  - Runtime loading of FIR coefficients
  - Stereo audio streaming via AXI DMA
  - On-the-fly filter reconfiguration (hot-swap)

- Console output is used for:
  - Register read-back verification
  - Basic functional sanity checks

---

## Validation Scope

- RTL simulation results are treated as the **primary reference**
- Hardware execution serves as:
  - Integration proof
  - AXI interconnect and DMA verification
  - Software–hardware interface validation

---

## Notes

- FIR coefficients are shared between Left and Right channels
- Clearing FIR state does not affect coefficient memory
- All validation is performed using fixed-point Q1.15 arithmetic
