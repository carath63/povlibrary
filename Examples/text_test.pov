/******************************************************************************
 * File: text_test.pov
 * Description:
 *      Simple scene demonstrating macros in libtext.inc
 ******************************************************************************/

#version 3.8;

//=============================================================================
// Includes
//
#declare View_POV_Include_Stack=true;

//-----------------------------------------------------------------------------
// Standard Includes
//
#include "camera35mm.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
#include "libscale.inc"
#include "liblights.inc"
#include "libtext.inc"

// End My Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project Includes
//

// End Project Includes
//-----------------------------------------------------------------------------

// End Includes
//=============================================================================

global_settings {
    assumed_gamma 1.0
    max_trace_level 5
}

#default {
    finish { ambient 0 diffuse 0.8 }
}        

//=============================================================================
// Scene default configuration
//

// Set the default values, which can be adjusted based on the scene view
//
#declare Scene_seed             = seed(now*24*60*60);

#ifndef (Scene_View)
#declare Scene_View = 1.1;
#end

// End Scene configuration
//=============================================================================

//=============================================================================
// Camera default configuration
//

#declare camera_location        = <0, Math_Scale(SCALE_INCH, 4), -Math_Scale(SCALE_INCH, 36)>;
#declare camera_lookat          = <0, 0, 0>;
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
// Scene_camera_light()
//
// This creates a light located at the camera position as a default
// light source
//
#macro Scene_camera_light()
    #local _l   = light_source {
        camera_location, 1
    }
    
    _l    
#end

// End Scene_camera_light
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_cylindrical_text_object()
//
#macro Scene_cylindrical_text_object()
    #local _fnt         = #ifdef (Scene_cyl_font) Scene_cyl_font; #else "timrom.ttf"; #end
    #local _str         = #ifdef (Scene_cyl_text) Scene_cyl_text; #else "What the heck?"; #end
    #local _r           = #ifdef (Scene_cyl_radius) Scene_cyl_radius; #else Math_Scale(SCALE_INCH, 4); #end
    #local _d           = #ifdef (Scene_cyl_text_depth) Scene_cyl_text_depth; #else Math_Scale(SCALE_INCH, 1/4); #end
    #local _o           = #ifdef (Scene_cyl_text_offset) Scene_cyl_text_offset; #else <0,0,0>; #end
    #local _tlen        = #ifdef (Scene_cyl_text_length) Scene_cyl_text_length; #else pi*_r; #end
    
    #local _text_metrics    = Text_metrics_get(_fnt, _str, _d, _o,,);
    #local _scale           = Text_metrics_scale_to_width(_text_metrics,_tlen);
    #local _scaled_metrics  = Text_metrics_scale(_text_metrics, _scale);
    
    
    #local _cyl_text    = object {
        Text_metrics_layout_cylindrical(_scaled_metrics,_r)
        pigment { color rgb <1, 0, 0> }
        rotate <0, 90, 0>
    }
    
    _cyl_text    
#end

// End Scene_cylindrical_text_object
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_helical_text_object()
//
#macro Scene_helical_text_object()
    #local _fnt         = #ifdef (Scene_helix_font) Scene_helix_font; #else "timrom.ttf"; #end
    #local _str         = #ifdef (Scene_helix_text) Scene_helix_text; #else "Now is the time for all good people to come to the aid of their country"; #end
    #local _r           = #ifdef (Scene_helix_radius) Scene_helix_radius; #else Math_Scale(SCALE_INCH, 2); #end
    #local _d           = #ifdef (Scene_helix_text_depth) Scene_helix_text_depth; #else Math_Scale(SCALE_INCH, 1/4); #end
    #local _o           = #ifdef (Scene_helix_text_offset) Scene_helix_text_offset; #else <0,0,0>; #end
    #local _tlen        = #ifdef (Scene_helix_text_length) Scene_helix_text_length; #else 4*pi*_r; #end
    #local _zrotate     = #ifdef (Scene_helix_zrotate) Scene_helix_zrotate; #else true; #end
    
    #local _text_metrics    = Text_metrics_get(_fnt, _str, _d, _o,,);
    #local _scale           = Text_metrics_scale_to_width(_text_metrics,_tlen);
    #local _scaled_metrics  = Text_metrics_scale(_text_metrics, _scale);

    #local _rise        = #ifdef (Scene_helix_rise) Scene_helix_rise; #else 2*_scaled_metrics.text_size.y; #end    
    
    #local _helix_text    = object {
        Text_metrics_layout_helical(_scaled_metrics,_r,_rise,_zrotate)
        pigment { color rgb <1, 1, 0> }
        translate <0, 0, _r>
    }
    
    _helix_text    
