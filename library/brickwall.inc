/******************************************************************************
 * File: brickwall.inc
 * Description:
 *      This is the wrapper header for the entire brickwall object library
 *      system.  
 ******************************************************************************/

#ifndef(Brickwall_Inc_Temp)

#declare Brickwall_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including brickwall.inc\n"
#end                     

//=============================================================================
// Required Include Files
//
#include "libfn.inc"
#include "brickwall_bricks.inc"
#include "brickwall_mortar.inc"

// End Required Include Files
//=============================================================================

//=============================================================================
// Brick Wall Utilities
//

//-----------------------------------------------------------------------------
// Brickwall_rotate_fn(F,R)
//
//      Returns a function(x,y,z,xoff,yoff,zoff) representing a shape
//      that is rotated around the X, Y, and Z axes as given in R in radians.
//
//      The absolute x,y, and z are not modified
//
#macro Brickwall_rotate_fn(F,R)
    #local _r   = <1,1,1>*R;
    #local _rx  = -_r.x;
    #local _ry  = -_r.y;
    #local _rz  = -_r.z;
    
    function(x,y,z,xoff,yoff,zoff) {
        F(x, y, z,
            f_rotate3d_x(xoff,yoff,zoff,_rx,_ry,_rz),
            f_rotate3d_y(xoff,yoff,zoff,_rx,_ry,_rz),
            f_rotate3d_z(xoff,yoff,zoff,_rx,_ry,_rz)
        )    
    }
#end                                              


// End Brickwall_rotate_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_rotate_fn2(F,R)
//
//      Returns a function(x,y,z,xoff,yoff,zoff) representing a shape
//      that is rotated around the X, Y, and Z axes as given in R in radians.
//
#macro Brickwall_rotate_fn2(F,R)
    #local _r   = <1,1,1>*R;
    #local _rx  = -_r.x;
    #local _ry  = -_r.y;
    #local _rz  = -_r.z;
    
    function(x,y,z,xoff,yoff,zoff) {
        F(
            f_rotate3d_x(x,y,z,_rx,_ry,_rz),
            f_rotate3d_y(x,y,z,_rx,_ry,_rz),
            f_rotate3d_z(x,y,z,_rx,_ry,_rz),
            f_rotate3d_x(xoff,yoff,zoff,_rx,_ry,_rz),
            f_rotate3d_y(xoff,yoff,zoff,_rx,_ry,_rz),
            f_rotate3d_z(xoff,yoff,zoff,_rx,_ry,_rz)
        )    
    }
#end                                              


// End Brickwall_rotate_fn2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_translate_fn(F,T)
//
//      Returns a function(x,y,z,xoff,yoff,zoff) representing a shape
//      that is translated to point T.
//
//      The absolute x,y, and z are not modified
//
#macro Brickwall_translate_fn(F,T)
    #local _t   = <1,1,1>*T;
    #local _tx  = _t.x;
    #local _ty  = _t.y;
    #local _tz  = _t.z;
    
    function(x,y,z,xoff,yoff,zoff) {
        F(x, y, z, xoff-_tx, yoff-_ty, zoff-_tz)
    }
#end                                              


// End Brickwall_translate_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_translate_fn2(F,T)
//
//      Returns a function(x,y,z,xoff,yoff,zoff) representing a shape
//      that is translated to point T.
//
#macro Brickwall_translate_fn2(F,T)
    #local _t   = <1,1,1>*T;
    #local _tx  = _t.x;
    #local _ty  = _t.y;
    #local _tz  = _t.z;
    
    function(x,y,z,xoff,yoff,zoff) {
        F(x-_tx, y-_ty, z-_tz, xoff-_tx, yoff-_ty, zoff-_tz)
    }
#end                                              


// End Brickwall_translate_fn2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_rotate_translate_fn(F,R,T)
//
#macro Brickwall_rotate_translate_fn(F,R,T)
    #local _r_fn    = Brickwall_rotate_fn(F,R)
    Brickwall_translate_fn(_r_fn,T)
#end

// End Brickwall_rotate_translate_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_rotate_translate_fn2(F,R,T)
//
#macro Brickwall_rotate_translate_fn2(F,R,T)
    #local _r_fn    = Brickwall_rotate_fn2(F,R)
    Brickwall_translate_fn2(_r_fn,T)
#end

