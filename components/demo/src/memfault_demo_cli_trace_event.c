//! @file
//!
//! Copyright (c) Memfault, Inc.
//! See License.txt for details
//!
//! @brief
//! CLI command that exercises the MEMFAULT_TRACE_EVENT API, capturing a
//! Trace Event with the error reason set to "MemfaultDemoCli_Error".

#include "memfault/core/debug_log.h"
#include "memfault/core/trace_event.h"

int memfault_demo_cli_cmd_trace_event_capture(int argc, char *argv[]) {
  // For more information on user-defined error reasons, see
  // the MEMFAULT_TRACE_REASON_DEFINE macro in trace_reason_user.h .
#if defined(MEMFAULT_TRACE_REASON_USER_DEFS_FILE)
  MEMFAULT_TRACE_EVENT(Unknown);
#else
  MEMFAULT_TRACE_EVENT(MemfaultDemoCli_Error);
#endif
  return 0;
}
