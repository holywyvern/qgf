#ifndef QGF_KERNEL_H
#define QGF_KERNEL_H 1

#include "qgf/types.h"

//==============================================================================================
// ** Global variables
//----------------------------------------------------------------------------------------------
// The global variables defined in RGSS
//----------------------------------------------------------------------------------------------
	static ruby_value rb_gDEBUG;        // $DEBUG          # The debug flag used when debugging
	static ruby_value rb_gTEST;         // $TEST           # Same as $DEBUG
	static ruby_value rb_gBTEST;        // $BTEST          # Debug flag used when battle testing
	static ruby_value rb_gRGSS_SCRIPTS; // $RGSS_SCRIPTS   # Array with data of all the RGSS scripts
//==============================================================================================
// ** Global functions
//----------------------------------------------------------------------------------------------
// The built in global functions
//----------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	// * Kernel#rgss_main {...}
	// Evaluates the given block only one time.
	// It restarts the block when a RGSSReset is catched.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - {...} : block to evaluate.
	//_____________________________________________________________________________________
	ruby_value
	rb_mKernel_rgss_main(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Kernel#rgss_stop
	// Stops the given block in rgss_main. Stops all but Screen refreshing.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// None
	//_____________________________________________________________________________________	
	ruby_value
	rb_mKernel_rgss_stop(ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Kernel#load_data(filename)
	// Loads de data inside the File named filename.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - filename : the name of the file to load the data.
	//_____________________________________________________________________________________	
	ruby_value
	rb_mKernel_load_data(ruby_value self, ruby_value filename);
	//--------------------------------------------------------------------------------------
	// * Kernel#save_data(filename)
	// Saves the data inside the File named filename.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - filename : the name of the file to save the data. 
	//_____________________________________________________________________________________	
	ruby_value
	rb_mKernel_save_data(ruby_value self, ruby_value object, ruby_value filename);
	//--------------------------------------------------------------------------------------
	// * Kernel#msgbox(arg, [...])
	// Prints the objects inside a message window. Non String objects will be converted with
	// to_s.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - arg : First argument.
	// [...] : optional arguments
	//_____________________________________________________________________________________	
	ruby_value
	rb_mKernel_msgbox(int size, ruby_value* args, ruby_value self);
	//--------------------------------------------------------------------------------------
	// * Kernel#msgbox_p(arg, [...])
	// Prints the objects in human redeable format. I't will use object.inspect insthead of
	// .to_s to convert non String objects.
	//-------------------------------------------------------------------------------------
	// Parameters:
	// - arg : First argument.
	// [...] : optional arguments
	//_____________________________________________________________________________________	
	ruby_value
	rb_mKernel_msgbox_p(int size, ruby_value* args, ruby_value self);

	void
	Init_QGF_Kernel();

#endif
