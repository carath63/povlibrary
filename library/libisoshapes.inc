/******************************************************************************
 * File: libisoshapes.inc
 * Description:
 *      This library contains macros for defining functions useful for
 *      creating isosurface objects.
 ******************************************************************************/

#ifndef(Libisoshapes_Inc_Temp)

#declare Libisoshapes_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libisoshapes.inc\n"
#end

#include "functions.inc"                     

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

// End Required Include Files
//=============================================================================

//=============================================================================
// Iso_box()
//      returns function(x,y,z,sx,sy,sz)
//
// Iso_fixed_box(Size)
//      returns function(x,y,z)
//
// These macros generate functions that take a point <x,y,z> and compute the
// distance from that point to the surface of a box of a variable or fixed
// size.  If the point is inside the box, the distance is negative, if it is
// on the surface it returns 0, otherwise it returns a positive distance.
//

//-----------------------------------------------------------------------------
// Iso_box()
//
#macro Iso_box()
    function(x,y,z,sx,sy,sz) {
        select(abs(x) - sx,                                        
            select(abs(y) - sy,                                     // x is in
                select(abs(z) - sz,                                 // x is in, y is in
                    max(abs(x) - sx, abs(y) - sy, abs(z) - sz),     // x is in, y is in, z is in
                    abs(z) - sz                                     // x is in, y is in, z is out
                ),
                select(abs(z) - sz,                                 // x is in, y is out
                    abs(y) - sy,                                    // x is in, y is out, z is in
                    sqrt(pow(abs(z) - sz,2) + pow(abs(y) - sy,2))   // x is in, y is out, z is out
                )
            ),
            select(abs(y) - sy,                                     // x is out
                select(abs(z) - sz,                                 // x is out, y is in
                    abs(x) - sx,                                    // x is out, y is in, z is in
                    sqrt(pow(abs(x) - sx,2) + pow(abs(z) - sz,2))   // x is out, y is in, z is out
                ),
                select(abs(z) - sz,                                 // x is out, y is out
                    sqrt(pow(abs(x) - sx,2) + pow(abs(y) - sy,2)),  // x is out, y is out, z is in
                    sqrt(pow(abs(x) - sx,2) + pow(abs(y) - sy, 2) + pow(abs(z) - sz, 2)) // x is out, y is out, z is out
                )
            )
        )            
    }
#end

// End Iso_box
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_box(Size)
//
#macro Iso_fixed_box(Size)
    #local _sx  = Size.x/2;
    #local _sy  = Size.y/2;
    #local _sz  = Size.z/2;
    
    #local _ifb_box_fn  = Iso_box()
    
    function(x,y,z) {
        _ifb_box_fn(x,y,z,_sx,_sy,_sz)
    }    
        
#end

// End Iso_fixed_box
//-----------------------------------------------------------------------------

// End Iso_box(), Iso_fixed_box()
//=============================================================================

//=============================================================================
// Iso_cylinder()
//      returns function(x,y,z,r,sy)
//
// Iso_fixed_cylinder(Radius,Height)
//
// Cylinder shape functions for Y axis cylinders

//-----------------------------------------------------------------------------
// Iso_cylinder()
//
#macro Iso_cylinder()
    #local _ic_aux_fn   = function(x,y,z,r,sy,rd) {
        select(rd,
            max(rd, abs(y) - sy),
            select(abs(y) - sy,
                rd,
                sqrt(pow(rd,2) + pow(abs(y) - sy,2))
            )
        )        
    }
    
    function(x,y,z,r,sy) {
        _ic_aux_fn(x,y,z,r,sy,sqrt(x*x+z*z) - r)
    }
#end

// End Iso_cylinder
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_cylinder(Radius,Height)
//
#macro Iso_fixed_cylinder(Radius,Height)
    #local _sy  = Height/2;
    #local _ifc_cylinder_fn = Iso_cylinder()
    
    function(x,y,z) {
        _ifc_cylinder_fn(x,y,z,Radius,_sy)
    }
#end

// End Iso_fixed_cylinder
//-----------------------------------------------------------------------------

// End Iso_cylinder, Iso_fixed_cylinder
//=============================================================================

//=============================================================================
// Iso_rounded_box()
//      returns function(x,y,z,rnd,sx,sy,sz)
//
// Iso_fixed_rounded_box(Size,Rnd)
//      returns function(x,y,z)
//

