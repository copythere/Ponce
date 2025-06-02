//! \file
/*
**  Copyright (c) 2020 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

#pragma once

#ifdef BUILD_HEXRAYS_SUPPORT
#include <hexrays.hpp>
ssize_t idaapi ponce_hexrays_callback(void*, hexrays_event_t event, va_list va);
#endif

extern bool hexrays_present;