/******************************************************************************
 * File: brickwall_english_layout.inc
 * Description:
 *      FILL IN DESCRIPTION HERE
 ******************************************************************************/

#ifndef(Brickwall_english_layout_Inc_Temp)

#declare Brickwall_english_layout_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including brickwall_english_layout.inc\n"
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
// Brick Wall English Layout
//
//      The English layout alternates courses of stretcher-oriented bricks
//      with courses of header-oriented bricks, useful in bonding 2-half-brick
//      and larger walls together.  This implementation supports 1-half-brick,
//      where the header bricks are cut to a half-brick length, 2-half-brick
//      where the stretcher layers are doubled and the header layer bonds
//      the two together, and 3 half-brick where the middle layer is all
//      headers and the front and back alternate, opposite.
//
//      The English layout does allow for creating corners for all three
//      half-brick sizes.
//

//-----------------------------------------------------------------------------
// Brickwall_english_layout_create(WallSpec,BrickSpec,Parity)
//
#macro Brickwall_english_layout_create(WallSpec,BrickSpec,Parity)
    // I have designs for 3 and 4 half-brick walls, but the corners are more complicated
    // and will need some development work...
    //
    #if (WallSpec.half_bricks > 2)
        #error concat("Brickwall_english_layout_create: Max half-brick size supported is 2\n")
    #end
        
    #local _bx          = BrickSpec.brick_sz.x;
    #local _by          = BrickSpec.brick_sz.y;
    #local _bz          = BrickSpec.brick_sz.z;
    #local _ux          = BrickSpec.unit_sz.x;
    #local _uy          = BrickSpec.unit_sz.y;
    #local _uz          = BrickSpec.unit_sz.z;
    
    #local _even_full_bricks    = int((WallSpec.length+BrickSpec.mortar_sz)/BrickSpec.unit_sz.x);
    #local _even_full_size      = _even_full_bricks * BrickSpec.unit_sz.x;
    #local _even_gap            = WallSpec.length - _even_full_size;
    
    #local _layout_length   = _even_full_size - BrickSpec.mortar_sz;
    #if (_even_gap > BrickSpec.mortar_sz)
        #warning concat("Brickwall_english_layout_create: Gap in the stretcher row of ", str(_even_gap, 0, 6), " not allowed.  Resizing the wall to ", str(_layout_length, 0, 6), "\n")
    #end
    
    #local _odd_left_space          = (BrickSpec.brick_sz.x + BrickSpec.mortar_sz/2) - BrickSpec.brick_sz.z/2;
    #local _odd_left_closer_space   = _odd_left_space - BrickSpec.unit_sz.z;
    #local _odd_left_closer_sz      = _odd_left_closer_space - BrickSpec.mortar_sz; 
    #local _odd_left_closer_max_x   = BrickSpec.unit_sz.z + _odd_left_closer_space;
    #if (WallSpec.left_end_type = Brickwall_end_type_corner)
        #local _odd_min_x               = BrickSpec.unit_sz.z;
        #local _odd_left_brick_index    = -1;
    #else
        #local _odd_min_x   = 0;
        #local _odd_left_brick_index    = 0;
    #end     
    #local _odd_right_space         = (BrickSpec.brick_sz.x + BrickSpec.mortar_sz/2) - (BrickSpec.brick_sz.z/2 + BrickSpec.mortar_sz);
    #local _odd_right_closer_space  = _odd_left_space - BrickSpec.unit_sz.z;
    #local _odd_right_closer_sz     = _odd_left_closer_space - BrickSpec.mortar_sz;
    #local _odd_right_closer_min_x  = _layout_length - (BrickSpec.brick_sz.z + _odd_right_closer_space);
    #local _odd_right_closer_max_x  = _odd_right_closer_min_x + _odd_right_closer_space;
    #if (WallSpec.right_end_type = Brickwall_end_type_corner)
        #local _odd_max_x   = _layout_length - BrickSpec.unit_sz.z;
    #else
        #local _odd_max_x   = _layout_length;
    #end
    #local _odd_middle_bricks   = int((_odd_right_closer_min_x - _odd_left_closer_max_x)/BrickSpec.unit_sz.z);
    
    // Corners are a little trickier in the English layout and depend on the half-brick size of
    // the wall
    //
    #if (WallSpec.left_end_type = Brickwall_end_type_corner)
        #switch(WallSpec.half_bricks)
            #case(1)
                #local _even_min_x_fn           = function(x,y,z) { 0 }
                #local _even_left_brick_index   = 0;
                #local _odd_min_x_fn            = function(x,y,z) { _uz }
                #local _odd_left_brick_index    = -1; 
            #break
            #case(2)
                #local _even_min_x_fn           = function(x,y,z) { _ux }
                #local _even_left_brick_index   = -1;
                #local _odd_min_x_fn            = function(x,y,z) { 0 }
                #local _odd_left_brick_index    = 0;
            #break
        #end
    #else
        #local _even_min_x_fn           = function(x,y,z) { 0 }
        #local _even_left_brick_index   = 0;
        #local _odd_min_x_fn            = function(x,y,z) { 0 }
        #local _odd_left_brick_index    = 0;
    #end        
    
    #if (WallSpec.right_end_type = Brickwall_end_type_corner)
        #switch(WallSpec.half_bricks)
            #case(1)
                #local _even_max_x_fn           = function(x,y,z) { _layout_length }
                #local _odd_max_x_fn            = function(x,y,z) { _layout_length - _uz }
            #break
            #case(2)
                #local _even_max_x_fn           = function(x,y,z) { _layout_length - _ux }
                #local _odd_max_x_fn            = function(x,y,z) { _layout_length }
            #break
        #end
    #else
        #local _even_max_x_fn           = function(x,y,z) { _layout_length }
        #local _odd_max_x_fn            = function(x,y,z) { _layout_length }
    #end        
    
    #local _y_origin_fn = function(x,y,z) {
        f_unit_pin(y,_uy)
    }
    #local _y_index_fn  = function(x,y,z) {
        f_unit_index(y,_uy)
    }
    #local _y_size_fn   = function(x,y,z) {
        _by
    }
    
    #switch(WallSpec.half_bricks)
        #case(1)
            #local _z_origin_fn = function(x,y,z) {
                0
            }
            #local _z_index_fn  = function(x,y,z) {
                0
            }
            #local _z_size_fn   = function(x,y,z) {
                _bz
            }
        #break
        #case(2)
            #local _z_origin_fn = function(x,y,z) {
                select(mod(_y_index_fn(x,y,z)+Parity,2) - 0.5,
                    f_unit_pin(z,_uz),
                    0
                )    
            }
            #local _z_index_fn  = function(x,y,z) {
                select(mod(_y_index_fn(x,y,z)+Parity,2) - 0.5,
                    f_unit_index(z,_uz),
                    0
                )
            }
            #local _z_size_fn   = function(x,y,z) {
                select(mod(_y_index_fn(x,y,z)+Parity,2) - 0.5,
                    _bz,
                    _bx
                )    
            }        
        #break
        #case(3)
            #local _z_origin_fn = function(x,y,z) {
                select(mod(_y_index_fn(x,y,z)+Parity,2) - 0.5,
                    select(z - _uz,
                        0,
                        _uz
                    ),
                    select(z - _ux,
                        0,
                        _ux
                    )
                )            
            }
            #local _z_index_fn  = function(x,y,z) {
                select(mod(_y_index_fn(x,y,z)+Parity,2) - 0.5,
                    select(z - _uz,
                        0,
                        1
                    ),
                    select(z - _ux,
                        0,
                        1
                    )
                )
            }
            #local _z_size_fn   = function(x,y,z) {
                select(mod(_y_index_fn(x,y,z)+Parity,2) - 0.5,
                    select(z - _uz,
                        _bz,
                        _bx
                    ),
                    select(z - _ux,
                        _bx,
                        _bz
                    )
                )
            }                              
        #break
        #else
            #error concat("Brickwall_english_layout_create: Illegal half-brick wall size ", str(WallSpec.half_bricks, 0, 0), "\n")
        #break    
    #end
    
    #local _even_x_origin_fn    = function(x,y,z,mnx,mxx) {
        select(x - mnx,
            0,
            mnx + f_unit_pin(x - mnx,_ux)
        )    
    }
    #local _even_x_index_fn     = function(x,y,z,mnx,mxx) {
        select(x - mnx,
            -1,
            select(mxx - x,
                -1,
                f_unit_index(x-mnx,_ux)
            )    
        )    
    }
    #local _even_x_size_fn      = function(x,y,z,mnx,mxx) {
        /*
        select(x - mnx,
            0,
            select(mxx - x,
                0,
                _bx
            )    
        ) 
        */
        _bx   
    }
    
    #local _odd_x_origin_fn     = function(x,y,z,mnx,mxx) {
        select(x - mnx,
            0,
            select(x - _uz,
                0,
                select(x - _odd_left_closer_max_x,
                    _uz,
                    select(x - _odd_right_closer_min_x,
                        _odd_left_closer_max_x + f_unit_pin(x - _odd_left_closer_max_x, _uz),
                        select(x - _odd_right_closer_max_x,
                            _odd_right_closer_min_x,
                            _odd_right_closer_max_x
                        )
                    )
                )
            )
        )
    }                        
    
    #local _odd_x_index_fn     = function(x,y,z,mnx,mxx) {
        select(x - mnx,
            -1,
            select(x - _uz,
                _odd_left_brick_index,
                select(x - _odd_left_closer_max_x,
                    _odd_left_brick_index + 1,
                    select(x - _odd_right_closer_min_x,
                        _odd_left_brick_index + 2 + f_unit_index(x - _odd_left_closer_max_x, _uz),
                        select(x - _odd_right_closer_max_x,
                            _odd_left_brick_index + 2 + _odd_middle_bricks + 1,
                            select(x - mxx,
                                _odd_left_brick_index + 2 + _odd_middle_bricks + 2,
                                -1
                            )    
                        )
                    )
                )
            )
        )
    }                        
    
    #local _odd_x_size_fn     = function(x,y,z,mnx,mxx) {
        select(x - mnx,
            0,
            select(x - _uz,
                _bz,
                select(x - _odd_left_closer_max_x,
                    _odd_left_closer_sz,
                    select(x - _odd_right_closer_min_x,
                        _bz,
                        select(x - _odd_right_closer_max_x,
                            _odd_right_closer_sz,
                            select(x - mxx,
                                _bz,
                                0
                            )    
                        )
                    )
                )
            )
        )
    }                        
    
    #local _x_origin_fn = function(x,y,z) {
        select(mod(_y_index_fn(x,y,z)+_z_index_fn(x,y,z)+Parity,2) - 0.5,
            _even_x_origin_fn(x,y,z,_even_min_x_fn(x,y,z),_even_max_x_fn(x,y,z)),
            _odd_x_origin_fn(x,y,z,_odd_min_x_fn(x,y,z),_odd_max_x_fn(x,y,z))
        )    
    }
    #local _x_index_fn  = function(x,y,z) {
        select(mod(_y_index_fn(x,y,z)+_z_index_fn(x,y,z)+Parity,2) - 0.5,
            _even_x_index_fn(x,y,z,_even_min_x_fn(x,y,z),_even_max_x_fn(x,y,z)),
            _odd_x_index_fn(x,y,z,_odd_min_x_fn(x,y,z),_odd_max_x_fn(x,y,z))
        )    
    }
    #local _x_size_fn  = function(x,y,z) {
        select(mod(_y_index_fn(x,y,z)+_z_index_fn(x,y,z)+Parity,2) - 0.5,
            _even_x_size_fn(x,y,z,_even_min_x_fn(x,y,z),_even_max_x_fn(x,y,z)),
            _odd_x_size_fn(x,y,z,_odd_min_x_fn(x,y,z),_odd_max_x_fn(x,y,z))
        )    
    }
    
    #local _wall_size       = <_layout_length, WallSpec.courses*BrickSpec.unit_sz.y, WallSpec.half_bricks*BrickSpec.unit_sz.z - BrickSpec.mortar_sz>;
    
    #local _english_layout  = Brickwall_base_layout_create(
        WallSpec,
        _wall_size,
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
    
    #local _english_layout.layout_length    = _layout_length;
    #local _english_layout.brick_spec       = BrickSpec;
    #local _english_layout.parity           = Parity;
    #local _english_layout.even_min_x_fn    = _even_min_x_fn;
    #local _english_layout.even_max_x_fn    = _even_max_x_fn;
    #local _english_layout.odd_min_x_fn     = _odd_min_x_fn;
    #local _english_layout.odd_max_x_fn     = _odd_max_x_fn;
    #local _english_layout.odd_left_closer_sz   = _odd_left_closer_sz;
    #local _english_layout.odd_left_closer_max_x    = _odd_left_closer_max_x;
    #local _english_layout.odd_middle_bricks        = _odd_middle_bricks;
    #local _english_layout.odd_right_closer_sz  = _odd_right_closer_sz;
    #local _english_layout.odd_right_closer_min_x   = _odd_right_closer_min_x;
    #local _english_layout.odd_right_closer_max_x   = _odd_right_closer_max_x;
    
    _english_layout