#end

// End Scene_helical_text_object
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_spline_text_object()
//
#macro Scene_spline_text_object()
    #local _fnt         = #ifdef (Scene_spline_font) Scene_spline_font; #else "timrom.ttf"; #end
    #local _str         = #ifdef (Scene_spline_text) Scene_spline_text; #else "Now is the time for all good people to come to the aid of their country"; #end
    #local _d           = #ifdef (Scene_spline_text_depth) Scene_spline_text_depth; #else Math_Scale(SCALE_INCH, 1/4); #end
    #local _o           = #ifdef (Scene_spline_text_offset) Scene_spline_text_offset; #else <0,0,0>; #end
    #local _tlen        = #ifdef (Scene_spline_text_length) Scene_spline_text_length; #else 100; #end
    #local _yrotate     = #ifdef (Scene_spline_yrotate) Scene_spline_yrotate; #else true; #end
    #local _zrotate     = #ifdef (Scene_spline_zrotate) Scene_helix_zrotate; #else false; #end
    #local _r_cyl       = #ifdef (Scene_cyl_radius) Scene_cyl_radius; #else Math_Scale(SCALE_INCH, 4); #end
    #local _epsilon     = #ifdef (Scene_spline_epsilon) Scene_spline_epsilon; #else 1e-4; #end

    #local _text_metrics    = Text_metrics_get(_fnt, _str, _d, _o,,);
    #local _scale           = Text_metrics_scale_to_width(_text_metrics,_tlen);
    #local _scaled_metrics  = Text_metrics_scale(_text_metrics, _scale);
    #debug concat("Spline text size=<", vstr(3, _scaled_metrics.text_size*SCALE_INCH, ",", 0, 2), ">\n")
    
    #ifdef (Scene_spline_spline)
        #local _spline  = Scene_spline_spline;
    #else
        #local _spline  = spline {
            quadratic_spline
            0, <-25, 0, -1.2*_r_cyl>,
            50, <0, 0, -2*_r_cyl>,
            100, <100, 0, -1.2*_r_cyl>,
            200, <200, 0, -1.2*_r_cyl>
        }       
    #end
    
    #local _spline_text = object {
        Text_metrics_layout_spline(_scaled_metrics,_spline,_yrotate,_zrotate,-_d/2,_epsilon)
        pigment { color rgb <0, 0, 1> }
    }
    
    _spline_text    

#end

// End Scene_spline_text_object
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_object_create()
//
// Redefine this to create the scene.  It will be instantiated below
#macro Scene_object_create()
    #ifndef (Scene_use_camera_light)
        #declare Scene_use_camera_light = false;
    #end    
    #local _scene   = union {
        plane {
            y, 0
            texture {
                pigment { color rgb 0.75 }
                finish {
                    fresnel
                    specular albedo 1.0
                    roughness 0.01
                    diffuse albedo 1.0
                    reflection { 1 }
                }
            }
            interior { ior 1.5 }        
        }
        
        object { Scene_cylindrical_text_object() }
        object { Scene_helical_text_object() }
        object { Scene_spline_text_object() }
        
        #if (Scene_use_camera_light)    
        object { Scene_camera_light() }
        #end
    }
    
    _scene
#end

// End Scene_object_create
//-----------------------------------------------------------------------------


// End Scene object declarations
//=============================================================================

//=============================================================================
// View-specific configuration overrides
//

#switch(val(str(Scene_View,0,2)))
    #case(1.1)
        #declare Scene_use_camera_light = true;
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
#declare Scene_object    = object {
    Scene_object_create()
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
    