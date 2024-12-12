/******************************************************************************
 * File: liblightbulbs.inc
 * Description:
 *      Models for various types of lightbulbs and sockets for them, including
 * common textures for them.
 ******************************************************************************/

#ifndef(Lib_lightbulbs_Inc_Temp)

#declare Lib_lightbulbs_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including liblightbulbs.inc\n"
#end                     

//=============================================================================
// Includes
//

//-----------------------------------------------------------------------------
// Standard Includes
//
#include "shapes.inc"
#include "shapes3.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
#include "libscale.inc"
#include "libgeom.inc"
#include "libisoshapes.inc"
#include "libfn.inc"

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
// Common Lightbulb and Socket Materials
//

// End Common Lightbulb and Socket Materials
//=============================================================================

//=============================================================================
// Lightbulb Bases and Sockets
//

//-----------------------------------------------------------------------------
// Rounded_tip(Height,TipR,TopR)
//
#macro Rounded_tip(Height,TipR,TopR)
    #local _cone_height = Height - TipR;
    #if (_cone_height <= 0)
        #error concat("Rounded_tip: Invalid Height for given tip radius\n")
    #end
    
    #local _tip = union {
        intersection {
            sphere { <0,0,0>, TipR }
            plane { y, 0 }
            translate <0, TipR, 0>
        }
        cone {
            <0, TipR, 0>, TipR,
            <0, Height, 0>, TopR
        }        
    }
    
    _tip    
#end

// End Rounded_tip
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Lightbulb_base_e26(ConductorMaterial,InsulatorMaterial)
//
#declare Lightbulb_base_e26_height              = Math_Scale(SCALE_INCH, 1.05);
#declare Lightbulb_base_e26_radius              = Math_Scale(SCALE_MM, 26)/2;
#declare Lightbulb_base_e26_thread_height       = Math_Scale(SCALE_INCH, 1)/7;
#declare Lightbulb_base_e26_tip_height          = Math_Scale(SCALE_INCH, 1/7);
#declare Lightbulb_base_e26_tip_conductor_h     = 0.25*Lightbulb_base_e26_tip_height;
#declare Lightbulb_base_e26_tip_radius          = Lightbulb_base_e26_radius - Lightbulb_base_e26_thread_height;
#declare Lightbulb_base_e26_threads_height      = 4.5*Lightbulb_base_e26_thread_height;
#declare Lightbulb_base_e26_threads_tail_height = 0.5*Lightbulb_base_e26_thread_height;
#declare Lightbulb_base_e26_cap_height          = Lightbulb_base_e26_height - (Lightbulb_base_e26_threads_height + Lightbulb_base_e26_tip_height);
#declare Lightbulb_base_e26_cap_rnd             = 0.2*Lightbulb_base_e26_cap_height;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Lightbulb_base_e26_base_tip_conductor()
//
#macro Lightbulb_base_e26_tip_conductor()
    #local _tip_slope       = Lightbulb_base_e26_tip_radius/Lightbulb_base_e26_tip_height;
    #local _conductor_r     = Lightbulb_base_e26_tip_conductor_h*_tip_slope;
    #local _tip_r           = 0.25*_conductor_r;
    
    #local _conductor   = object {
        Rounded_tip(Lightbulb_base_e26_tip_conductor_h, _tip_r, _conductor_r)
    }
    
    _conductor    

#end

// End Lightbulb_base_e26_base_tip_conductor
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Lightbulb_base_e26_tip_insulator()
//
#macro Lightbulb_base_e26_tip_insulator()
    #local _tip_slope       = Lightbulb_base_e26_tip_radius/Lightbulb_base_e26_tip_height;
    #local _conductor_r     = Lightbulb_base_e26_tip_conductor_h*_tip_slope;
    #local _h               = Lightbulb_base_e26_tip_height - Lightbulb_base_e26_tip_conductor_h;

    #local _insulator   = cone {
        <0, 0, 0>, _conductor_r,
        <0, _h, 0>, Lightbulb_base_e26_tip_radius
    }
    
    _insulator    
#end

// End Lightbulb_base_e26_tip_insulator
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Lightbulb_base_e26_threads()
//
#macro Lightbulb_base_e26_threads()
    #local _r1  = Lightbulb_base_e26_radius - Lightbulb_base_e26_thread_height/2;
    #local _r2  = Lightbulb_base_e26_thread_height/2;
    
    #local _lbet_helix_fn       = Iso_fixed_helix1(_r1,_r2,2*_r2,Iso_helix_dir_counterclockwise)
    #local _lbet_helix_top_fn   = Iso_fixed_plane(y, -Lightbulb_base_e26_threads_height/2)
    #local _lbet_helix_bot_fn   = Iso_fixed_plane(-y, -Lightbulb_base_e26_threads_height/2)
    #local _lbet_threads_fn = function(x,y,z) {
        Iso_intersection(
            Iso_intersection(
                _lbet_helix_fn(x,y,z),
                _lbet_helix_top_fn(x,y,z)
            ),
            _lbet_helix_bot_fn(x,y,z)
        )        
    }
    
    #local _rnd                 = Lightbulb_base_e26_threads_tail_height;
    #local _hght                = Lightbulb_base_e26_threads_height + 2*_rnd;
    #local _lbet_inner_cyl_fn   = Iso_fixed_rounded_cylinder(_r1,_hght,_rnd)
    #local _lbet_inner_cap_fn   = Iso_fixed_plane(y, -(_hght/2 - _rnd))
    #local _lbet_inner_shape_fn   = function(x,y,z) {
        Iso_intersection(
            _lbet_inner_cyl_fn(x,y,z),
            _lbet_inner_cap_fn(x,y,z)
        )
    }
    
    #local _lbet_shape_fn   = function(x,y,z) {
        Iso_union(
            _lbet_inner_shape_fn(x,y,z),
            _lbet_threads_fn(x,y,z)
        )
    }                
    
    #local _lbounds = <-Lightbulb_base_e26_radius, -_hght/2, -Lightbulb_base_e26_radius>;
    #local _ubounds = <Lightbulb_base_e26_radius, _hght/2, Lightbulb_base_e26_radius>;
    
    #local _threads = isosurface {
        function { _lbet_shape_fn(x,y,z) }
        threshold 0
        contained_by { box { _lbounds, _ubounds } }
        accuracy 0.005
        max_gradient 4
        
        translate <0, _hght/2, 0>
    }
    
    _threads
