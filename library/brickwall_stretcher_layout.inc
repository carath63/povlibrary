/******************************************************************************
 * File: brickwall_stretcher_layout.inc
 * Description:
 *      FILL IN DESCRIPTION HERE
 ******************************************************************************/

#ifndef(Brickwall_stretcher_layout_Inc_Temp)

#declare Brickwall_stretcher_layout_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including lib_template.inc\n"
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
// Brick Wall Stretcher Layout
//
//      The stretcher layout is the common brick layout method, where courses
//      of bricks are offset by some amount from the course below so that
//      the stretcher length of the bricks cover two adjacent bricks and the
//      mortar between them below.
//
//      This implementation of the layout supports the standard stretcher
//      layout, both for flush and corner ended walls, as well as a variant
//      where the first layer starts with a brick shorter than a full brick
//      size, causing the overlaps to be of different sizes between layers.
//
//      To support corners, the stretcher layout introduces a parity parameter
//      that determines whether the bottom layer is "even" or "odd".  Even
//      layers are always full length, and start with a full length or raked
//      brick.  Odd layers may be shorter to accommodate bricks from crossing
//      walls intersecting at the corner.  The odd layers also start with a
//      shorter brick, depending on the raking, to ensure an overlap between
//      successive layers of bricks.
//
//      In a stretcher wall, if the full length of the wall is not an integer
//      multiple of the brick length + mortar, a bat is inserted before the
//      rightmost brick (on an even layer) to allow for a full brick at both
//      ends of the wall, (modulo raking).  Odd layers are adjusted as well.
//