#end

// End Supporting Library Component
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_english_bricks_shape_fn(Layout,BrickShape)
//
#macro Brickwall_english_bricks_shape_fn(Layout,BrickShape)
    #if (Layout.wall_spec.half_bricks = 1)
        #local _bebs_temp_header_shape  = Brickwall_brick_shape_cut_stretcher(BrickShape,BrickShape.brick_spec.brick_sz.z,0);
    #else
        #local _bebs_temp_header_shape  = BrickShape;
    #end
    #local _bebs_temp_left_closer_shape     = Brickwall_brick_shape_cut_header(_bebs_temp_header_shape,Layout.odd_left_closer_sz,0);
    #local _bebs_temp_right_closer_shape    = Brickwall_brick_shape_cut_header(_bebs_temp_header_shape,Layout.odd_right_closer_sz,0);
    
    #local _bebs_header_shape       = Brickwall_brick_shape_header(_bebs_temp_header_shape);
    #local _bebs_left_closer_shape  = Brickwall_brick_shape_header(_bebs_temp_left_closer_shape);
    #local _bebs_right_closer_shape = Brickwall_brick_shape_header(_bebs_temp_right_closer_shape);
    
    #local _bebs_even_shape_fn  = function(x,y,z,xoff,yoff,zoff) {
        max(
            Layout.even_min_x_fn(x,y,z) - x,
            x - Layout.even_max_x_fn(x,y,z),
            BrickShape.brick_fn(x,y,z,xoff,yoff,zoff)
        )    
    }
    
    #local _ux  = BrickShape.brick_spec.unit_sz.x;
    #local _uz  = BrickShape.brick_spec.unit_sz.z;
    
    #local _bebs_odd_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
        max(
            Layout.odd_min_x_fn(x,y,z) - x,
            x - Layout.odd_max_x_fn(x,y,z),
            select(x - _uz,
                _bebs_header_shape.brick_fn(x,y,z,xoff,yoff,zoff),
                select(x - Layout.odd_left_closer_max_x,
                    _bebs_left_closer_shape.brick_fn(x,y,z,xoff,yoff,zoff),
                    select(x - Layout.odd_right_closer_min_x,
                        _bebs_header_shape.brick_fn(x,y,z,xoff,yoff,zoff),
                        select(x - Layout.odd_right_closer_max_x,
                            _bebs_right_closer_shape.brick_fn(x,y,z,xoff,yoff,zoff),
                            _bebs_header_shape.brick_fn(x,y,z,xoff,yoff,zoff)
                        )
                    )
                )
            )
        )                    
    }
    
    #local _mortar_yoff     = BrickShape.brick_spec.mortar_sz;
    #local _bebs_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
        max(
            Layout.y_index_fn(x,y,z) - Layout.wall_spec.courses,
            -z,
            Layout.z_index_fn(x,y,z) - Layout.wall_spec.half_bricks,
            select(mod(Layout.y_index_fn(x,y,z)+Layout.parity,2) - 0.5,
                _bebs_even_shape_fn(x,y,z,xoff,yoff-_mortar_yoff,zoff),
                _bebs_odd_shape_fn(x,y,z,xoff,yoff-_mortar_yoff,zoff)
            )
        )        
    }
    
    #local _bebs_shape_aux_fn   = function(x,y,z) {
        _bebs_shape_fn(x,y,z,x-Layout.x_origin_fn(x,y,z),y-Layout.y_origin_fn(x,y,z),z-Layout.z_origin_fn(x,y,z))
    }
    
    function(x,y,z) {
    #ifdef (Layout.layout_jitter)
        _bebs_shape_aux_fn(
            x + Layout.layout_jitter.jitter_x_fn(x,y,z),
            y + Layout.layout_jitter.jitter_y_fn(x,y,z),
            z + Layout.layout_jitter.jitter_z_fn(x,y,z)
        )
    #else
        _bebs_shape_aux_fn(x,y,z)
    #end
    }