//-----------------------------------------------------------------------------
// Iso_rounded_box()
//
#macro Iso_rounded_box()
    #local _irb_box_fn  = Iso_box()
    #local _irb_ycyl_fn = Iso_cylinder()
    #local _irb_xcyl_fn = function(x,y,z,r,sx) {
        _irb_ycyl_fn(y,x,z,r,sx)
    }
    #local _irb_zcyl_fn = function(x,y,z,r,sz) {
        _irb_ycyl_fn(x,z,y,r,sz)
    }
    
    function(x,y,z,rnd,sx,sy,sz) {
        min(
            _irb_box_fn(x,y,z,sx,sy-rnd,sz-rnd),
            _irb_box_fn(x,y,z-(sz-rnd/2),sx-rnd,sy-rnd,rnd/2),
            _irb_box_fn(x,y,z+(sz-rnd/2),sx-rnd,sy-rnd,rnd/2),
            _irb_box_fn(x,y-(sy-rnd/2),z,sx-rnd,rnd/2,sz-rnd),
            _irb_box_fn(x,y+(sy-rnd/2),z,sx-rnd,rnd/2,sz-rnd),
            _irb_ycyl_fn(x-(sx-rnd),y,z-(sz-rnd),rnd,sy-rnd),
            _irb_ycyl_fn(x+(sx-rnd),y,z-(sz-rnd),rnd,sy-rnd),
            _irb_ycyl_fn(x-(sx-rnd),y,z+(sz-rnd),rnd,sy-rnd),
            _irb_ycyl_fn(x+(sx-rnd),y,z+(sz-rnd),rnd,sy-rnd),
            _irb_xcyl_fn(x,y-(sy-rnd),z-(sz-rnd),rnd,sx-rnd),
            _irb_xcyl_fn(x,y+(sy-rnd),z-(sz-rnd),rnd,sx-rnd),
            _irb_xcyl_fn(x,y-(sy-rnd),z+(sz-rnd),rnd,sx-rnd),
            _irb_xcyl_fn(x,y+(sy-rnd),z+(sz-rnd),rnd,sx-rnd),
            _irb_zcyl_fn(x-(sx-rnd),y-(sy-rnd),z,rnd,sz-rnd),
            _irb_zcyl_fn(x+(sx-rnd),y-(sy-rnd),z,rnd,sz-rnd),
            _irb_zcyl_fn(x-(sx-rnd),y+(sy-rnd),z,rnd,sz-rnd),
            _irb_zcyl_fn(x+(sx-rnd),y+(sy-rnd),z,rnd,sz-rnd),
            f_sphere(x-(sx-rnd),y-(sy-rnd),z-(sz-rnd),rnd),
            f_sphere(x+(sx-rnd),y-(sy-rnd),z-(sz-rnd),rnd),
            f_sphere(x-(sx-rnd),y+(sy-rnd),z-(sz-rnd),rnd),
            f_sphere(x+(sx-rnd),y+(sy-rnd),z-(sz-rnd),rnd),
            f_sphere(x-(sx-rnd),y-(sy-rnd),z+(sz-rnd),rnd),
            f_sphere(x+(sx-rnd),y-(sy-rnd),z+(sz-rnd),rnd),
            f_sphere(x-(sx-rnd),y+(sy-rnd),z+(sz-rnd),rnd),
            f_sphere(x+(sx-rnd),y+(sy-rnd),z+(sz-rnd),rnd)
        )
    }
#end

// End Iso_rounded_box
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Iso_fixed_rounded_box(Size,Rnd)
//
#macro Iso_fixed_rounded_box(Size,Rnd)
    #local _ifrb_rb_fn  = Iso_rounded_box()
    #local _sx          = Size.x/2;
    #local _sy          = Size.y/2;
    #local _sz          = Size.z/2;
    
    function(x,y,z) {
        _ifrb_rb_fn(x,y,z,Rnd,_sx,_sy,_sz)
    }    
#end

// End Iso_fixed_rounded_box
//-----------------------------------------------------------------------------


// End Iso_rounded_Box, Iso_fixed_rounded_Box
//=============================================================================

#version Libisoshapes_Inc_Temp;
#end // ifndef(Libisoshapes_Inc_Temp)