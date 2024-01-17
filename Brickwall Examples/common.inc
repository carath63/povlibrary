/******************************************************************************
 * File: common.inc
 * Description:
 *      Common elements for all of the Brickwall Example scenes
 ******************************************************************************/

#ifndef(Common_Inc_Temp)

#declare Common_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including common.inc\n"
#end

#include "brickwall.inc"
#include "libscale.inc"
#include "libnoise.inc"

//=============================================================================
// Brick Shapes and Textures
//

//-----------------------------------------------------------------------------
// Common_brick_spec
//
// Defines the size of the bricks and mortar used for the example scene.
// To change, just declare Common_brick_spec to one of the other values
// in brickwall_bricks.inc
//
#ifndef (Common_brick_spec)
#declare Common_brick_spec  = Brickwall_us_modular_spec;
#end 

//-----------------------------------------------------------------------------
// Common_clean_brick_shape()
//
// Defines a clean, sharp-edged brick shape using the Common_brick_spec
//
#macro Common_clean_brick_shape()
    Brickwall_fixed_square_brick_shape_create(Common_brick_spec)    
#end       

// End Common_clean_brick_shape
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Common_noisy_brick_shape()
//
#ifndef (Common_brick_noise_size)
    #declare Common_brick_noise_size    = Math_Scale(SCALE_INCH, 1/16);
#end    
#macro Common_noisy_brick_shape()
    #local _clean_brick_shape   = Common_clean_brick_shape();
    #local _noise_size          = Common_brick_noise_size;
    #local _noise_scale         = <1,0.5,1>/Math_Scale(SCALE_INCH, 1/8);
    #local _noise_fn            = Noise_create2(<0,0,0>, f_noise3d, <0,0,0>, _noise_scale, _noise_size)
    
    Brickwall_brick_shape_add_noise(_clean_brick_shape,_noise_fn)
#end

// End Common_noisy_brick_shape
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Common_mortar_joint
//
//      Other options:
//          Mortar_joint_rake_fn(Common_mortar_joint_depth,Common_brick_spec.mortar_sz)
//          Mortar_joint_weather_fn(Common_mortar_joint_depth,Common_brick_spec.mortar_sz)
//          Mortar_joint_struck_fn(Common_mortar_joint_depth,Common_brick_spec.mortar_sz)
//          Mortar_joint_vee_fn(Common_mortar_joint_depth,Common_brick_spec.mortar_sz)
//          Mortar_joint_concave_fn(Common_mortar_joint_depth,Common_brick_spec.mortar_sz)
//          Mortar_joint_extruded_fn(Common_mortar_joint_depth,Common_brick_spec.mortar_sz)
//          Mortar_joint_grapefine_fn(Common_mortar_joint_depth)
//          Mortar_joint_beaded_fn(Common_mortar_joint_depth)
//          Mortar_joint_flush_fn()
//
#ifndef (Common_mortar_joint)
    #ifndef (Common_mortar_joint_depth)
        #declare Common_mortar_joint_depth  = 0;
    #end    
    #ifndef (Common_mortar_joint_fn)
        #declare Common_mortar_joint_fn = Mortar_joint_flush_fn()
    #end
    #declare Common_mortar_joint    = Mortar_joint_create(Common_mortar_joint_depth,Common_mortar_joint_fn);
#end        

// End Common_mortar_joint_fn
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Common_mortar_joint_spec
//
#ifndef (Common_mortar_joint_spec)
    #declare Common_mortar_joint_spec   = Mortar_joint_spec_create(Common_mortar_joint,Common_mortar_joint,Common_mortar_joint);
#end    

// End Common_mortar_joint_spec
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Common_pile_size
//
// Defines the scale factor used for picking bricks out of a pile when applying
// texture using the Brickwall_palette_selector_fn
//
#ifndef (Common_pile_size)
    #declare Common_pile_size   = Common_brick_spec.unit_sz*<100,50,2>;