//-----------------------------------------------------------------------------
// Brickwall_stretcher_layout_create(WallSpec,BrickSpec,Parity,Raking)
//
#macro Brickwall_stretcher_layout_create(WallSpec,BrickSpec,Parity,Raking)

    #if (Raking > 0)
        #local _even_left_brick_size    = (1 - Raking)*BrickSpec.brick_sz.x;
        #local _even_right_brick_size   = (1 - Raking)*BrickSpec.brick_sz.x;
    #else
        #local _even_left_brick_size    = BrickSpec.brick_sz.x;
        #local _even_right_brick_size   = BrickSpec.brick_sz.x;
    #end
    
    #local _even_middle_space   = WallSpec.length - (_even_left_brick_size + BrickSpec.mortar_sz + _even_right_brick_size);
    #if (_even_middle_space < 0)
        #error concat("Brickwall_stretcher_layout_create: WallSpec.length ", str(WallSpec.length, 0, 6), " is too short for a stretcher wall\n")
    #end    
    #local _even_middle_bricks  = int(_even_middle_space/BrickSpec.unit_sz.x);
    #local _even_middle_size    = _even_middle_bricks*BrickSpec.unit_sz.x;
    #local _even_gap            = _even_middle_space - _even_middle_size;
    #if (_even_gap > BrickSpec.mortar_sz)
        #local _even_right_bat_size = _even_gap - BrickSpec.mortar_sz;    
    #else
        #local _even_right_bat_size = 0;
    #end
    #local _even_full_min_x = _even_left_brick_size + BrickSpec.mortar_sz;
    #local _even_full_max_x = _even_full_min_x + _even_middle_size;
    
    #if (WallSpec.left_end_type = Brickwall_end_type_corner)
        #local _odd_min_x           = BrickSpec.unit_sz.z;
        #local _odd_left_brick_size = BrickSpec.brick_sz.x;
    #else
        #local _odd_min_x   = 0;
        #local _odd_left_brick_size = BrickSpec.brick_sz.x/2;
    #end
    
    #if (WallSpec.right_end_type = Brickwall_end_type_corner)
        #local _odd_max_x               = WallSpec.length - BrickSpec.unit_sz.z;
        #local _odd_right_brick_size    = BrickSpec.brick_sz.x;
    #else
        #local _odd_max_x               = WallSpec.length;
        #local _odd_right_brick_size    = BrickSpec.brick_sz.x/2;
    #end
    
    #local _odd_middle_space    = (_odd_max_x - _odd_min_x) - (_odd_left_brick_size + BrickSpec.mortar_sz + _odd_right_brick_size);
    #if (_odd_middle_space < 0)
        #error concat("Brickwall_stretcher_layout_create: WallSpec.length ", str(WallSpec.length, 0, 6), " is too short for a stretcher wall\n")
    #end
    #local _odd_middle_bricks   = int(_odd_middle_space/BrickSpec.unit_sz.x);
    #local _odd_middle_size     = _odd_middle_bricks*BrickSpec.unit_sz.x;
    #local _odd_gap             = _odd_middle_space - _odd_middle_size;
    #if (_odd_gap > BrickSpec.mortar_sz)                  
        #local _odd_left_bat_size   = _odd_gap - BrickSpec.mortar_sz;
    #else
        #local _odd_left_bat_size   = 0;
    #end
    
    #local _odd_full_min_x  = _odd_min_x + _odd_left_brick_size + BrickSpec.mortar_sz + _odd_left_bat_size;
    #if (_odd_left_bat_size > 0)
        #local _odd_full_min_x  = _odd_full_min_x + BrickSpec.mortar_sz;
    #end    
    #local _odd_full_max_x  = _odd_full_min_x + _odd_middle_size;

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
    
    #local _even_left_max_x     = _even_left_brick_size + BrickSpec.mortar_sz;
    #if (_even_right_bat_size > 0)
        #local _even_right_bat_max_x    = _even_full_max_x + _even_right_bat_size + BrickSpec.mortar_sz;
    #end
    #local _odd_left_max_x      = _odd_min_x + _odd_left_brick_size + BrickSpec.mortar_sz;
    #if (_odd_left_bat_size > 0)
        #local _odd_left_bat_max_x      = _odd_left_max_x + _odd_left_bat_size + BrickSpec.mortar_sz;
    #else
        #local _odd_left_bat_max_x      = _odd_left_max_x;
    #end
        
    #local _even_x_origin_fn    = function(x,y,z) {
        select(x - _even_left_max_x,
            0,
            select(x - _even_full_max_x,
                _even_left_max_x + f_unit_pin(x - _even_left_max_x, _ux),
                #if (_even_right_bat_size > 0)
                select(x - _even_right_bat_max_x,
                    _even_full_max_x,
                    _even_right_bat_max_x
                )    
                #else
                _even_full_max_x
                #end
            )
        )        
    }
    #local _odd_x_origin_fn     = function(x,y,z) {
        select(x - _odd_min_x,
            x - _odd_min_x,
            select(x - _odd_left_max_x,
                _odd_min_x,
                #if (_odd_left_bat_size > 0)
                select(x - _odd_left_bat_max_x,
                    _odd_left_max_x,
                    select(x - _odd_full_max_x,
                        _odd_left_bat_max_x + f_unit_pin(x - _odd_left_bat_max_x, _ux),
                        _odd_full_max_x
                    )    
                )    
                #else
                select(x - _odd_full_max_x,
                    _odd_left_max_x + f_unit_pin(x - _odd_left_max_x, _ux),
                    _odd_full_max_x
                )    
                #end
            )
        )        
    }
    #local _x_origin_fn         = function(x,y,z) {
        select(mod(_y_index_fn(x,y,z)+Parity,2) - 0.5,
            _even_x_origin_fn(x,y,z),
            _odd_x_origin_fn(x,y,z)
        )    
    }
    
    #local _even_x_index_fn    = function(x,y,z) {
        select(x - _even_left_max_x,
            0,
            select(x - _even_full_max_x,
                1 + f_unit_index(x - _even_left_max_x, _ux),
                #if (_even_right_bat_size > 0)
                select(x - _even_right_bat_max_x,
                    2 + _even_middle_bricks,
                    3 + _even_middle_bricks
                )    
                #else
                2 + _even_middle_bricks
                #end
            )
        )        
    }
    #local _odd_x_index_fn     = function(x,y,z) {
        select(x - _odd_min_x,
            -1,
            select(x - _odd_left_max_x,
                0,
                #if (_odd_left_bat_size > 0)
                select(x - _odd_left_bat_max_x,
                    1,
                    select(x - _odd_full_max_x,
                        2 + f_unit_index(x - _odd_left_bat_max_x, _ux),
                        2 + _odd_middle_bricks
                    )    
                )    
                #else
                select(x - _odd_full_max_x,
                    1 + f_unit_index(x - _odd_left_max_x, _ux),
                    1 + _odd_middle_bricks
                )    
                #end
            )
        )        
    }
    #local _x_index_fn         = function(x,y,z) {
        select(mod(_y_index_fn(x,y,z)+Parity,2) - 0.5,
            _even_x_index_fn(x,y,z),
            _odd_x_index_fn(x,y,z)
        )    
    }
    
    #local _even_x_size_fn    = function(x,y,z) {
        select(x - _even_left_max_x,
            _even_left_brick_size,
            select(x - _even_full_max_x,
                _bx,
                #if (_even_right_bat_size > 0)
                select(x - _even_right_bat_max_x,
                    _even_right_bat_size,
                    _even_right_brick_size
                )    
                #else
                _even_right_brick_size
                #end
            )
        )        
    }
    #local _odd_x_size_fn     = function(x,y,z) {
        select(x - _odd_min_x,
            0,
            select(x - _odd_left_max_x,
                _odd_left_brick_size,
                #if (_odd_left_bat_size > 0)
                select(x - _odd_left_bat_max_x,
                    _odd_left_bat_size,
                    select(x - _odd_full_max_x,
                        _bx,
                        _odd_right_brick_size
                    )    
                )    
                #else
                select(x - _odd_full_max_x,
                    _bx,
                    _odd_right_brick_size
                )    
                #end
            )
        )        
    }
    #local _x_size_fn         = function(x,y,z) {
        select(mod(_y_index_fn(x,y,z)+Parity,2) - 0.5,
            _even_x_size_fn(x,y,z),
            _odd_x_size_fn(x,y,z)
        )    
    }
    
    #local _wall_size           = <WallSpec.length, WallSpec.courses*BrickSpec.unit_sz.y, WallSpec.half_bricks*BrickSpec.unit_sz.z - BrickSpec.mortar_sz>;
    
    #local _stretcher_layout  = Brickwall_base_layout_create(
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
    
    #local _stretcher_layout.brick_spec = BrickSpec;
    #local _stretcher_layout.parity     = Parity;
    #local _stretcher_layout.raking     = Raking;
    
    #local _stretcher_layout.even_left_brick_size   = _even_left_brick_size;
    #local _stretcher_layout.even_right_brick_size  = _even_right_brick_size;
    #local _stretcher_layout.even_right_bat_size    = _even_right_bat_size;
    #if (_even_right_bat_size > 0)
        #local _stretcher_layout.even_right_bat_max_x   = _even_right_bat_max_x;
    #end    
    #local _stretcher_layout.even_full_min_x        = _even_full_min_x;
    #local _stretcher_layout.even_full_max_x        = _even_full_max_x;
    #local _stretcher_layout.even_left_max_x        = _even_left_max_x;
    
    #local _stretcher_layout.odd_left_brick_size    = _odd_left_brick_size;
    #local _stretcher_layout.odd_left_bat_size      = _odd_left_bat_size;
    #local _stretcher_layout.odd_left_max_x         = _odd_left_max_x;
    #local _stretcher_layout.odd_right_brick_size   = _odd_right_brick_size;
    #local _stretcher_layout.odd_full_min_x         = _odd_full_min_x;
    #local _stretcher_layout.odd_full_max_x         = _odd_full_max_x;
    #if (_odd_left_bat_size > 0)
        #local _stretcher_layout.odd_left_bat_max_x = _odd_left_bat_max_x;
    #end    
    #local _stretcher_layout.odd_min_x              = _odd_min_x;
    #local _stretcher_layout.odd_max_x              = _odd_max_x;
    
    _stretcher_layout
