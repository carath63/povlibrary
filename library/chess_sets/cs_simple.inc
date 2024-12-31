/******************************************************************************
 * File: cs_simple.inc
 * Description:
 *      CS_simple ".inc" file
 ******************************************************************************/

#ifndef(CS_simple_Inc_Temp)

#declare CS_simple_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including cs_simple.inc\n"
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
#include "libscale.inc"
#include "libgeom.inc"

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
// Common
//

//-----------------------------------------------------------------------------
// CS_simple_black_material()
//
#macro CS_simple_black_material()
    #local _m   = material {
        texture {
            pigment { color rgb 0.01 }
            finish {
                fresnel
                specular albedo 1.0
                roughness 0.1
                diffuse albedo 1.0
                reflection { 0.1 }
                conserve_energy
            }    
        }
        interior { ior 1.5 }
    }
    
    _m
#end

// End CS_simple_black_material
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_simple_white_material()
//
#macro CS_simple_white_material()
    #local _m   = material {
        texture {
            pigment { color rgb <0.99, 0.975, 0.975> }
            finish {
                fresnel
                specular albedo 1.0
                roughness 0.1
                diffuse albedo 1.0
                reflection { 0.1 }
                conserve_energy
            }    
        }
        interior { ior 1.5 }
    }
    
    _m
#end

// End CS_simple_white_material
//-----------------------------------------------------------------------------


// End Common
//=============================================================================

//=============================================================================
// Piece Shapes
//
#declare CS_simple_pawn_size    = Math_Scale(SCALE_INCH, <1, 1.5, 1>);
#declare CS_simple_rook_size    = Math_Scale(SCALE_INCH, <1, 2, 1>);
#declare CS_simple_knight_size  = Math_Scale(SCALE_INCH, <1, 2+1/4, 1+1/4>);
#declare CS_simple_bishop_size  = Math_Scale(SCALE_INCH, <1, 1+7/8, 1>);
#declare CS_simple_queen_size   = Math_Scale(SCALE_INCH, <1, 2+1/2, 1>);
#declare CS_simple_king_size    = Math_Scale(SCALE_INCH, <1, 2+3/4, 1>);
#declare CS_simple_base_height  = Math_Scale(SCALE_INCH, 1/8);

//-----------------------------------------------------------------------------
// CS_simple_pawn()
//
#macro CS_simple_pawn()
    #local _pawn_cap_radius         = Math_Scale(SCALE_INCH, 3/8);
    #local _pawn_body_top_radius    = Math_Scale(SCALE_INCH, 1/4);
    #local _pawn_cap_overlap        = ChordRadius2Height(2*_pawn_body_top_radius,_pawn_cap_radius);
    #local _pawn_body_height        = CS_simple_pawn_size.y - (CS_simple_base_height + 2*_pawn_cap_radius - _pawn_cap_overlap);
    
    #local _pawn    = union {
        cylinder {
            <0, 0, 0>,
            <0, CS_simple_base_height, 0>,
            CS_simple_pawn_size.x/2
        }
        cone {
            <0, 0, 0>, CS_simple_pawn_size.x/2,
            <0, _pawn_body_height, 0>, _pawn_body_top_radius
            translate <0, CS_simple_base_height, 0>
        }
        sphere {
            <0, 0, 0>, _pawn_cap_radius
            translate <0, _pawn_cap_radius + CS_simple_base_height + _pawn_body_height - _pawn_cap_overlap, 0>
        }            
    }
    
    _pawn
#end

// End CS_simple_pawn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_simple_rook()
//
#macro CS_simple_rook()
    #local _rook_cap_height         = Math_Scale(SCALE_INCH, 1/2);
    #local _rook_cap_radius         = CS_simple_rook_size.x/2;
    #local _rook_body_top_radius    = Math_Scale(SCALE_INCH, 3/8);
    #local _rook_body_height        = CS_simple_rook_size.y - (CS_simple_base_height + _rook_cap_height);
    
    #local _rook    = union {
        cylinder {
            <0, 0, 0>,
            <0, CS_simple_base_height, 0>,
            CS_simple_rook_size.x/2
        }
        cone {
            <0, 0, 0>, CS_simple_pawn_size.x/2,
            <0, _rook_body_height, 0>, _rook_body_top_radius
            translate <0, CS_simple_base_height, 0>
        }
        cylinder {
            <0, 0, 0>,
            <0, _rook_cap_height, 0>, _rook_cap_radius
            translate <0, CS_simple_base_height + _rook_body_height>
        }    
    }    
    
    _rook