#end

// End Lightbulb_base_e26_threads
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Lightbulb_base_e26_cap()
//
#macro Lightbulb_base_e26_cap()
    #local _h           = Lightbulb_base_e26_cap_height + Lightbulb_base_e26_cap_rnd;
    #local _lbec_cyl_fn = Iso_fixed_rounded_cylinder(Lightbulb_base_e26_radius,_h,Lightbulb_base_e26_cap_rnd)
    #local _lbec_cap_fn = Iso_fixed_plane(y, -(_h/2 - Lightbulb_base_e26_cap_rnd))
    
    #local _lbec_shape_fn   = function(x,y,z) {
        Iso_intersection(
            _lbec_cyl_fn(x,y,z),
            _lbec_cap_fn(x,y,z)
        )
    }
    
    #local _lbounds = <-Lightbulb_base_e26_radius, -_h/2, -Lightbulb_base_e26_radius>;
    #local _ubounds = <Lightbulb_base_e26_radius, _h/2, Lightbulb_base_e26_radius>;
    
    #local _cap = isosurface {
        function { _lbec_shape_fn(x,y,z) }
        threshold 0
        contained_by { box { _lbounds, _ubounds } }
        accuracy 0.001
        max_gradient 2
        
        translate <0, _h/2, 0>
    }
    
    _cap        
    
#end

// End Lightbulb_base_e26_cap
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#macro Lightbulb_base_e26(ConductorMaterial,InsulatorMaterial)
    #local _base    = union {
        object { Lightbulb_base_e26_tip_conductor() material { ConductorMaterial } }
        object { Lightbulb_base_e26_tip_insulator() translate <0, Lightbulb_base_e26_tip_conductor_h, 0> material { InsulatorMaterial } }
        object { Lightbulb_base_e26_threads() translate <0, Lightbulb_base_e26_tip_height, 0> material { ConductorMaterial } }
        object { Lightbulb_base_e26_cap() translate <0, Lightbulb_base_e26_tip_height + Lightbulb_base_e26_threads_height, 0> material { ConductorMaterial } }
    }
    
    _base
#end

// End Lightbulb_base_e26
//-----------------------------------------------------------------------------


// End Lightbulb Bases and Sockets
//=============================================================================

//=============================================================================
// Lightbulb Bulbs
//

//-----------------------------------------------------------------------------
// Lightbulb_bulb_a19()
//
#declare Lightbulb_bulb_a19_ball_radius = Math_Scale(SCALE_MM, 60)/2;
#declare Lightbulb_bulb_a19_height      = Math_Scale(SCALE_MM, 110) - Lightbulb_base_e26_height;
#declare Lightbulb_bulb_a19_neck_height = Math_Scale(SCALE_MM, 30);
#declare Lightbulb_bulb_a19_neck_radius = Math_Scale(SCALE_MM, 15);

#macro Lightbulb_bulb_a19()
    #local _lb_ball_fn  = Iso_fixed_sphere(Lightbulb_bulb_a19_ball_radius)
    
    #local _ball_height         = Lightbulb_bulb_a19_height - Lightbulb_bulb_a19_neck_height;
    #local _ball_bottom         = Lightbulb_bulb_a19_ball_radius - _ball_height;
    #local _lb_ball_trans_fn    = Translate_fn(_lb_ball_fn, <0, Lightbulb_bulb_a19_neck_height + _ball_height - Lightbulb_bulb_a19_ball_radius, 0>)
    
    #local _neck_rnd    = Lightbulb_bulb_a19_neck_radius - Lightbulb_base_e26_radius;
    #local _lb_rcyl_fn  = Iso_fixed_rounded_cylinder(Lightbulb_bulb_a19_neck_radius,Lightbulb_bulb_a19_neck_height,_neck_rnd)
    #local _lb_rcyl_trans_fn    = Translate_fn(_lb_rcyl_fn, <0, Lightbulb_bulb_a19_neck_height/2, 0>)
    
    #local _k   = 1;
    #local _lb_smooth_union = Iso_fixed_smooth_union(_k)
    
    #local _lb_shape_fn = function(x,y,z) {
        _lb_smooth_union(
            _lb_ball_trans_fn(x,y,z),
            _lb_rcyl_trans_fn(x,y,z)
        )    
    }
    
    #local _lbounds = <-Lightbulb_bulb_a19_ball_radius, 0, -Lightbulb_bulb_a19_ball_radius>;
    #local _ubounds = <Lightbulb_bulb_a19_ball_radius, Lightbulb_bulb_a19_height, Lightbulb_bulb_a19_ball_radius>;
    
    #local _bulb    = isosurface {
        function { _lb_shape_fn(x,y,z) }
        threshold 0
        contained_by { box { _lbounds, _ubounds } }
        accuracy 0.001
        max_gradient 2
        
        hollow
    }
    
    _bulb
    
    
#end

// End Lightbulb_bulb_a19
//-----------------------------------------------------------------------------

// End Lightbulb Bulbs
//=============================================================================

#version Lib_lightbulbs_Inc_Temp;

#end // ifndef (Lib_lightbulbs_Inc_Temp)
