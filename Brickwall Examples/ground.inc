/******************************************************************************
 * File: ground.inc
 * Description:
 *      FILL IN DESCRIPTION HERE
 ******************************************************************************/

#ifndef(Ground_Inc_Temp)

#declare Ground_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including ground.inc\n"
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
#include "libnoise.inc"
#include "libfn.inc"

// End Required Include Files
//=============================================================================

//=============================================================================
// Ground(SunDist,NSeed)
//
#declare Ground_sun_ratio   = 0.75;

//-----------------------------------------------------------------------------
// Ground_sphere(SunDist,NSeed)
//
#macro Ground_sphere(SunDist,NSeed)
    #local _sphere_r    = SunDist * Ground_sun_ratio;
    #local _rnoise_size = Math_Scale(SCALE_FEET, 10);
    #local _rnoise_scale    = <1,1,1>/Math_Scale(SCALE_FEET, 100);
    #local _rnoise_fn       = Noise_create2(<0,0,0>, f_snoise3d, 55973*<rand(NSeed),rand(NSeed),rand(NSeed)>, _rnoise_scale, _rnoise_size)
    
    #local _sphere_r_fn = function(x,y,z) {
        _sphere_r + _rnoise_fn(x,y,z)
    }    
    #local _sphere_fn   = function(x,y,z) {
        sqrt(x*x+y*y+z*z) - _sphere_r_fn(x,y,z)
    }
    
    #local _shape_fn    = Translate_fn(_sphere_fn, <0, -_sphere_r, 0>)
    
    #local _sphere_max_r    = _sphere_r + _rnoise_size;
    
    #local _ground_sphere   = isosurface {
        function {
            _shape_fn(x,y,z)
        }
        threshold 0
        accuracy 0.1
        max_gradient 2
        contained_by { sphere { <0,-_sphere_r, 0>, _sphere_max_r } }    
    }
    
    _ground_sphere
    
#end

// End Ground_sphere
//-----------------------------------------------------------------------------

#macro Ground(SunDist,NSeed)
    #local _ground  = union {
        object { Ground_sphere(SunDist,NSeed) }
    }
    
    _ground
#end
// Ground
//=============================================================================

#version Ground_Inc_Temp;
#end // ifndef(Ground_Inc_Temp)