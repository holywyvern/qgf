#ifndef QGF_GRAPHICS_PLANE_H
#define QGF_GRAPHICS_PLANE_H 1

#include "qgf/types.h"  
  
//==============================================================================================
// ** Plane Class
//----------------------------------------------------------------------------------------------
// This class handles Wrapping Sprites
//----------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	// GC functions
	//--------------------------------------------------------------------------------------
	static ruby_value
	rb_gc_cPlane_alloc(ruby_value klass);
	static void 
	rb_gc_cPlane_free(TZObject* obj);
	static void 
	rb_gc_cPlane_mark(TZObject* obj);
	//--------------------------------------------------------------------------------------
	// Drawable Functions
	//--------------------------------------------------------------------------------------
	void 
	rb_cPlane_draw(TPlane* obj, long gw, long gh);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - viewport :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_initialize(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// None.
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_dispose(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_disposedQ(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value
	rb_cPlane_update(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value
	rb_cPlane_flash(ruby_value self, ruby_value color, ruby_value duration);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETbitmap(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETbitmap(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETviewport(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETviewport(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETvisible(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETvisible(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETz(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETz(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETox(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETox(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GEToy(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SEToy(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETzoom_x(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETzoom_x(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETzoom_y(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETzoom_y(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETopacity(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETopacity(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETblend_type(ruby_value self);	
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETblend_type(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETcolor(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETcolor(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETtone(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETtone(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETwave_amp(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value
	rb_cPlane_SETwave_amp(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETwave_length(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value
	rb_cPlane_SETwave_length(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETwave_speed(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETwave_speed(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_GETwave_phase(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Plane#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cPlane_SETwave_phase(ruby_value self, ruby_value value);

	//======================================================================================
	static ruby_value rb_cPlane;
	//======================================================================================
  
	void
	Init_QGF_Graphics_Plane();

#endif  