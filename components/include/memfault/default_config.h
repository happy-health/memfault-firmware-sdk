#pragma once

//! @file
//!
//! Copyright (c) Memfault, Inc.
//! See License.txt for details
//!
//! Default configuration settings for the Memfault SDK
//! This file should always be picked up through "memfault/config.h"
//! and never included directly

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MEMFAULT_PLATFORM_CONFIG_INCLUDE_DEFAULTS
#error "Include "memfault/config.h" instead of "memfault/default_config.h" directly"
#endif

//
// Core Components
//

//! Controls the
#ifndef MEMFAULT_USE_GNU_BUILD_ID
#define MEMFAULT_USE_GNU_BUILD_ID 0
#endif

//! While it is recommended that MEMFAULT_SDK_ASSERT be left enabled, they can
//! be disabled by adding -DMEMFAULT_SDK_ASSERT_ENABLED=0 to the CFLAGS used to
//! compile the SDK
#ifndef MEMFAULT_SDK_ASSERT_ENABLED
#define MEMFAULT_SDK_ASSERT_ENABLED 1
#endif

//! Controls whether or not device serial is encoded in events
//!
//! When disabled (default), the device serial is derived from the API route
//! used to post data to the cloud
#ifndef MEMFAULT_EVENT_INCLUDE_DEVICE_SERIAL
#define MEMFAULT_EVENT_INCLUDE_DEVICE_SERIAL 0
#endif

//! Controls whether or not run length encoding (RLE) is used when packetizing
//! data
//!
//! Significantly reduces transmit size of data with repeated patterns such as
//! coredumps at the cost of ~1kB of codespace
#ifndef MEMFAULT_DATA_SOURCE_RLE_ENABLED
#define MEMFAULT_DATA_SOURCE_RLE_ENABLED 1
#endif

//! Controls default log level that will be saved to https://mflt.io/logging
#ifndef MEMFAULT_RAM_LOGGER_DEFAULT_MIN_LOG_LEVEL
#define MEMFAULT_RAM_LOGGER_DEFAULT_MIN_LOG_LEVEL kMemfaultPlatformLogLevel_Info
#endif

// Shouldn't typically be needed but allows for persisting of MEMFAULT_LOG_*'s
// to be disabled via a CFLAG: CFLAGS += -DMEMFAULT_SDK_LOG_SAVE_DISABLE=1
#ifndef MEMFAULT_SDK_LOG_SAVE_DISABLE
#define MEMFAULT_SDK_LOG_SAVE_DISABLE 0
#endif

// Allows for the MEMFAULT_LOG APIs to be re-mapped to another macro rather
// than the  memfault_platform_log dependency function
//
// In this mode a user of the SDK will need to define the following macros:
//   MEMFAULT_LOG_DEBUG(...)
//   MEMFAULT_LOG_INFO(...)
//   MEMFAULT_LOG_WARN(...)
//   MEMFAULT_LOG_ERROR(...)
//
// And if the "demo" component is being used:
//   MEMFAULT_LOG_RAW(...)
#ifndef MEMFAULT_PLATFORM_HAS_LOG_CONFIG
#define MEMFAULT_PLATFORM_HAS_LOG_CONFIG 0
#endif

//! Controls whether or not multiple events will be batched into a single
//! message when reading information via the event storage data source.
//!
//! This can be a useful strategy when trying to maximize the amount of data
//! sent in a single transmission unit.
//!
//! To enable, you will need to update the compiler flags for your project, i.e
//!   CFLAGS += -DMEMFAULT_EVENT_STORAGE_READ_BATCHING_ENABLED=1
#ifndef MEMFAULT_EVENT_STORAGE_READ_BATCHING_ENABLED
#define MEMFAULT_EVENT_STORAGE_READ_BATCHING_ENABLED 0
#endif

#if MEMFAULT_EVENT_STORAGE_READ_BATCHING_ENABLED != 0

