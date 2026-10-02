// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026, The OpenROAD Authors

//
// syn IR backend for slang-elab
//
// Logger/source-manager plumbing for the slang-elab frontend.
//
// The frontend's own Yosys-style logging entry points (log, log_warning,
// log_error, ...) are provided inline by slang_frontend.h under
// SLANG_NO_YOSYS, so they are not defined here. What remains is the
// OpenROAD-side scope that lets our abort_helpers route diagnostics that
// the frontend hands back to us through utl::Logger.
//
// This TU pulls utl/Logger.h (and transitively spdlog's bundled fmt) and so
// must not include any slang header: slang brings a different fmt version,
// and mixing the two in one TU causes inline-namespace collisions on fmt::vNN.
//

#include "diagnostics.h"

#include <string_view>

#include "utl/Logger.h"

namespace slang_frontend {

namespace {

// Icky, see povik/sv-elab#383
utl::Logger* elab_logger = nullptr;
const slang::SourceManager* elab_source_manager = nullptr;

}  // namespace

void reportError(utl::Logger* logger, int code, std::string_view message)
{
  logger->error(utl::SYN, code, "{}", message);
}

utl::Logger* elabLogger()
{
  return elab_logger;
}

const slang::SourceManager* elabSourceManager()
{
  return elab_source_manager;
}

ElabDiagnosticScope::ElabDiagnosticScope(
    utl::Logger* logger,
    const slang::SourceManager* source_manager)
    : previous_logger_(elab_logger),
      previous_source_manager_(elab_source_manager)
{
  elab_logger = logger;
  elab_source_manager = source_manager;
}

ElabDiagnosticScope::~ElabDiagnosticScope()
{
  elab_logger = previous_logger_;
  elab_source_manager = previous_source_manager_;
}

}  // namespace slang_frontend
