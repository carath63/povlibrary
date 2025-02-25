/******************************************************************************
 * File: cs_stone.inc
 * Description:
 *      CS_stone ".inc" file
 ******************************************************************************/

#ifndef(CS_Stone_Inc_Temp)

#declare CS_Stone_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including cs_stone.inc\n"
#end                     

//=============================================================================
// Includes
//

//-----------------------------------------------------------------------------
// Standard Includes
//
#include "rand.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
#include "libscale.inc"
#include "libisoshapes.inc"
#include "libfn.inc"
#include "libgeom.inc"
#include "libnoise.inc"
#include "libchess.inc"

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
// Chess Set Dimensions
//
#declare CS_stone_board_size                    = Math_Scale(SCALE_CM, <32, 1, 32>);
#declare CS_stone_board_edge_rnd                = Math_Scale(SCALE_MM, 0.5);
#declare CS_stone_board_corner_rnd              = Math_Scale(SCALE_MM, 5);
#declare CS_stone_board_square_size             = Math_Scale(SCALE_CM, <3, 3>);

#declare CS_stone_piece_base_d                  = Math_Scale(SCALE_CM, 2.4);
#declare CS_stone_piece_base_radius             = CS_stone_piece_base_d/2;
#declare CS_stone_piece_base_md                 = Math_Scale(SCALE_CM, 2.6);
#declare CS_stone_piece_base_mradius            = CS_stone_piece_base_md/2;
#declare CS_stone_piece_base_height             = Math_Scale(SCALE_CM, 0.6);

#declare CS_stone_pawn_base_d                   = Math_Scale(SCALE_CM, 2.2);
#declare CS_stone_pawn_base_radius              = CS_stone_pawn_base_d/2;
#declare CS_stone_pawn_base_md                  = Math_Scale(SCALE_CM, 2.5);
#declare CS_stone_pawn_base_mradius             = CS_stone_pawn_base_md/2;
#declare CS_stone_pawn_base_height              = Math_Scale(SCALE_CM, 0.65);

#declare CS_stone_felt_disc_thickness           = Math_Scale(SCALE_MM, 0.6);

// Rook
#declare CS_stone_rook_tower_base_d             = Math_Scale(SCALE_CM, 2.3);
#declare CS_stone_rook_tower_base_radius        = CS_stone_rook_tower_base_d/2;
#declare CS_stone_rook_tower_base_height        = Math_Scale(SCALE_MM, 3);
#declare CS_stone_rook_tower_mid_d              = Math_Scale(SCALE_CM, 1.4);
#declare CS_stone_rook_tower_mid_radius         = CS_stone_rook_tower_mid_d/2;
#declare CS_stone_rook_tower_mid_height         = Math_Scale(SCALE_CM, 1.2);
#declare CS_stone_rook_tower_top_d              = Math_Scale(SCALE_CM, 1.55);
#declare CS_stone_rook_tower_top_radius         = CS_stone_rook_tower_top_d/2;
#declare CS_stone_rook_tower_cap_d              = Math_Scale(SCALE_CM, 1.4);
#declare CS_stone_rook_tower_cap_radius         = CS_stone_rook_tower_cap_d/2;
#declare CS_stone_rook_tower_cap_height         = Math_Scale(SCALE_MM, 3);
#declare CS_stone_rook_tower_rnd                = Math_Scale(SCALE_MM, 0.1);
#declare CS_stone_rook_tower_height             = Math_Scale(SCALE_CM, 2.85);
#declare CS_stone_rook_cap_height               = Math_Scale(SCALE_CM, 1.6);
#declare CS_stone_rook_cap_d                    = Math_Scale(SCALE_CM, 1.7);
#declare CS_stone_rook_cap_radius               = CS_stone_rook_cap_d/2;
#declare CS_stone_rook_height                   = CS_stone_piece_base_height + CS_stone_rook_tower_height + CS_stone_rook_cap_height;
#declare CS_stone_rook_radius                   = max(
    CS_stone_piece_base_mradius,
    CS_stone_rook_tower_base_radius,
    CS_stone_rook_tower_mid_radius,
    CS_stone_rook_tower_top_radius,
    CS_stone_rook_tower_cap_radius,
    CS_stone_rook_cap_radius
);

// Bishop
#declare CS_stone_bishop_tower_base_d           = Math_Scale(SCALE_CM, 2.4);
#declare CS_stone_bishop_tower_base_radius      = CS_stone_bishop_tower_base_d/2;
#declare CS_stone_bishop_tower_base_height      = Math_Scale(SCALE_MM, 2.5);
#declare CS_stone_bishop_tower_mid_d            = Math_Scale(SCALE_CM, 1.3);
#declare CS_stone_bishop_tower_mid_radius       = CS_stone_bishop_tower_mid_d/2;
#declare CS_stone_bishop_tower_mid_height       = Math_Scale(SCALE_CM, 1.4);
#declare CS_stone_bishop_tower_top_d            = Math_Scale(SCALE_CM, 1.5);
#declare CS_stone_bishop_tower_top_radius       = CS_stone_bishop_tower_top_d/2;
#declare CS_stone_bishop_tower_cap_d            = Math_Scale(SCALE_CM, 1.2);
#declare CS_stone_bishop_tower_cap_radius       = CS_stone_bishop_tower_cap_d/2;
#declare CS_stone_bishop_tower_cap_height       = Math_Scale(SCALE_MM, 1.5);
#declare CS_stone_bishop_tower_rnd              = Math_Scale(SCALE_MM, 0.1);
#declare CS_stone_bishop_tower_height           = Math_Scale(SCALE_CM, 2.65);

#declare CS_stone_bishop_cap_sphere_d           = Math_Scale(SCALE_CM, 1.6);
#declare CS_stone_bishop_cap_sphere_radius      = CS_stone_bishop_cap_sphere_d/2;
#declare CS_stone_bishop_cap_sphere_height      = 2.5*CS_stone_bishop_cap_sphere_radius;
#declare CS_stone_bishop_cap_miter_base_d       = Math_Scale(SCALE_CM, 0.6);
#declare CS_stone_bishop_cap_miter_base_radius  = CS_stone_bishop_cap_miter_base_d/2;
#declare CS_stone_bishop_cap_miter_height       = Math_Scale(SCALE_CM, 0.5);
#declare CS_stone_bishop_cap_miter_mid_d        = Math_Scale(SCALE_CM, 0.75);
#declare CS_stone_bishop_cap_miter_mid_radius   = CS_stone_bishop_cap_miter_mid_d/2;
#declare CS_stone_bishop_cap_tower_offset       = ChordRadius2Height(2*CS_stone_bishop_tower_cap_radius,CS_stone_bishop_cap_sphere_radius);
#declare CS_stone_bishop_cap_height             = CS_stone_bishop_cap_sphere_height + CS_stone_bishop_cap_miter_height - CS_stone_bishop_cap_tower_offset;

#declare CS_stone_bishop_height                 = CS_stone_piece_base_height + CS_stone_bishop_tower_height + CS_stone_bishop_cap_height;
#declare CS_stone_bishop_radius                 = max(
    CS_stone_piece_base_mradius,
    CS_stone_bishop_tower_base_radius,
    CS_stone_bishop_tower_mid_radius,
    CS_stone_bishop_tower_top_radius,
    CS_stone_bishop_tower_cap_radius,
    CS_stone_bishop_cap_sphere_radius
);

// Queen
#declare CS_stone_queen_tower_base_d            = Math_Scale(SCALE_CM, 2.2);
#declare CS_stone_queen_tower_base_radius       = CS_stone_queen_tower_base_d/2;
#declare CS_stone_queen_tower_base_height       = Math_Scale(SCALE_MM, 2);
#declare CS_stone_queen_tower_mid_d             = Math_Scale(SCALE_CM, 1.35);
#declare CS_stone_queen_tower_mid_radius        = CS_stone_queen_tower_mid_d/2;
#declare CS_stone_queen_tower_mid_height        = Math_Scale(SCALE_CM, 1.8);
#declare CS_stone_queen_tower_top_d             = Math_Scale(SCALE_CM, 1.5);
#declare CS_stone_queen_tower_top_radius        = CS_stone_queen_tower_top_d/2;
#declare CS_stone_queen_tower_cap_height        = Math_Scale(SCALE_CM, 0.25);
#declare CS_stone_queen_tower_cap_d             = Math_Scale(SCALE_CM, 1.35);
#declare CS_stone_queen_tower_cap_radius        = CS_stone_queen_tower_cap_d/2;
#declare CS_stone_queen_tower_height            = Math_Scale(SCALE_CM, 3.25);
#declare CS_stone_queen_tower_rnd               = Math_Scale(SCALE_MM, 0.15);

#declare CS_stone_queen_cap_base_cyl_d          = Math_Scale(SCALE_CM, 1.8);
#declare CS_stone_queen_cap_base_cyl_radius     = CS_stone_queen_cap_base_cyl_d/2;
#declare CS_stone_queen_cap_base_cyl_rnd        = Math_Scale(SCALE_MM, 3);
#declare CS_stone_queen_cap_base_cyl_height     = Math_Scale(SCALE_CM, 1);
#declare CS_stone_queen_cap_crown_bottom_height = Math_Scale(SCALE_CM, 0.5);
#declare CS_stone_queen_cap_crown_top_d         = Math_Scale(SCALE_CM, 1.85);
#declare CS_stone_queen_cap_crown_top_radius    = CS_stone_queen_cap_crown_top_d/2;
#declare CS_stone_queen_cap_crown_cone_height   = Math_Scale(SCALE_CM, 0.5);
#declare CS_stone_queen_cap_crown_cone_d        = Math_Scale(SCALE_CM, 0.3);
#declare CS_stone_queen_cap_crown_cone_radius   = CS_stone_queen_cap_crown_cone_d/2;
#declare CS_stone_queen_cap_height              = CS_stone_queen_cap_base_cyl_height + CS_stone_queen_cap_crown_bottom_height + CS_stone_queen_cap_crown_cone_height;

#declare CS_stone_queen_height                  = CS_stone_piece_base_height + CS_stone_queen_tower_height + CS_stone_queen_cap_height;
#declare CS_stone_queen_radius                  = max(
    CS_stone_piece_base_mradius,
    CS_stone_queen_tower_base_radius,
    CS_stone_queen_tower_mid_radius,
    CS_stone_queen_tower_top_radius,
    CS_stone_queen_tower_cap_radius,
    CS_stone_queen_cap_base_cyl_radius,
    CS_stone_queen_cap_crown_top_radius,
    CS_stone_queen_cap_crown_cone_radius
);    

