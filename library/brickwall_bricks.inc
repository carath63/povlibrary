/******************************************************************************
 * File: brickwall_bricks.inc
 * Description:
 *      This file contains declarations for defining individual bricks that
 *      are used to build a brick wall using this system.
 ******************************************************************************/

#ifndef(Brickwall_bricks_Inc_Temp)

#declare Brickwall_bricks_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including brickwall_bricks.inc\n"
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
#include "libscale.inc"
#include "libisoshapes.inc"

// End Required Include Files
//=============================================================================

//=============================================================================
// Brickwall_brick_spec
//
//      A Brickwall_brick_spec is a dictionary that contains the size of a
//      brick and the size of the mortar that goes with the brick.  When created
//      it automatically computes a value called the "unit_sz" that is the size
//      of the block including the brick, the mortar layer beneath it, to the
//      right of it, and behind it when viewed in its standard stretcher layout
//      from the front.
//

//-----------------------------------------------------------------------------
// Brickwall_brick_spec_create(BrickSize,MortarSize)
//
#macro Brickwall_brick_spec_create(BrickSize,MortarSize)
    #local _spec    = dictionary {
        .brick_sz:  BrickSize,
        .mortar_sz: MortarSize,
        .unit_sz:   BrickSize + MortarSize
    }
    ;
    
    _spec
#end

// End Brickwall_brick_spec_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// US Modular Brick Specs
//
#declare Brickwall_us_modular_spec              = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <7+5/8,2+1/4,3+5/8>), Math_Scale(SCALE_INCH, 3/8));
#declare Brickwall_us_closure1_modular_spec     = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <7+5/8,3+5/8,3+5/8>), Math_Scale(SCALE_INCH, 3/8));
#declare Brickwall_us_closure2_modular_spec     = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <7+1/2,3+1/2,3+1/2>), Math_Scale(SCALE_INCH, 1/2));
#declare Brickwall_us_engineer1_modular_spec    = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <7+5/8,2+13/16,3+5/8>), Math_Scale(SCALE_INCH, 3/8));
#declare Brickwall_us_engineer2_modular_spec    = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <7+1/2,2+3/4,3+1/2>), Math_Scale(SCALE_INCH, 1/2));
#declare Brickwall_us_jumbo1_modular_spec       = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <8, 2+3/4, 3+5/8>), Math_Scale(SCALE_INCH, 3/8));
#declare Brickwall_us_jumbo2_modular_spec       = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <8, 2+1/2, 3+1/2>), Math_Scale(SCALE_INCH, 1/2));
#declare Brickwall_us_roman1_modular_spec       = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <11+5/8, 1+5/8, 3+5/8>), Math_Scale(SCALE_INCH, 3/8));
#declare Brickwall_us_roman2_modular_spec       = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <11+1/2, 1+1/2, 3+1/2>), Math_Scale(SCALE_INCH, 1/2));
#declare Brickwall_us_norman1_modular_spec      = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <11+5/8, 2+1/4, 3+5/8>), Math_Scale(SCALE_INCH, 3/8));
#declare Brickwall_us_norman2_modular_spec      = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <11+1/2, 2+1/4, 3+1/2>), Math_Scale(SCALE_INCH, 1/2));
#declare Brickwall_us_engineer_norman1_modular_spec = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <11+5/8, 2+13/16, 3+5/8>), Math_Scale(SCALE_INCH, 3/8));
#declare Brickwall_us_engineer_norman2_modular_spec = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <11+1/2, 2+3/4, 3+1/2>), Math_Scale(SCALE_INCH, 1/2));
#declare Brickwall_us_utility1_modular_spec     = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <11+5/8, 3+5/8, 3+5/8>), Math_Scale(SCALE_INCH, 3/8));
#declare Brickwall_us_utility2_modular_spec     = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <11+1/2, 3+1/2, 3+1/2>), Math_Scale(SCALE_INCH, 1/2));
#declare Brickwall_us_meridian1_modular_spec    = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <15+5/8, 3+5/8, 3+5/8>), Math_Scale(SCALE_INCH, 3/8));
#declare Brickwall_us_meridian2_modular_spec    = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <15+1/2, 3+1/2, 3+1/2>), Math_Scale(SCALE_INCH, 1/2));
#declare Brickwall_us_double_meridian1_modular_spec = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <15+5/8, 7+5/8, 3+5/8>), Math_Scale(SCALE_INCH, 3/8));
#declare Brickwall_us_double_meridian2_modular_spec = Brickwall_brick_spec_create(Math_Scale(SCALE_INCH, <15+1/2, 7+1/2, 3+1/2>), Math_Scale(SCALE_INCH, 1/2));


// End US Modular Brick Specs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_brick_spec_cut_stretcher(BrickSpec,Length)
//
//      Creates a new Brickwall_brick_spec identical to the given one but
//      with an X length of Length instead of the original.
//
#macro Brickwall_brick_spec_cut_stretcher(BrickSpec,Length)
    Brickwall_brick_spec_create(<Length,BrickSpec.brick_sz.y,BrickSpec.brick_sz.z>,BrickSpec.mortar_sz)
