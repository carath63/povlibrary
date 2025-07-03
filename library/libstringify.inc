/******************************************************************************
 * File: libstringify.inc
 * Description:
 *      This library contains a number of macros for writing different types
 * of SDL objects to files in various ways so they can be read back in as .inc
 * files or as dictionaries.
 ******************************************************************************/

#ifndef(Libstringify_Inc_Temp)

#declare Libstringify_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libstringify.inc\n"
#end                     

//=============================================================================
// Includes
//

//-----------------------------------------------------------------------------
// Standard Includes
//

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//

// End My Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Pre-include Constants
//

// End Pre-include Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project Includes
//

// End Project Includes
//-----------------------------------------------------------------------------

// End Includes
//=============================================================================

//=============================================================================
// File Handling
//
//      Since there is no way to pass file handles to macros, the macros
//  that would write to a file need a global file handle identifier.  So,
//  the SFY_file_open and SFY_file_close macros will create/undefine the file
//  handle SFY_file_handle, which is used in all file writing macros.
//

#declare SFY_file_mode_read     = 0;
#declare SFY_file_mode_write    = 1;
#declare SFY_file_mode_append   = 2;

//-----------------------------------------------------------------------------
// SFY_file_open(FileName,ModeID)
//
#macro SFY_file_open(FileName,ModeID)
    #ifdef(SFY_file_handle)
        #error concat("SFY_file_open: SFY_file_handle is already open for file ", SFY_file_name, ". Close it before opening a new one\n")
    #else
        #declare SFY_file_name  = FileName;
        #switch(ModeID)
            #case(SFY_file_mode_read)
                #fopen SFY_file_handle FileName read
            #break
            #case(SFY_file_mode_write)              
                #fopen SFY_file_handle FileName write
            #break
            #case(SFY_file_mode_append)              
                #fopen SFY_file_handle FileName append
            #break
            #else
                #error concat("SFY_file_open: unknown file mode ", str(ModeID, 0, 0), "\n")
            #break
        #end
    #end    
#end

// End SFY_file_open
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SFY_file_close()
//
#macro SFY_file_close()
    #ifdef (SFY_file_handle)
        #fclose SFY_file_handle
        #undef SFY_file_name
    #end
#end

// End SFY_file_close
//-----------------------------------------------------------------------------

// End File Handling
//=============================================================================

//=============================================================================
// String Utilities
//

//-----------------------------------------------------------------------------
// SFY_indent(Count)
//
#macro SFY_indent(Count)
    #local _indent  = "";
    #for (i, 1, Count, 1)
        #local _indent  = concat(_indent, " ");
    #end
    
    _indent    
#end

// End SFY_indent
//-----------------------------------------------------------------------------


// End String Utilities
//=============================================================================

//=============================================================================
// Transform Representation
//
#declare SFY_transform_type_translate   = 0;
#declare SFY_transform_type_rotate      = 1;
#declare SFY_transform_type_scale       = 2;
#declare SFY_transform_type_matrix      = 3;

//-----------------------------------------------------------------------------
// SFY_transform_create()
//
#macro SFY_transform_create(optional FirstInstance)
    #local _transform   = dictionary {
        .num_transforms:    0,
        .transforms:    array
    }
    ;
    
    #ifdef(FirstInstance)
        SFY_transform_add(_transform, FirstInstance)
    #end    
    
    _transform
#end

// End SFY_transform_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SFY_transform_add(Transform,TInstance)
//
#macro SFY_transform_add(Transform,TInstance)
    #local Transform.transforms[Transform.num_transforms]   = TInstance;
    #local Transform.num_transforms                         = Transform.num_transforms + 1;
#end

// End SFY_transform_add
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SFY_transform_instance_create(Type,Vector)
// SFY_transform_matrix_create(Values)
//      Values is an array[12] with the elements of the matrix vector
//
#macro SFY_transform_instance_create(Type,Vector)
    #local _instance    = dictionary {
        .itype:     Type,
        .ivector:   Vector
    }
    ;
    
    _instance
#end

#macro SFY_transform_matrix_create(Values)
    #local _instance    = dictionary {
        .itype:     SFY_transform_type_matrix,
        .mvals:     Values
    };
    
    _instance    
#end

// End SFY_transform_instance_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SFY_transform_sdl(Transform)
//
//      This macro creates a transform{} object from the Transform dictionary{}
//
#macro SFY_transform_sdl(Transform)
    #local _transform   = transform {
        #for (i, 0, Transform.num_transforms-1, 1)
            #switch(Transform.transforms[i].itype)
                #case(SFY_transform_type_translate)
                    translate Transform.transforms[i].ivector
                #break
                #case(SFY_transform_type_rotate)
                    rotate Transform.transforms[i].ivector
                #break
                #case(SFY_transform_type_scale)
                    scale Transform.transforms[i].ivector
                #break
                #case(SFY_transform_type_matrix)
                    matrix <Transform.transforms[i].mvals[0],Transform.transforms[i].mvals[1],Transform.transforms[i].mvals[2],Transform.transforms[i].mvals[3],Transform.transforms[i].mvals[4],Transform.transforms[i].mvals[5],Transform.transforms[i].mvals[6],Transform.transforms[i].mvals[7],Transform.transforms[i].mvals[8],Transform.transforms[i].mvals[9],Transform.transforms[i].mvals[10],Transform.transforms[i].mvals[11]>
                #break
                #else
                    #error concat("SFY_transform_sdl: Unknown transform type ", str(Transform.itype, 0, 0), "\n")
                #break
            #end
        #end
    }
    
    _transform