#end

// End Brickwall_stretcher_layout_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_stretcher_bricks_shape_fn(Layout,BrickShape)
//
#macro Brickwall_stretcher_bricks_shape_fn(Layout,BrickShape)
    #if (Layout.even_left_brick_size < Layout.brick_spec.brick_sz.x)
        #local _bsbs_even_left_brick_shape  = Brickwall_brick_shape_cut_stretcher(BrickShape,Layout.even_left_brick_size,0);
    #else
        #local _bsbs_even_left_brick_shape  = BrickShape;
    #end
    #if (Layout.even_right_brick_size < Layout.brick_spec.brick_sz.x)
        #local _bsbs_even_right_brick_shape = Brickwall_brick_shape_cut_stretcher(BrickShape,Layout.even_right_brick_size,1);
    #else
        #local _bsbs_even_right_brick_shape = BrickShape;
    #end
    #if (Layout.even_right_bat_size > 0)
        #local _bsbs_even_right_bat_shape   = Brickwall_brick_shape_cut_stretcher(BrickShape,Layout.even_right_bat_size,1);
    #end
    #if (Layout.odd_left_brick_size < Layout.brick_spec.brick_sz.x)
        #local _bsbs_odd_left_brick_shape  = Brickwall_brick_shape_cut_stretcher(BrickShape,Layout.odd_left_brick_size,0);
    #else
        #local _bsbs_odd_left_brick_shape  = BrickShape;
    #end
    #if (Layout.odd_right_brick_size < Layout.brick_spec.brick_sz.x)
        #local _bsbs_odd_right_brick_shape = Brickwall_brick_shape_cut_stretcher(BrickShape,Layout.odd_right_brick_size,1);
    #else
        #local _bsbs_odd_right_brick_shape = BrickShape;
    #end
    #if (Layout.odd_left_bat_size > 0)
        #local _bsbs_odd_left_bat_shape   = Brickwall_brick_shape_cut_stretcher(BrickShape,Layout.odd_left_bat_size,1);
    #end
    
    #local _bsbs_even_shape_fn  = function(x,y,z,xoff,yoff,zoff) {
        max(
            -x,
            x - Layout.wall_spec.length,
            select(x - Layout.even_left_max_x,
                _bsbs_even_left_brick_shape.brick_fn(x,y,z,xoff,yoff,zoff),
                select(x - Layout.even_full_max_x,
                    BrickShape.brick_fn(x,y,z,xoff,yoff,zoff),
                    #if (Layout.even_right_bat_size > 0)
                    select(x - Layout.even_right_bat_max_x,
                        _bsbs_even_right_bat_shape.brick_fn(x,y,z,xoff,yoff,zoff),
                        _bsbs_even_right_brick_shape.brick_fn(x,y,z,xoff,yoff,zoff)
                    )    
                    #else
                    _bsbs_even_right_brick_shape.brick_fn(x,y,z,xoff,yoff,zoff)
                    #end
                )
            )        
        )
    }
    
    #local _bsbs_odd_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
        max(
            Layout.odd_min_x - x,
            x - Layout.odd_max_x,
            select(x - Layout.odd_min_x,
                Layout.odd_min_x - x,
                select(x - Layout.odd_left_max_x,
                    _bsbs_odd_left_brick_shape.brick_fn(x,y,z,xoff,yoff,zoff),
                    #if (Layout.odd_left_bat_size > 0)
                    select(x - Layout.odd_left_bat_max_x,
                        _bsbs_odd_left_bat_shape.brick_fn(x,y,z,xoff,yoff,zoff),
                        select(x - Layout.odd_full_max_x,
                            BrickShape.brick_fn(x,y,z,xoff,yoff,zoff),
                            _bsbs_odd_right_brick_shape.brick_fn(x,y,z,xoff,yoff,zoff)
                        )    
                    )    
                    #else
                    select(x - Layout.odd_full_max_x,
                        BrickShape.brick_fn(x,y,z,xoff,yoff,zoff),
                        _bsbs_odd_right_brick_shape.brick_fn(x,y,z,xoff,yoff,zoff)
                    )    
                    #end
                )

            )
        )    
    }
    
    #local _mortar_yoff     = BrickShape.brick_spec.mortar_sz;
    #local _bsbs_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
        max(
            Layout.y_index_fn(x,y,z) - Layout.wall_spec.courses,
            Layout.z_index_fn(x,y,z) - Layout.wall_spec.half_bricks,
            select(mod(Layout.y_index_fn(x,y,z)+Layout.parity,2) - 0.5,
                _bsbs_even_shape_fn(x,y,z,xoff,yoff-_mortar_yoff,zoff),
                _bsbs_odd_shape_fn(x,y,z,xoff,yoff-_mortar_yoff,zoff)
            )
        )        
    }
    
    #local _bsbs_aux_fn = function(x,y,z) {
        _bsbs_shape_fn(x,y,z,x-Layout.x_origin_fn(x,y,z),y-Layout.y_origin_fn(x,y,z),z-Layout.z_origin_fn(x,y,z))
    }
    
    function(x,y,z) {
    #ifdef (Layout.layout_jitter)
        _bsbs_aux_fn(
            x + Layout.layout_jitter.jitter_x_fn(x,y,z),
            y + Layout.layout_jitter.jitter_y_fn(x,y,z),
            z + Layout.layout_jitter.jitter_z_fn(x,y,z)
        )    
    #else
        _bsbs_aux_fn(x,y,z)
    #end
    }
