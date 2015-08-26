#ifndef QGF_GRAPHICS_TONE_H
#define QGF_GRAPHICS_TONE_H 1

#include "qgf/types.h"

//==============================================================================================
// ** Tone Class
//----------------------------------------------------------------------------------------------
// This class Manages tone.
//----------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	// GC functions
	//--------------------------------------------------------------------------------------
	ruby_value
	rb_gc_cTone_alloc(ruby_value klass);
	void 
	rb_gc_cTone_free(TTone* tone);
	void 
	rb_gc_cTone_mark(TTone* tone);		
	//--------------------------------------------------------------------------------------
	// * Tone#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	ruby_value 
	rb_cTone_initialize(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Tone#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	ruby_value 
	rb_cTone_set(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Tone#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	ruby_value 
	rb_cTone_GETred(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Tone#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	ruby_value 
	rb_cTone_SETred(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Tone#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	ruby_value 
	rb_cTone_GETgreen(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Tone#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	ruby_value 
	rb_cTone_SETgreen(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Tone#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	ruby_value 
	rb_cTone_GETblue(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Tone#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	ruby_value 
	rb_cTone_SETblue(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Tone#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	ruby_value 
	rb_cTone_GETgray(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Tone#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	ruby_value 
	rb_cTone_SETgray(ruby_value self, ruby_value value);
	//======================================================================================
	// Marshalling
	//======================================================================================
	ruby_value 
	rb_cTone__dump(ruby_value self);
	ruby_value 
	rb_cToneCLASS__load(ruby_value self, ruby_value str);  
	//======================================================================================
	static ruby_value rb_cTone;
	//======================================================================================

	void
	Init_QGF_Graphics_Tone();

  #endif