//! When batching is enabled, controls the maximum amount of event data bytes
//! that will be in a single message.
//!
//! By default, there is no limit. To set a limit you will need to update the
//! compiler flags for your project the following would cap the data payload
//! size at 1024 bytes
//!  CFLAGS += -DMEMFAULT_EVENT_STORAGE_READ_BATCHING_MAX_BYTES=1024
#ifndef MEMFAULT_EVENT_STORAGE_READ_BATCHING_MAX_BYTES
#define MEMFAULT_EVENT_STORAGE_READ_BATCHING_MAX_BYTES UINT32_MAX
#endif

#endif /* MEMFAULT_EVENT_STORAGE_READ_BATCHING_ENABLED */

//! The max size of a chunk. Should be a size suitable to write to transport
//! data is being dumped over.
#ifndef MEMFAULT_DATA_EXPORT_CHUNK_MAX_LEN
#define MEMFAULT_DATA_EXPORT_CHUNK_MAX_LEN 80
#endif

#ifndef MEMFAULT_COREDUMP_COLLECT_LOG_REGIONS
#define MEMFAULT_COREDUMP_COLLECT_LOG_REGIONS 0
#endif

#ifndef MEMFAULT_TRACE_REASON_USER_DEFS_FILE
#define MEMFAULT_TRACE_REASON_USER_DEFS_FILE \
  "memfault_trace_reason_user_config.def"
#endif

//! By default, the Memfault SDK allows for trace events with logs to be
//! captured for trace events from ISRs.
//!
//! However, this can be disabled to reduce static RAM usage by
//! MEMFAULT_TRACE_EVENT_MAX_LOG_LEN
#ifndef MEMFAULT_TRACE_EVENT_WITH_LOG_FROM_ISR_ENABLED
#define MEMFAULT_TRACE_EVENT_WITH_LOG_FROM_ISR_ENABLED 1
#endif

//! The maximum size allocated for a trace event log
#ifndef MEMFAULT_TRACE_EVENT_MAX_LOG_LEN
#define MEMFAULT_TRACE_EVENT_MAX_LOG_LEN 80
#endif

//
// Metrics Component Configurations
//

//! The frequency in seconds to collect heartbeat metrics. The suggested
//! interval is once per hour but the value can be overriden to be as low as
//! once every 15 minutes.
#ifndef MEMFAULT_METRICS_HEARTBEAT_INTERVAL_SECS
#define MEMFAULT_METRICS_HEARTBEAT_INTERVAL_SECS 3600
#endif

#ifndef MEMFAULT_METRICS_USER_HEARTBEAT_DEFS_FILE
#define MEMFAULT_METRICS_USER_HEARTBEAT_DEFS_FILE \
  "memfault_metrics_heartbeat_config.def"
#endif

//
// Panics Component Configs
//

// By default, enable collection of interrupt state at the time the coredump is
// collected. User of the SDK can disable collection by adding the following
// compile flag: -DMEMFAULT_COLLECT_INTERRUPT_STATE=0
//
// NB: The default Interrupt collection configuration requires ~150 bytes of
// coredump storage space to save.
#ifndef MEMFAULT_COLLECT_INTERRUPT_STATE
#define MEMFAULT_COLLECT_INTERRUPT_STATE 1
#endif

// ARMv7-M supports at least 32 external interrupts in the NVIC and a maximum of
// 496.
//
// By default, only collect the minimum set. For a typical application this will
// generally cover all the interrupts in use.
//
// If there are more interrupts implemented than analyzed a note will appear in
// the "ISR Analysis" tab for the Issue analyzed in the Memfault UI about the
// appropriate setting needed.
//
// NB: For each additional 32 NVIC interrupts analyzed, 40 extra bytes are
// needed for coredump storage.
#ifndef MEMFAULT_NVIC_INTERRUPTS_TO_COLLECT
#define MEMFAULT_NVIC_INTERRUPTS_TO_COLLECT 32
#endif