#end

// End Brickwall_stretcher_bricks_shape_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_stretcher_mortar_block_create(Layout,Inset)
//
//      Creates a function(x,y,z) that encompasses the extent of the wall
//      specified in the given Layout.  It can then be wrapped in a function
//      that adds textural noise before supplying it to the Brickwall_stretcher_mortar_shape_fn
//
#macro Brickwall_stretcher_mortar_block_create(Layout,Inset)
    #local _l_inset     = <Inset, 0, Inset>;
    #local _u_inset     = <Inset, Inset, Inset>;
    #local _block_size  = <Layout.wall_spec.length, Layout.wall_spec.courses*Layout.brick_spec.unit_sz.y, Layout.wall_spec.half_bricks*Layout.brick_spec.unit_sz.z - Layout.brick_spec.mortar_sz> - (_l_inset + _u_inset);
    #local _bsmb_box_fn = Iso_fixed_box(_block_size)
    #local _bsmb_mortar_fn  = Translate_fn(_bsmb_box_fn, _block_size/2)
    
    #local _even_max_x                  = Layout.wall_spec.length;
    #if (Layout.wall_spec.left_end_type = Brickwall_end_type_corner | Layout.wall_spec.right_end_type = Brickwall_end_type_corner)
        #local _bsmb_shape_fn   = function(x,y,z) {
            max(
                _bsmb_mortar_fn(x,y,z),
                select(mod(Layout.y_index_fn(x,y,z)+Layout.parity,2) - 0.5,
                    max(
                        -x,
                        x-_even_max_x
                    ),
                    max(
                        Layout.odd_min_x - x,
                        x - Layout.odd_max_x
                    )
                )            
            )
        }    
    #else
        #local _bsmb_shape_fn   = _bsmb_mortar_fn;
    #end
    
    #local _bsmb_trans_shape_fn = Translate_fn(_bsmb_shape_fn, _l_inset)
    
    Mortar_block_create(_l_inset, _block_size, _bsmb_trans_shape_fn)
