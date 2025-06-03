//! \file
/*
**  Copyright (c) 2020 - Ponce
**  Authors:
**         Alberto Garcia Illera        agarciaillera@gmail.com
**         Francisco Oca                francisco.oca.gonzalez@gmail.com
**
**  This program is under the terms of the BSD License.
*/

//IDA
#include <pro.h>
#include <idp.hpp>
#include <loader.hpp>
#include <kernwin.hpp>

//Ponce
#include "formTaintSymbolizeInput.hpp"
#include "globals.hpp"
#include "utils.hpp"

/*Function to show a dialog to the user asking for an address and a size to taint/symbolize.
It returns a MemoryAccess with the memory address and the size indicated. the caller need to free this object*/
bool prompt_window_taint_symbolize(ea_t address, sval_t size, ea_t *selection_start, ea_t *selection_end)
{
	char format[125] = { 0 };
	/*
	// DEBUG: Print input values before showing dialog
	msg("[DEBUG] prompt_window_taint_symbolize input:\n");
	msg("[DEBUG]   initial address = " MEM_FORMAT "\n", address);
	msg("[DEBUG]   initial size = %d (0x%x)\n", (int)size, (int)size);
	// END DEBUG
	*/
	if (ask_form(formTaintSymbolizeInput,
		NULL,
		&address,
		&size
		) > 0)
	{
		/*
		// DEBUG: Print user input values after dialog
		msg("[DEBUG] User input from dialog:\n");
		msg("[DEBUG]   user address = " MEM_FORMAT "\n", address);
		msg("[DEBUG]   user size = %d (0x%x)\n", (int)size, (int)size);
		// END DEBUG
		*/
		*selection_start = address;
		*selection_end = address + size;
		/*
		// DEBUG: Print calculated output values
		msg("[DEBUG] Calculated output:\n");
		msg("[DEBUG]   selection_start = " MEM_FORMAT "\n", *selection_start);
		msg("[DEBUG]   selection_end = " MEM_FORMAT "\n", *selection_end);
		msg("[DEBUG]   calculated length = %d (0x%x)\n", (int)(*selection_end - *selection_start), (int)(*selection_end - *selection_start));
		// END DEBUG
		*/
		return true;
	}
	return false;
}