// King
#declare CS_stone_king_tower_base_d             = Math_Scale(SCALE_CM, 2.2);
#declare CS_stone_king_tower_base_radius        = CS_stone_king_tower_base_d/2;
#declare CS_stone_king_tower_base_height        = Math_Scale(SCALE_MM, 2);
#declare CS_stone_king_tower_mid_d              = Math_Scale(SCALE_CM, 1.3);
#declare CS_stone_king_tower_mid_radius         = CS_stone_king_tower_mid_d/2;
#declare CS_stone_king_tower_mid_height         = Math_Scale(SCALE_CM, 1.45);
#declare CS_stone_king_tower_top_d              = Math_Scale(SCALE_CM, 1.6);
#declare CS_stone_king_tower_top_radius         = CS_stone_king_tower_top_d/2;
#declare CS_stone_king_tower_cap_height         = Math_Scale(SCALE_CM, 0.2);
#declare CS_stone_king_tower_cap_d              = Math_Scale(SCALE_CM, 1.5);
#declare CS_stone_king_tower_cap_radius         = CS_stone_king_tower_cap_d/2;
#declare CS_stone_king_tower_height             = Math_Scale(SCALE_CM, 3.2);
#declare CS_stone_king_tower_rnd                = Math_Scale(SCALE_MM, 0.15);

#declare CS_stone_king_belt_height              = Math_Scale(SCALE_CM, 0.5);
#declare CS_stone_king_belt_radius              = CS_stone_king_tower_top_radius;
#declare CS_stone_king_belt_thickness           = Math_Scale(SCALE_MM, 2);
#declare CS_stone_king_belt_offset              = Math_Scale(SCALE_MM, 0.5);

#declare CS_stone_king_cap_tower_height         = Math_Scale(SCALE_CM, 1.9);
#declare CS_stone_king_cap_tower_mid_d          = Math_Scale(SCALE_CM, 1.2);
#declare CS_stone_king_cap_tower_mid_radius     = CS_stone_king_cap_tower_mid_d/2; 
#declare CS_stone_king_cap_tower_mid_height     = Math_Scale(SCALE_CM, 0.6);
#declare CS_stone_king_cap_tower_top_d          = Math_Scale(SCALE_CM, 1.9);
#declare CS_stone_king_cap_tower_top_radius     = CS_stone_king_cap_tower_top_d/2;
#declare CS_stone_king_cap_cone_height          = Math_Scale(SCALE_CM, 0.3); 
#declare CS_stone_king_cap_cone_d               = Math_Scale(SCALE_CM, 0.7);
#declare CS_stone_king_cap_cone_radius          = CS_stone_king_cap_cone_d/2;
#declare CS_stone_king_cap_miter_cone_height    = Math_Scale(SCALE_CM, 0.185);
#declare CS_stone_king_cap_miter_offset         = Math_Scale(SCALE_CM, 0.1);
#declare CS_stone_king_cap_miter_height         = 2*CS_stone_king_cap_miter_cone_height - CS_stone_king_cap_miter_offset;
#declare CS_stone_king_cap_miter_d              = Math_Scale(SCALE_CM, 0.7);
#declare CS_stone_king_cap_miter_radius         = CS_stone_king_cap_miter_d/2;
#declare CS_stone_king_cap_miter_top_radius     = Math_Scale(SCALE_MM, 0.25);

#declare CS_stone_king_cap_height               = CS_stone_king_cap_tower_height + CS_stone_king_cap_cone_height + CS_stone_king_cap_miter_height;

#declare CS_stone_king_height                   = CS_stone_piece_base_height + CS_stone_king_tower_height + CS_stone_king_cap_height;
#declare CS_stone_king_radius                   = max(
    CS_stone_piece_base_mradius,
    CS_stone_king_tower_base_radius,
    CS_stone_king_tower_mid_radius,
    CS_stone_king_tower_top_radius,
    CS_stone_king_tower_cap_radius,
    CS_stone_king_cap_height
);

// Knight
//
#declare CS_stone_knight_base_d                 = Math_Scale(SCALE_CM, 2.3);
#declare CS_stone_knight_base_radius            = CS_stone_knight_base_d/2;
#declare CS_stone_knight_base_height            = Math_Scale(SCALE_MM, 3);
#declare CS_stone_knight_base_cone_height       = Math_Scale(SCALE_MM, 3.5);
#declare CS_stone_knight_head_height            = Math_Scale(SCALE_CM, 4.2);
#declare CS_stone_knight_head_depth             = Math_Scale(SCALE_CM, 2.5);
#declare CS_stone_knight_head_base_width        = Math_Scale(SCALE_CM, 1.18);
#declare CS_stone_knight_head_top_width         = Math_Scale(SCALE_CM, 1.2);
#declare CS_stone_knight_base_cone_radius       = CS_stone_knight_head_base_width/2;
#declare CS_stone_knight_nose_width             = Math_Scale(SCALE_CM, 0.5);
#declare CS_stone_knight_mane_width             = Math_Scale(SCALE_CM, 0.25);
#declare CS_stone_knight_head_rnd               = Math_Scale(SCALE_MM, 1);

#declare CS_stone_knight_height                 = CS_stone_piece_base_height + CS_stone_knight_base_height + CS_stone_knight_head_height;
#declare CS_stone_knight_radius                 = max(
    CS_stone_piece_base_mradius,
    CS_stone_knight_base_radius
);    

#declare CS_stone_pawn_tower_base_d             = Math_Scale(SCALE_CM, 2.0);
#declare CS_stone_pawn_tower_base_radius        = CS_stone_pawn_tower_base_d/2;
#declare CS_stone_pawn_tower_base_height        = Math_Scale(SCALE_MM, 1.5);
#declare CS_stone_pawn_tower_mid_d              = Math_Scale(SCALE_CM, 0.9);
#declare CS_stone_pawn_tower_mid_radius         = CS_stone_pawn_tower_mid_d/2;
#declare CS_stone_pawn_tower_mid_height         = Math_Scale(SCALE_CM, 1.35);
#declare CS_stone_pawn_tower_top_d              = Math_Scale(SCALE_CM, 1.0);
#declare CS_stone_pawn_tower_top_radius         = CS_stone_pawn_tower_top_d/2;
#declare CS_stone_pawn_tower_cap_d              = Math_Scale(SCALE_CM, 0.6);
#declare CS_stone_pawn_tower_cap_radius         = CS_stone_pawn_tower_cap_d/2;
#declare CS_stone_pawn_tower_cap_height         = Math_Scale(SCALE_MM, 1);
#declare CS_stone_pawn_tower_rnd                = Math_Scale(SCALE_MM, 0.1);
#declare CS_stone_pawn_tower_height             = Math_Scale(SCALE_CM, 2.15);

#declare CS_stone_pawn_cap_d                    = Math_Scale(SCALE_CM, 1);
#declare CS_stone_pawn_cap_radius               = CS_stone_pawn_cap_d/2;
#declare CS_stone_pawn_cap_tower_offset         = ChordRadius2Height(2*CS_stone_pawn_tower_cap_radius,CS_stone_pawn_cap_radius);
#declare CS_stone_pawn_cap_height               = Math_Scale(SCALE_CM, 0.7) + CS_stone_pawn_cap_tower_offset;

#declare CS_stone_pawn_height                   = CS_stone_pawn_base_height + CS_stone_pawn_tower_height + CS_stone_pawn_cap_height;
#declare CS_stone_pawn_radius                   = max(
    CS_stone_pawn_base_mradius,
    CS_stone_pawn_tower_base_radius,
    CS_stone_pawn_tower_mid_radius,
    CS_stone_pawn_tower_top_radius,
    CS_stone_pawn_tower_cap_radius,
    CS_stone_pawn_cap_radius
);

// End Chess Set Dimensions
//=============================================================================

//=============================================================================
// CS_stone_board()
//

//-----------------------------------------------------------------------------
// CS_stone_board_shape_fn()
//
#macro CS_stone_board_shape_fn()
    #local _corner_r            = CS_stone_board_corner_rnd - CS_stone_board_edge_rnd;
    #local _bh                  = CS_stone_board_size.y - 2*CS_stone_board_edge_rnd;
    
    #local _cbs_corner_fn       = Iso_fixed_cylinder(_corner_r,_bh)
    #local _cb_size             = <CS_stone_board_size.x - 2*CS_stone_board_edge_rnd, _bh, CS_stone_board_size.z - 2*(CS_stone_board_corner_rnd + CS_stone_board_edge_rnd)>;
    #local _cbs_center_box_fn   = Iso_fixed_box(_cb_size)
    #local _fb_size             = <_cb_size.x - 2*_corner_r, _bh, _corner_r>;
    #local _cbs_front_box_fn    = Iso_fixed_box(_fb_size)
    #local _fb_offset           = _cb_size.z/2 + _fb_size.z/2;
    #local _corner_x_offset     = _cb_size.x/2 - _corner_r;
    #local _corner_z_offset     = _cb_size.z/2;
    
    #local _cbs_preround_shape_fn   = function(x,y,z) {
        min(
            _cbs_center_box_fn(x,y,z),
            _cbs_front_box_fn(x,y,z-_fb_offset),
            _cbs_front_box_fn(x,y,z+_fb_offset),
            _cbs_corner_fn(x+_corner_x_offset, y, z+_corner_z_offset),
            _cbs_corner_fn(x-_corner_x_offset, y, z+_corner_z_offset),
            _cbs_corner_fn(x+_corner_x_offset, y, z-_corner_z_offset),
            _cbs_corner_fn(x-_corner_x_offset, y, z-_corner_z_offset)
        )    
    }
    
    #local _cbs_rounding    = Iso_rounding(_cbs_preround_shape_fn)
    
    #local _edge_rnd_noise_size     = 0.25*CS_stone_board_edge_rnd;
    #local _cbs_rnd_noise_pfn       = function {
        pigment {
            wrinkles
            color_map {
                [0.0 rgb 0]
                [0.7 rgb 0]
                [1.0 rgb 1]
            }
            scale Math_Scale(SCALE_MM, <1,5,1>)
        }
    }
    #local _cbs_rnd_noise_fn    = function(x,y,z) {
        _cbs_rnd_noise_pfn(x,y,z).gray * _edge_rnd_noise_size
    }                
    #local _ex                      = CS_stone_board_size.x/2 - 1*CS_stone_board_edge_rnd;
    #local _ey                      = CS_stone_board_size.y/2 - 1*CS_stone_board_edge_rnd;
    #local _ez                      = CS_stone_board_size.z/2 - 1.5*CS_stone_board_edge_rnd;
    #local _ec                      = _corner_r - 1*CS_stone_board_edge_rnd;
    
    #local _cbs_edge_size_fn    = function(x,y,z) {
        CS_stone_board_edge_rnd -
            select(_ey - abs(y),
                select(_ex - abs(x),
                    _cbs_rnd_noise_fn(x,y,z),
                    select(_ez - abs(z),
                        _cbs_rnd_noise_fn(x,y,z),
                        select(
                            max(
                                _ec - sqrt(pow(abs(x) - _corner_x_offset,2) + pow(abs(z) - _corner_z_offset,2)),
                                _ex - abs(x),
                                _ez - abs(z)
                            ),    
                            _cbs_rnd_noise_fn(x,y,z),
                            0
                        )    
                    )
                ),
                0        
            )
    }
    
    function(x,y,z) {
        _cbs_rounding(x,y,z,_cbs_edge_size_fn(x,y,z))
    }    
#end

