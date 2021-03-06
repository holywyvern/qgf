#ifndef QGF_GRAPHICS_VIEWPORT_H
#define QGF_GRAPHICS_VIEWPORT_H 1

#include "qgf/types.h"

//==============================================================================================
// ** Viewport Class
//----------------------------------------------------------------------------------------------
// This class manages viewports
//----------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	// GC functions
	//--------------------------------------------------------------------------------------
	static ruby_value
	rb_gc_cViewport_alloc(ruby_value klass);
	static void 
	rb_gc_cViewport_free(TZObject* obj);
	static void 
	rb_gc_cViewport_mark(TZObject* obj);		
	//--------------------------------------------------------------------------------------
	// Drawable Functions
	//--------------------------------------------------------------------------------------
	void 
	rb_cViewport_draw(TViewport* obj, long gw, long gh);     
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_initialize(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_dispose(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_disposedQ(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_flash(ruby_value self, ruby_value color, ruby_value duration);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_update(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_GETrect(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_SETrect(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_GETvisible(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_SETvisible(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_GETz(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_SETz(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_GETox(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_SETox(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_GEToy(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_SEToy(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_GETcolor(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_SETcolor(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_GETtone(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Viewport#
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  :
	//_____________________________________________________________________________________
	static ruby_value 
	rb_cViewport_SETtone(ruby_value self, ruby_value value);
	//======================================================================================
	static ruby_value rb_cViewport;
	//======================================================================================

	void
	Init_QGF_Graphics_Viewport();

	void
	rb_viewport_add_object(ruby_value self, ruby_value obj);

	void
	rb_viewport_remove_object(ruby_value self, ruby_value obj);

  #endif