#end

// End Brickwall_stretcher_mortar_block_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Brickwall_stretcher_mortar_shape_fn(Layout,MortarBlock,JointSpec,BricksShapeFn)
//
#macro Brickwall_stretcher_mortar_shape_fn(Layout,MortarBlock,JointSpec,BricksShapeFn)
    #if (JointSpec.bed.depth != 0)
        #local _bsms_bed_front_joint_fn = Brickwall_translate_fn2(JointSpec.bed.shape_fn, <0,Layout.brick_spec.mortar_sz/2,MortarBlock.origin.z>)
        #local _bsms_bed_back_joint_fn  = Brickwall_rotate_translate_fn2(JointSpec.bed.shape_fn, <0, pi, 0>, <0, Layout.brick_spec.mortar_sz/2, MortarBlock.origin.z + MortarBlock.sz.z>)
        #local _bsms_bed_right_joint_fn = Brickwall_rotate_translate_fn2(JointSpec.bed.shape_fn, <0, -pi/2, 0>, <0, Layout.brick_spec.mortar_sz/2, 0>)
        #local _bsms_bed_left_joint_fn  = Brickwall_rotate_translate_fn2(JointSpec.bed.shape_fn, <0, pi/2, 0>, <0, Layout.brick_spec.mortar_sz/2, 0>)
        
        #local _even_max_x  = MortarBlock.origin.x + MortarBlock.sz.x;
        #local _even_min_x  = MortarBlock.origin.x;
        #local _odd_max_x   = Layout.odd_max_x - MortarBlock.origin.x;
        #local _odd_min_x   = Layout.odd_min_x + MortarBlock.origin.x;
        #local _bsms_bed_side_joint_fn  = function(x,y,z,xoff,yoff,zoff) {
            select(mod(Layout.y_index_fn(x,y,z)+Layout.parity,2) - 0.5,
                min(
                    _bsms_bed_right_joint_fn(x, y, z, x - _even_max_x, yoff, z),
                    _bsms_bed_left_joint_fn(x, y, z, x - _even_min_x, yoff, z)
                ),
                min(
                    _bsms_bed_right_joint_fn(x,y,z,x-_odd_max_x,yoff,z),
                    _bsms_bed_left_joint_fn(x,y,z,x-_odd_min_x,yoff,z)
                )
            )            
        }
        
        #local _bsms_bed_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
            min(
                _bsms_bed_front_joint_fn(x,y,z,x,yoff,z),
                _bsms_bed_back_joint_fn(x,y,z,x,yoff,z),
                _bsms_bed_side_joint_fn(x,y,z,xoff,yoff,zoff)
            )    
        }
    #end
    
    #if (JointSpec.header.depth != 0)
        #local _msz                 = Layout.brick_spec.mortar_sz/2;
        #local _bsms_header_xoff_fn = function(x,y,z,xoff,yoff,zoff) {
            xoff - (Layout.x_size_fn(x,y,z)+_msz)
        }    
        #local _bsms_header_front_joint_fn          = Brickwall_rotate_translate_fn(JointSpec.header.shape_fn, <0, 0, pi/2>, <0, 0, MortarBlock.origin.z>)
        #local _bsms_header_front_joint_shape_fn    = function(x,y,z,xoff,yoff,zoff) {
            max(
                _bsms_header_front_joint_fn(x,y,z,_bsms_header_xoff_fn(x,y,z,xoff,yoff,zoff),yoff,z),
                Layout.brick_spec.mortar_sz - yoff
            )
        }
        #local _bsms_header_back_joint_fn   = Brickwall_rotate_translate_fn(JointSpec.header.shape_fn, <0, pi, pi/2>, <0, Layout.brick_spec.mortar_sz/2, MortarBlock.origin.z + MortarBlock.sz.z>)
        #local _bsms_header_back_joint_shape_fn = function(x,y,z,xoff,yoff,zoff) {
            max(
                _bsms_header_back_joint_fn(x,y,z,_bsms_header_xoff_fn(x,y,z,xoff,yoff,zoff),yoff,z),
                Layout.brick_spec.mortar_sz - yoff
            )    
        }
        #local _bsms_header_top_joint_fn    = Brickwall_rotate_translate_fn(JointSpec.header.shape_fn, <pi/2, 0, pi/2>, <0, MortarBlock.origin.y + MortarBlock.sz.y, 0>)
        #local _max_z   = Layout.brick_spec.brick_sz.z;
        #local _bsms_header_top_joint_shape_fn  = function(x,y,z,xoff,yoff,zoff) {
            max(
                _bsms_header_top_joint_fn(x,y,z,_bsms_header_xoff_fn(x,y,z,xoff,yoff,zoff),y,zoff),
                zoff - _max_z
            )    
        }
        
        #local _bsms_header_shape_fn    = function(x,y,z,xoff,yoff,zoff) {
            min(
                _bsms_header_front_joint_shape_fn(x,y,z,xoff,yoff,zoff),
                _bsms_header_back_joint_shape_fn(x,y,z,xoff,yoff,zoff),
                _bsms_header_top_joint_shape_fn(x,y,z,xoff,y,zoff)
            )    
        }
    #end
    
    #if (JointSpec.face.depth != 0 & Layout.wall_spec.half_bricks > 1)
        #local _bsms_face_top_joint_fn      = Brickwall_rotate_translate_fn(JointSpec.face.shape_fn, <pi/2, 0, 0>, <0, MortarBlock.origin.y + MortarBlock.sz.y, Layout.brick_spec.brick_sz.z + Layout.brick_spec.mortar_sz/2>)
        #local _bsms_face_side_joint_fn     = Brickwall_rotate_fn(JointSpec.face.shape_fn, <0, 0, -pi/2>)
        #local _bsms_face_left_joint_fn     = Brickwall_rotate_translate_fn(_bsms_face_side_joint_fn, <0, pi/2, 0>, <0, Layout.brick_spec.mortar_sz/2, Layout.brick_spec.brick_sz.z + Layout.brick_spec.mortar_sz/2>)
        #local _bsms_face_right_joint_fn    = Brickwall_rotate_translate_fn(_bsms_face_side_joint_fn, <0, -pi/2, 0>, <0, Layout.brick_spec.mortar_sz/2, Layout.brick_spec.brick_sz.z + Layout.brick_spec.mortar_sz/2>)
         
        #local _even_max_x  = MortarBlock.origin.x + MortarBlock.sz.x;
        #local _even_min_x  = MortarBlock.origin.x;
        #local _bsms_face_side_joint_fn2  = function(x,y,z,xoff,yoff,zoff) {
        #local _odd_max_x   = Layout.odd_max_x - MortarBlock.origin.x;
        #local _odd_min_x   = Layout.odd_min_x + MortarBlock.origin.x;
            select(mod(Layout.y_index_fn(x,y,z)+Layout.parity,2) - 0.5,
                min(
                    _bsms_face_right_joint_fn(x, y, z, x - _even_max_x, y, zoff),
                    _bsms_face_left_joint_fn(x, y, z, x - _even_min_x, y, zoff)
                ),
                min(
                    _bsms_face_right_joint_fn(x,y,z,x-_odd_max_x,y,zoff),
                    _bsms_face_left_joint_fn(x,y,z,x-_odd_min_x,y,zoff)
                )
            )            
        }
        
        #local _bsms_face_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
            min(
                _bsms_face_top_joint_fn(x,y,z,x,y,zoff),
                _bsms_face_side_joint_fn2(x,y,z,xoff,yoff,zoff)
            )    
        }
    #end
    
    #local _bsms_shape_fn   = function(x,y,z,xoff,yoff,zoff) {
        max(
            min(
                MortarBlock.shape_fn(x,y,z),
                #if (JointSpec.bed.depth < 0)
                    _bsms_bed_shape_fn(x,y,z,xoff,yoff,zoff),
                #end
                #if (JointSpec.header.depth < 0)
                    _bsms_header_shape_fn(x,y,z,xoff,yoff,zoff),
                #end
                #if (JointSpec.face.depth < 0 & Layout.wall_spec.half_bricks > 1)
                    _bsms_face_shape_fn(x,y,z,xoff,yoff,zoff),
                #end    
                999999
            ),
            -min(    
                BricksShapeFn(x,y,z),
                #if (JointSpec.bed.depth > 0)
                    _bsms_bed_shape_fn(x,y,z,xoff,yoff,zoff),
                #end
                #if (JointSpec.header.depth > 0)
                    _bsms_header_shape_fn(x,y,z,xoff,yoff,zoff),
                #end
                #if (JointSpec.face.depth > 0 & Layout.wall_spec.half_bricks > 1)
                    _bsms_face_shape_fn(x,y,z,xoff,yoff,zoff),
                #end        
                999999
            )    
        ) 
    }
    
    #local _bsms_aux_fn = function(x,y,z,jx,jy,jz) {
        _bsms_shape_fn(x,y,z,jx-Layout.x_origin_fn(jx,jy,jz),jy-Layout.y_origin_fn(jx,jy,jz),jz-Layout.z_origin_fn(jx,jy,jz))
    }
    
    function(x,y,z) {
    #ifdef (Layout.layout_jitter)
        _bsms_aux_fn(
            x,y,z,
            x + Layout.layout_jitter.jitter_x_fn(x,y,z),
            y + Layout.layout_jitter.jitter_y_fn(x,y,z),
            z + Layout.layout_jitter.jitter_z_fn(x,y,z)
        )    
    #else
        _bsms_aux_fn(x,y,z,x,y,z)
    #end
    }        
#end

// End Brickwall_stretcher_mortar_shape_fn
//-----------------------------------------------------------------------------

// End Brick Wall Stretcher Layout
//=============================================================================

#version Brickwall_stretcher_layout_Inc_Temp;
#end // ifndef(Brickwall_stretcher_layout_Inc_Temp)