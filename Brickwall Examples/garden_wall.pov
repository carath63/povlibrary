/******************************************************************************
 * File: garden_wall.pov
 * Description:
 *      Template for rendering scenes
 ******************************************************************************/

#version 3.8;

#ifndef (use_radiosity)
    #declare use_radiosity  = 0;
#end

#include "my_raddef.inc"    
//=============================================================================
// Includes
//
#declare View_POV_Include_Stack=true;

//-----------------------------------------------------------------------------
// Standard Includes
//
#include "camera35mm.inc"
#include "lightsys.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Library Includes
//
#include "libscale.inc"
#include "libtime.inc"

// End My Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project Includes
//
#include "garden_wall.inc"
#include "sky.inc"

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
            pretrace_start 64/image_width
            pretrace_end 8/image_width
            count 100, 1000
            nearest_count 10,5
            error_bound 1
            recursion_limit 2
            low_error_factor 0.3
            gray_threshold 0.0
            minimum_reuse 0.015
            maximum_reuse 0.1
            brightness 1
            adc_bailout 0.01/2
            normal on
            media Media
            always_sample off
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

#declare Scene_Seed = seed(20240104);
#declare Scene_sundate  = DateTime_create(20204,1,11,13,00,00,TimeZone_UK_GMT);
#ifndef (Scene_View)
#declare Scene_View = 1.1;
#end

// End Scene configuration
//=============================================================================

//=============================================================================
// Camera default configuration
//

#declare camera_location        = <0, Math_Scale(SCALE_FEET, 6), -Math_Scale(SCALE_FEET, 6)>;
#declare camera_lookat          = <0, Math_Scale(SCALE_FEET, 6), 0>;
#declare camera_blur_focus      = camera_lookat;
#declare camera_use_blur        = false;
#declare camera_focal_length    = Math_Scale(SCALE_MM, 50);
#declare camera_blur_fstop      = 8;

// End Camera configuration
//=============================================================================

//=============================================================================
// Scene object declarations
//

//-----------------------------------------------------------------------------
// Camera_light_source()
//
#macro Camera_light_source()
    #local _ls  = light_source {
        camera_location,
        1
    }
    
    _ls
#end

// End Camera_light_source
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Single_brick_object()
//
#macro Single_brick_object()
    #local _brick_shape = Garden_wall_brick_shape(Garden_wall_front,Scene_Seed);
    
    #local _lbounds = <0, 0, 0>;
    #local _ubounds = _brick_shape.brick_spec.brick_sz;
    
    #local _single_brick    = isosurface {
        function {
            _brick_shape.brick_fn(x,y,z,x,y,z)
        }
        threshold 0
        accuracy 0.01
        max_gradient 10
        contained_by { box { _lbounds, _ubounds } }    
    }
    
    _single_brick
#end

// End Single_brick_object
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standing_person()
//
#macro Standing_person()
    #local _person  = union {
        cylinder {
            <0,0,0>,
            <0, Math_Scale(SCALE_FEET, 5+8/12), 0>,
            Math_Scale(SCALE_INCH, 9)
        }
        sphere {
            <0,0,0>, Math_Scale(SCALE_INCH, 5)
            translate <0, Math_Scale(SCALE_FEET, 5+8/12) + Math_Scale(SCALE_INCH, 5), 0>
        }        
    }
    
    _person
#end

// End Standing_person
//-----------------------------------------------------------------------------

// End Scene object declarations
//=============================================================================

//=============================================================================
// View-specific configuration overrides
//