#end

// End SFY_transform_sdl
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SFY_transform_sdl_write(Transform,optional indent)
//
#macro SFY_transform_sdl_write(Transform,optional indent)
    #local indent       = #ifdef(indent) indent; #else 0; #end
    #local _indent_str  = SFY_indent(indent);
    #local _indent2_str = SFY_indent(indent+4);
    
    #write (SFY_file_handle, _indent_str, "transform {\n")
    #for (i, 0, Transform.num_transforms-1, 1)
        #switch(Transform.transforms[i].itype)
            #case(SFY_transform_type_translate)
                #write(SFY_file_handle, _indent2_str, "translate ", Transform.transforms[i].ivector, "\n")
            #break
            #case(SFY_transform_type_rotate)
                #write(SFY_file_handle, _indent2_str, "rotate ", Transform.transforms[i].ivector, "\n")
            #break
            #case(SFY_transform_type_scale)
                #write(SFY_file_handle, _indent2_str, "scale ", Transform.transforms[i].ivector, "\n")
            #break
            #case(SFY_transform_type_matrix)
                #write(SFY_file_handle, _indent2_str, "matrix <")
                #for (j, 0, 10, 1)
                    #write(SFY_file_handle, Transform.transforms[i].mvals[j], ",")
                #end
                #write(SFY_file_handle, Transform.transforms[i].mvals[11], ">\n")
            #break
            #else
                #error concat("SFY_transform_sdl: Unknown transform type ", str(Transform.itype, 0, 0), "\n")
            #break
        #end
    #end
    #write (SFY_file_handle, _indent_str, "}\n")
#end

// End SFY_transform_sdl_string
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SFY_transform_dict_write(Transform,optional indent)
//
#macro SFY_transform_dict_write(Transform,optional indent)
    #local indent       = #ifdef(indent) indent; #else 0; #end
    #local _indent_str  = SFY_indent(indent);
    #local _indent2_str = SFY_indent(indent+4);
    #local _indent3_str = SFY_indent(indent+8);
    
    #write (SFY_file_handle, _indent_str, "dictionary {\n")
    #write (SFY_file_handle, _indent2_str, ".num_transforms: ", Transform.num_transforms, ",\n")
    #write (SFY_file_handle, _indent2_str, ".transforms: array[", Transform.num_transforms, "]")
    #if (Transform.num_transforms > 0)
        #write (SFY_file_handle, "{\n")
        #for (i, 0, Transform.num_transforms-1, 1)
            #write(SFY_file_handle, _indent3_str, "dictionary { .itype: ", Transform.transforms[i].itype, ", .ivector: ", Transform.transforms[i].ivector, "}")
            #if (i+1 < Transform.num_transforms)
                #write(SFY_file_handle, ",\n")
            #else
                #write(SFY_file_handle, "\n")
            #end        
        #end
        #write (SFY_file_handle, _indent2_str, "},\n")
    #else
        #write (SFY_file_handle, ",\n")
    #end
    #write (SFY_file_handle, _indent_str, "}\n")
    
#end

// End SFY_transform_dict_write
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SFY_transform_add_axis_rotate_trans(Transform,Axis,Angle)
//
#macro SFY_transform_add_axis_rotate_trans(Transform,Axis,Angle)
   #local vX = vaxis_rotate(x,Axis,Angle);
   #local vY = vaxis_rotate(y,Axis,Angle);
   #local vZ = vaxis_rotate(z,Axis,Angle);
   #local _mvalues  = array[12]{ vX.x,vX.y,vX.z, vY.x,vY.y,vY.z, vZ.x,vZ.y,vZ.z, 0,0,0 };
   SFY_transform_add(Transform, SFY_transform_matrix_create(_mvalues))
#end

// End SFY_transform_add_axis_rotate_trans
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SFY_transform_add_reorient_trans(Transform,Axis1,Axis2)
//
#macro SFY_transform_add_reorient_trans(Transform,Axis1,Axis2)
   #local vX1 = vnormalize(Axis1);
   #local vX2 = vnormalize(Axis2);
   #local Y = vcross(vX1, vX2);
   #if(vlength(Y) > 0)
      #local vY = vnormalize(Y);
      #local vZ1 = vnormalize(vcross(vX1, vY));
      #local vZ2 = vnormalize(vcross(vX2, vY));
      #local _mv1   = array[12] { vX1.x, vY.x,vZ1.x, vX1.y,vY.y,vZ1.y, vX1.z,vY.z, vZ1.z, 0,0,0 };
      #local _mv2   = array[12] { vX2.x,vX2.y,vX2.z,  vY.x,vY.y, vY.z, vZ2.x,vZ2.y,vZ2.z, 0,0,0 };
      SFY_transform_add(Transform, SFY_transform_matrix_create(_mv1))
      SFY_transform_add(Transform, SFY_transform_matrix_create(_mv2))
   #else
      #if (vlength(vX1-vX2)>0)
         #local vZ = VPerp_To_Vector(vX2);
         SFY_transform_add_axis_rotate_trans(Transform,vZ,180)
      #end
   #end
#end

// End SFY_transform_add_reorient_trans
//-----------------------------------------------------------------------------

// End Transform Representation
//=============================================================================

#version Libstringify_Inc_Temp;

#end // ifndef (Libstringify_Inc_Temp)