// End Brickwall_rotate_translate_fn2
//-----------------------------------------------------------------------------

// End Brick Wall Utilities
//=============================================================================

//=============================================================================
// Brick Wall Specification
//
//      a Brick wall specification is a dictionary that contains specifications
//      for a brick wall including:
//
//      Length - the "x" dimension of the wall starting at the origin
//      Courses - the "y" dimension of the wall in whole brick units
//      Half Bricks - the "z" dimension of the wall in "half" brick units
//      Left/Right End types - whether the left and right ends are ragged or flush
//
#declare Brickwall_end_type_flush   = 0;
#declare Brickwall_end_type_corner  = 1;

//-----------------------------------------------------------------------------
// Brickwall_wall_spec_create(Length,Courses,HalfBricks)
// 
#macro Brickwall_wall_spec_create(Length,Courses,HalfBricks)
    #local _spec    = dictionary {
        .length:            Length,
        .courses:           Courses,
        .half_bricks:       HalfBricks,
        .left_end_type:     Brickwall_end_type_flush,
        .right_end_type:    Brickwall_end_type_flush
    }
    ;
    
    _spec
#end

// End Brickwall_wall_spec_create
//-----------------------------------------------------------------------------


// End Brick Wall Specification
//=============================================================================

//=============================================================================
// Brick Wall Layout
//
//      A Brick wall layout is a dictionary that contains specifications and
//      functions relevant to laying bricks out using a particular bonding
//      method.
//
//      Although individual layouts may have additional specs and functions,
//      all layouts must include the following:
//
//      .wall_spec:         The Brickwall_wall_spec defining the wall the layout applies to
//      .wall_size:         The basic extent of the wall, not taking any noise or jitter into account
//      .[x,y,z]_origin_fn: function(x,y,z) that returns the [x,y,z] element of
//                          the left, bottom, front corner of the nearest brick/mortar
//                          block to <x,y,z>
//      .[x,y,z]_index_fn:  function(x,y,z) that returns an integer brick/mortar block count
//                          offset from the left, bottom, front brick of the wall
//                          for the nearest brick/mortar block to <x,y,z>
//      .[x,y,z]_size_fn:   function(x,y,z) that returns the [x,y,z] dimension of
//                          brick within the brick/mortar block nearest to <x,y,z>
//
// Optional elements that may be added after the Brickwall_base_layout_create creates the dictionary:
//      .layout_jitter:     Brickwall_jitter_create() dictionary with jitter information for the layout
//
//
// Typically a specific layout will define macros in a set of 3:
//      Brickwall_[bondname]_layout_create(WallSpec,Bond-specific-parameters,BrickSpec[s],...)
//          - creates a Brickwall_base_layout with the required functions and extensions
//      Brickwall_[bondname]_bricks_shape_fn(Layout,BrickShape[s],...)
//          - function(x,y,z) that can be used in an isosurface to define the bricks
//      Brickwall_[bondname]_mortar_shape_fn(Layout,BrickShapeFn,MortarParameters,...)
//          - function(x,y,z) that can be used in an isosurface to define the mortar

//-----------------------------------------------------------------------------
// Brickwall_base_layout_create(WallSpec,WallSize,XOFN,YOFN,ZOFN,XIFN,YIFN,ZIFN,XSFN,YSFN,ZSFN)
//
#macro Brickwall_base_layout_create(WallSpec,WallSize,XOFN,YOFN,ZOFN,XIFN,YIFN,ZIFN,XSFN,YSFN,ZSFN)
    #local _layout  = dictionary {
        .wall_spec:     WallSpec,
        .wall_sz:       WallSize,
        .x_origin_fn:   XOFN,
        .y_origin_fn:   YOFN,
        .z_origin_fn:   ZOFN,
        .x_index_fn:    XIFN,
        .y_index_fn:    YIFN,
        .z_index_fn:    ZIFN,
        .x_size_fn:     XSFN,
        .y_size_fn:     YSFN,
        .z_size_fn:     ZSFN
    }
    ;
    
    _layout
#end 