#end

// End Brickwall_brick_spec_cut_stretcher
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_brick_spec_cut_header(BrickSpec,Length)
//
//      Creates a new Brickwall_brick_spec identical to the given one but
//      with an Z length of Length instead of the original.
//
#macro Brickwall_brick_spec_cut_header(BrickSpec,Length)
    Brickwall_brick_spec_create(<BrickSpec.brick_sz.x,BrickSpec.brick_sz.y,Length>,BrickSpec.mortar_sz)
#end

// End Brickwall_brick_spec_cut_header
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_brick_spec_rot_header(BrickSpec)
//
//      Creates a new Brickwall_brick_spec where the X dimension is now the
//      header size, and the Z dimension is the stretcher.  The Y dimension
//      is unchanged.
//
#macro Brickwall_brick_spec_rot_header(BrickSpec)
    Brickwall_brick_spec_create(<BrickSpec.brick_sz.z,BrickSpec.brick_sz.y,BrickSpec.brick_sz.z>,BrickSpec.mortar_sz)
#end

// End Brickwall_brick_spec_rot_header
//-----------------------------------------------------------------------------

// End Brickwall_brick_spec
//=============================================================================

//=============================================================================
// Brickwall_brick_shape
//
//      A Brickwall_brick_shape is a dictionary that contains the Brickwall_brick_spec
//      defining the size of the brick and its mortar, and a function(x,y,z,xoffset,yoffset,zoffset)
//      that should return the distance from <xoffset,yoffset,zoffset> to the surface
//      of the brick shape.  Negative distances are considered inside the brick,
//      positive outside of the brick, and 0 is on the surface itself.
//
//      The function will be called with the absolute x, y, and z coordinates of
//      a point within the wall boundaries, as well as the computed offsets from
//      the nearest brick instance for that point.
//
//      The offsets are computed such that <0,0,0> is the left, bottom, front
//      edge of the brick, and <BrickSpec.brick_sz.x,BrickSpec.brick_sz.y,BrickSpec.brick_sz.z>
//      is the right, top, back corner of the brick.  
//      
//      The function can use the
//      absolute <x,y,z> position to vary the parameters of the shape or add
//      texture to the surface that vary across the entire wall.
//

//-----------------------------------------------------------------------------
// Brickwall_brick_shape_create(BrickSpec,BrickFn)
//
#macro Brickwall_brick_shape_create(BrickSpec,BrickFn)
    #local _shape   = dictionary {
        .brick_spec:    BrickSpec,
        .brick_fn:      BrickFn
    }
    ;
    
    _shape
#end

// End Brickwall_brick_shape_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_brick_shape_cut_stretcher(BrickShape,Length,Side)
//
//      This is a utility function to take a BrickShape and create a new one
//      that has been cut along its stretcher side to the given Length from
//      the given side of the brick.  (Side 0 is left, Side 1 is right), leaving
//      the cut edge on the opposite side of the brick.
//
//      The resulting shape has a new BrickSpec cut to the new size, and
//      a new BrickFn that creates the cut.
//
#macro Brickwall_brick_shape_cut_stretcher(BrickShape,Length,Side)
    #local _cut_brick_spec  = Brickwall_brick_spec_cut_stretcher(BrickShape.brick_spec,Length);
    
    #local _bbscs_fn    = function(x,y,z,xoff,yoff,zoff) {
    #if (Side = 0)
        max(
            BrickShape.brick_fn(x,y,z,xoff,yoff,zoff),
            xoff - Length
        )    
    #else                   
        #local _xtrans  = Length - BrickShape.brick_spec.brick_sz.x;
        BrickShape.brick_fn(x,y,z,xoff-_xtrans,yoff,zoff)
    #end
    }
    
    Brickwall_brick_shape_create(_cut_brick_spec, _bbscs_fn)
    
#end

// End Brickwall_brick_shape_cut_stretcher
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_brick_shape_cut_header(BrickShape,Length,Side)
//
//      This is a utility function to take a BrickShape and create a new one
//      that has been cut along its header side to the given Length from
//      the given side of the brick.  (Side 0 is front, Side 1 is back), leaving
//      the cut edge on the opposite side of the brick.
//
//      The resulting shape has a new BrickSpec cut to the new size, and
//      a new BrickFn that creates the cut.
//
#macro Brickwall_brick_shape_cut_header(BrickShape,Length,Side)
    #local _cut_brick_spec  = Brickwall_brick_spec_cut_header(BrickShape.brick_spec,Length);
    
    #local _bbscs_fn    = function(x,y,z,xoff,yoff,zoff) {
    #if (Side = 0)
        max(
            BrickShape.brick_fn(x,y,z,xoff,yoff,zoff),
            zoff - Length
        )    
    #else                   
        #local _ztrans  = Length - BrickShape.brick_spec.brick_sz.z;
        BrickShape.brick_fn(x,y,z,xoff,yoff,zoff-_ztrans)
    #end
    }
    
    Brickwall_brick_shape_create(_cut_brick_spec, _bbscs_fn)
    
