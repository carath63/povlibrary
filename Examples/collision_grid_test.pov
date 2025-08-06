/******************************************************************************
 * File: collision_grid_test.pov
 * Description:
 *      A test scene using the Col_rest_grid_on_surface_transform() macro
 *  to place various objects on an uneven surface.
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
#include "rand.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
#include "libscale.inc"
#include "liblights.inc"
#include "libmesh.inc"
#include "libcollide.inc"
#include "libstringify.inc"

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

#declare camera_location        = <0, 10, -20>;
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
//
#declare Scene_object_type_disc     = 0;
#declare Scene_object_type_box      = 1;
#declare Scene_object_type_sphere   = 2;
#declare Scene_object_type_random   = 3;

//-----------------------------------------------------------------------------
// Scene_set_parameters()
//
//  This macro ensures values have been set for all of the following
//  global variables that are used in generating the scene view.  Predefined
//  values may come from command line Declare= commands, or from values
//  set with different Declare=Scene_View= values, set in case blocks in
//  the select(Scene_View) block below.
//
//  It is called by Scene_object_create, but can be called ahead of time as well.
//
//      Scene_surface_resolution: Set to a float value that makes the surface
//      have steps instead of smooth bumps.  int(Y*Scene_surface_resolution)/Scene_surface_resolution
//
#macro Scene_set_parameters()
    #ifndef(Scene_surface_size)
        #local _sx  = #ifdef(Scene_surface_x_size) Scene_surface_x_size; #else 10; #end
        #local _sy  = #ifdef(Scene_surface_y_size) Scene_surface_y_size; #else 2; #end
        #local _sz  = #ifdef(Scene_surface_z_size) Scene_surface_z_size; #else 10; #end
        #declare Scene_surface_size = <_sx, _sy, _sz>;
    #end
    #ifndef(Scene_surface_pigment)
        #declare Scene_surface_pigment  = pigment {
            bozo
            color_map {
                [0.0 rgb 0]
                [1.0 rgb 1]
            }
            scale Scene_surface_size/<6, 1, 6>    
        }
    #end
    #ifndef(Scene_surface_resolution)
        #declare Scene_surface_resolution   = 0;
    #end    
    #ifndef(Scene_object_type)
        #declare Scene_object_type  = Scene_object_type_random;
    #end
    #ifndef(Scene_object_grid_size)
        #local _sx  = #ifdef(Scene_object_grid_x_size) Scene_object_grid_x_size; #else 10; #end
        #local _sz  = #ifdef(Scene_object_grid_z_size) Scene_object_grid_z_size; #else 10; #end
        #declare Scene_object_grid_size = <_sx, _sz>;
    #end
    #ifndef(Scene_object_count)
        #declare Scene_object_count = 10;
    #end 
    #ifndef(Scene_object_max_float)
        #declare Scene_object_max_float = 0.05;
    #end 
#end

// End Scene_set_parameters
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
// Scene_test_object(ObjectId)
//
// Creates an object of the given type
//
#macro Scene_test_object(ObjectId)
    #if (ObjectId = Scene_object_type_random)
        #local _object_id   = max(0, min(2, int(3*rand(Scene_seed))));
    #else
        #local _object_id   = ObjectId;        
    #end
    
    #local _object  = #switch(_object_id)
        #case(Scene_object_type_disc)
            cylinder {
                <0, 0, 0>,
                <0, 0.1, 0>,
                1
            }    
        #break
        
        #case(Scene_object_type_sphere)
            sphere {
                <0,0,0>, 1
                translate <0, 1, 0>
            }    
        #break
        
        #case(Scene_object_type_box)
            box {
                <-1, 0, -1>,
                <1, 2, 1>
                scale 0.5
            }    
        #break
        
        #else
            #error concat("Unknown object id ", str(_object_id, 0, 0), "\n")
        #break
    #end
    
    _object
        
#end
// End Scene_test_object
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_test_surface()
//
// Creates a square surface whose height is determined by the Scene_surface_pigment
//
#macro Scene_test_surface()
    #local _lbounds = <-Scene_surface_size.x/2, 0, -Scene_surface_size.z/2>;
    #local _ubounds = <Scene_surface_size.x/2, Scene_surface_size.y, Scene_surface_size.z/2>;
    #local _sy      = Scene_surface_size.y;
    
    #local _rts_bumps_pfn   = function {
        pigment { Scene_surface_pigment }
    }
    #local _rts_bumps_fn    = function(x,y,z) {
        #if (Scene_surface_resolution > 0)
        int(_rts_bumps_pfn(x,0,z).gray * _sy * Scene_surface_resolution)/Scene_surface_resolution
        #else
        _rts_bumps_pfn(x,0,z).gray * _sy
        #end
    }
    #local _rts_shape_fn    = function(x,y,z) {
        y - _rts_bumps_fn(x,y,z)    
    }
    
    #local _surface = isosurface {
        function { _rts_shape_fn(x,y,z) }
        threshold 0
        contained_by { box { _lbounds, _ubounds } }
        accuracy 0.01
        max_gradient 2.1
        
        pigment { color rgb <1, 0, 0> }
    }
    
    _surface        
#end

// End Scene_test_surface
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_object_create()
//
// Redefine this to create the scene.  It will be instantiated below
#macro Scene_object_create()
    Scene_set_parameters()
    #ifndef (Scene_use_camera_light)
        #declare Scene_use_camera_light = false;
    #end
    
    #local _surface = object { Scene_test_surface() }
    #for (i, 1, Scene_object_count, 1)
        #local _object      = object { Scene_test_object(Scene_object_type) }
        #local _mesh_grid   = Mesh_grid_surface_create(_object,,Scene_object_grid_size,-1,,)

        #if (i > 1)
            #local _loc_x   = 0.5*Scene_surface_size.x * SRand(Scene_seed);
            #local _loc_z   = 0.5*Scene_surface_size.z * SRand(Scene_seed);
        #else
            #local _loc_x   = 0;
            #local _loc_z   = 0;
        #end
        
        #local _object_float    = 10*Scene_object_max_float;
        #while (_object_float > Scene_object_max_float)        
            #debug concat("Trying to place object at <", str(_loc_x, 0, 6), ",", str(_loc_z, 0, 6), ">\n")
            #local _sfy_transform   = Col_rest_grid_on_surface_transform(_mesh_grid,_surface,<_loc_x,0,_loc_z>,0);
            SFY_transform_dict_dump(_sfy_transform,)
            #local _object_float    = max(
                (_sfy_transform.rest_points.op0 - _sfy_transform.rest_points.sp0).y,
                (_sfy_transform.rest_points.op1 - _sfy_transform.rest_points.sp1).y,
                (_sfy_transform.rest_points.op2 - _sfy_transform.rest_points.sp2).y
            )
            ;
            #if (_object_float > Scene_object_max_float)
                #debug concat("_object_float = ", str(_object_float, 0, 6), " exceeds Scene_object_max_float ", str(Scene_object_max_float, 0, 6), "; retrying\n")
                #local _loc_x   = _loc_x + 0.25*SRand(Scene_seed)*_mesh_grid.grid_bb.bbsize.x;
                #local _loc_z   = _loc_z + 0.25*SRand(Scene_seed)*_mesh_grid.grid_bb.bbsize.z;
            #end
        #end
        #debug concat("_sfy_transform[", str(i, 0, 0), "]=\n")
        #local _transform       = transform { SFY_transform_sdl(_sfy_transform) }

        #local _surface = union {
            object { _surface }
            object { 
                _object
                transform { _transform }
            }    
        }
    #end
        
    #local _scene   = union {
        plane {
            y, 0
            pigment { color rgb 0.25 }
        }
        object { _surface }
        
        object { Scene_camera_light() }
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
    