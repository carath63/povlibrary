/******************************************************************************
 * File: chain_test.pov
 * Description:
 *      This is a simple scene using the sample silver chain definition
 *  of the various chain generation functions in libchain.inc
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
#include "libchain.inc"
#include "chains/samples.inc"

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

#declare camera_location        = <0, Math_Scale(SCALE_INCH, 6), -Math_Scale(SCALE_INCH, 36)>;
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
// Scene_chain_segment(ChainLayout)
//
#macro Scene_chain_segment(ChainLayout)
    #local _a   = ChainLayout.resting_link_angle;
    
    #local _scs_z_rot_fn    = function(N,x,y,z) {
        (0.5 - mod(N,2))*_a
    }
    #local _options         = Chain_layout_options_create(,,,_z_offset_fn,);
    
    Chain_layout_add_segment(_chain_layout,Scene_chain_straight_segment_end,_options)
    Chain_layout_add_segment(_chain_layout,Scene_chain_slack_segment_start,)
#end

// End Scene_chain_segment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_chain_slack_segment(ChainLayout)
//
#macro Scene_chain_slack_segment(ChainLayout)
    Chain_layout_add_slack_segment(ChainLayout,Scene_chain_slack_segment_end,Scene_chain_slack_segment_length,)
#end

// End Scene_chain_slack_segment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_chain_catenary_segment(ChainLayout)
//
#macro Scene_chain_catenary_segment(ChainLayout)
    Chain_layout_add_catenary_segment2(ChainLayout,Scene_chain_catenary_end,Scene_chain_catenary_length,,)
#end

// End Scene_chain_catenary_segment
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_chain()
//
#macro Scene_chain()
    #declare Chain_layout_verbose   = 2;
    // Define the locations of each of the chain segments
    //
    #ifndef(Scene_chain_start)
        #declare Scene_chain_start  = <-Math_Scale(SCALE_CM, 25), 0, 0>;
    #end                                                                                                                     
    #local _link_length = #ifdef(Scene_chain_link_length) Scene_chain_link_length; #else Math_Scale(SCALE_CM, 1.15); #end
    #local _link_width  = #ifdef(Scene_chain_link_width) Scene_chain_link_width; #else Math_Scale(SCALE_CM, 0.5); #end
    #local _link_radius = #ifdef(Scene_chain_link_radius) Scene_chain_link_radius; #else Math_Scale(SCALE_CM, 0.085); #end
    
    #local _chain_spec      = Simple_silver_chain_spec_create(_link_length,_link_width,_link_radius);
    
    #local _link_rest_angle = Simple_silver_chain_rest_angle(_chain_spec.link_specs[0]);
    
    #local _link            = object { Simple_silver_chain_link_shape(_link_length,_link_width,_link_radius) }
    
    #local _link_pair       = union {
        object { _link rotate <0, 0, -_link_rest_angle/2> }
        object { _link rotate <0, 0, _link_rest_angle/2> translate <0, 0, _chain_spec.link_specs[0].link_inner.y> }    
    }
    
    #local _resting_link_pair_min    = min_extent(_link_pair);
    #local _resting_link_pair_max    = max_extent(_link_pair);
    
    #local _chain_layout                            = Chain_layout_create(_chain_spec,Scene_chain_start+<0, -_resting_link_pair_min.y, 0>,);
    #declare _chain_layout.resting_link_angle       = _link_rest_angle;
    #declare _chain_layout.resting_link_pair_min    = _resting_link_pair_min;
    #declare _chain_layout.resting_link_pair_max    = _resting_link_pair_max;
    
    #ifndef(Scene_chain_straight_segment_end)
        #declare Scene_chain_straight_segment_end   = <0, -_chain_layout.resting_link_pair_min.y, -Math_Scale(SCALE_CM, 10)>;
    #end
    #ifndef(Scene_chain_slack_segment_start)
        #declare Scene_chain_slack_segment_start    = <Math_Scale(SCALE_CM, 5), Math_Scale(SCALE_CM, 2), 0>;
    #end
    #ifndef(Scene_chain_slack_segment_length)
        #declare Scene_chain_slack_segment_length   = Math_Scale(SCALE_CM, 17);
    #end    
    #ifndef(Scene_chain_slack_segment_end)
        #declare Scene_chain_slack_segment_end      = Scene_chain_slack_segment_start + 0.95*Scene_chain_slack_segment_length*x;
    #end
    #ifndef(Scene_chain_catenary_end)
        #declare Scene_chain_catenary_end           = Scene_chain_slack_segment_end + <0, 0, -Math_Scale(SCALE_CM, 20)>;
    #end
    #ifndef(Scene_chain_catenary_length)
        #declare Scene_chain_catenary_length        = 1.005*vlength(Scene_chain_catenary_end - Scene_chain_slack_segment_end);
    #end                    
    
    Scene_chain_segment(_chain_layout)
    Scene_chain_slack_segment(_chain_layout)
    Scene_chain_catenary_segment(_chain_layout)
    
    #local _chain   = object {
        Chain_layout_render(_chain_layout)
    }
    
    _chain
#end

// End Scene_chain
//-----------------------------------------------------------------------------

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
            pigment {
                checker
                color rgb 0
                color rgb 1
                scale 2
            }    
        }
        object { Scene_chain() }
        #if(Scene_use_camera_light)    
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
    