#end

// End CS_simple_rook
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_simple_knight()
//
#macro CS_simple_knight()
    #local _knight_head_height      = Math_Scale(SCALE_INCH, 1/2);
    #local _knight_head_length      = Math_Scale(SCALE_INCH, 1);
    #local _knight_head_back_r      = _knight_head_height/2;
    #local _knight_head_front_r     = 0.5*_knight_head_back_r;
    #local _knight_body_top_radius  = Math_Scale(SCALE_INCH, 1/4);
    #local _knight_head_overlap     = ChordRadius2Height(2*_knight_body_top_radius,_knight_head_back_r);
    #local _knight_body_height      = CS_simple_knight_size.y - (CS_simple_base_height + _knight_head_height - _knight_head_overlap);
    
    #local _knight  = union {
        cylinder {
            <0, 0, 0>,
            <0, CS_simple_base_height, 0>,
            CS_simple_knight_size.x/2
        }
        cone {
            <0, 0, 0>, CS_simple_pawn_size.x/2,
            <0, _knight_body_height, 0>, _knight_body_top_radius
            translate <0, CS_simple_base_height, 0>
        }
        union {
            sphere { <0,0,0>, _knight_head_back_r }
            cone {
                <0,0,0>, _knight_head_back_r,
                <0,0,-_knight_head_length>, _knight_head_front_r
            }
            translate <0, _knight_head_back_r + CS_simple_base_height + _knight_body_height - _knight_head_overlap>    
        }
    }
    
    _knight
#end

// End CS_simple_knight
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_simple_bishop()
//
#macro CS_simple_bishop()
    #local _bishop_cap_radius         = Math_Scale(SCALE_INCH, 3/8);
    #local _bishop_body_top_radius    = Math_Scale(SCALE_INCH, 1/4);
    #local _bishop_cap_overlap        = ChordRadius2Height(2*_bishop_body_top_radius,_bishop_cap_radius);
    #local _bishop_body_height        = CS_simple_bishop_size.y - (CS_simple_base_height + 2*_bishop_cap_radius - _bishop_cap_overlap);
    #local _bishop_slot_size          = <2*_bishop_cap_radius, Math_Scale(SCALE_INCH, 1/16), _bishop_cap_radius/2>;
    
    #local _bishop    = union {
        cylinder {
            <0, 0, 0>,
            <0, CS_simple_base_height, 0>,
            CS_simple_bishop_size.x/2
        }
        cone {
            <0, 0, 0>, CS_simple_bishop_size.x/2,
            <0, _bishop_body_height, 0>, _bishop_body_top_radius
            translate <0, CS_simple_base_height, 0>
        }
        difference {
            sphere {
                <0, 0, 0>, _bishop_cap_radius
            }
            box {
                -_bishop_slot_size/2, _bishop_slot_size/2
                translate <0, 0, -_bishop_slot_size.z/2 - (_bishop_cap_radius - _bishop_slot_size.z)>
            }
            rotate <45, 0, 0>               
            translate <0, _bishop_cap_radius + CS_simple_base_height + _bishop_body_height - _bishop_cap_overlap, 0>
        }    
    }
    
    _bishop
#end

// End CS_simple_bishop
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_simple_queen()
//
#macro CS_simple_queen()
    #local _queen_cap_radius        = Math_Scale(SCALE_INCH, 7/16);
    #local _queen_crown_major       = Math_Scale(SCALE_INCH, 3/8);
    #local _queen_crown_minor       = Math_Scale(SCALE_INCH, 1/16);
    #local _queen_body_top_radius   = Math_Scale(SCALE_INCH, 3/8);
    #local _queen_cap_overlap       = ChordRadius2Height(2*_queen_body_top_radius,_queen_cap_radius);
    #local _queen_body_height       = CS_simple_queen_size.y - (CS_simple_base_height + 2*_queen_cap_radius - _queen_cap_overlap);
    #local _queen_crown_offset      = ChordRadius2Height(2*_queen_crown_major,_queen_cap_radius);
    
    #local _queen   = union {
        cylinder {
            <0, 0, 0>,
            <0, CS_simple_base_height, 0>,
            CS_simple_queen_size.x/2
        }
        cone {
            <0, 0, 0>, CS_simple_queen_size.x/2,
            <0, _queen_body_height, 0>, _queen_body_top_radius
            translate <0, CS_simple_base_height, 0>
        }
        union {
            sphere { <0,0,0>, _queen_cap_radius }
            torus { _queen_crown_major _queen_crown_minor translate <0, _queen_cap_radius - _queen_crown_offset, 0> }
            translate <0, _queen_cap_radius + CS_simple_base_height + _queen_body_height - _queen_cap_overlap, 0>
        }    
    }
    
    _queen