// End CS_stone_board_shape_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_board_marble_finish()
//
#macro CS_stone_board_marble_finish()
    #local _refl    = 0.4;
    
    #local _f   = finish {
        fresnel
        specular albedo 1.0
        roughness 0.01
        diffuse albedo 1.0
        reflection { _refl }
    }
    
    _f
#end

// End CS_stone_board_marble_finish
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_board_outer_marble_texture()
//
#macro CS_stone_board_outer_marble_texture()
    #local _c_black     = rgb <0.001, 0.0005, 0.0005>;
    #local _c_deep_red  = rgb <0.04, 0.0075, 0.002>;
    #local _c_light_tan = rgb <0.44, 0.3, 0.15>;
    #local _c_gray_tan  = (0.3 + _c_light_tan)/2;
    
    #local _t_finish    = texture {
        pigment { color rgbf 1 }
        finish { CS_stone_board_marble_finish() }
    }    
    
    #local _t_base_texture  = texture {
        pigment {
            bozo
            color_map {
                [0.0 _c_deep_red]
                [0.6 _c_deep_red]
                //[0.8 _c_light_tan]
                [1.0 _c_light_tan]
            }
            warp { turbulence 1.6 lambda 3 }
            scale 10
        }        
    }
    #local _t_gray_texture  = texture {
        pigment {
            agate
            color_map {
                [0.0 _c_gray_tan transmit 1.0]
                [0.995 _c_gray_tan transmit 1.0]
                [1.0 _c_gray_tan transmit 0.5]
            }
            warp { turbulence 0.5 }
            scale 10
        }        
    }
    #local _t_black_texture = texture {
        pigment {
            marble
            color_map {
                [0.0 _c_black transmit 1.0]
                [0.9 _c_black transmit 1.0]
                [1.0 _c_black transmit 0.1]
            }
            warp { turbulence 2.1 }
            scale 7
        }
    }            
    
    #local _t   = texture { _t_base_texture }
    texture { _t_gray_texture }
    texture { _t_black_texture }
    texture { _t_finish }
    ;
    
    _t
    
#end

// End CS_stone_board_outer_marble_texture
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_board_white_square_texture()
//
#macro CS_stone_board_white_square_texture()
    #local _c_light_tan = rgb <0.82, 0.8, 0.7>;
    #local _c_med_tan   = rgb <0.65, 0.6, 0.4>;
    #local _c_dark_tan  = rgb <0.33, 0.2, 0.07>;
    #local _c_gray_tan  = (0.3 + _c_light_tan)/2;
    #local _c_gray      = rgb 0.2;
    
    #local _t_finish    = texture {
        pigment { color rgbf 1 }
        finish { CS_stone_board_marble_finish() }
    }    
    
    #local _t_base_texture  = texture {
        pigment {
            bozo
            color_map {
                [0.0 _c_light_tan]
                [0.7 _c_med_tan]
                [0.9 _c_dark_tan]
                [1.0 _c_gray_tan]
            }
            warp { turbulence 2.1 lambda 4 omega 0.6 }
            scale 0.5
        }        
    }
    
    #local _p_dark_tan  = pigment {
        granite
        color_map {
            [0.0 _c_dark_tan transmit 1.0]
            [0.8 _c_dark_tan transmit 1.0]
            [0.8 _c_dark_tan transmit 0.0]
            [1.0 _c_gray_tan transmit 0.0]
        }
        scale CS_stone_board_square_size.x
    }
    ;
    
    #local _p_clear = pigment { color rgbf 1 }
    
    #local _t_dark_texture  = texture {
        pigment {
            pigment_pattern {
                granite
                color_map {
                    [0.0 rgb 0]
                    [0.5 rgb 0]
                    [1.0 rgb 1]
                }
                scale CS_stone_board_square_size.x/8
            }
            pigment_map {
                [0.0 _p_clear]
                [1.0 _p_dark_tan]
            }
        }                
    }            
    
    #local _t_gray_texture  = texture {
        pigment {
            wrinkles
            color_map {
                [0.0 _c_gray_tan transmit 1.0]
                [0.5 _c_gray_tan transmit 1.0]
                [0.7 _c_gray_tan transmit 0.2]
                [1.0 _c_gray transmit 0.1]
            }
            warp { turbulence 1.1 lambda 4 }
            scale CS_stone_board_square_size.x/10
        }        
    }
    
    #local _t   = texture { _t_base_texture }
    texture { _t_dark_texture }
    texture { _t_gray_texture }
    texture { _t_finish }
    ;
    
    _t
    
#end

// End CS_stone_board_white_square_texture
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_board_black_square_texture()
//
#macro CS_stone_board_black_square_texture()
    #local _c_black     = rgb <0.001, 0.0005, 0.0005>;
    #local _c_deep_red  = rgb <0.04, 0.0075, 0.002>;
    #local _c_light_tan = rgb <0.44, 0.3, 0.15>;
    #local _c_gray_tan  = (0.3 + _c_light_tan)/2;
    
    #local _t_finish    = texture {
        pigment { color rgbf 1 }
        finish { CS_stone_board_marble_finish() }
    }    
    
    #local _t_base_texture  = texture {
        pigment {
            bozo
            color_map {
                [0.0 _c_deep_red]
                [0.6 _c_deep_red]
                //[0.8 _c_light_tan]
                [1.0 _c_light_tan]
            }
            translate CS_stone_board_size
            warp { turbulence 1.6 lambda 3 }
            scale 10
        }        
    }
    #local _t_gray_texture  = texture {
        pigment {
            agate
            color_map {
                [0.0 _c_gray_tan transmit 1.0]
                [0.995 _c_gray_tan transmit 1.0]
                [1.0 _c_gray_tan transmit 0.5]
            }
            translate CS_stone_board_size*2
            warp { turbulence 0.5 }
            scale 10
        }        
    }
    #local _t_black_texture = texture {
        pigment {
            marble
            color_map {
                [0.0 _c_black transmit 1.0]
                [0.9 _c_black transmit 1.0]
                [1.0 _c_black transmit 0.1]
            }
            translate CS_stone_board_size*3
            warp { turbulence 2.1 }
            scale 7
        }
    }            
    
    #local _t   = texture { _t_base_texture }
    texture { _t_gray_texture }
    texture { _t_black_texture }
    texture { _t_finish }
    ;
    
    _t
    
#end

// End CS_stone_board_black_square_texture
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_board_game_texture()
//
#macro CS_stone_board_game_texture()
    #local _stream  = seed(2025011401);
    
    #local _tmap    = texture_map {
        #for (r, 0, 7, 1)
            #for (c, 0, 7, 1)
                #local _i   = (8*r + c)/64;
                #local _ti  = mod(mod(r,2)+mod(c,2),2);
                #if (_ti = 1)
                    #local _tc  = texture { CS_stone_board_white_square_texture() translate 97731*<rand(_stream),rand(_stream),rand(_stream)> }

                #else
                    #local _tc  = texture { CS_stone_board_black_square_texture() translate 97731*<rand(_stream),rand(_stream),rand(_stream)> }
                #end
                [_i _tc]
            #end
        #end    
    }
    
    #local _ox  = 4*CS_stone_board_square_size.x;
    #local _oz  = 4*CS_stone_board_square_size.y;
    
    #local _cx  = CS_stone_board_square_size.x;
    #local _cz  = CS_stone_board_square_size.y;
    
    #local _idx_fn  = function(x,z) {
        (8*f_clamp(int((z + _oz)/_cz),0,7) + f_clamp(int((x + _ox)/_cx),0,7))/64    
    }
    
    #local _t   = texture {
        pigment_pattern {
            function {
                _idx_fn(x,z)    
            }
        }
        texture_map { _tmap }
    }
    ;
    
    _t
#end

// End CS_stone_board_game_texture
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_board_material()
//
#macro CS_stone_board_material()
    
    #local _t_outer_marble  = texture { CS_stone_board_outer_marble_texture() }
    #local _t_board         = texture { CS_stone_board_game_texture() }
    
    #local _ss              = <8,8>*CS_stone_board_square_size;
    #local _sx              = _ss.x/2;
    #local _sz              = _ss.y/2;
    
    #local _t   = texture {
        pigment_pattern {
            function {
                select(
                    max(abs(x) - _sx, abs(z) - _sz),
                    1,
                    0
                )    
            }    
        }
        texture_map {
            [0.0 _t_outer_marble]
            [1.0 _t_board]
        }    
    }
    
    #local _m   = material {
        texture { _t }
        interior { ior 1.5 }
    }
    
    _m    
    
#end

// End CS_stone_board_material
//-----------------------------------------------------------------------------

#macro CS_stone_board()
    #local _lbounds = -CS_stone_board_size/2 - CS_stone_board_edge_rnd;
    #local _ubounds = CS_stone_board_size/2 + CS_stone_board_edge_rnd;
    #local _cmb_shape_fn    = CS_stone_board_shape_fn()
    
    #local _board   = isosurface {
        function { _cmb_shape_fn(x,y,z) }
        threshold 0
        contained_by { box { _lbounds, _ubounds } }
        accuracy 0.001
        max_gradient 17
        
        material { CS_stone_board_material() }
        
        translate <0, CS_stone_board_size.y/2, 0>
    }
    
    _board
#end

// End CS_stone_board
//=============================================================================

//=============================================================================
// CS_stone_felt_disc(Radius,RStream)
//

//-----------------------------------------------------------------------------
// CS_stone_felt_material(RStream)
//
#macro CS_stone_felt_material(RStream)
    #local _m   = material {
        texture {           
            pigment {
                granite
                color_map {
                    [0.0 color rgb <0.01, 0.125, 0.05>]
                    [1.0 color rgb <0.02, 0.125, 0.02>]
                }
                scale Math_Scale(SCALE_MM, 1)    
            }
            normal { bumps 0.1 scale 0.01 }
            finish {
                fresnel
                specular albedo 1.0
                roughness 0.5
                diffuse albedo 1.0
            }        
        }
        interior { ior 1.5 }
    }
    
    _m
#end

// End CS_stone_felt_material
//-----------------------------------------------------------------------------