#switch(val(str(Scene_View,0,2)))
    #case(1.1)
    #break
    
    #case(2.1)
        #declare camera_location    = <0, 4*Garden_wall_brick_spec.brick_sz.y, -1*Garden_wall_brick_spec.brick_sz.x>;
        #declare camera_lookat      = <0, Garden_wall_brick_spec.brick_sz.y, 0>;
        #declare Scene_object   = union {
            plane { y, -0.0001 pigment { color rgb 0.25 } }
            object { Camera_light_source() }
            object { Single_brick_object() }
        }    
    #break
    
    #case(2.2)
        #local _ht                  = Garden_wall_courses*Garden_wall_brick_spec.unit_sz.y;
        #declare camera_location    = <0, 0.5*_ht, -1.5*Garden_wall_front_length>;
        #declare camera_lookat      = <0, 0.5*_ht, 0>;
        #declare Scene_object   = union {
            plane { y, -0.0001 pigment { color rgb 0.25 } }
            object { Camera_light_source() }
            box {
                <-Garden_wall_front_length/2, 0, 0>,
                <Garden_wall_front_length/2, _ht, Garden_wall_brick_spec.unit_sz.x>
                pigment { Garden_wall_front_brick_cracks_pigment() }
            }
        }    
    #break
    
    #case(2.3)
        #local _ht                  = Garden_wall_courses*Garden_wall_brick_spec.unit_sz.y;
        #declare camera_location    = <0, 0.5*_ht, -0.5*Garden_wall_front_length>;
        #declare camera_lookat      = <0, 0.5*_ht, 0>;
        #declare Scene_object   = union {
            plane { y, -0.0001 pigment { color rgb 0.25 } }
            object { Camera_light_source() }
            object { Garden_wall_front_bricks() translate <-Garden_wall_front_size.x/2, 0, 0>}
        }    
    #break
    
    #case(2.4)
        #local _tpoint              = <Garden_wall_brick_spec.brick_sz.x + Garden_wall_brick_spec.mortar_sz/2, Garden_wall_brick_spec.unit_sz.y/2, 0>;
        #local _ht                  = Garden_wall_courses*Garden_wall_brick_spec.unit_sz.y;
        #declare camera_location    = <0, 0.5*_ht, -0.25*Garden_wall_front_length>;
        #declare camera_lookat      = <0, 0.5*_ht, 0>;
        #declare Scene_object   = union {
            plane { y, -0.0001 pigment { color rgb 0.25 } }
            object { Camera_light_source() }
            object { Garden_wall_front_mortar() }
        }    
    #break
    
    #case(2.5)
        #local _ht                  = Garden_wall_courses*Garden_wall_brick_spec.unit_sz.y;
        #declare camera_location    = <0, 0.5*_ht, -1.5*Garden_wall_front_length>;
        #declare camera_lookat      = <0, 0.5*_ht, 0>;
        #declare Scene_object   = union {
            plane { y, -0.0001 pigment { color rgb 0.25 } }
            object { Camera_light_source() }
            object { Garden_wall_front_object() }
            object { Standing_person() translate <-Math_Scale(SCALE_INCH, 12), 0, -Math_Scale(SCALE_INCH, 12)> } 
        }    
    #break
    
    #case(2.6)
        #local _ht                  = Garden_wall_courses*Garden_wall_brick_spec.unit_sz.y;
        #declare camera_location    = <0, 0.5*_ht, -1*Garden_wall_side_length>;
        #declare camera_lookat      = <0, 0.5*_ht, 0>;
        #declare Scene_object   = union {
            plane { y, -0.0001 pigment { color rgb 0.25 } }
            object { Camera_light_source() }
            object { Garden_wall_side_object() translate <-Garden_wall_side_size.x/2, 0, 0> }
        }    
    #break
    
    #case(2.7)
        #local _ht                  = Garden_wall_courses*Garden_wall_brick_spec.unit_sz.y;
        #declare camera_location    = <0, 0.5*_ht, -0.5*Garden_wall_front_length>;
        #declare camera_lookat      = <0, 0.5*_ht, 0>;
        #declare Scene_object   = union {
            plane { y, -0.0001 pigment { color rgb 0.25 } }
            object { Sky(Scene_sundate,Scene_Seed) }
            /*
            object { Camera_light_source() }
            light_source {
                Math_Scale(SCALE_FEET, <-10, 2, 10>),
                1
            } 
            */   
            object { Garden_wall_object() }
            //object { Standing_person() translate <-Math_Scale(SCALE_INCH, 12), 0, -Math_Scale(SCALE_INCH, 12)> }
            rotate <0, -45, 0> 
        }    
    #break
    
    #case(2.8)
        #local _ht                  = Garden_wall_courses*Garden_wall_brick_spec.unit_sz.y;
        #declare camera_location    = <0, 0.5*_ht, -0.5*Garden_wall_front_length>;
        #declare camera_lookat      = <0, 0.5*_ht, 0>;
        #declare Scene_object       = union {
            plane { y, -0.0001 pigment { color rgb 0.25 } }
            object { Camera_light_source() }
            box {
                -Garden_wall_front_size/2,
                Garden_wall_front_size/2
                translate Garden_wall_front_size/2
                pigment {
                    Garden_wall_bricks_palette_pigment()
                    pigment_map {
                        Garden_wall_bricks_pigment_map()
                    }    
                }
                translate <-Garden_wall_front_length/2, 0, 0>
            }    
        }
    #break
    
    #case(3.1)
        #local _ht                  = Garden_wall_courses*Garden_wall_brick_spec.unit_sz.y;
        #declare camera_location    = <0, 0.5*_ht, -0.5*Garden_wall_front_length>;
        #declare camera_lookat      = <0, 0.5*_ht, 0>;
        #declare Scene_object       = union {
            object { Sky(Scene_sundate,Scene_Seed) }
        }
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
} 
#end

// End Scene declaration
//=============================================================================

//=============================================================================
// Camera
//        
#ifdef (camera_blur_samples)
    Camera35mm_SetFocalSamples(camera_blur_samples)
#end
#ifdef (camera_blur_variance)
    Camera35mm_SetFocalVariance(camera_blur_variance)
#end
#ifdef (camera_blur_confidence)
    Camera35mm_SetFocalConfidence(camera_blur_confidence)
#end

#if (camera_use_blur)
    #ifdef (camera_blur_focus)
        Camera35mm_Point( camera_location, camera_lookat, camera_blur_focus, camera_focal_length, camera_blur_fstop )
    #else
        Camera35mm( camera_location, camera_lookat, camera_focal_length, camera_blur_fstop )
    #end
#else
    Camera35mm_NoBlur(camera_location, camera_lookat, camera_focal_length, camera_blur_fstop )
#end    
Camera35mm_PrintInfo( camera_location, camera_lookat, camera_focal_length, camera_blur_fstop, camera_blur_focus )

// End Camera
//=============================================================================

//=============================================================================
// Scene Instantiation
//

object { Scene_object }

// End Scene Instantiation
//=============================================================================
    