#ifndef QGF_GRAPHICS_BITMAP_H
#define QGF_GRAPHICS_BITMAP_H 1

#include "qgf/types.h"  
//==============================================================================================
// ** Bitmap Class
//----------------------------------------------------------------------------------------------
// This class Controls bitmap images.
//----------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	// GC functions
	//--------------------------------------------------------------------------------------
	ruby_value
	rb_gc_cBitmap_alloc(ruby_value klass);
	void 
	rb_gc_cBitmap_free(TBitmap* bitmap);
	void 
	rb_gc_cBitmap_mark(TBitmap* bitmap);
	//--------------------------------------------------------------------------------------
	// * Bitmap#initialize(filename)
	// * Bitmap#initialize(width, height)
	// * Bitmap#initialize(bitmap)
	// Creates a bitmap object.
	// If filename is given, it will take the name of the file. extensions may be omited.
	// If width and height are the parameters, creates an empty, transparent bitmap
	// with that width and height.
	// If bitmap is given it will make a copy of that bitmap.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - filename : the name of the image to load.
	// - bitmap   : the bitmap to copy data.
	// - width    : the width of the bitmap.
	// - height   : the height of the bitmap.
	//_____________________________________________________________________________________
	ruby_value 
	rb_cBitmap_initialize(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#dispose
	// Disposes the current bitmap.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// None.
	//_____________________________________________________________________________________
	ruby_value 
	rb_cBitmap_dispose(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#disposed?
	// Returns true if the bitmap is disposed, false if not.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// None.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_disposedQ(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#width
	// Gets the width of the bitmap.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// None.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_width(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#height
	// Gets the Bitmap's height.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// None.
	//_____________________________________________________________________________________		
	ruby_value 
	rb_cBitmap_height(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#rect
	// Gets the bitmap rect as Rect#new(0, 0, width, height).
	//-------------------------------------------------------------------------------------
	// Parameters:
	// None.
	//_____________________________________________________________________________________		
	ruby_value 
	rb_cBitmap_rect(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#blt(x, y, bitmap, rect[, opacity])
	// Performs a Block transfer (blit) over the bitmap.
	// It performs alpha blending between the bitmap and the block transfer.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// -  x       : the x coordinate over the current bitmap.
	// -  y       : the y position of the top left corner to blit.
	// -  bitmap  : the <Bitmap> to blit
	// -  rect    : the <Rect> of the bitmap to rect.
	// -  opacity : the opacity of blitting (DEFAULT 255)
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_blt(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#stretch_blt(dest_rect, bitmap, src_rect[, opacity])
	// Performs a block transfer between the current bitmap and the bitmap parameter.
	// The transfered bitmap will be scaled as the dest_rect.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - dest_rect : the destiny <Rect> for the block transfer.
	// - bitmap    : the <Bitmap> to transfer pixels.
	// - src_rect  : the <Rect> of the source <Bitmap> to transfer.
	// - opacity   : the opacity of the source <Bitmap> (DEFAULT 255)
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_stretch_blt(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#fill_rect(rect, color)
	// * Bitmap#fill_rect(x, y, width, height)
	// Fills the Rect given or a rect as (x, y, width, height) with the color.
	// Alpha blending is performed.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - x      : the x coordinate of the <Rect>
	// - y      : the y coordinate of the <Rect>
	// - width  : the width of the <Rect>
	// - height : the height of the <Rect>
	// - rect   : the <Rect> to fill with the <Color>
	// - color  : the <Color> to fill the <Rect>
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_fill_rect(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#gradient_fill_rect(x, y, width, height, color1, color2[, angle])
	// * Bitmap#gradient_fill_rect(rect, color1, color2[, angle])
	// Performs a gradient rect over the bitmap.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - x        : the start x coordinate of the <Rect>
	// - y        : the start y coordinate of the <Rect>
	// - width    : the width of the <Rect>
	// - height   : the height of the <Rect>
	// - rect     : the <Rect> to perform the gradient rect
	// - color1   : the start <Color>
	// - color2   : the end <Color>
	// - angle    : the angle of the gradient rect. if false is send it will be
	//              the same as 0 and if true it will be the same as 90.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_gradient_fill_rect(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#clear
	//-------------------------------------------------------------------------------------
	// Parameters:
	// None.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_clear(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#clear_rect(x, y, width, height)
	// * Bitmap#clear_rect(rect)
	// Sets the bitmaps defined in the rect as Color#new(0, 0, 0, 0)
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - x      : the x coordinate to clear.
	// - y      : the y coordinate to clear.
	// - width  : the width of the rect to clear.
	// - height : the height of the rect to clear.
	// - rect   : the rect to clear.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_clear_rect(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#get_pixel(x, y)
	// Gets the color of the pixel at (x, y) returns Color#new(0, 0, 0, 0) if the
	// coordinates doesn't correspond to a valid position of the bitmap.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - x : the x coordinate to get the color
	// - y : the y coordinate to get the color
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_get_pixel(ruby_value self, ruby_value x, ruby_value y);
	//--------------------------------------------------------------------------------------
	// * Bitmap#set_pixel(x, y, color)
	// Set the pixel in the bitmap at (x, y) with the selected color.
	// Alpha blending is not performed.
	// transfer is ignored if the coordinates are not valid.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - x     : the x coordinate to set the color.
	// - y     : the y coordinate to set the color.
	// - color : the color to set.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_set_pixel(ruby_value self, ruby_value x, ruby_value y, ruby_value color);
	//--------------------------------------------------------------------------------------
	// * Bitmap#hue_change(hue)
	// Changes the hue of the bitmap. The process is time consuming and the original bitmap
	// is altered.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - hue : the hue to change the bitmap in the range 0...360.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_hue_change(ruby_value self, ruby_value hue);
	//--------------------------------------------------------------------------------------
	// * Bitmap#sat_change(sat)
	// Changes the saturation of the bitmap. The process is time consuming and the original
	// bitmap is altered.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - sat : the saturation to change the bitmap in the range 0...100.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_sat_change(ruby_value self, ruby_value sat);
	//--------------------------------------------------------------------------------------
	// * Bitmap#lum_change(lum)
	// Changes the luminance of the bitmap. The process is time consuming and the original
	// bitmap is altered.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - lum : the luminance to change the bitmap in the range 0...100.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_lum_change(ruby_value self, ruby_value lum);
	//--------------------------------------------------------------------------------------
	// * Bitmap#lhsl_change(hue, sat, lum)
	// Changes the hue, saturation and luminance of the bitmap. The process is time 
	// consuming and the original bitmap is altered.
	// Consumes less time than called hue_change, lum_change and sat_change separately.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - hue : the hue to change the bitmap in the range 0...360.
	// - sat : the saturation to change the bitmap in the range 0...100.
	// - lum : the luminance to change the bitmap in the range 0...100.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_hsl_change(ruby_value self, ruby_value hue, ruby_value sat, ruby_value lum);
	//--------------------------------------------------------------------------------------
	// * Bitmap#blur([size[, opacity]])
	// Performs a blur effect over the bitmap. This process is time consuming.
	// It alters the original bitmap.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// size    : The size of bluring distance. The deafault is 2. Bigger numbers are more
	//            time consuming.
	// opacity : the opacity of the blur effect from 0 to 255. Default is 50.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_blur(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#radial_blur(angle, division)
	// Performs a radial blur into the bitmap. This process is time consuming.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - angle    : the angle of divisions in the blur effect.
	// - division : the divisions of the blur effect.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_radial_blur(ruby_value self, ruby_value angle, ruby_value division);
	//--------------------------------------------------------------------------------------
	// * Bitmap#draw_text(x, y, width, height, text[, align])
	// * Bitmap#draw_text(rect, text[, align])
	// Draws text based on the font. 
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - x      : the x of the rect to draw the bitmap.
	// - y      : the y of the rect to draw the bitmap
	// - width  : the width of the rect to draw the bitmap.
	// - height : the height of the rect
	// - rect   : the <Rect> to draw the text if the text doesn't match the size it will be
	//            stretched.
	// - text   : the text to draw. if it's not an string, it will be converted via to_s
	// - align  : the align, 0 left, 1 center, 2 right
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_draw_text(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#gradient_draw_text(x, y, width, height, text[, align[, vertical]])
	// * Bitmap#gradient_draw_text(rect, text[, align[, vertical]])
	// Draws text based on the font. it will use the gradient colors.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - x        : the x of the rect to draw the bitmap.
	// - y        : the y of the rect to draw the bitmap
	// - width    : the width of the rect to draw the bitmap.
	// - height   : the height of the rect
	// - rect     : the <Rect> to draw the text if the text doesn't match the size it will 
	//              be stretched.
	// - text     : the text to draw. if it's not an string, it will be converted via to_s
	// - align    : the align, 0 left, 1 center, 2 right
	// - vertical : determines if the gradient should be vertical. default is false.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_gradient_draw_text(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#text_size(str)
	// Gets a rect with the size evaluated with the font calculating its size.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - str :
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_text_size(ruby_value self, ruby_value str);
	//--------------------------------------------------------------------------------------
	// * Bitmap#font
	// Gets the font of the bitmap.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// None.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_GETfont(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Bitmap#font=(value)
	// Sets the font of the bitmap.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - value : the new <Font> of the <Bitmap>
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_SETfont(ruby_value self, ruby_value value);
	//--------------------------------------------------------------------------------------
	// * Bitmap#to_bmp(filename)
	// Saves the bitmap as a bmp file.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - filename : the name of the filename to save. Extension may be omitted.
	//_____________________________________________________________________________________	
	ruby_value 
	rb_cBitmap_to_bmp(ruby_value self, ruby_value filename);	
	//======================================================================================
	static ruby_value rb_cBitmap;
	//======================================================================================	

	void
	Init_QGF_Graphics_Bitmap();

#endif