#macro CS_stone_felt_disc(Radius,RStream) 
    #local _cmfd_disc_fn    = Iso_fixed_rounded_cylinder(Radius,0.5*CS_stone_felt_disc_thickness,0.45*CS_stone_felt_disc_thickness)
    
    #local _wrinkle_size    = Math_Scale(SCALE_MM, 0.1);
    #local _cmfd_wrinkle_pfn    = function {
        pigment {
            wrinkles
            color_map {
                [0.0 rgb 0]
                [1.0 rgb 1]
            }
            warp { turbulence 1.1 }
            scale Math_Scale(SCALE_MM, <0.15, 0.15, 1>)
            translate 88321*<rand(RStream),rand(RStream),rand(RStream)>
        }
    }
    
    #local _thread_size     = Math_Scale(SCALE_MM, 0.1);
    #local _cmfd_thread_pfn = function {
        pigment {
            wrinkles
            color_map {
                [0.0 rgb 0]
                [0.5 rgb 0]
                [1.0 rgb 1]
            }
            rotate <0, 5, 0>
            scale Math_Scale(SCALE_MM, <0.05, 0.05, 0.5>)
            translate 88321*<rand(RStream),rand(RStream),rand(RStream)>
        }
    }                    
    
    #local _cmfd_shape_fn   = function(x,y,z) {
        _cmfd_disc_fn(x,y,z) - 2*(0.5 - _cmfd_wrinkle_pfn(x,y,z).gray) * _wrinkle_size * (1 + _cmfd_thread_pfn(x,y,z).gray * _thread_size)
    }
    
    #local _ext = _wrinkle_size + _thread_size;                
    
    #local _lbounds = <-Radius, -CS_stone_felt_disc_thickness/2, -Radius> - _ext;
    #local _ubounds = <Radius, CS_stone_felt_disc_thickness/2, Radius> + _ext;
    
    #local _felt    = isosurface {
        function { _cmfd_shape_fn(x,y,z) }
        threshold 0
        contained_by { box { _lbounds, _ubounds } }
        accuracy 0.001
        max_gradient 2
        
        material { CS_stone_felt_material(RStream) }
        translate <0, CS_stone_felt_disc_thickness/2, 0>
    }
    
    _felt
#end

// End CS_stone_felt_disc
//=============================================================================

//=============================================================================
// CS_stone_piece_base_fn(RStream)
// CS_stone_pawn_base_fn(RStream)
//

#macro CS_stone_piece_base_fn(RStream)
    #local _rnd     = Math_Scale(SCALE_MM, 1);
    #local _h       = CS_stone_piece_base_height;
    #local _rdelta  = CS_stone_piece_base_mradius - CS_stone_piece_base_radius;
    #local _tminor  = ChordHeight2Radius(_h,_rdelta);
    #local _tmajor  = CS_stone_piece_base_mradius - _tminor;
    
    #local _cmpb_torus_fn   = Iso_fixed_torus(_tmajor,_tminor)
    #local _cmpb_bounds_fn  = Iso_fixed_capped_cylinder(CS_stone_piece_base_mradius,_h)
    
    #local _cmpb_cut_torus_fn   = function(x,y,z) {
        Iso_intersection(
            _cmpb_torus_fn(x,y,z),
            _cmpb_bounds_fn(x,y,z)
        )
    }
    
    #local _cmpb_core_fn    = Iso_fixed_capped_cylinder(CS_stone_piece_base_radius,CS_stone_piece_base_height)
    
    #local _cmpb_shape_fn   = function(x,y,z) {
        Iso_union(
            _cmpb_cut_torus_fn(x,y,z),
            _cmpb_core_fn(x,y,z)
        )
    }
    
    function(x,y,z) {
        _cmpb_shape_fn(x,y-_h/2,z)
    }    
    
#end

#macro CS_stone_pawn_base_fn(RStream)
    #local _rnd     = Math_Scale(SCALE_MM, 1);
    #local _h       = CS_stone_pawn_base_height;
    #local _rdelta  = CS_stone_pawn_base_mradius - CS_stone_pawn_base_radius;
    #local _tminor  = ChordHeight2Radius(_h,_rdelta);
    #local _tmajor  = CS_stone_pawn_base_mradius - _tminor;
    
    #local _cmpb_torus_fn   = Iso_fixed_torus(_tmajor,_tminor)
    #local _cmpb_bounds_fn  = Iso_fixed_capped_cylinder(CS_stone_pawn_base_mradius,_h)
    
    #local _cmpb_cut_torus_fn   = function(x,y,z) {
        Iso_intersection(
            _cmpb_torus_fn(x,y,z),
            _cmpb_bounds_fn(x,y,z)
        )
    }
    
    #local _cmpb_core_fn    = Iso_fixed_capped_cylinder(CS_stone_pawn_base_radius,CS_stone_pawn_base_height)
    
    #local _cmpb_shape_fn   = function(x,y,z) {
        Iso_union(
            _cmpb_cut_torus_fn(x,y,z),
            _cmpb_core_fn(x,y,z)
        )
    }
    
    function(x,y,z) {
        _cmpb_shape_fn(x,y-_h/2,z)
    }    
    
#end

//-----------------------------------------------------------------------------
// CS_stone_piece_base_object(RStream)
//
#macro CS_stone_piece_base_object(RStream)
    #local _cmpbo_shape_fn  = CS_stone_piece_base_fn(RStream)
    
    #local _lbounds = <-CS_stone_piece_base_mradius, 0, -CS_stone_piece_base_mradius>;
    #local _ubounds = <CS_stone_piece_base_mradius, CS_stone_piece_base_height, CS_stone_piece_base_radius>;
    
    #local _base    = isosurface {
        function { _cmpbo_shape_fn(x,y,z) }
        threshold 0
        contained_by { box { _lbounds, _ubounds } }
        accuracy 0.001
        max_gradient 1.1
        
        translate <0, CS_stone_piece_base_height/2, 0>
    }
    
    _base                          
    
#end
          
// End CS_stone_piece_base_object
//-----------------------------------------------------------------------------


// End CS_stone_piece_base_fn
//=============================================================================

//=============================================================================
// CM Piece Materials
//

//-----------------------------------------------------------------------------
// CS_stone_piece_white_texture(RStream)
//
#macro CS_stone_piece_white_texture(RStream)
    #local _c_light_tan = rgb <0.82, 0.8, 0.7>;
    #local _c_med_tan   = rgb <0.65, 0.6, 0.4>;
    #local _c_dark_tan  = rgb <0.33, 0.2, 0.07>;
    #local _c_vdark_tan = rgb <0.033, 0.02, 0.007>;
    
    #local _c_gray_tan  = (0.3 + _c_light_tan)/2;
    #local _c_gray      = rgb 0.01;
    
    #local _key     = 91725;
    
    #local _t_base_texture  = texture {
        pigment {
            bozo
            color_map {
                [0.0 _c_light_tan]
                [0.7 _c_med_tan]
                [0.9 _c_dark_tan]
                [1.0 _c_gray_tan]
            }
            warp { turbulence 2.1 lambda 4 omega 0.6 }
            scale 0.5
            translate _key*<rand(RStream),rand(RStream),rand(RStream)>
        }        
    }
    
    #local _p_dark_tan  = pigment {
        granite
        color_map {
            [0.0 _c_dark_tan transmit 1.0]
            [0.8 _c_dark_tan transmit 1.0]
            [0.8 _c_dark_tan transmit 0.0]
            [1.0 _c_gray_tan transmit 0.0]
        }
        scale CS_stone_board_square_size.x
        translate _key*<rand(RStream),rand(RStream),rand(RStream)>
    }
    ;
    
    #local _p_clear = pigment { color rgbf 1 }
    
    #local _t_dark_texture  = texture {
        pigment {
            pigment_pattern {
                granite
                color_map {
                    [0.0 rgb 0]
                    [0.5 rgb 0]
                    [1.0 rgb 1]
                }
                scale CS_stone_board_square_size.x/8
                translate _key*<rand(RStream),rand(RStream),rand(RStream)>
            }
            pigment_map {
                [0.0 _p_clear]
                [1.0 _p_dark_tan]
            }
        }                
    }
    
    #local _t_vdark_texture = texture {
        pigment {
            agate
            color_map {
                [0.0 _c_vdark_tan transmit 1.0]
                [0.95 _c_vdark_tan transmit 1.0]
                [0.975 _c_vdark_tan transmit 0.8]
                [1.0 _c_vdark_tan transmit 0.3]    
            }
            warp { turbulence 1.1 }                                  
            scale 0.25
            translate _key*<rand(RStream),rand(RStream),rand(RStream)>
        }
    }        
    
    #local _t_gray_texture  = texture {
        pigment {
            wrinkles
            color_map {
                [0.0 _c_gray_tan transmit 1.0]
                [0.5 _c_gray_tan transmit 1.0]
                [0.7 _c_gray_tan transmit 0.2]
                [1.0 _c_gray transmit 0.1]
            }
            warp { turbulence 1.1 lambda 4 }
            scale CS_stone_board_square_size.x/10
            translate _key*<rand(RStream),rand(RStream),rand(RStream)>
        }        
    }
    
    #local _t   = texture { _t_base_texture }
    texture { _t_dark_texture }
    texture { _t_gray_texture }
    texture { _t_vdark_texture }
    texture {
        pigment { color rgbf 1 }
        finish {
            fresnel
            specular albedo 1.0
            roughness 0.005
            diffuse albedo 1.0
            reflection { 0.1 }
        }
    }        
    ;
    
    _t
    
#end

// End CS_stone_piece_white_texture
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_piece_black_texture(RStream)
//
#macro CS_stone_piece_black_texture(RStream)
    #local _c_black     = rgb <0.001, 0.0005, 0.0005>;
    #local _c_deep_red  = rgb <0.08, 0.015, 0.004>;
    #local _c_light_tan = rgb <0.44, 0.3, 0.15>;
    #local _c_gray_tan  = (0.3 + _c_light_tan)/2;
    
    #local _t   = texture {
        pigment { color _c_deep_red }
    }    
    texture {
        pigment {
            wood
            color_map {
                [0.0 _c_light_tan transmit 1]
                [0.8 _c_light_tan transmit 1]
                [1.0 _c_light_tan transmit 0.8]    
            }
            scale 2 warp { turbulence 0.1 lambda 3 omega 0.5 } scale 1/2
            scale CS_stone_piece_base_radius/4
            rotate <-15, 0, 0>
            translate CS_stone_piece_base_radius*<rand(RStream),rand(RStream),10*rand(RStream)>
            rotate <-90, 90, 0>
        }
    }
    texture {
        pigment {
            wood
            color_map {
                [0.0 _c_gray_tan transmit 1]
                [0.9 _c_gray_tan transmit 1]
                [1.0 _c_gray_tan transmit 0.8]    
            }
            scale 2 warp { turbulence 0.4 lambda 4 omega 0.7 } scale 1/2
            scale CS_stone_piece_base_radius/2
            rotate <-15, 0, 0>
            translate CS_stone_piece_base_radius*<rand(RStream),rand(RStream),10*rand(RStream)>
            rotate <-90, 0, 0>
        }
    }
    texture {
        pigment { color rgbf 1 }
        finish {
            fresnel
            specular albedo 1.0
            roughness 0.005
            diffuse albedo 1.0
            reflection { 0.1 }
        }
    }        
    ;
    
    _t
#end

// End CS_stone_piece_black_texture
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_piece_texture(Player,RStream)
//
#macro CS_stone_piece_texture(Player,RStream)
    #switch(Player)
        #case(Chess_player_white)
            #local _t   = texture { CS_stone_piece_white_texture(RStream) };
        #break
        
        #case(Chess_player_black)                                    
            #local _t   = texture { CS_stone_piece_black_texture(RStream) };
        #break
        
        #default
            #error concat("Illegal piece player ", str(Player, 0, 0), "\n")
        #break    
    #end
    
    _t
#end

// End CS_stone_piece_texture
//-----------------------------------------------------------------------------

