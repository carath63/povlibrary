/******************************************************************************
 * File: english_bricks.pov
 * Description:
 *      Example of the Brickwall system using an English layout
 ******************************************************************************/

#version 3.8;

#ifndef (use_radiosity)
    #declare use_radiosity  = 0;
#end

//=============================================================================
// Includes
//
#declare View_POV_Include_Stack=true;

//-----------------------------------------------------------------------------
// Standard Includes
//
#include "rad_def.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project Includes
//
#include "brickwall.inc"
#include "brickwall_english_layout.inc"
#include "common.inc"

// End Project Includes
//-----------------------------------------------------------------------------

// End Includes
//=============================================================================

global_settings {
    assumed_gamma 1.0
    max_trace_level 10
    #if (use_radiosity > 0)
        ambient_light 0
        radiosity {
            Rad_Settings(use_radiosity,off,off)
        }    
    #end
    #ifdef (use_photons)
    #debug "Using photons\n"
    photons {
        count 10000
    }
    #end
    #ifdef (use_sslt)
    #if (use_sslt > 0)
    #debug "Using SSLT\n"
    subsurface {
        #ifdef (use_sslt_samples1)
            #ifdef (use_sslt_samples2)
        samples use_sslt_samples1, use_sslt_samples2
            #else
        samples use_sslt_samples1
            #end
        #end    
    }
    #end
    #end
}

#default {
    finish { ambient 0 diffuse 0.8 }
    //radiosity { importance 250/8000 }
}        

//=============================================================================
// Scene default configuration
//

#ifndef (Scene_View)
#declare Scene_View = 1.1;
#end

#ifndef (Scene_left_end_type)
    #declare Scene_left_end_type    = Brickwall_end_type_flush;
#end
#ifndef (Scene_right_end_type)
    #declare Scene_right_end_type   = Brickwall_end_type_flush;
#end
#ifndef (Scene_wall_halfbricks)
    #declare Scene_wall_halfbricks  = 2;
#end
#ifndef (Scene_wall_length)
    #declare Scene_wall_length      = Common_brick_spec.unit_sz.x*20 - Common_brick_spec.mortar_sz;
#end
#ifndef (Scene_wall_courses)
    #declare Scene_wall_courses     = 10;
#end
#ifndef (Scene_wall_parity)
    #declare Scene_wall_parity      = 0;    // Can also be 1 to shift the courses up by one
#end
#ifndef (Scene_mortar_inset)
    #declare Scene_mortar_inset     = 0;    // Can be any positive value and causes the mortar to sit back from the face of the bricks by the given amount
#end                                

// End Scene configuration
//=============================================================================

//=============================================================================
// Camera default configuration
//

#declare camera_location        = <0, 10, -10>;
#declare camera_lookat          = <0, 0, 0>;

// End Camera configuration
//=============================================================================

//=============================================================================
// Scene object declarations
//

