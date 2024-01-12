/******************************************************************************
 * File: brickwall_stacked_layout.inc
 * Description:
 *      FILL IN DESCRIPTION HERE
 ******************************************************************************/

#ifndef(Brickwall_stacked_layout_Inc_Temp)

#declare Brickwall_stacked_layout_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including brickwall_stacked_layout.inc\n"
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
#include "brickwall.inc"

// End Required Include Files
//=============================================================================

//=============================================================================
// Brick Wall Stacked Layout
//
//      The stacked layout just lays bricks of the same size 
//      in vertical columns with no overlap.
//
//      If the length of the wall in the WallSpec does not align with a number
//      of full-length bricks, the Brickwall_stacked_layout_gap_loc values
//      determine where the off-sized bricks should be located:
//
//          _none:  ignore the extra space in the WallSpec and only place full bricks
//          _left:  Add a column of gap-sized bricks on the left end of the wall
//          _right: Add a column of gap-sized bricks on the right end of the wall
//          _both:  Add a column of 1/2 gap-sized bricks on the right and left end
//
#declare Brickwall_stacked_layout_gap_loc_none  = 0;
#declare Brickwall_stacked_layout_gap_loc_left  = 1;
#declare Brickwall_stacked_layout_gap_loc_right = 2;
#declare Brickwall_stacked_layout_gap_loc_both  = 3;