#declare CS_stone_piece_no_cracks_fn  = function(x,y,z) { 0 }

//-----------------------------------------------------------------------------
// CS_stone_white_piece_cracks_fn(Scale,RStream)
//

#macro CS_stone_white_piece_cracks_fn(Scale,RStream)
    #local _d           = Math_Scale(SCALE_MM, 0.5);
    
    #local _p_cracks    = pigment {
        agate
        color_map {
            [0.0 rgb 0]
            [0.9995 rgb 0]
            [0.99975 rgb 1]
            [1.0 rgb 1]
        }
        translate 11973*<rand(RStream),rand(RStream),rand(RStream)>
        warp { turbulence 1.1 lambda 2.5 omega 0.6 }
        scale Scale    
    }
    
    #local _cmwpc_cracks_fn   = function {
        pigment { _p_cracks }
    }
    
    #local _d_noise_scale   = <1,1,1>/Math_Scale(SCALE_MM, 0.1);
    #local _d_noise_fn      = Noise_create2(<0,0,0>, f_noise3d, 11973*<rand(RStream),rand(RStream),rand(RStream)>, _d_noise_scale, 0.5)
    #local _cmwpc_cracks_depth_fn   = function(x,y,z) {
        _d * (0.5 + _d_noise_fn(x,y,z))
    }         
    
    function(x,y,z) {
        _cmwpc_cracks_fn(x,y,z).gray * _cmwpc_cracks_depth_fn(x,y,z)
    }        
    
#end

// End CS_stone_white_piece_cracks_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_black_piece_cracks_fn(Scale,RStream)
//
#macro CS_stone_black_piece_cracks_fn(Scale,RStream)
    #local _d           = Math_Scale(SCALE_MM, 0.05);
    
    #local _p_cracks    = pigment {
        marble
        color_map {
            [0.0 rgb 0]
            [0.95 rgb 0]
            [1.0 rgb 1]
        }
        scale 5 warp { turbulence 0.5 } scale 1/5
        translate 93557*<rand(RStream),rand(RStream),rand(RStream)>
        scale Scale
        rotate <0, 0, 15*rand(RStream)>
    }
    
    #local _cmbpc_cracks_fn  = function {
        pigment { _p_cracks }
    }
    
    #local _depth_noise_scale       = <1,0.01,1>/Math_Scale(SCALE_MM, 1);
    #local _cmbpc_depth_noise_fn    = Noise_create2(<0,0,0>, f_snoise3d, 93557*<rand(RStream),rand(RStream),rand(RStream)>, _depth_noise_scale, 1.0)
    #local _cmbpc_depth_fn          = function(x,y,z) {
        max(0, _cmbpc_depth_noise_fn(x,y,0))*_d
    } 
    
    function {
        _cmbpc_cracks_fn(x,y,z).gray * _cmbpc_depth_fn(x,y,z)
    }                
#end

// End CS_stone_black_piece_cracks_fn
//-----------------------------------------------------------------------------

// End CM Piece Materials
//=============================================================================

//=============================================================================
// Common Piece Shape Functions
//

//-----------------------------------------------------------------------------
// CS_stone_body_cylinder(Height,BaseRadius,BaseHeight,MidRadius,MidHeight,TopRadius,CapRadius,CapHeight,Rnd)
//
#macro CS_stone_body_cylinder(Height,BaseRadius,BaseHeight,MidRadius,MidHeight,TopRadius,CapRadius,CapHeight,Rnd)
    
    #local _cmbc_cone_fn    = Iso_fixed_capped_cone(CapHeight,CapRadius,TopRadius)
    #local _cmbc_trans_cone_fn  = Translate_fn(_cmbc_cone_fn, <0, Height - CapHeight/2, 0>) 
    
    #local _cmbc_cyl_fn     = Iso_rounded_cylinder()
    #local _theight         = Height - CapHeight;
    #local _th              = _theight/2;

    #local _ybo = BaseHeight;
    #local _ymo = MidHeight;
    #local _bmr = BaseRadius - MidRadius;
    #local _bmh = _ymo - _ybo;
    #local _mgr = TopRadius - MidRadius;
    #local _mgh = _theight - _ymo;
    
    #local _cmbc_radius_fn  = function(x,y,z) {
        select(y - _ybo,
            BaseRadius,
            select(y - _ymo,
                MidRadius + _bmr*(pow((_ymo - y)/_bmh,2)),
                MidRadius + _mgr*(pow((y - _ymo)/_mgh,2))
            ) 
        )            
    }
    
    #local _cmbc_body_fn    = function(x,y,z) {
        _cmbc_cyl_fn(x,y-_th,z,_cmbc_radius_fn(x,y,z),_th,Rnd)
    }
    
    function(x,y,z) {
        Iso_union(
            _cmbc_body_fn(x,y,z),
            _cmbc_trans_cone_fn(x,y,z)
        )    
    }    
    
    
#end

// End CS_stone_body_cylinder
//-----------------------------------------------------------------------------


// End Common Piece Shape FUnctions
//=============================================================================

//=============================================================================
// CS_stone_king(Player,CracksFn,RStream)
//

#declare CS_stone_white_king_cracks_scale = 4*<CS_stone_king_tower_base_radius,CS_stone_king_height,CS_stone_king_tower_base_radius>;
#declare CS_stone_black_king_cracks_scale = 0.1*<CS_stone_king_tower_base_radius,CS_stone_king_height,CS_stone_king_tower_base_radius>;

//-----------------------------------------------------------------------------
// CS_stone_king_belt_shape_fn()
//
#macro CS_stone_king_belt_shape_fn()
    #local _cmkbs_cyl_fn        = Iso_fixed_capped_cylinder(CS_stone_king_tower_cap_radius,CS_stone_king_belt_height)
    #local _cmkbs_belt_cyl_fn   = Iso_fixed_rounded_cylinder(CS_stone_king_tower_top_radius, CS_stone_king_belt_height/2, 0.2*CS_stone_king_belt_height)
    
    #local _cmkbs_belt_fn   = function(x,y,z) {
        Iso_union(
            _cmkbs_cyl_fn(x,y,z),
            _cmkbs_belt_cyl_fn(x,y,z)
        )
    }        
    
    function(x,y,z) {
        _cmkbs_belt_fn(x,y-CS_stone_king_belt_height/2,z)
    }            
#end

// End CS_stone_king_belt_shape_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_king_cap_shape_fn()
//
#macro CS_stone_king_cap_shape_fn()
    #local _cmkcs_body_fn   = CS_stone_body_cylinder(CS_stone_king_cap_tower_height,CS_stone_king_tower_top_radius,0,CS_stone_king_cap_tower_mid_radius,CS_stone_king_cap_tower_mid_height,CS_stone_king_cap_tower_top_radius,0,0,CS_stone_king_tower_rnd)
    #local _cmkcs_cone_fn   = Iso_fixed_capped_cone(CS_stone_king_cap_cone_height,CS_stone_king_cap_cone_radius,CS_stone_king_cap_tower_top_radius)
    #local _cone_offset                 = CS_stone_king_cap_tower_height+CS_stone_king_cap_cone_height/2;
    
    #local _cmkcs_miter_bottom_cone_fn  = Iso_fixed_capped_cone(CS_stone_king_cap_miter_cone_height, CS_stone_king_cap_miter_radius, CS_stone_king_cap_miter_top_radius)
    #local _cmkcs_miter_top_cone_fn     = Iso_fixed_capped_cone(CS_stone_king_cap_miter_cone_height, CS_stone_king_cap_miter_top_radius, CS_stone_king_cap_miter_radius)
    #local _cmkcs_miter_union_fn        = Iso_fixed_smooth_union(0.01)
    
    #local _cmkcs_miter_fn  = function(x,y,z) {
        _cmkcs_miter_union_fn(
            _cmkcs_miter_bottom_cone_fn(x,y-CS_stone_king_cap_miter_cone_height/2,z),
            _cmkcs_miter_top_cone_fn(x,y-1.5*CS_stone_king_cap_miter_cone_height,z)
        )    
    }
    #local _miter_offset                = CS_stone_king_cap_tower_height + CS_stone_king_cap_cone_height - CS_stone_king_cap_miter_offset;
    #local _cmkcs_miter_crown_union_fn        = Iso_fixed_smooth_union(0.05)
    function(x,y,z) {
        Iso_union(
            _cmkcs_body_fn(x,y,z),
            _cmkcs_miter_crown_union_fn(
                _cmkcs_cone_fn(x,y-_cone_offset,z),
                _cmkcs_miter_fn(x,y-_miter_offset,z)
            )
        )        
    }
#end

// End CS_stone_king_cap_shape_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_king_shape_fn(CracksFn,RStream)
//
#macro CS_stone_king_shape_fn(CracksFn,RStream)
    #local _cmks_base_fn    = CS_stone_piece_base_fn(RStream)
    #local _cmks_body_fn    = CS_stone_body_cylinder(CS_stone_king_tower_height,CS_stone_king_tower_base_radius,CS_stone_king_tower_base_height,CS_stone_king_tower_mid_radius,CS_stone_king_tower_mid_height,CS_stone_king_tower_top_radius,CS_stone_king_tower_cap_radius,CS_stone_king_tower_cap_height,CS_stone_king_tower_rnd)
    #local _cmks_belt_fn    = CS_stone_king_belt_shape_fn()
    #local _belt_offset     = CS_stone_king_tower_height;
    #local _cmks_cap_fn     = CS_stone_king_cap_shape_fn()
    #local _cap_offset      = _belt_offset + CS_stone_king_belt_height;
    
    #local _cmks_kunion_fn  = Iso_fixed_smooth_union(0.1)
    #local _cmks_king_fn    = function(x,y,z) {
        _cmks_kunion_fn(
            Iso_union(
                _cmks_body_fn(x,y,z),
                _cmks_belt_fn(x,y-_belt_offset,z)
            ),    
            _cmks_cap_fn(x,y-_cap_offset,z)
        )
    }    
    
    
    #local _cmks_union_fn   = Iso_fixed_smooth_union(0.1)
    #local _cmks_shape_fn   = function(x,y,z) {
        _cmks_union_fn(
            _cmks_base_fn(x,y,z),
            _cmks_king_fn(x,y-CS_stone_piece_base_height,z)
        ) + CracksFn(x,y,z)    
    }
    
    function(x,y,z) {
        _cmks_shape_fn(x,y,z)
    }
#end

// End CS_stone_king_shape_fn
//-----------------------------------------------------------------------------

#macro CS_stone_king(Player,CracksFn,RStream)
    #local _cmk_shape_fn    = CS_stone_king_shape_fn(CracksFn,RStream)
    
    #local _lbounds         = <-CS_stone_king_radius, -0.1, -CS_stone_king_radius>;
    #local _ubounds         = <CS_stone_king_radius,  CS_stone_king_height+2, CS_stone_king_radius>;
    
    #local _king    = union {
        object {
            CS_stone_felt_disc(0.9*CS_stone_piece_base_radius,RStream)
        }    
        isosurface {
            function { _cmk_shape_fn(x,y,z) }
            threshold 0
            contained_by { box { _lbounds, _ubounds } }
            accuracy 0.001
            max_gradient 1.2
            
            material {
                texture { CS_stone_piece_texture(Player,RStream) }
                interior { ior 1.5 }
            } 
            
            translate <0, CS_stone_felt_disc_thickness, 0>
        }
    }    
    
    _king    
