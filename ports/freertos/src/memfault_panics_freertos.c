//! @file
//!
//! Copyright (c) Memfault, Inc.
//! See License.txt for details
//!
//! Hooks for collecting coredumps from failure paths in FreeRTOS kernel

#if defined(HPY_MEMFAULT_ASSERTS_ENABLED)

#include "memfault/panics/assert.h"

#include "memfault/core/compiler.h"

#include "FreeRTOS.h"
#include "task.h"

#if !defined(IS_BOOTLOADER) && !defined(RF_TOOLS_CLI)
#include "hpy_assert.h"
// DVC-2263: Flag defined in happy/memfault/src/memfault_platform_port.c.
// Setting it before MEMFAULT_ASSERT lets memfault_platform_reboot() know
// this was an intentional assert (not a real HardFault) even though the
// `udf` trap inside MEMFAULT_ASSERT will create a CPU exception frame.
extern volatile bool s_assert_in_progress;
#endif

//! Collect a coredump when a FreeRTOS assert takes place
//!
//! Implementation for assert function present in FreeRTOSConfig.h:
//!  #define configASSERT(x) if ((x) == 0) vAssertCalled( __FILE__, __LINE__ )
void vAssertCalled(MEMFAULT_UNUSED const char *file,
                   MEMFAULT_UNUSED int line) {
#if !defined(IS_BOOTLOADER) && !defined(RF_TOOLS_CLI)
  s_assert_in_progress = true;
#endif
  MEMFAULT_ASSERT(0);
}

//! Collects a coredump when a stack overflow takes place in FreeRTOS kernel
//!
//! @note: Depends on FreeRTOS kernel being compiled with
//!  configCHECK_FOR_STACK_OVERFLOW != 0
void vApplicationStackOverflowHook(MEMFAULT_UNUSED TaskHandle_t xTask,
                                   MEMFAULT_UNUSED char *pcTaskName) {

// DVC-2046: Use the LOCK version of OS_ASSERT_ID since we want to hard capture
//           the assert caller. Without this, the assert ID gets lost prior
//           to the final write to NVM.

#if !defined(IS_BOOTLOADER) && !defined(RF_TOOLS_CLI)
  OS_ASSERT_LOCK_ID(HPY_ASSERT_ID_MF_STACK_OVERFLOW);
  s_assert_in_progress = true;
#endif

  MEMFAULT_ASSERT(0);
}
#endif