#end

// End Brickwall_brick_shape_cut_header
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_brick_shape_header(BrickShape)
//
//      This utility alters the shape function to work as if the brick were
//      rotated into its header position rather than its stretcher position.
//
#macro Brickwall_brick_shape_header(BrickShape)
    // Essentially map X to Z and Z to X, leaving Y alone
    //
    #local _maxz    = BrickShape.brick_spec.brick_sz.z;
    #local _bbsh_fn = function(x,y,z,xoff,yoff,zoff) {
        BrickShape.brick_fn(x,y,z,zoff,yoff,_maxz-xoff)
    }
    
    Brickwall_brick_shape_create(Brickwall_brick_spec_rot_header(BrickShape.brick_spec),_bbsh_fn) 
#end

// End Brickwall_brick_shape_header
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_brick_shape_add_noise(BrickShape,NoiseFn)
//
//      Creates a new Brickwall_brick_shape with the same brick specs, but
//      adds the result of calling NoiseFn(x,y,z) to the result of calling
//      the original brick_fn(x,y,z,xoff,yoff,zoff)
//
#macro Brickwall_brick_shape_add_noise(BrickShape,NoiseFn)
    #local _bbsan_fn    = function(x,y,z,xoff,yoff,zoff) {
        BrickShape.brick_fn(x,y,z,xoff,yoff,zoff) + NoiseFn(x,y,z)
    }                                                             
    
    Brickwall_brick_shape_create(BrickShape.brick_spec,_bbsan_fn)
#end

// End Brickwall_brick_shape_add_noise
//-----------------------------------------------------------------------------

// End Brickwall_brick_shape
//=============================================================================

//=============================================================================
// Brickwall_brick_shape Creation Utilities
//
//      These macros create Brickwall_brick_shape objects in some standard
//      shapes with various common customizations.
//

//-----------------------------------------------------------------------------
// Brickwall_fixed_square_brick_shape_create(BrickSpec)
//
#macro Brickwall_fixed_square_brick_shape_create(BrickSpec)
    #local _bfsbs_box_fn    = Iso_fixed_box(BrickSpec.brick_sz)
    
    #local _x_offset    = BrickSpec.brick_sz.x/2;
    #local _y_offset    = BrickSpec.brick_sz.y/2;
    #local _z_offset    = BrickSpec.brick_sz.z/2;
    
    #local _bfsbs_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
        _bfsbs_box_fn(xoff-_x_offset,yoff-_y_offset,zoff-_z_offset)
    }
    
    Brickwall_brick_shape_create(BrickSpec,_bfsbs_shape_fn)
#end

// End Brickwall_fixed_square_brick_shape_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_fixed_rounded_brick_shape_create(BrickSpec,Rounding)
//
#macro Brickwall_fixed_rounded_brick_shape_create(BrickSpec,Rounding)
    #local _bfrbs_box_fn = Iso_fixed_rounded_box(BrickSpec.brick_sz,Rounding)
    
    #local _x_offset    = BrickSpec.brick_sz.x/2;
    #local _y_offset    = BrickSpec.brick_sz.y/2;
    #local _z_offset    = BrickSpec.brick_sz.z/2;
    
    #local _bfrbs_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
        _bfrbs_box_fn(xoff-_x_offset,yoff-_y_offset,zoff-_z_offset)
    }
    
    Brickwall_brick_shape_create(BrickSpec,_bfrbs_shape_fn)
#end

// End Brickwall_fixed_rounded_brick_shape_create(BrickSpec,Rounding)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_variable_rounded_brick_shape_create(BrickSpec,RoundingFn)
//
#macro Brickwall_variable_rounded_brick_shape_create(BrickSpec,RoundingFn)
    #local _bvrbs_box_fn = Iso_rounded_box()
    
    #local _xs          = BrickSpec.brick_sz.x/2;
    #local _ys          = BrickSpec.brick_sz.y/2;
    #local _zs          = BrickSpec.brick_sz.z/2;
    
    #local _x_offset    = BrickSpec.brick_sz.x/2;
    #local _y_offset    = BrickSpec.brick_sz.y/2;
    #local _z_offset    = BrickSpec.brick_sz.z/2;
    
    #local _bvrbs_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
        _bvrbs_box_fn(xoff-_x_offset,yoff-_y_offset,zoff-_z_offset, RoundingFn(x,y,z), _xs, _ys, _zs)
    }
    
    Brickwall_brick_shape_create(BrickSpec,_bvrbs_shape_fn)
#end

// End Brickwall_variable_rounded_brick_shape_create(BrickSpec,Rounding)
//-----------------------------------------------------------------------------

// End Brickwall_brick_shape Creation Utilities
//=============================================================================

#version Brickwall_bricks_Inc_Temp;
#end // ifndef(Brickwall_bricks_Inc_Temp)