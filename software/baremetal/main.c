#include <stdio.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "xil_cache.h"   // Cache control utilities

// Ensure this include path matches your project structure
#include "fir_stereo.h"

// ============================================================================
// USER CONFIGURATION
// ============================================================================
// Make sure this IP name matches the one generated in xparameters.h.
// If you get an "undeclared" error, inspect xparameters.h and adjust
// the macro name accordingly.
#if defined(XPAR_FIR_AXIS_WRAPPER_0_BASEADDR)
    #define FIR_BASE_ADDR  XPAR_FIR_AXIS_WRAPPER_0_BASEADDR
#else
    // Manual fallback (update with the actual address from Vivado Address Editor)
    #define FIR_BASE_ADDR  0x43C00000
#endif

#define HARDWARE_TAPS  129

// Driver instance
FirStereo_Config MyFir;

// ============================================================================
// FILTER COEFFICIENT EXAMPLES (Q1.15 FORMAT)
// ============================================================================

// 1. Impulse response (pass-through)
const s16 Filter_Impulse[1] = {
    32767   // ~1.0
};

// 2. Simple moving average (5-tap low-pass)
const s16 Filter_MovAvg[5] = {
    6553, 6553, 6553, 6553, 6553
};

// ============================================================================
// MAIN APPLICATION
// ============================================================================
int main(void)
{
    // NOTE:
    // init_platform() is intentionally omitted.
    // Standard Xilinx BSP already initializes UART.

    // Optional: Disable data cache to ensure direct register access
    // (avoids cache coherency issues when accessing AXI peripherals)
    Xil_DCacheDisable();

    xil_printf("\033[2J\033[H"); // Clear terminal
    xil_printf("=== FIR Stereo Hardware Test (Vitis 2024.1) ===\n\r");

    // ------------------------------------------------------------------------
    // 1. Driver Initialization
    // ------------------------------------------------------------------------
    int Status = FIR_Init(&MyFir, FIR_BASE_ADDR, HARDWARE_TAPS);
    if (Status != XST_SUCCESS) {
        xil_printf("ERROR: FIR driver initialization failed!\n\r");
        return -1;
    }

    xil_printf("FIR initialized at base address: 0x%08x\n\r",
               FIR_BASE_ADDR);

    // ------------------------------------------------------------------------
    // 2. Load Initial Filter (Moving Average)
    // ------------------------------------------------------------------------
    xil_printf("Loading moving average filter (5 taps)...\n\r");
    FIR_LoadConfig(&MyFir, Filter_MovAvg, 5);

    // Read-back verification
    s16 ReadBack = FIR_GetCoeff(&MyFir, 0);
    xil_printf("Verification -> Wrote: %d, Read: %d ... ",
               Filter_MovAvg[0], ReadBack);

    if (ReadBack == Filter_MovAvg[0])
        xil_printf("OK\n\r");
    else
        xil_printf("FAIL\n\r");

    // ------------------------------------------------------------------------
    // 3. Enable FIR Filter
    // ------------------------------------------------------------------------
    FIR_Enable(&MyFir, 1);
    xil_printf("Filter ENABLED. Audio should sound muffled (low-pass).\n\r");

    // ------------------------------------------------------------------------
    // 4. Runtime Filter Hot-Swap Demonstration
    // ------------------------------------------------------------------------
    xil_printf("Waiting before switching to pass-through...\n\r");

    // Simple blocking delay
    for (volatile int i = 0; i < 10000000; i++);

    xil_printf("Switching to impulse response (pass-through)...\n\r");
    FIR_LoadConfig(&MyFir, Filter_Impulse, 1);

    xil_printf("Filter updated. Audio should sound clear.\n\r");

    // cleanup_platform() intentionally omitted
    return 0;
}