//-----------------------------------------------------------------------------
// Brickwall_stacked_layout_create(WallSpec,BrickSpec,GapLoc)
//
//      Creates a Brickwall_stacked_layout consisting of the required elements
//      of a Brickwall_base_layout plus:
//
//          .brick_spec:        BrickSpec
//          .gap_loc:           GapLoc
//          .full_min_x:        Starting location for full sized bricks
//          .full_max_x:        Ending location for full sized bricks
//          .left_brick_sz:     Length of the left brick if not full-sized, else 0
//          .right_brick_sz:    Length of the right brick if not full-sized, else 0
//
#macro Brickwall_stacked_layout_create(WallSpec,BrickSpec,GapLoc)
    // See how many full bricks fit within the length in the WallSpec and set up
    // any gap filling bricks in the requested location
    //
    #local _num_full_bricks     = int((WallSpec.length+BrickSpec.mortar_sz)/BrickSpec.unit_sz.x);
    #local _size_full_bricks    = _num_full_bricks * BrickSpec.unit_sz.x;
    
    #local _full_min_x          = 0;
    #local _full_max_x          = WallSpec.length;
    
    #local _left_brick_sz       = 0;
    #local _right_brick_sz      = 0;
    #if (_size_full_bricks < WallSpec.length)
        #switch(GapLoc)
            #case(Brickwall_stacked_layout_gap_loc_left)
                #local _gap = WallSpec.length - (_size_full_bricks - BrickSpec.mortar_sz);
                #local _left_brick_sz   = _gap - BrickSpec.mortar_sz;
                #if (_left_brick_sz <= 0)
                    #error concat("Brickwall_Stacked_layout_create: gap of ", str(_left_Brick_sz, 0, 6), " is smaller than mortar size.\n")
                #end
            #break
            
            #case(Brickwall_stacked_layout_gap_loc_right)
                #local _right_brick_sz  = WallSpec.length - _size_full_bricks;
            #break
            
            #case(Brickwall_stacked_layout_gap_loc_both)
                #local _gap = WallSpec.length - _size_full_bricks;
                #local _left_brick_sz   = (_gap - BrickSpec.mortar_sz)/2;
                #local _right_brick_sz  = _gap - (_left_brick_sz + BrickSpec.mortar_sz);
            #break
            #else
                #error concat("Brickwall_stacked_layout_create: Invalid GapLoc ", str(GapLoc, 0, 0), "\n")
            #break    
        #end
    #end
    
    #if (_left_brick_sz > 0)
        #local _full_min_x  = _left_brick_sz + BrickSpec.mortar_sz;
    #else
        #local _full_min_x  = 0;
    #end
    #local _full_max_x      = min(_full_min_x + _size_full_bricks,WallSpec.length);
    
    // Define the layout functions    
    #local _ux  = BrickSpec.unit_sz.x;
    #local _uy  = BrickSpec.unit_sz.y;
    #local _uz  = BrickSpec.unit_sz.z;
    
    #local _bx  = BrickSpec.brick_sz.x;
    #local _by  = BrickSpec.brick_sz.y;
    #local _bz  = BrickSpec.brick_sz.z;
    
    #local _y_origin_fn = function(x,y,z) {
        f_unit_pin(y,_uy)
    }
    #local _z_origin_fn = function(x,y,z) {
        f_unit_pin(z,_uz)
    }
    
    #local _y_index_fn  = function(x,y,z) {
        f_unit_index(y,_uy)
    }
    #local _z_index_fn  = function(x,y,z) {
        f_unit_index(z,_uz)
    }
    
    #local _y_size_fn   = function(x,y,z) {
        _by
    }
    #local _z_size_fn   = function(x,y,z) {
        _bz
    }
    
    #if (_left_brick_sz > 0)
        #if (_right_brick_sz > 0)
            #local _x_origin_fn = function(x,y,z) {
                select(x - _full_min_x,
                    0,
                    select(x - _full_max_x,
                        _full_min_x + f_unit_pin(x - _full_min_x,_ux),
                        _full_max_x
                    )
                )        
            }
            #local _x_index_fn  = function(x,y,z) {
                select(x - _full_min_x,
                    0,
                    1 + f_unit_index(x - _full_min_x, _ux)
                )
            }
            #local _x_size_fn   = function(x,y,z) {
                select(x - _full_min_x,
                    _left_brick_sz,
                    select(x - _full_max_x,
                        _bx,
                        _right_brick_sz
                    )
                )
            }                    
        #else
            #local _x_origin_fn = function(x,y,z) {
                select(x - _full_min_x,
                    0,
                    _full_min_x + f_unit_pin(x - _full_min_x, _ux)
                )
            }
            #local _x_index_fn  = function(x,y,z) {
                select(x - _full_min_x,
                    0,
                    1 + f_unit_index(x - _full_min_x,_ux)
                )
            }
            #local _x_size_fn   = function(x,y,z) {
                select(x - _full_min_x,
                    _left_brick_sz,
                    _bx
                )
            }                        
        #end
    #else
        #if (_right_brick_sz > 0)
            #local _x_origin_fn = function(x,y,z) {
                select(x - _full_max_x,
                    f_unit_pin(x,_ux),
                    _full_max_x
                )
            }
            #local _x_index_fn  = function(x,y,z) {
                f_unit_index(x,_ux)
            }
            #local _x_size_fn   = function(x,y,z) {
                select(x - _full_max_x,
                    _bx,
                    _right_brick_sz
                )
            }            
        #else
            #local _x_origin_fn = function(x,y,z) {
                f_unit_pin(x,_ux)
            }
            #local _x_index_fn  = function(x,y,z) {
                f_unit_index(x,_ux)
            }    
            #local _x_size_fn   = function(x,y,z) {
                _bx
            }            
        #end
    #end
    
    #local _stacked_layout  = Brickwall_base_layout_create(
        WallSpec,
        _x_origin_fn,
        _y_origin_fn,
        _z_origin_fn,
        _x_index_fn,
        _y_index_fn,
        _z_index_fn,
        _x_size_fn,
        _y_size_fn,
        _z_size_fn
    );
    
    #local _stacked_layout.brick_spec       = BrickSpec;
    #local _stacked_layout.gap_loc          = GapLoc;
    #local _stacked_layout.left_brick_sz    = _left_brick_sz;
    #local _stacked_layout.right_brick_sz   = _right_brick_sz;
    #local _stacked_layout.full_min_x       = _full_min_x;
    #local _stacked_layout.full_max_x       = _full_max_x;
    #local _stacked_layout.max_x            = _full_max_x + _right_brick_sz;
    
    _stacked_layout