#end

// End CS_stone_king
//=============================================================================

//=============================================================================
// CS_stone_queen(Player,CracksFn,RStream)
//

#declare CS_stone_white_queen_cracks_scale = 4*<CS_stone_queen_tower_base_radius,CS_stone_queen_height,CS_stone_queen_tower_base_radius>;
#declare CS_stone_black_queen_cracks_scale = 0.1*<CS_stone_queen_tower_base_radius,CS_stone_queen_height,CS_stone_queen_tower_base_radius>;

//-----------------------------------------------------------------------------
// CS_stone_queen_cap_shape_fn()
//
#macro CS_stone_queen_cap_shape_fn()
    #local _h                   = CS_stone_queen_cap_base_cyl_height+CS_stone_queen_cap_base_cyl_rnd;
    #local _cmqcs_rcyl_fn       = Iso_fixed_rounded_cylinder(CS_stone_queen_cap_base_cyl_radius,_h,CS_stone_queen_cap_base_cyl_rnd)
    #local _cmqcs_rcyl_cap_fn   = Iso_fixed_plane(y, 0)
    #local _cmqcs_base_fn       = function(x,y,z) {
        Iso_intersection(
            _cmqcs_rcyl_fn(x,y-_h/2,z),
            _cmqcs_rcyl_cap_fn(x,y-CS_stone_queen_cap_base_cyl_height,z)
        )
    }
    
    #local _cmqcs_bottom_cone_fn    = Iso_fixed_capped_cone(CS_stone_queen_cap_crown_bottom_height,CS_stone_queen_cap_crown_top_radius,CS_stone_queen_cap_base_cyl_radius)
    #local _bottom_cone_offset      = CS_stone_queen_cap_base_cyl_height + CS_stone_queen_cap_crown_bottom_height/2;
    #local _cmqcs_top_cone_fn       = Iso_fixed_capped_cone(CS_stone_queen_cap_crown_cone_height,CS_stone_queen_cap_crown_cone_radius,CS_stone_queen_cap_crown_top_radius)
    #local _top_cone_offset         = CS_stone_queen_cap_base_cyl_height + CS_stone_queen_cap_crown_bottom_height + CS_stone_queen_cap_crown_cone_height/2;
            
    function(x,y,z) {
        Iso_union(
            _cmqcs_base_fn(x,y,z),
            Iso_union(
                _cmqcs_bottom_cone_fn(x,y-_bottom_cone_offset,z),
                _cmqcs_top_cone_fn(x,y-_top_cone_offset,z)
            )    
        )    
    }
#end

// End CS_stone_queen_cap_shape_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_queen_shape_fn(CracksFn,RStream)
//
#macro CS_stone_queen_shape_fn(CracksFn,RStream)
    #local _cmqs_base_fn    = CS_stone_piece_base_fn(RStream)
    #local _cmqs_body_fn    = CS_stone_body_cylinder(CS_stone_queen_tower_height,CS_stone_queen_tower_base_radius,CS_stone_queen_tower_base_height,CS_stone_queen_tower_mid_radius,CS_stone_queen_tower_mid_height,CS_stone_queen_tower_top_radius,CS_stone_queen_tower_cap_radius,CS_stone_queen_tower_cap_height,CS_stone_queen_tower_rnd)
    #local _cmqs_cap_fn     = CS_stone_queen_cap_shape_fn()
    #local _cap_offset      = CS_stone_queen_tower_height;
    
    #local _cmqs_kunion_fn  = Iso_fixed_smooth_union(0.1)
    #local _cmqs_queen_fn    = function(x,y,z) {
        Iso_union(
            _cmqs_body_fn(x,y,z),
            _cmqs_cap_fn(x,y-_cap_offset,z)
        )
    }    
    
    
    #local _cmqs_union_fn   = Iso_fixed_smooth_union(0.1)
    #local _cmqs_shape_fn   = function(x,y,z) {
        _cmqs_union_fn(
            _cmqs_base_fn(x,y,z),
            _cmqs_queen_fn(x,y-CS_stone_piece_base_height,z)
        ) + CracksFn(x,y,z)    
    }
    
    function(x,y,z) {
        _cmqs_shape_fn(x,y,z)
    }
#end

// End CS_stone_queen_shape_fn
//-----------------------------------------------------------------------------

#macro CS_stone_queen(Player,CracksFn,RStream)
    #local _cmq_shape_fn    = CS_stone_queen_shape_fn(CracksFn,RStream)
    
    #local _lbounds         = <-CS_stone_queen_radius, -0.1, -CS_stone_queen_radius>;
    #local _ubounds         = <CS_stone_queen_radius,  CS_stone_queen_height+2, CS_stone_queen_radius>;
    
    #local _queen    = union {
        object {
            CS_stone_felt_disc(0.9*CS_stone_piece_base_radius,RStream)
        }    
        isosurface {
            function { _cmq_shape_fn(x,y,z) }
            threshold 0
            contained_by { box { _lbounds, _ubounds } }
            accuracy 0.001
            max_gradient 1.2
            
            material {
                texture { CS_stone_piece_texture(Player,RStream) }
                interior { ior 1.5 }
            } 
            
            translate <0, CS_stone_felt_disc_thickness, 0>
        }
    }    
    
    _queen    
#end

// End CS_stone_queen
//=============================================================================

//=============================================================================
// CS_stone_rook(Player,CracksFn,RStream)
//

#declare CS_stone_white_rook_cracks_scale = 4*<CS_stone_rook_tower_base_radius,CS_stone_rook_height,CS_stone_rook_tower_base_radius>;
#declare CS_stone_black_rook_cracks_scale = 0.1*<CS_stone_rook_tower_base_radius,CS_stone_rook_height,CS_stone_rook_tower_base_radius>;

//-----------------------------------------------------------------------------
// CS_stone_rook_shape_fn(CracksFn,RStream)
//
#macro CS_stone_rook_shape_fn(CracksFn,RStream)
    #local _cmrs_base_fn    = CS_stone_piece_base_fn(RStream)
    #local _cmrs_body_fn    = CS_stone_body_cylinder(CS_stone_rook_tower_height,CS_stone_rook_tower_base_radius,CS_stone_rook_tower_base_height,CS_stone_rook_tower_mid_radius,CS_stone_rook_tower_mid_height,CS_stone_rook_tower_top_radius,CS_stone_rook_tower_cap_radius,CS_stone_rook_tower_cap_height,CS_stone_rook_tower_rnd)
    #local _cap_re          = Math_Scale(SCALE_MM, 1);
    #local _cmrs_cap_fn     = Iso_fixed_rounded_cylinder(CS_stone_rook_cap_radius,CS_stone_rook_cap_height,_cap_re)
    #local _cap_offset      = CS_stone_rook_tower_height + CS_stone_rook_cap_height/2;
    
    #local _cmrs_rook_fn    = function(x,y,z) {
        Iso_union(
            _cmrs_body_fn(x,y,z),
            _cmrs_cap_fn(x,y-_cap_offset,z)
        )
    }    
    
    #local _cmrs_union_fn   = Iso_fixed_smooth_union(0.1)
    
    #local _cmrs_shape_fn   = function(x,y,z) {
        _cmrs_union_fn(
            _cmrs_base_fn(x,y,z),
            _cmrs_rook_fn(x,y-CS_stone_piece_base_height,z)
        ) + CracksFn(x,y,z)    
    }
    
    function(x,y,z) {
        _cmrs_shape_fn(x,y,z)
    }
#end

// End CS_stone_rook_shape_fn
//-----------------------------------------------------------------------------


#macro CS_stone_rook(Player,CracksFn,RStream)
    #local _cmr_shape_fn    = CS_stone_rook_shape_fn(CracksFn,RStream)
    
    #local _lbounds         = <-CS_stone_rook_radius, -0.1, -CS_stone_rook_radius>;
    #local _ubounds         = <CS_stone_rook_radius,  CS_stone_rook_height+0.1, CS_stone_rook_radius>;
    
    #local _rook    = union {
        object {
            CS_stone_felt_disc(0.9*CS_stone_piece_base_radius,RStream)
        }    
        isosurface {
            function { _cmr_shape_fn(x,y,z) }
            threshold 0
            contained_by { box { _lbounds, _ubounds } }
            accuracy 0.001
            max_gradient 1.2
            
            material {
                texture { CS_stone_piece_texture(Player,RStream) }
                interior { ior 1.5 }
            } 
            
            translate <0, CS_stone_felt_disc_thickness, 0>
        }
    }    
    
    _rook    
#end

// End CS_stone_rook
//=============================================================================

//=============================================================================
// CS_stone_bishop(Player,CracksFn,RStream)
//

#declare CS_stone_white_bishop_cracks_scale = 4*<CS_stone_bishop_tower_base_radius,CS_stone_bishop_height,CS_stone_bishop_tower_base_radius>;
#declare CS_stone_black_bishop_cracks_scale = 0.1*<CS_stone_bishop_tower_base_radius,CS_stone_bishop_height,CS_stone_bishop_tower_base_radius>;

//-----------------------------------------------------------------------------
// CS_stone_bishop_cap_shape_fn()
//
#macro CS_stone_bishop_cap_shape_fn()
    #local _cmbcs_sphere_fn = Iso_fixed_sphere(CS_stone_bishop_cap_sphere_radius)
    #local _cmbcs_sph_plane_fn  = Iso_fixed_plane(y,0)
    #local _cmbcs_hemisphere_fn = function(x,y,z) {
        Iso_intersection(
            _cmbcs_sphere_fn(x,y,z),
            _cmbcs_sph_plane_fn(x,y,z)
        )
    }
    
    #local _r   = CS_stone_bishop_cap_sphere_radius - CS_stone_bishop_cap_miter_base_radius;
    #local _h   = CS_stone_bishop_cap_sphere_height - CS_stone_bishop_cap_sphere_radius;
    #local _cmbcs_cos_fn    = function(x,y) {
            x - _r*(1 - (-0.5*cos(pi*y/_h) + 0.5))
    }
    #local _cmbcs_cone_fn       = Iso_fixed_rotation(_cmbcs_cos_fn,CS_stone_bishop_cap_miter_base_radius)
    #local _cmbcs_cone_plane_fn = Iso_fixed_plane(-y,0)
    #local _cmbcs_cone_plane2_fn    = Iso_fixed_plane(y,0)
    #local _cmbcs_hemicone_fn   = function(x,y,z) {
        Iso_intersection(
            _cmbcs_cone_fn(x,y,z),
            Iso_intersection(
                _cmbcs_cone_plane_fn(x,y,z),
                _cmbcs_cone_plane2_fn(x,y-_h,z)
            )    
        )
    }
    
    #local _cmbcs_miter_sphere_fn   = Iso_fixed_sphere(CS_stone_bishop_cap_miter_mid_radius)
    #local _miter_sphere_overlap    = ChordRadius2Height(2*CS_stone_bishop_cap_miter_base_radius,CS_stone_bishop_cap_miter_mid_radius);
    #local _miter_sphere_yscale     = 0.5*CS_stone_bishop_cap_miter_height/CS_stone_bishop_cap_miter_mid_radius;
    #local _cmbcs_miter_shape_fn    = Scale_fn(_cmbcs_miter_sphere_fn, <1, _miter_sphere_yscale, 1>)
    #local _miter_offset            = _h + (CS_stone_bishop_cap_miter_height/2 - _miter_sphere_overlap*_miter_sphere_yscale);
    
    #local _cmbcs_shape_fn  = function(x,y,z) {
        Iso_union(
            Iso_union(
                _cmbcs_hemisphere_fn(x,y,z),
                _cmbcs_hemicone_fn(x,y,z)
            ),
            _cmbcs_miter_shape_fn(x,y-_miter_offset,z)
        )        
    }
    
    function(x,y,z) {
        _cmbcs_shape_fn(x,y-CS_stone_bishop_cap_sphere_radius,z)
    }    
                
