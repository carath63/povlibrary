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
    #local _trunk_height    = Tree_min_trunk_height + (Tree_max_trunk_height - Tree_min_trunk_height)*rand(RStream);
    #local _trunk_radius    = Tree_min_trunk_radius + (Tree_max_trunk_radius - Tree_min_trunk_radius)*rand(RStream);
    #local _cone_ratio      = Tree_min_cone_rratio + (Tree_max_cone_rratio - Tree_min_cone_rratio)*rand(RStream);
    #local _cone_radius     = _trunk_radius * _cone_ratio;
    #local _cone_height     = Tree_min_cone_height + (Tree_max_cone_height - Tree_min_cone_height)*rand(RStream);
    
    #local _tree    = object { Tree(_trunk_radius,_trunk_height,_cone_radius,_cone_height) }
    
    _tree
#end

// End Tree
//=============================================================================

//=============================================================================
// Scene object declarations
//

//-----------------------------------------------------------------------------
// Scene_trees(optional NumTrees)
//
#macro Scene_trees(optional NumTrees)
    #local _first_tree  = object { Tree(Tree_max_trunk_radius,Tree_max_trunk_height,Tree_max_trunk_radius*Tree_max_cone_rratio,Tree_max_cone_height) }
    #local _grove       = object { _first_tree }
    
    #ifdef (NumTrees)
        #local _grove_trees     = NumTrees;
    #else    
        #local _grove_trees     = 10;
    #end    
    #local _grove_gap       = 0;
    
    #local _tree_rres       = 45;
    #local _tree_vres       = 0.01;
    #local _tolerance       = 0.01;
    
    #for (i, 1, _grove_trees-1)
        #local _grove_ctr   = (max_extent(_grove) + min_extent(_grove))/2;
        #local _new_tree    = object { Tree_random(Scene_seed) }
        #local _from3d      = _grove_ctr + vrotate(z*100, <0, 360*rand(Scene_seed), 0>);
        #local _from        = <_from3d.x,_from3d.z>;
        #local _trans       = Col_slide_object_to_object(_new_tree,_from,_grove,_tree_rres,_tree_vres,_grove_gap,_tolerance);
        #local _grove       = union {
            object { _grove }
            object { _new_tree translate _trans }
        }    
    #end
    
    #declare Scene_grove_ctr    = (max_extent(_grove) + min_extent(_grove))/2;
    #declare Scene_grove_sz     = (max_extent(_grove) - min_extent(_grove));
    
    _grove
#end

// End Scene_trees
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
        #ifdef (Scene_trees_obj)
            object { Scene_trees_obj }
        #else    
            object { Scene_trees() }
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
        #declare Scene_trees_obj    = object { Scene_trees() }
        #declare camera_location    = <Scene_grove_ctr.x, 20, Scene_grove_ctr.z - 0.1>;
        #declare camera_lookat      = <Scene_grove_ctr.x, 0, Scene_grove_ctr.z>;
    #break
    
    #case(1.2)
        #declare Scene_trees_obj    = object { Scene_trees(25) }
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
    