#end

// End Brickwall_english_bricks_shape_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_english_mortar_block_create(Layout,Inset)
//
//      Creates a function(x,y,z) that encompasses the extent of the wall
//      specified in the given Layout.  It can then be wrapped in a function
//      that adds textural noise before supplying it to the Brickwall_english_mortar_shape_fn
//
#macro Brickwall_english_mortar_block_create(Layout,Inset)
    #local _l_inset     = <Inset, 0, Inset>;
    #local _u_inset     = <Inset, Inset, Inset>;
    #local _block_size  = <Layout.wall_spec.length, Layout.wall_spec.courses*Layout.brick_spec.unit_sz.y, Layout.wall_spec.half_bricks*Layout.brick_spec.unit_sz.z - Layout.brick_spec.mortar_sz> - (_l_inset + _u_inset);
    #local _bsmb_box_fn = Iso_fixed_box(_block_size)
    #local _bsmb_mortar_fn  = Translate_fn(_bsmb_box_fn, _block_size/2 + _l_inset)
    
    #if (Layout.wall_spec.left_end_type = Brickwall_end_type_corner | Layout.wall_spec.right_end_type = Brickwall_end_type_corner)
        #local _bsmb_shape_fn   = function(x,y,z) {
            max(
                _bsmb_mortar_fn(x,y,z),
                select(mod(Layout.y_index_fn(x,y,z)+Layout.parity,2) - 0.5,
                    max(
                        -x,
                        (Layout.even_min_x_fn(x,y,z) - Layout.brick_spec.mortar_sz) - x,
                        x-Layout.layout_length,
                        x-(Layout.even_max_x_fn(x,y,z) + Layout.brick_spec.mortar_sz)
                    ),
                    max(
                        Layout.odd_min_x_fn(x,y,z) - x,
                        x - Layout.odd_max_x_fn(x,y,z)
                    )
                )            
            )
        }    
    #else
        #local _bsmb_shape_fn   = _bsmb_mortar_fn;
    #end
    
    //#local _bsmb_trans_shape_fn = Translate_fn(_bsmb_shape_fn, _l_inset)
    
    Mortar_block_create(_l_inset, _block_size, _bsmb_shape_fn)