#end

// End CS_stone_bishop_cap_shape_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_bishop_shape_fn(CracksFn,RStream)
//
#macro CS_stone_bishop_shape_fn(CracksFn,RStream)
    #local _cmbs_base_fn    = CS_stone_piece_base_fn(RStream)
    #local _cmbs_body_fn    = CS_stone_body_cylinder(CS_stone_bishop_tower_height,CS_stone_bishop_tower_base_radius,CS_stone_bishop_tower_base_height,CS_stone_bishop_tower_mid_radius,CS_stone_bishop_tower_mid_height,CS_stone_bishop_tower_top_radius,CS_stone_bishop_tower_cap_radius,CS_stone_bishop_tower_cap_height,CS_stone_bishop_tower_rnd)
    #local _cmbs_cap_fn     = CS_stone_bishop_cap_shape_fn()
    #local _cap_offset      = CS_stone_bishop_tower_height - CS_stone_bishop_cap_tower_offset;    
    
    #local _cmbs_union_fn   = Iso_fixed_smooth_union(0.1)
    #local _cmbs_bishop_fn    = function(x,y,z) {
        _cmbs_union_fn(
            _cmbs_body_fn(x,y,z),
            _cmbs_cap_fn(x,y-_cap_offset,z)
        )
    }    
    
    
    #local _cmbs_shape_fn   = function(x,y,z) {
        _cmbs_union_fn(
            _cmbs_base_fn(x,y,z),
            _cmbs_bishop_fn(x,y-CS_stone_piece_base_height,z)
        ) + CracksFn(x,y,z)    
    }
    
    function(x,y,z) {
        _cmbs_shape_fn(x,y,z)
    }
#end

// End CS_stone_bishop_shape_fn
//-----------------------------------------------------------------------------


#macro CS_stone_bishop(Player,CracksFn,RStream)
    #local _cmr_shape_fn    = CS_stone_bishop_shape_fn(CracksFn,RStream)
    
    #local _lbounds         = <-CS_stone_bishop_radius, -0.1, -CS_stone_bishop_radius>;
    #local _ubounds         = <CS_stone_bishop_radius,  CS_stone_bishop_height+0.1, CS_stone_bishop_radius>;
    
    #local _bishop    = union {
        object {
            CS_stone_felt_disc(0.9*CS_stone_piece_base_radius,RStream)
        }    
        isosurface {
            function { _cmr_shape_fn(x,y,z) }
            threshold 0
            contained_by { box { _lbounds, _ubounds } }
            accuracy 0.001
            max_gradient 1.2
            
            material {
                texture { CS_stone_piece_texture(Player,RStream) }
                interior { ior 1.5 }
            } 
            
            translate <0, CS_stone_felt_disc_thickness, 0>
        }
    }    
    
    _bishop    
#end

// End CS_stone_bishop
//=============================================================================

//=============================================================================
// CS_stone_knight(Player,CracksFn,RStream)
//

#declare CS_stone_white_knight_cracks_scale = 4*<CS_stone_knight_base_radius,CS_stone_knight_height,CS_stone_knight_base_radius>;
#declare CS_stone_black_knight_cracks_scale = 0.1*<CS_stone_knight_base_radius,CS_stone_knight_height,CS_stone_knight_base_radius>;

//-----------------------------------------------------------------------------
// CS_stone_knight_base_fn()
//
#macro CS_stone_knight_base_fn()
    #local _cmkb_cyl_fn = Iso_fixed_capped_cylinder(CS_stone_knight_base_radius,CS_stone_knight_base_height)
    #local _cyl_offset  = CS_stone_knight_base_height/2;
    
    #local _cmkb_cone_fn    = Iso_fixed_capped_cone(CS_stone_knight_base_cone_height,CS_stone_knight_base_cone_radius,CS_stone_knight_base_radius)
    #local _cone_offset = CS_stone_knight_base_height + CS_stone_knight_base_cone_height/2;
    
    function(x,y,z) {
        Iso_union(
            _cmkb_cyl_fn(x,y-_cyl_offset,z),
            _cmkb_cone_fn(x,y-_cone_offset,z)
        )
    }        
#end

// End CS_stone_knight_base_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_knight_head_fn()
//
#macro CS_stone_knight_head_fn()
    #local _cmkh_rbox_fn    = Iso_rounded_box()
    
    #local _csx             = CS_stone_knight_head_base_width/2;
    #local _dsx             = (CS_stone_knight_head_top_width - CS_stone_knight_head_base_width)/2;
    #local _csy             = CS_stone_knight_head_height/2;
    #local _csz             = CS_stone_knight_head_depth/2;
    
    #local _cmkh_mx_fn      = function(x,y,z) {
        _csx + pow((y/CS_stone_knight_head_height),2)*_dsx
    }
    
    #local _cmkh_sx_fn      = function(x,y,z,mx) {
        select(z,
            mx - (pow(z/_csz,2)*(mx - CS_stone_knight_nose_width/2)),
            mx - (pow(z/_csz,2)*(mx - CS_stone_knight_mane_width/2))
        )    
    }
    
    // Back side
    #local _back_min_sz = CS_stone_knight_base_cone_radius;
    #local _back_max_sz = CS_stone_knight_head_depth/2;
    #local _back_dsz    = (_back_max_sz - _back_min_sz);
    
    // Front side
    
    #local _front_ear_sz            = Math_Scale(SCALE_MM, 1.5);
    #local _front_ear_ht            = Math_Scale(SCALE_MM, 6);
    
    #local _front_neck_ht           = Math_Scale(SCALE_CM, 1.5);
    #local _front_neck_bottom_sz    = CS_stone_knight_base_radius;
    #local _front_neck_top_sz       = _front_ear_sz;
    #local _front_neck_dz           = _front_neck_bottom_sz - _front_neck_top_sz;
    
    #local _front_lip_sz            = CS_stone_knight_base_radius - Math_Scale(SCALE_MM, 3);
    #local _front_nose_sz           = CS_stone_knight_base_radius - Math_Scale(SCALE_MM, 1);
    #local _front_lip_nose_dz       = (_front_nose_sz - _front_lip_sz);
    #local _front_nose_ht           = Math_Scale(SCALE_MM, 5);
    #local _front_nose_y            = _front_neck_ht + _front_nose_ht;
    #local _front_nose_dy           = CS_stone_knight_nose_width/2;
    #local _front_nose_ridge_ht     = Math_Scale(SCALE_MM, 5);
    
    #local _front_ear_y             = CS_stone_knight_head_height - _front_ear_ht;
    #local _front_ear_dz            = _front_nose_sz - _front_ear_sz;
    #local _front_ear_dy            = _front_ear_y - _front_nose_y;
    
    #local _cmkh_sz_fn      = function(x,y,z,sx) {
        select(z,
            select(y - _front_neck_ht,
                _front_neck_bottom_sz - pow(Iso_clamp((y/_front_neck_ht),0,1),0.5)*_front_neck_dz,
                select(y - _front_nose_y,
                    _front_lip_sz + _front_lip_nose_dz*pow(Iso_clamp((y - _front_neck_ht)/_front_nose_ht,0,1),0.5),
                    select(y - _front_ear_y,
                        _front_nose_sz - _front_ear_dz*pow(Iso_clamp(((y - _front_nose_y + _front_nose_ridge_ht*pow(Iso_clamp(abs(x)/sx,0,1),1.5))/_front_ear_dy),0,1),0.9),  
                        _front_ear_sz
                    )    
                )    
            ),        
            _back_max_sz - pow((y-_csy)/_csy,2)*_back_dsz
        )
    }
            
    #local _front_nose_cyl_r    = Math_Scale(SCALE_MM, 3);
    #local _cmkh_nose_cyl_fn    = Iso_fixed_capped_cylinder(_front_nose_cyl_r, 2*CS_stone_knight_base_radius)
    #local _cmkh_nose_cyl_rot_fn    = function(x,y,z) {
        _cmkh_nose_cyl_fn(y,x,z)
    }
    
    #local _ear_radius          = Math_Scale(SCALE_MM, 3.5);
    #local _ear_h               = Math_Scale(SCALE_MM, 2);
    #local _cmkh_ear_cyl_fn     = Iso_fixed_capped_cylinder(_ear_radius, 2*CS_stone_knight_base_radius)
    #local _cmkh_ear_rot_fn     = function(x,y,z) {
        _cmkh_ear_cyl_fn(x,z,y)
    }
    #local _cmkh_ear_scale_fn   = Scale_fn(_cmkh_ear_rot_fn, <1, _ear_h/_ear_radius, 1>)
    #local _cmkh_ear_trans_fn   = Translate_fn(_cmkh_ear_scale_fn, <0, CS_stone_knight_head_height, 0>)    
    
    
    #local _cmkh_head_fn    = function(x,y,z,sx) {
        Iso_subtraction(
            _cmkh_ear_trans_fn(x,y,z),
            _cmkh_rbox_fn(x,y-_csy,z,
                CS_stone_knight_head_rnd,
                sx,
                _csy,
                _cmkh_sz_fn(x,y,z,sx)
            )
        )        
    }
    
    function(x,y,z) {
        _cmkh_head_fn(x,y,z,_cmkh_sx_fn(x,y,z,_cmkh_mx_fn(x,y,z)))
    }
#end

// End CS_stone_knight_head_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_knight_shape_fn(CracksFn,RStream)
//
#macro CS_stone_knight_shape_fn(CracksFn,RStream)
    #local _cmks_base_fn    = CS_stone_piece_base_fn(RStream)
    #local _cmks_hbase_fn   = CS_stone_knight_base_fn()
    #local _cmks_head_fn    = CS_stone_knight_head_fn()
    #local _head_offset     = CS_stone_knight_base_height;    
    
    #local _cmks_knight_fn    = function(x,y,z) {
        Iso_union(
            _cmks_hbase_fn(x,y,z),
            _cmks_head_fn(x,y-_head_offset,z)
        )
    }    
    
    #local _cmks_union_fn   = Iso_fixed_smooth_union(0.1)
    #local _cmks_shape_fn   = function(x,y,z) {
        _cmks_union_fn(
            _cmks_base_fn(x,y,z),
            _cmks_knight_fn(x,y-CS_stone_piece_base_height,z)
        ) + CracksFn(x,y,z)    
    }
    
    function(x,y,z) {
        _cmks_shape_fn(x,y,z)
    }