#end

// End CS_simple_queen
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CS_simple_king()
//
#macro CS_simple_king()
    #local _king_cap_radius        = Math_Scale(SCALE_INCH, 7/16);
    #local _king_crown_radius      = Math_Scale(SCALE_INCH, 3/8);
    #local _king_crown_height      = Math_Scale(SCALE_INCH, 3/8);
    #local _king_body_top_radius   = Math_Scale(SCALE_INCH, 3/8);
    #local _king_cap_overlap       = ChordRadius2Height(2*_king_body_top_radius,_king_cap_radius);
    #local _king_crown_offset      = ChordRadius2Height(2*_king_crown_radius,_king_cap_radius);
    #local _king_body_height       = CS_simple_king_size.y - (CS_simple_base_height + 2*_king_cap_radius - _king_cap_overlap + _king_crown_height - _king_crown_offset);
    
    #local _king   = union {
        cylinder {
            <0, 0, 0>,
            <0, CS_simple_base_height, 0>,
            CS_simple_king_size.x/2
        }
        cone {
            <0, 0, 0>, CS_simple_king_size.x/2,
            <0, _king_body_height, 0>, _king_body_top_radius
            translate <0, CS_simple_base_height, 0>
        }
        union {
            sphere { <0,0,0>, _king_cap_radius }
            cylinder {
                <0, 0, 0>,
                <0, _king_crown_height, 0>,
                _king_crown_radius
                translate <0, _king_cap_radius - _king_crown_offset, 0>
            }    
            translate <0, _king_cap_radius + CS_simple_base_height + _king_body_height - _king_cap_overlap, 0>
        }    
    }
    
    _king
#end

// End CS_simple_king
//-----------------------------------------------------------------------------

// End Piece Shapes
//=============================================================================

//=============================================================================
// CS_simple_board()
//
#declare CS_simple_board_square_size    = Math_Scale(SCALE_INCH, 1+1/8);
#declare CS_simple_board_size           = <2*Math_Scale(SCALE_INCH, 1/2) + 8*CS_simple_board_square_size, Math_Scale(SCALE_INCH, 1/4), 2*Math_Scale(SCALE_INCH, 1/2) + 8*CS_simple_board_square_size>;
#macro CS_simple_board()
    #local _checker_size    = <8*CS_simple_board_square_size, CS_simple_board_size.y, 8*CS_simple_board_square_size>;
    #local _checker = box {
        -_checker_size/2,
        _checker_size/2
    }
    
    #local _outer   = difference {
        box { -CS_simple_board_size/2, CS_simple_board_size/2 }
        object { _checker scale <1, 1.01, 1> }
    }
    
    #local _board   = union {
        object {
            _checker
            pigment {
                checker
                color rgb 0
                color rgb 1
                scale CS_simple_board_square_size
            }
        }
        object {
            _outer
            pigment { color rgb 0.5 }
        }            
        translate <0, CS_simple_board_size.y/2, 0>
    }
    
    _board    
        
#end

// End CS_simple_board
//=============================================================================

//=============================================================================
// CS_simple_chess_set_create()
// 
#macro CS_simple_chess_set_create()
    #local _board       = object { CS_simple_board() }
    #local _cs_board    = Chess_set_board_create(CS_simple_board_size,CS_simple_board_square_size,_board);
    
    #local _cs  = Chess_set_create(_cs_board);
    
    #for (i, Chess_player_white, Chess_player_black, 1)
        #local _king_obj    = object { CS_simple_king() }
    #end
#end

// End CS_simple_chess_set_create
//=============================================================================

#version CS_simple_Inc_Temp;

#end // ifndef (CS_simple_Inc_Temp)