// End Brickwall_base_layout_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_jitter_create(Layout,JitterSize,NoiseFn)
//
//      Creates a dictionary{} containing:
//          .jitter_size
//          .jitter_x_fn(x,y,z)
//          .jitter_y_fn(x,y,z)
//          .jitter_z_fn(x,y,z)
//
//  This dictionary can be used when calling Brickwall brick and mortar shape
//  functions to apply a random jitter to an entire brick.  It can also be
//  used when calling other functions that rely on the origin of a brick, 
//  such as the Brickwall_palette_selector_fn for assigning textures
//
//  Layout: a Brickwall layout dictionary with x_origin_fn, y_origin_fn, and z_origin_fn defined
//  JitterSize: The amount of jitter in the x, y, and z directions
//  NoiseFn:    function(x,y,z) that returns a value between -1 and 1, (such as f_snoise3d, and f_noise3d)
//
#macro Brickwall_jitter_create(Layout,JitterSize,NoiseFn)
    #local _jsize   = JitterSize*<1,1,1>;
    #local _jx      = _jsize.x;
    #local _jy      = _jsize.y;
    #local _jz      = _jsize.z;
    #local _jitter  = dictionary {
        .jitter_size:   _jsize,
        .jitter_x_fn:   function(x,y,z) {
            NoiseFn(
                0,
                Layout.y_origin_fn(x,y,z),
                Layout.z_origin_fn(x,y,z)
            )*_jx    
        },
        .jitter_y_fn:   function(x,y,z) {
            NoiseFn(
                Layout.x_origin_fn(x,y,z),
                0,
                Layout.z_origin_fn(x,y,z)
            )*_jy    
        }
        .jitter_z_fn:   function(x,y,z) {
            NoiseFn(
                Layout.x_origin_fn(x,y,z),
                Layout.y_origin_fn(x,y,z),
                0
            )*_jz    
        }
    }
    ;
    
    _jitter
#end

// End Brickwall_jitter_create
//-----------------------------------------------------------------------------

// End Brick Wall Layout
//=============================================================================

//=============================================================================
// Brickwall_palette_selector_fn(Layout,RandFn,Scale,PaletteFn)
//
// Creates a function(x,y,z) that returns the grayscale value from the given
// Palette pigment by randomly selecting a point within that Palette using
// RandFn(x,y,z) based on the origin of the brick nearest the given <x,y,z> 
// and the offset from that origin.
//
#macro Brickwall_palette_selector_fn(Layout,RandFn,Scale,PaletteFn)
    #local _scale       = Scale*<1,1,1>;
    #local _sx          = _scale.x;
    #local _sy          = _scale.y;
    #local _sz          = _scale.z;
    
    #local _bps_x_fn    = function(x,y,z,xo,yo,zo) {
        RandFn(
            xo,
            yo,
            zo
        )*_sx + (x - xo)    
    }
    #local _bps_y_fn    = function(x,y,z,xo,yo,zo) {
        RandFn(
            xo,
            yo,
            zo
        )*_sy + (y - yo)
    }
    #local _bps_z_fn    = function(x,y,z,xo,yo,zo) {
        RandFn(
            xo,
            yo,
            zo
        )*_sz + (z - zo)
    }
    
    #local _bps_shape_fn  = function(x,y,z,xo,yo,zo) {
        PaletteFn(
            _bps_x_fn(x,y,z,xo,yo,zo),
            _bps_y_fn(x,y,z,xo,yo,zo),
            _bps_z_fn(x,y,z,xo,yo,zo)
        )
    }
    #local _bps_aux_fn  = function(x,y,z) {
        _bps_shape_fn(x,y,z,Layout.x_origin_fn(x,y,z),Layout.y_origin_fn(x,y,z),Layout.z_origin_fn(x,y,z))
    }                
    
    function(x,y,z) {
        #ifdef(Layout.layout_jitter)
            _bps_aux_fn(
                x + Layout.layout_jitter.jitter_x_fn(x,y,z),
                y + Layout.layout_jitter.jitter_y_fn(x,y,z),
                z + Layout.layout_jitter.jitter_z_fn(x,y,z)
            )    
        #else
            _bps_aux_fn(x,y,z)
        #end
    }
#end

#macro Brickwall_default_palette_selector_fn(Layout,Scale,PaletteFn)
    Brickwall_palette_selector_fn(Layout,f_noise3d,Scale,PaletteFn)
#end

// End Brickwall_palette_selector_fn
//=============================================================================
 
#version Brickwall_Inc_Temp;
#end // ifndef(Brickwall_Inc_Temp)