//-----------------------------------------------------------------------------
// Scene_default_object()
//
#macro Scene_default_object()
    #local _ground  = plane {
        y, 0
        pigment { color rgb 0.5 }
    }
    
    #local _wall_spec                   = Brickwall_wall_spec_create(Scene_wall_length,Scene_wall_courses,Scene_wall_halfbricks);
    #local _wall_spec.left_end_type     = Scene_left_end_type;
    #local _wall_spec.right_end_type    = Scene_right_end_type;
    #local _layout                      = Brickwall_english_layout_create(_wall_spec,Common_brick_spec,Scene_wall_parity);
    
    #local _bricks_shape_fn = Brickwall_english_bricks_shape_fn(_layout,Scene_brick_shape)
    #local _mortar_block    = Brickwall_english_mortar_block_create(_layout,Scene_mortar_inset)
    #local _mortar_shape_fn = Brickwall_english_mortar_shape_fn(_layout,_mortar_block,Common_mortar_joint_spec,_bricks_shape_fn)
    
    #local _lbounds         = <0,0,0>;
    #local _ubounds         = _layout.wall_sz;
    
    #local _bricks  = isosurface {
        function {
            _bricks_shape_fn(x,y,z)
        }
        threshold 0
        contained_by { box { _lbounds, _ubounds } }
        accuracy 0.1
        max_gradient 10
        material {
            texture { Common_bricks_texture(_layout) }
            interior { ior 1.5 }
        }        
    }
    
    #local _mortar  = isosurface {
        function {
            _mortar_shape_fn(x,y,z)
        }
        threshold 0
        contained_by { box { _lbounds, _ubounds } }
        accuracy 0.1
        max_gradient 10
        material {
            texture { Common_mortar_texture() }
            interior { ior 1.5 }
        }
    }
    
    #local _result  = union {
        plane { y, 0 pigment { color rgb 0.25 } }
        object { _bricks }
        object { _mortar }
        translate <-_layout.wall_sz.x/2, 0, 0>
    }
    
    _result                
        
#end

// End Scene_default_object
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_camera_light()
//
#macro Scene_camera_light()
    #local _l   = light_source {
        camera_location
        1
    }    
    
    _l
#end

// End Scene_camera_light
//-----------------------------------------------------------------------------

// End Scene object declarations
//=============================================================================

//=============================================================================
// View-specific configuration overrides
//

#switch(val(str(Scene_View,0,2)))
    #case(1.1)
        #declare Scene_brick_shape  = Common_clean_brick_shape();
        #declare camera_location    = <0, Scene_wall_courses*Common_brick_spec.unit_sz.y/2, -0.8*Scene_wall_length>;
        #declare camera_lookat      = <0, Scene_wall_courses*Common_brick_spec.unit_sz.y/2, 0>;
    #break
    
    #case(1.2)
        #declare Scene_brick_shape  = Common_clean_brick_shape();
        #declare Scene_left_end_type    = Brickwall_end_type_corner;
        #declare camera_location    = <0, Scene_wall_courses*Common_brick_spec.unit_sz.y/2, -0.8*Scene_wall_length>;
        #declare camera_lookat      = <0, Scene_wall_courses*Common_brick_spec.unit_sz.y/2, 0>;
    #break
    
    #case(1.3)
        #declare Scene_brick_shape  = Common_clean_brick_shape();
        #declare Scene_left_end_type    = Brickwall_end_type_corner;
        #declare Scene_wall_parity      = 1;
        #declare camera_location    = <0, Scene_wall_courses*Common_brick_spec.unit_sz.y/2, -0.8*Scene_wall_length>;
        #declare camera_lookat      = <0, Scene_wall_courses*Common_brick_spec.unit_sz.y/2, 0>;
    #break
    
    #case(2.1)
        #declare Scene_brick_shape  = Common_noisy_brick_shape();
        #declare Scene_mortar_inset = Common_brick_noise_size;
        #declare camera_location    = <0, Scene_wall_courses*Common_brick_spec.unit_sz.y/2, -0.8*Scene_wall_length>;
        #declare camera_lookat      = <0, Scene_wall_courses*Common_brick_spec.unit_sz.y/2, 0>;
    #break

    #else
        #warning concat("Unknown Scene_View: ", str(Scene_View, 0, 2), "\n")
    #break
#end // switch(Scene_View)

// End View-specific configuration overrides
//=============================================================================

//=============================================================================
// Scene declaration
//
#ifndef(Scene_object)
#declare Scene_object    = union {
    object { Scene_default_object() }
    object { Scene_camera_light() }
} 
#end

// End Scene declaration
//=============================================================================

//=============================================================================
// Camera
//
camera {
    location camera_location
    look_at camera_lookat
}
        
// End Camera
//=============================================================================

//=============================================================================
// Scene Instantiation
//

object { Scene_object }

// End Scene Instantiation
//=============================================================================
    