#end

// End CS_stone_knight_shape_fn
//-----------------------------------------------------------------------------

#macro CS_stone_knight(Player,CracksFn,RStream)
    #local _cmr_shape_fn    = CS_stone_knight_shape_fn(CracksFn,RStream)
    
    #local _lbounds         = <-CS_stone_knight_radius, -0.1, -CS_stone_knight_radius>;
    #local _ubounds         = <CS_stone_knight_radius,  CS_stone_knight_height+0.1, CS_stone_knight_radius>;
    
    #local _knight    = union {
        object {
            CS_stone_felt_disc(0.9*CS_stone_piece_base_radius,RStream)
        }    
        isosurface {
            function { _cmr_shape_fn(x,y,z) }
            threshold 0
            contained_by { box { _lbounds, _ubounds } }
            accuracy 0.01
            max_gradient 20
            
            material {
                texture { CS_stone_piece_texture(Player,RStream) }
                interior { ior 1.5 }
            } 
            
            translate <0, CS_stone_felt_disc_thickness, 0>
        }
    }    
    
    _knight    
#end

// End CS_stone_knight
//=============================================================================


//=============================================================================
// CS_stone_pawn(Player,RStream)
//
#declare CS_stone_white_pawn_cracks_scale = 4*<CS_stone_pawn_base_radius,CS_stone_pawn_height,CS_stone_pawn_base_radius>;
#declare CS_stone_black_pawn_cracks_scale = 0.1*<CS_stone_pawn_base_radius,CS_stone_pawn_height,CS_stone_pawn_base_radius>;

//-----------------------------------------------------------------------------
// CS_stone_pawn_cap_shape_fn()
//
#macro CS_stone_pawn_cap_shape_fn()
    #local _cmpcs_sphere_fn         = Iso_fixed_sphere(CS_stone_pawn_cap_radius)
    #local _cmpcs_trans_sphere_fn   = Translate_fn(_cmpcs_sphere_fn, <0, CS_stone_pawn_cap_radius, 0>)
    #local _cmpcs_sphere_scale_fn   = Scale_fn(_cmpcs_trans_sphere_fn, <1, 0.5*CS_stone_pawn_cap_height/CS_stone_pawn_cap_radius, 1>)
    
    function(x,y,z) {
        _cmpcs_sphere_scale_fn(x,y,z)
    }    
                
#end

// End CS_stone_pawn_cap_shape_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_stone_pawn_shape_fn(CracksFn,RStream)
//
#macro CS_stone_pawn_shape_fn(CracksFn,RStream)
    #local _cmps_base_fn    = CS_stone_pawn_base_fn(RStream)
    #local _cmps_body_fn    = CS_stone_body_cylinder(CS_stone_pawn_tower_height,CS_stone_pawn_tower_base_radius,CS_stone_pawn_tower_base_height,CS_stone_pawn_tower_mid_radius,CS_stone_pawn_tower_mid_height,CS_stone_pawn_tower_top_radius,CS_stone_pawn_tower_cap_radius,CS_stone_pawn_tower_cap_height,CS_stone_pawn_tower_rnd)
    #local _cmps_cap_fn     = CS_stone_pawn_cap_shape_fn()
    #local _cap_offset      = CS_stone_pawn_tower_height - CS_stone_pawn_cap_tower_offset;    
    
    #local _cmps_union_fn   = Iso_fixed_smooth_union(0.1)
    #local _cmps_pawn_fn    = function(x,y,z) {
        _cmps_union_fn(
            _cmps_body_fn(x,y,z),
            _cmps_cap_fn(x,y-_cap_offset,z)
        )
    }    
    
    
    #local _cmps_shape_fn   = function(x,y,z) {
        _cmps_union_fn(
            _cmps_base_fn(x,y,z),
            _cmps_pawn_fn(x,y-CS_stone_piece_base_height,z)
        ) + CracksFn(x,y,z)    
    }
    
    function(x,y,z) {
        _cmps_shape_fn(x,y,z)
    }
#end

// End CS_stone_pawn_shape_fn
//-----------------------------------------------------------------------------

#macro CS_stone_pawn(Player,CracksFn,RStream)
    #local _cmr_shape_fn    = CS_stone_pawn_shape_fn(CracksFn,RStream)
    
    #local _lbounds         = <-CS_stone_pawn_base_mradius, 0, -CS_stone_pawn_base_mradius>;
    #local _ubounds         = <CS_stone_pawn_base_mradius,  CS_stone_pawn_height, CS_stone_pawn_base_mradius>;
    
    #local _pawn    = union {
        object {
            CS_stone_felt_disc(0.9*CS_stone_piece_base_radius,RStream)
        }    
        isosurface {
            function { _cmr_shape_fn(x,y,z) }
            threshold 0
            contained_by { box { _lbounds, _ubounds } }
            accuracy 0.001
            max_gradient 2
            
            material {
                texture { CS_stone_piece_texture(Player,RStream) }
                interior { ior 1.5 }
            } 
            
            translate <0, CS_stone_felt_disc_thickness, 0>
        }
    }    
    
    _pawn    
#end


// End CS_stone_pawn
//=============================================================================

//=============================================================================
// CS_stone_chess_set_create()
//
#macro CS_stone_chess_set_create()
    #local _stream      = seed(2025012801);
    
    #local _board_obj   = object {
        CS_stone_board()
    }    
    #local _board   = Chess_set_board_create(
        CS_stone_board_size,
        CS_stone_board_square_size.x,
        _board_obj
    );
    
    #local _cs   = Chess_set_create(_board);
    
    #for (i, Chess_player_white, Chess_player_black, 1)
        #if (i = Chess_player_white)
            #local _piece_rot   = <0, 180, 0>;
        #else
            #local _piece_rot   = <0, 0, 0>;
        #end
        #switch(i)
            #case(Chess_player_white)
                #local _king_cracks_fn      = CS_stone_white_piece_cracks_fn(CS_stone_white_king_cracks_scale,_stream)
                #local _queen_cracks_fn     = CS_stone_white_piece_cracks_fn(CS_stone_white_queen_cracks_scale,_stream)
                #local _rook_cracks_fn      = CS_stone_white_piece_cracks_fn(CS_stone_white_rook_cracks_scale,_stream)
                #local _knight_cracks_fn    = CS_stone_white_piece_cracks_fn(CS_stone_white_knight_cracks_scale,_stream)
                #local _bishop_cracks_fn    = CS_stone_white_piece_cracks_fn(CS_stone_white_bishop_cracks_scale,_stream)
                #local _pawn_cracks_fn      = CS_stone_white_piece_cracks_fn(CS_stone_white_pawn_cracks_scale,_stream)
            #break
            #case(Chess_player_black)
                #local _king_cracks_fn      = CS_stone_black_piece_cracks_fn(CS_stone_black_king_cracks_scale,_stream)
                #local _queen_cracks_fn     = CS_stone_black_piece_cracks_fn(CS_stone_black_queen_cracks_scale,_stream)
                #local _rook_cracks_fn      = CS_stone_black_piece_cracks_fn(CS_stone_black_rook_cracks_scale,_stream)
                #local _knight_cracks_fn    = CS_stone_black_piece_cracks_fn(CS_stone_black_knight_cracks_scale,_stream)
                #local _bishop_cracks_fn    = CS_stone_black_piece_cracks_fn(CS_stone_black_bishop_cracks_scale,_stream)
                #local _pawn_cracks_fn      = CS_stone_black_piece_cracks_fn(CS_stone_black_pawn_cracks_scale,_stream)
            #break
        #end
        
        
        #local _king_obj    = object { CS_stone_king(i,_king_cracks_fn,_stream) }
        #local _king_sz     = max_extent(_king_obj) - min_extent(_king_obj);
        #local _king_set_pc = Chess_set_piece_create(i,Chess_piece_type_king,_king_sz,_king_obj);
        Chess_set_add_piece(_cs,_king_set_pc)
        
        #local _queen_obj    = object { CS_stone_queen(i,_queen_cracks_fn,_stream) }
        #local _queen_sz     = max_extent(_queen_obj) - min_extent(_queen_obj);
        #local _queen_set_pc = Chess_set_piece_create(i,Chess_piece_type_queen,_queen_sz,_queen_obj);
        Chess_set_add_piece(_cs,_queen_set_pc)
        Chess_set_add_piece(_cs,_queen_set_pc)  // Extra queen for pawn promotion
        
        #for (j, 0, 1, 1)
            #local _rook_obj    = object { CS_stone_rook(i,_rook_cracks_fn,_stream) }
            #local _rook_sz     = max_extent(_rook_obj) - min_extent(_rook_obj);
            #local _rook_set_pc = Chess_set_piece_create(i,Chess_piece_type_rook,_rook_sz,_rook_obj);
            Chess_set_add_piece(_cs,_rook_set_pc)
            
            #local _knight_obj    = object { CS_stone_knight(i,_knight_cracks_fn,_stream) rotate _piece_rot }
            #local _knight_sz     = max_extent(_knight_obj) - min_extent(_knight_obj);
            #local _knight_set_pc = Chess_set_piece_create(i,Chess_piece_type_knight,_knight_sz,_knight_obj);
            Chess_set_add_piece(_cs,_knight_set_pc)
            
            #local _bishop_obj    = object { CS_stone_bishop(i,_bishop_cracks_fn,_stream) rotate _piece_rot }
            #local _bishop_sz     = max_extent(_bishop_obj) - min_extent(_bishop_obj);
            #local _bishop_set_pc = Chess_set_piece_create(i,Chess_piece_type_bishop,_bishop_sz,_bishop_obj);
            Chess_set_add_piece(_cs,_bishop_set_pc)
        #end
        
        #for (j, 1, 8, 1)
            #local _pawn_obj    = object { CS_stone_pawn(i,_pawn_cracks_fn,_stream) }
            #local _pawn_sz     = max_extent(_pawn_obj) - min_extent(_pawn_obj);
            #local _pawn_set_pc = Chess_set_piece_create(i,Chess_piece_type_pawn,_pawn_sz,_pawn_obj);
            Chess_set_add_piece(_cs,_pawn_set_pc)
        #end
        
        #undef _king_cracks_fn
        #undef _queen_cracks_fn
        #undef _rook_cracks_fn
        #undef _knight_cracks_fn
        #undef _bishop_cracks_fn
        #undef _pawn_cracks_fn
    #end
    
    _cs
#end

// End CS_stone_chess_set_create
//=============================================================================

#version CS_Stone_Inc_Temp;

#end // ifndef (CS_Stone_Inc_Temp)