#end

// End Brickwall_stacked_layout_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_stacked_bricks_shape_fn(Layout,BrickShape)
//
#macro Brickwall_stacked_bricks_shape_fn(Layout,BrickShape)

    // Cut the brick shape if needed for right and left end short bricks
    //
    #local _mortar_y_offset = Layout.brick_spec.mortar_sz;
    #if (Layout.left_brick_sz > 0)
        #local _left_brick_shape    = Brickwall_brick_shape_cut_stretcher(BrickShape,Layout.left_brick_sz,0);
        #if (Layout.right_brick_sz > 0)
            #local _right_brick_shape   = Brickwall_brick_shape_cut_stretcher(BrickShape,Layout.right_brick_sz,1);
            #local _bsbs_shape_fn   = function(x,y,z) {
                select(x - Layout.full_min_x,
                    _left_brick_shape.brick_fn(x,y,z,x-Layout.x_origin_fn(x,y,z),y-Layout.y_origin_fn(x,y,z)-_mortar_y_offset,z-Layout.z_origin_fn(x,y,z)),
                    select(x - Layout.full_max_x,
                        BrickShape.brick_fn(x,y,z,x-Layout.x_origin_fn(x,y,z),y-Layout.y_origin_fn(x,y,z)-_mortar_y_offset,z-Layout.z_origin_fn(x,y,z)),
                        _right_brick_shape.brick_fn(x,y,z,x-Layout.x_origin_fn(x,y,z),y-Layout.y_origin_fn(x,y,z)-_mortar_y_offset,z-Layout.z_origin_fn(x,y,z))
                    )
                )        
            }
        #else
            #local _bsbs_shape_fn   = function(x,y,z) {
                select(x - Layout.full_min_x,
                    _left_brick_shape.brick_fn(x,y,z,x-Layout.x_origin_fn(x,y,z),y-Layout.y_origin_fn(x,y,z)-_mortar_y_offset,z-Layout.z_origin_fn(x,y,z)),
                    BrickShape.brick_fn(x,y,z,x-Layout.x_origin_fn(x,y,z),y-Layout.y_origin_fn(x,y,z)-_mortar_y_offset,z-Layout.z_origin_fn(x,y,z))
                )
            }        
        #end
    #else
        #if (Layout.right_brick_sz > 0)
            #local _right_brick_shape   = Brickwall_brick_shape_cut_stretcher(BrickShape,Layout.right_brick_sz,1);
            #local _bsbs_shape_fn   = function(x,y,z) {
                select(x - Layout.full_max_x,
                    BrickShape.brick_fn(x,y,z,x-Layout.x_origin_fn(x,y,z),y-Layout.y_origin_fn(x,y,z)-_mortar_y_offset,z-Layout.z_origin_fn(x,y,z)),
                    _right_brick_shape.brick_fn(x,y,z,x-Layout.x_origin_fn(x,y,z),y-Layout.y_origin_fn(x,y,z)-_mortar_y_offset,z-Layout.z_origin_fn(x,y,z))
                )    
            }
        #else
            #local _bsbs_shape_fn   = function(x,y,z) {
                BrickShape.brick_fn(x,y,z,x-Layout.x_origin_fn(x,y,z),y-Layout.y_origin_fn(x,y,z)-_mortar_y_offset,z-Layout.z_origin_fn(x,y,z))
            }
        #end            
    #end

    function(x,y,z) {
        max(
            _bsbs_shape_fn(x,y,z),
            x - Layout.max_x
        )    
    }
#end

// End Brickwall_stacked_bricks_shape_fn
//-----------------------------------------------------------------------------

// End Brick Wall Stacked Layout
//=============================================================================

#version Brickwall_stacked_layout_Inc_Temp;
#end // ifndef(Brickwall_stacked_layout_Inc_Temp)