#end

// End Brickwall_english_mortar_block_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_english_mortar_shape_fn(Layout,MortarBlock,JointSpec,BricksShapeFn)
//
#macro Brickwall_english_mortar_shape_fn(Layout,MortarBlock,JointSpec,BricksShapeFn)
    #if (JointSpec.bed.depth != 0)
        #local _bems_bed_front_joint_fn = Brickwall_translate_fn2(JointSpec.bed.shape_fn, <0,Layout.brick_spec.mortar_sz/2,MortarBlock.origin.z>)
        #local _bems_bed_back_joint_fn  = Brickwall_rotate_translate_fn2(JointSpec.bed.shape_fn, <0, pi, 0>, <0, Layout.brick_spec.mortar_sz/2, MortarBlock.origin.z + MortarBlock.sz.z>)
        #local _bems_bed_right_joint_fn = Brickwall_rotate_translate_fn2(JointSpec.bed.shape_fn, <0, -pi/2, 0>, <0, Layout.brick_spec.mortar_sz/2, 0>)
        #local _bems_bed_left_joint_fn  = Brickwall_rotate_translate_fn2(JointSpec.bed.shape_fn, <0, pi/2, 0>, <0, Layout.brick_spec.mortar_sz/2, 0>)
        
        #local _bems_bed_side_joint_fn  = function(x,y,z,xoff,yoff,zoff) {
            select(mod(Layout.y_index_fn(x,y,z)+Layout.parity,2) - 0.5,
                min(
                    #if (Layout.wall_spec.right_end_type = Brickwall_end_type_flush)
                    _bems_bed_right_joint_fn(x, y, z, x - Layout.even_max_x_fn(x,y,z), yoff, z),
                    #end
                    #if (Layout.wall_spec.left_end_type = Brickwall_end_type_flush)
                    _bems_bed_left_joint_fn(x, y, z, x - Layout.even_min_x_fn(x,y,z), yoff, z),
                    #end
                    999999,
                    999999
                ),
                min(
                    #if (Layout.wall_spec.right_end_type = Brickwall_end_type_flush)
                    _bems_bed_right_joint_fn(x,y,z,x-Layout.odd_max_x_fn(x,y,z),yoff,z),
                    #end
                    #if (Layout.wall_spec.left_end_type = Brickwall_end_type_flush)
                    _bems_bed_left_joint_fn(x,y,z,x-Layout.odd_min_x_fn(x,y,z),yoff,z),
                    #end
                    999999,
                    999999
                )
            )            
        }
        
        #local _bems_bed_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
            min(
                _bems_bed_front_joint_fn(x,y,z,x,yoff,z),
                #if (JointSpec.back_joints)
                _bems_bed_back_joint_fn(x,y,z,x,yoff,z),
                #end
                _bems_bed_side_joint_fn(x,y,z,xoff,yoff,zoff)
            )    
        }
    #end
    
    #if (JointSpec.header.depth != 0)
        #local _msz                 = Layout.brick_spec.mortar_sz/2;
        #local _bems_header_xoff_fn = function(x,y,z,xoff,yoff,zoff) {
            xoff - (Layout.x_size_fn(x,y,z)+_msz)
        }
        
        #local _bems_header_front_joint_fn          = Brickwall_rotate_translate_fn2(JointSpec.header.shape_fn, <0, 0, pi/2>, <0, 0, MortarBlock.origin.z>)
        #local _bems_header_front_joint_shape_fn    = function(x,y,z,xoff,yoff,zoff) {
            max(
                _bems_header_front_joint_fn(x,y,z,_bems_header_xoff_fn(x,y,z,xoff,yoff,zoff),yoff,z),
                Layout.brick_spec.mortar_sz - yoff
            )
        }
        #local _bems_header_back_joint_fn   = Brickwall_rotate_translate_fn2(JointSpec.header.shape_fn, <0, pi, pi/2>, <0, Layout.brick_spec.mortar_sz/2, MortarBlock.origin.z + MortarBlock.sz.z>)
        #local _bems_header_back_joint_shape_fn = function(x,y,z,xoff,yoff,zoff) {
            max(
                _bems_header_back_joint_fn(x,y,z,_bems_header_xoff_fn(x,y,z,xoff,yoff,zoff),yoff,z),
                Layout.brick_spec.mortar_sz - yoff
            )    
        }
        #local _bems_header_top_joint_fn    = Brickwall_rotate_translate_fn2(JointSpec.header.shape_fn, <pi/2, 0, pi/2>, <0, MortarBlock.origin.y + MortarBlock.sz.y, 0>)
        #local _max_z   = Layout.brick_spec.brick_sz.z;
        #local _bems_header_top_joint_shape_fn  = function(x,y,z,xoff,yoff,zoff) {
            max(
                _bems_header_top_joint_fn(x,y,z,_bems_header_xoff_fn(x,y,z,xoff,yoff,zoff),y,zoff),
                zoff - _max_z
            )    
        }
        
        #local _bems_header_shape_fn    = function(x,y,z,xoff,yoff,zoff) {
            min(
                _bems_header_front_joint_shape_fn(x,y,z,xoff,yoff,zoff),
                #if (JointSpec.back_joints)
                _bems_header_back_joint_shape_fn(x,y,z,xoff,yoff,zoff),
                #end
                _bems_header_top_joint_shape_fn(x,y,z,xoff,y,zoff)
            )    
        }
    #end
    
    #if (JointSpec.face.depth != 0 & Layout.wall_spec.half_bricks > 1)
        #local _bems_face_top_joint_fn      = Brickwall_rotate_translate_fn2(JointSpec.face.shape_fn, <pi/2, 0, 0>, <0, MortarBlock.origin.y + MortarBlock.sz.y, Layout.brick_spec.brick_sz.z + Layout.brick_spec.mortar_sz/2>)
        #local _bems_face_side_joint_fn     = Brickwall_rotate_fn2(JointSpec.face.shape_fn, <0, 0, -pi/2>)
        #local _bems_face_left_joint_fn     = Brickwall_rotate_translate_fn2(_bems_face_side_joint_fn, <0, pi/2, 0>, <0, Layout.brick_spec.mortar_sz/2, Layout.brick_spec.brick_sz.z + Layout.brick_spec.mortar_sz/2>)
        #local _bems_face_right_joint_fn    = Brickwall_rotate_translate_fn2(_bems_face_side_joint_fn, <0, -pi/2, 0>, <0, Layout.brick_spec.mortar_sz/2, Layout.brick_spec.brick_sz.z + Layout.brick_spec.mortar_sz/2>)
         
        #local _bems_face_side_joint_fn2  = function(x,y,z,xoff,yoff,zoff) {
            select(mod(Layout.y_index_fn(x,y,z)+Layout.parity,2) - 0.5,
                min(
                    _bems_face_right_joint_fn(x, y, z, x - Layout.even_max_x_fn(x,y,z), y, zoff),
                    _bems_face_left_joint_fn(x, y, z, x - Layout.even_min_x_fn(x,y,z), y, zoff)
                ),
                min(
                    _bems_face_right_joint_fn(x,y,z,x-Layout.odd_max_x_fn(x,y,z),y,zoff),
                    _bems_face_left_joint_fn(x,y,z,x-Layout.odd_min_x_fn(x,y,z),y,zoff)
                )
            )            
        }
        
        #local _bems_face_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
            min(
                _bems_face_top_joint_fn(x,y,z,x,y,zoff),
                _bems_face_side_joint_fn2(x,y,z,xoff,yoff,zoff)
            )    
        }
    #end
    
    #local _bems_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
        max(
            min(
                MortarBlock.shape_fn(x,y,z),
                #if (JointSpec.bed.depth < 0)
                    _bems_bed_shape_fn(x,y,z,xoff,yoff,zoff),
                #end
                #if (JointSpec.header.depth < 0)
                    _bems_header_shape_fn(x,y,z,xoff,yoff,zoff),
                #end
                #if (JointSpec.face.depth < 0 & Layout.wall_spec.half_bricks > 1)
                    _bems_face_shape_fn(x,y,z,xoff,yoff,zoff),
                #end    
                999999
            ),
            -min(    
                BricksShapeFn(x,y,z),
                #if (JointSpec.bed.depth > 0)
                    _bems_bed_shape_fn(x,y,z,xoff,yoff,zoff),
                #end
                #if (JointSpec.header.depth > 0)
                    _bems_header_shape_fn(x,y,z,xoff,yoff,zoff),
                #end
                #if (JointSpec.face.depth > 0 & Layout.wall_spec.half_bricks > 1)
                    _bems_face_shape_fn(x,y,z,xoff,yoff,zoff),
                #end        
                999999
            )    
        ) 
    }
    
    #local _bems_aux_fn = function(x,y,z,jx,jy,jz) {
        _bems_shape_fn(x,y,z,jx-Layout.x_origin_fn(jx,jy,jz),jy-Layout.y_origin_fn(jx,jy,jz),jz-Layout.z_origin_fn(jx,jy,jz))
    }
    
    function(x,y,z) {
    #ifdef (Layout.layout_jitter)
        _bems_aux_fn(
            x, y, z,
            x + Layout.layout_jitter.jitter_x_fn(x,y,z),
            y + Layout.layout_jitter.jitter_y_fn(x,y,z),
            z + Layout.layout_jitter.jitter_z_fn(x,y,z)
        )    
    #else
        _bems_aux_fn(x,y,z,x,y,z)
    #end
    }        
#end
// End Brickwall_english_mortar_shape_fn
//-----------------------------------------------------------------------------

// End Brick Wall English Layout
//=============================================================================

#version Brickwall_english_layout_Inc_Temp;
#end // ifndef(Brickwall_english_layout_Inc_Temp)