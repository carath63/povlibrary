/******************************************************************************
 * File: libfn.inc
 * Description:
 *      Useful function and function transformation code
 ******************************************************************************/

#ifndef(Libfn_Inc_Temp)

#declare Libfn_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libfn.inc\n"
#end                     

//=============================================================================
// Required Include Files
//
// All files required for the library to work should be included here.  There
// should be no assumption that any given include file is included prior to
// the library.inc file.
//
// Further, to preserve this library as something that can be packaged up to
// export to news.povray.org, only include standard POV-Ray include files or
// other files in this library directory.
//
#include "functions.inc"

// End Required Include Files
//=============================================================================

//=============================================================================
// General Useful Functions
//

//-----------------------------------------------------------------------------
// f_unit_index(vl,unit)
//
// This function converts the given floating point vlue into an integer unit
// count from the origin for the given unit intervl.  Positive vl returns a
// unit index >= 0; negative vl returns a unit index < 0.
//
#declare f_unit_index   = function(vl,unit) {
    select(vl,
        (int(vl/unit)-1),
        int(vl/unit)
    )    
}

// End f_unit_index(vl,unit)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// f_unit_pin(vl,unit)
//
// This function returns the beginning value for the unit in which the given
// vlue lies.
//
#declare f_unit_pin = function(vl,unit) {
    select(vl,
        (int(vl/unit)-1),
        int(vl/unit)
    )*unit    
}

// End f_unit_pin
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// f_unit_offset(vl,unit)
//
// This function returns the offset of the given vlue within the unit in which
// it lies.
//
#declare f_unit_offset  = function(vl,unit) {
    vl - f_unit_pin(vl,unit)
}

// End f_unit_offset
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// f_unit_shift_offset(vl,unit,shift)
//
#declare f_unit_shift_offset    = function(vl,unit,shift) {
    vl - f_unit_pin(vl+shift,unit)
}    

// End f_unit_shift_offset
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Distance_fn_create(Point)
//
#macro Distance_fn_create(Point)
    #local _px  = Point.x;
    #local _py  = Point.y;
    #local _pz  = Point.z;
    
    function(x,y,z) {
        sqrt(pow(x-_px,2) + pow(y-_py,2) + pow(z-_pz,2))
    }
#end

// End Distance_fn_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// f_camera_distance(x,y,z)
//
#macro Camera_distance_fn_create()
    #ifndef(camera_location)
        #declare camera_location    = <0,0,0>;
    #end
    Distance_fn_create(camera_location)    
#end

// End f_camera_distance
//-----------------------------------------------------------------------------

// End General Useful Functions
//=============================================================================

//=============================================================================
// Geometric Transformation Functions
//

//-----------------------------------------------------------------------------
// f_rotate2d_x(x,y,th)
// f_rotate2d_y(x,y,th)
//
// Rotate the point <x,y> around the origin in 2D space and return the rotated
// X/Y vlue from the rotated point
//
#declare f_rotate2d_x   = function(x,y,th) {
    x*cos(th) - y*sin(th)
}
#declare f_rotate2d_y   = function(x,y,th) {
    y*cos(th) + x*sin(th)
}    

// End f_rotate2d_x, f_rotate2d_y
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// f_rotate3d_x(x,y,z,xth,yth,zth)
// f_rotate3d_y(x,y,z,xth,yth,zth)
// f_rotate3d_z(x,y,z,xth,yth,zth)
//
// Rotate the point <x,y,z> around the X axis by xth, then the y axis by yth,
// then the z axis by zth and return the rotated X/Y/Z vlue from the rotated point
//
#declare f_rotate3d_x   = function(x,y,z,xth,yth,zth) {
    f_rotate2d_x(       // Z axis
        f_rotate2d_x(   // Y axis
            x,
            f_rotate2d_x(
                z,
                y,
                -xth
            ),    
            -yth
        ),
        f_rotate2d_y(
            z,
            y,
            -xth
        ),    
        zth
    )    
}

#declare f_rotate3d_y   = function(x,y,z,xth,yth,zth) {
    f_rotate2d_y(
        f_rotate2d_x(
            x,
            f_rotate2d_x(
                z,
                y,
                -xth
            ),
            -yth
        ),
        f_rotate2d_y(
            z,
            y,
            -xth
        ),
        zth
    )                
}    

#declare f_rotate3d_z   = function(x,y,z,xth,yth,zth) {
    f_rotate2d_y(
        x,
        f_rotate2d_x(
            z,
            y,
            -xth
        ),
        -yth
    )        
}

// End f_rotate3d_x, f_rotate3d_y, f_rotate3d_z
//-----------------------------------------------------------------------------

// End Geometric Transformation Functions
//=============================================================================

//=============================================================================
// Trig functions
//

//-----------------------------------------------------------------------------
// f_normalized_atan2(x,y)
//
#declare f_normalized_atan2 = function(x,y) {
    select(x,
        2*pi + atan2(x,y),
        atan2(x,y)
    )    
}

// End f_normalized_atan2
//-----------------------------------------------------------------------------

// End Trig functions
//=============================================================================

//=============================================================================
// Function transformations
//

//-----------------------------------------------------------------------------
// Translate_fn(FN,T)
//
#macro Translate_fn(FN,T)
    #local _t   = <1,1,1>*T;
    #local _tx  = _t.x;
    #local _ty  = _t.y;
    #local _tz  = _t.z;
    
    function {
        FN(x-_tx,y-_ty,z-_tz)
    }    
#end

// End Translate_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scale_fn(FN,S)
//
#macro Scale_fn(FN,S)
    #local _s   = <1,1,1>*S;
    #local _sx  = _s.x;
    #local _sy  = _s.y;
    #local _sz  = _s.z;
    
    function {
        FN(x/_sx,y/_sy,z/_sz)
    }    
#end

// End Scale_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Rotate_fn(FN,R)
//
#macro Rotate_fn(FN,R)
    #local _r   = <1,1,1>*R;
    #local _rx  = -_r.x;
    #local _ry  = -_r.y;
    #local _rz  = -_r.z;
    
    function {
        FN(
            f_rotate3d_x(x,y,z,_rx,_ry,_rz),
            f_rotate3d_y(x,y,z,_rx,_ry,_rz),
            f_rotate3d_z(x,y,z,_rx,_ry,_rz)
        )
    }    
#end

// End Rotate_fn
//-----------------------------------------------------------------------------

// Function transformations
//=============================================================================

#version Libfn_Inc_Temp;
#end // ifndef(Libfn_Inc_Temp)