// By default, exception handlers use CMSIS naming conventions. By default, the
// CMSIS library provides a weak implementation for each handler that is
// implemented as an infinite loop. By using the same name, the Memfault SDK can
// override this default behavior to capture crash information when a fault
// handler runs.
//
// However, if needed, each handler can be renamed using the following
// preprocessor defines:

#ifndef MEMFAULT_EXC_HANDLER_HARD_FAULT
#define MEMFAULT_EXC_HANDLER_HARD_FAULT HardFault_Handler
#endif

#ifndef MEMFAULT_EXC_HANDLER_MEMORY_MANAGEMENT
#define MEMFAULT_EXC_HANDLER_MEMORY_MANAGEMENT MemoryManagement_Handler
#endif

#ifndef MEMFAULT_EXC_HANDLER_BUS_FAULT
#define MEMFAULT_EXC_HANDLER_BUS_FAULT BusFault_Handler
#endif

#ifndef MEMFAULT_EXC_HANDLER_USAGE_FAULT
#define MEMFAULT_EXC_HANDLER_USAGE_FAULT UsageFault_Handler
#endif

#ifndef MEMFAULT_EXC_HANDLER_NMI
#define MEMFAULT_EXC_HANDLER_NMI NMI_Handler
#endif

#ifndef MEMFAULT_EXC_HANDLER_WATCHDOG
#define MEMFAULT_EXC_HANDLER_WATCHDOG MemfaultWatchdog_Handler
#endif

//
// Http Configuration Options
//

#ifndef MEMFAULT_HTTP_CHUNKS_API_HOST
#define MEMFAULT_HTTP_CHUNKS_API_HOST "chunks.memfault.com"
#endif

#ifndef MEMFAULT_HTTP_DEVICE_API_HOST
#define MEMFAULT_HTTP_DEVICE_API_HOST "device.memfault.com"
#endif

#ifndef MEMFAULT_HTTP_APIS_DEFAULT_PORT
#define MEMFAULT_HTTP_APIS_DEFAULT_PORT (443)
#endif

//
// Demo Configuration Options
//

#ifndef MEMFAULT_CLI_LOG_BUFFER_MAX_SIZE_BYTES
#define MEMFAULT_CLI_LOG_BUFFER_MAX_SIZE_BYTES (80)
#endif

#ifndef MEMFAULT_DEMO_CLI_USER_CHUNK_SIZE
// Note: Arbitrary default size for CLI command. Can be as small as 9 bytes.
#define MEMFAULT_DEMO_CLI_USER_CHUNK_SIZE 1024
#endif

//
// Port Configuration Options
//

//! Controls whether or not MCU reboot info is printed when
//! memfault_reboot_reason_get() is called
#ifndef MEMFAULT_ENABLE_REBOOT_DIAG_DUMP
#define MEMFAULT_ENABLE_REBOOT_DIAG_DUMP 1
#endif

//! Controls whether or not MCU reboot info is reset when
//! memfault_reboot_reason_get() is called
#ifndef MEMFAULT_REBOOT_REASON_CLEAR
#define MEMFAULT_REBOOT_REASON_CLEAR 1
#endif

//! Timeout to set Software Watchdog expiration for
#ifndef MEMFAULT_WATCHDOG_SW_TIMEOUT_SECS
#define MEMFAULT_WATCHDOG_SW_TIMEOUT_SECS 10
#endif

//! The maximum number of tasks which can be tracked by this subsystem at one
//! time. If your system has more tasks than this, the number will need to be
//! bumped in order for the stacks to be fully recovered
#ifndef MEMFAULT_PLATFORM_MAX_TRACKED_TASKS
#define MEMFAULT_PLATFORM_MAX_TRACKED_TASKS 16
#endif

//! The default amount of stack for each task to collect in bytes.  The larger
//! the size, the more stack frames Memfault will be able to unwind when the
//! coredump is uploaded.
#ifndef MEMFAULT_PLATFORM_TASK_STACK_SIZE_TO_COLLECT
#define MEMFAULT_PLATFORM_TASK_STACK_SIZE_TO_COLLECT 256
#endif

#ifdef __cplusplus
}
#endif
