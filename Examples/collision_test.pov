/******************************************************************************
 * File: collision_test.pov
 * Description:
 *      Sample scene using the collision macros from libcollide.inc
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
#include "camera35mm.inc"
#include "Lightsys.inc"
#include "rand.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
#include "libcollide.inc"

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
    max_trace_level 10
}

#default {
    finish { ambient 0 diffuse 0.8 }
}        

//=============================================================================
// Scene default configuration
//

// Set the default values, which can be adjusted based on the scene view
//
#declare Scene_seed             = seed(20250219);

#ifndef (Scene_View)
#declare Scene_View = 1.1;
#end

// End Scene configuration
//=============================================================================

//=============================================================================
// Camera default configuration
//

#declare camera_location        = <0, Math_Scale(SCALE_INCH, 6), -Math_Scale(SCALE_INCH, 18)>;
#declare camera_lookat          = <0, 0, 0>;
#declare camera_blur_focus      = camera_lookat;
#declare camera_use_blur        = false;
#declare camera_focal_length    = Math_Scale(SCALE_MM, 50);
#declare camera_blur_fstop      = 8;

// End Camera configuration
//=============================================================================

//=============================================================================
// Tree(TrunkRadius,TrunkHeight,ConeRadius,ConeHeight)
// Tree_random(RStream)
//
#declare Tree_min_trunk_radius  = 0.1;
#declare Tree_max_trunk_radius  = 0.5;
#declare Tree_min_trunk_height  = 0.1;
#declare Tree_max_trunk_height  = 1.0;
#declare Tree_min_cone_rratio   = 2.0;
#declare Tree_max_cone_rratio   = 4.0;
#declare Tree_min_cone_height   = 1.0;
#declare Tree_max_cone_height   = 5.0;

#macro Tree(TrunkRadius,TrunkHeight,ConeRadius,ConeHeight)
    #local _tree    = union {
        cylinder {
            <0, 0, 0>,
            <0, TrunkHeight, 0>,
            TrunkRadius
            pigment { color rgb <0.8, 0.7, 0.5> }
        }
        cone {
            <0, 0, 0>, ConeRadius,
            <0, ConeHeight, 0>, 0
            pigment { color rgb <0, 1, 0> }
            translate <0, TrunkHeight, 0>
        }        
    }
    
    _tree
#end

#macro Tree_random(RStream)
    #local _trunk_height    = RRand(Tree_min_trunk_height,Tree_max_trunk_height,RStream);
    #local _trunk_radius    = RRand(Tree_min_trunk_radius,Tree_max_trunk_radius,RStream);
    #local _cone_ratio      = RRand(Tree_min_cone_rratio,Tree_max_cone_rratio,RStream);
    #local _cone_radius     = _trunk_radius * _cone_ratio;
    #local _cone_height     = RRand(Tree_min_cone_height,Tree_max_cone_height,RStream);
    
    #local _tree    = object { Tree(_trunk_radius,_trunk_height,_cone_radius,_cone_height) }
    
    _tree
#end

// End Tree
//=============================================================================

//=============================================================================
// Rock(Radius,Scale)
// Rock_random(RStream)
//
#declare Rock_min_radius    = 0.25;
#declare Rock_max_radius    = 1;
#declare Rock_min_scale     = 0.25;
#declare Rock_max_scale     = 1.0;

#macro Rock(Radius,Scale)
    #local _rock    = difference {
        sphere { <0, 0, 0>, Radius scale Scale }
        plane { y, 0 }
        pigment { color rgb 0.25 }
    }
    
    _rock    
#end

#macro Rock_random(RStream)
    #local _radius  = RRand(Rock_min_radius,Rock_max_radius,RStream);
    #local _scale   = <RRand(Rock_min_scale,Rock_max_scale,RStream),RRand(Rock_min_scale,Rock_max_scale,RStream),RRand(Rock_min_scale,Rock_max_scale,RStream)>;
    
    #local _rock    = Rock(_radius,_scale)
    
    _rock
#end

// End Rock
//=============================================================================

//=============================================================================
// Bush(Radius,Height,Rotation)
// Bush_random(RStream)
// 
#declare Bush_min_radius    = 0.25;
#declare Bush_max_radius    = 1;
#declare Bush_min_height    = 0.5;
#declare Bush_max_height    = 1;

#macro Bush(Radius,Height,Rotation)
    #local _bush    = difference {
        cylinder {
            <0, 0, 0>,
            <0, Height, 0>,
            Radius
        }
        cylinder {
            <0, -1, 0>,
            <0, Height+1, 0>,
            1.1*Radius
            translate <0, 0, Radius>
        }    
        
        pigment { color rgb <0, 0.25, 0> }
        rotate <0, Rotation, 0>    
    }
    
    _bush
#end

#macro Bush_random(RStream)
    #local _radius  = RRand(Bush_min_radius,Bush_max_radius,RStream);
    #local _height  = RRand(Bush_min_height,Bush_max_height,RStream);
    #local _rot     = 360*rand(RStream);
    
    #local _bush    = object { Bush(_radius,_height,_rot) }
    
    _bush
#end 

// End Bush
//=============================================================================

//=============================================================================
// Scene object declarations
//

//-----------------------------------------------------------------------------
// Scene_cluster(optional NumObjects)
//
#macro Scene_cluster(optional NumObjects)
    #local _first_tree  = object { Tree(Tree_max_trunk_radius,Tree_max_trunk_height,Tree_max_trunk_radius*Tree_max_cone_rratio,Tree_max_cone_height) }
    #local _grove       = object { _first_tree }
    
    #ifdef (NumObjects)
        #local _grove_objects     = NumObjects;
    #else    
        #local _grove_objects     = 10;
    #end    
    #local _grove_gap       = 0;
    
    #local _obj_rres        = 1;
    #local _obj_vres        = 0.05;
    #local _tolerance       = 0.01;
    
    #macro _pick_object()
        #local _rnd = rand(Scene_seed);
        #if (_rnd < 0.5)
            #local _obj = object { Tree_random(Scene_seed) }
        #elseif (_rnd < 0.85)
            #local _obj = object { Bush_random(Scene_seed) }
        #else
            #local _obj = object { Rock_random(Scene_seed) }
        #end
        
        _obj            
    #end
    
    #for (i, 1, _grove_objects-1)
        #local _grove_ctr   = (max_extent(_grove) + min_extent(_grove))/2;
        #local _new_obj    = object { _pick_object() }
        #local _from3d      = _grove_ctr + vrotate(z*100, <0, 360*rand(Scene_seed), 0>);
        #local _from        = <_from3d.x,_from3d.z>;
        #local _trans       = Col_slide_object_to_object(_new_obj,_from,_grove,_obj_rres,_obj_vres,_grove_gap,_tolerance);
        #local _grove       = union {
            object { _grove }
            object { _new_obj translate _trans }
        }    
    #end
    
    #declare Scene_grove_ctr    = (max_extent(_grove) + min_extent(_grove))/2;
    #declare Scene_grove_sz     = (max_extent(_grove) - min_extent(_grove));
    
    _grove
#end

// End Scene_cluster
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
    #local _scene   = union {
        #ifdef (Scene_cluster_obj)
            object { Scene_cluster_obj }
        #else    
            object { Scene_cluster() }
        #end    
        object { Scene_camera_light() }
        plane { y, 0 pigment { color rgb 0.5 } }
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
        #declare Scene_cluster_obj    = object { Scene_cluster() }
        #declare camera_location    = <Scene_grove_ctr.x, 20, Scene_grove_ctr.z - 0.1>;
        #declare camera_lookat      = <Scene_grove_ctr.x, 0, Scene_grove_ctr.z>;
    #break
    
    #case(1.2)
        #declare Scene_cluster_obj    = object { Scene_cluster(25) }
        #declare camera_location    = <Scene_grove_ctr.x, Tree_max_trunk_height, Scene_grove_ctr.z - 2*Scene_grove_sz.z>;
        #declare camera_lookat      = <Scene_grove_ctr.x, Tree_max_trunk_height, Scene_grove_ctr.z>;
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
    