#end    

// End Common_pile_size
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Common_palette_pattern
//
#ifndef (Common_palette_pattern)
    #declare Common_palette_pattern = pigment {
        bumps
        color_map {
            [0.0 rgb 0]
            [1.0 rgb 1]
        }
        warp {
            turbulence 0.5
            lambda 0.5
            omega 0.2
        }    
        scale 8*Common_brick_spec.brick_sz
    }
#end

// End Common_palette_pattern
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Common_brick_palette()
//
#macro Common_brick_palette()
    #local _p_dark      = pigment {
        granite
        color_map {
            [0.0 rgb <0.1, 0.02, 0.01>]
            [0.5 rgb <0.09, 0.015, 0.005>]
            [1.0 rgb <0.05, 0.015, 0.005>]
        }
    }
    
    #local _p_medium    = pigment {
        granite
        color_map {
            [0.0 rgb <0.25, 0.05, 0.025>]
            [0.5 rgb <0.15, 0.025, 0.0125>]
            [1.0 rgb <0.1, 0.0125, 0.005>]
        }
    }
    
    #local _p_light     = pigment {
        granite
        color_map {
            [0.0 rgb <0.35, 0.1, 0.05>]
            [0.5 rgb <0.25, 0.05, 0.025>]
            [1.0 rgb <0.15, 0.025, 0.01>]
        }
    }
    
    #local _pmap    = pigment_map {
        [0.0 _p_medium]
        [0.1 _p_medium]
        [0.3 _p_dark]
        [0.4 _p_dark]
        [0.9 _p_light]
        [1.0 _p_light]
    }
    
    _pmap
#end

// End Common_brick_palette
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Common_bricks_texture(Layout)
//
#macro Common_bricks_texture(Layout)
    #local _cbt_palette_pfn     = function {
        pigment { Common_palette_pattern }
    }
    #local _cbt_palette_fn      = function(x,y,z) {
        _cbt_palette_pfn(x,y,z).gray
    }        
    #local _cbt_palette_selector_fn    = Brickwall_default_palette_selector_fn(Layout,Common_pile_size,_cbt_palette_fn)
    
    #local _t_bricks    = texture {
        pigment {
            function {
                _cbt_palette_selector_fn(x,y,z)
            }
            pigment_map { Common_brick_palette() }
        }
        normal {
            bumps 0.01
            scale Math_Scale(SCALE_INCH, 1/32)
        }
        finish {
            fresnel
            specular albedo 1.0
            roughness 0.5
            diffuse albedo 1.0
            brilliance 10
        }
    }
    ;
    
    _t_bricks
    
#end

// End Common_bricks_texture
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Common_mortar_texture()
//
#macro Common_mortar_texture()
    #local _t   = texture {
        pigment {
            granite
            color_map {
                [0.0 rgb 0.5]
                [1.0 rgb 0.25]
            }
            scale Math_Scale(SCALE_INCH, 1/4)
        }
        normal {
            bumps 0.1
            scale Math_Scale(SCALE_INCH, 1/16)
        }
        finish {
            fresnel
            specular albedo 1.0
            roughness 0.1
            diffuse albedo 1.0
        }                
    }
    ;
    
    _t
#end

// End Common_mortar_texture
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Common_jitter(Layout)
//
#ifndef (Common_jitter_size)
    #declare Common_jitter_size = Math_Scale(SCALE_INCH, <1/8, 1/8, 1/4>);
#end
#ifndef (Common_jitter_fn)
    #declare Common_jitter_fn   = f_snoise3d;
#end 
#macro Common_jitter(Layout)
    Brickwall_jitter_create(Layout,Common_jitter_size,Common_jitter_fn)
#end

// End Common_Jitter
//-----------------------------------------------------------------------------

// End Brick Shapes and Textures
//=============================================================================

#version Common_Inc_Temp;
#end // ifndef(Common_Inc_Temp)