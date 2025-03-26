/******************************************************************************
 * File: chain_test.pov
 * Description:
 *      This is a simple scene using the sample silver chain definition
 *  of the various chain generation functions in libchain.inc
 *
 *  The scene contains the following:
 *
 *      - Coil segment around a cylinder
 *      - Straight segment from cylinder to the floor
 *      - Spline segment lying on the plane from coil to a point near the first pole
 *      - Straight segment from the end of the spline to the top of the first pole
 *      - Slack segment from first pole to second pole
 *      - Catenary segment from second pole to third pole
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

#declare camera_location        = <0, Math_Scale(SCALE_CM, 200), -Math_Scale(SCALE_M, 2)>;
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
// Scene_variables()
//
//      Set default values for any scene location, length, etc. variables
//  that are not defined by a particular Scene_View case below.
//
//  This macro is called first in the Scene_object_create() macro.  If a
//  Scene_View defines Scene_object without calling Scene_object_create,
//  be sure to call Scene_variables() first.
//
#macro Scene_variables()
    #ifndef(Scene_chain_link_size) #declare Scene_chain_link_size = Math_Scale(SCALE_CM, <0.5, 0.17, 1.15>); #end

    #declare Scene_chain_spec                       = Simple_silver_chain_spec_create(Scene_chain_link_size.z,Scene_chain_link_size.x,Scene_chain_link_size.y/2);
    #declare Scene_chain_spec.resting_link_angle    = Simple_silver_chain_rest_angle(Scene_chain_spec.link_specs[0]);
    
    #local _link            = object { Simple_silver_chain_link_shape(Scene_chain_link_size.z,Scene_chain_link_size.x,Scene_chain_link_size.y/2) }
    
    #local _link_pair       = union {
        object { _link rotate <0, 0, -Scene_chain_spec.resting_link_angle/2> }
        object { _link rotate <0, 0, Scene_chain_spec.resting_link_angle/2> translate <0, 0, Scene_chain_spec.link_specs[0].link_inner.y> }    
    }
    
    #declare Scene_chain_spec.resting_link_pair_min = min_extent(_link_pair);
    #declare Scene_chain_spec.resting_link_pair_max = max_extent(_link_pair);
    #declare Scene_chain_spec.resting_link_size     = Scene_chain_spec.resting_link_pair_max - Scene_chain_spec.resting_link_pair_min;

    #ifndef(Scene_cylinder_start) #declare Scene_cylinder_start = Math_Scale(SCALE_CM, <-25, 25, 10>); #end
    #ifndef(Scene_cylinder_end) #declare Scene_cylinder_end = Scene_cylinder_start + <25, 0, 0>; #end
    #ifndef(Scene_cylinder_radius) #declare Scene_cylinder_radius = Math_Scale(SCALE_CM, 4); #end
    #local _cylinder_length = vlength(Scene_cylinder_end - Scene_cylinder_start);
    #local _cylinder_dir    = vnormalize(Scene_cylinder_end - Scene_cylinder_start);
    #if (vlength(vcross(7,_cylinder_dir)) = 0)
        #local _right   = -z;
    #else
        #local _right   = VPerp_To_Plane(y,_cylinder_dir);
    #end
    #declare Scene_chain_coil_start                 = Scene_cylinder_start + (Scene_cylinder_radius + Scene_chain_spec.resting_link_pair_max.y)*_right + 0.125*_cylinder_length*_cylinder_dir;
    #ifndef(Scene_chain_coil_revolutions) #declare Scene_chain_coil_revolutions = 0.5 + int(0.75*_cylinder_length/Scene_chain_spec.resting_link_size.x); #end
    #ifndef(Scene_chain_coil_offset) #declare Scene_chain_coil_offset = 1.0; #end
    
    // Note: this defines the starting and ending points for the straight drop segment, so no new variables introduced here
    //
    #declare Scene_chain_start                      = <Scene_chain_coil_start.x, Scene_chain_link_size.y, Scene_chain_coil_start.z>;
    
    #declare Scene_chain_layout                     = Chain_layout_create(Scene_chain_spec,Scene_chain_start,);
    
    #debug concat(
        "Scene_cylinder_start=<", vstr(3, Scene_cylinder_start, ",", 0, 3), ">\n",
        "Scene_cylinder_end=<", vstr(3, Scene_cylinder_end, ",", 0, 3), ">\n",
        "Scene_cylinder_radius=", str(Scene_cylinder_radius, 0, 3), "\n",
        "Scene_chain_coil_start=<", vstr(3, Scene_chain_coil_start, ",", 0, 3), ">\n",
        "Scene_chain_start=<", vstr(3, Scene_chain_start, ",", 0, 3), ">\n" 
    )
    
    // Pole 1 and spline
    //
    #ifndef(Scene_pole1_location) #declare Scene_pole1_location = <Scene_cylinder_end.x + Math_Scale(SCALE_CM, 10), 0, Scene_cylinder_end.z>; #end
    #ifndef(Scene_pole1_height) #declare Scene_pole1_height = Math_Scale(SCALE_CM, 30); #end
    #ifndef(Scene_pole1_radius) #declare Scene_pole1_radius = Math_Scale(SCALE_CM, 1); #end
    #ifndef(Scene_spline_end) #declare Scene_spline_end = Scene_pole1_location - <Math_Scale(SCALE_CM, 5)+Scene_pole1_radius, -Scene_chain_spec.resting_link_pair_min.y, 0>; #end
    
    #debug concat(
        "Scene_pole1_location=<", vstr(3, Scene_pole1_location, ",", 0, 3), ">\n",
        "Scene_pole1_height=", str(Scene_pole1_height, 0, 3), "\n",
        "Scene_pole1_radius=", str(Scene_pole1_radius, 0, 3), "\n",
        "Scene_spline_end=<", vstr(3, Scene_spline_end, ",", 0, 3), ">\n"
    )
    
    // Pole 2 and slack
    //
    #ifndef(Scene_pole2_location) #declare Scene_pole2_location = Scene_pole1_location + <Math_Scale(SCALE_CM, 25), 0, 0>; #end
    #ifndef(Scene_pole2_height) #declare Scene_pole2_height = Scene_pole1_height; #end
    #ifndef(Scene_pole2_radius) #declare Scene_pole2_radius = Scene_pole1_radius; #end
    #ifndef(Scene_pole2_slack_len) #declare Scene_pole2_slack_len = 1.25*vlength(Scene_pole2_location - Scene_pole1_location); #end

    #debug concat(
        "Scene_pole2_location=<", vstr(3, Scene_pole2_location, ",", 0, 3), ">\n",
        "Scene_pole2_height=", str(Scene_pole2_height, 0, 3), "\n",
        "Scene_pole2_radius=", str(Scene_pole2_radius, 0, 3), "\n",
        "Scene_pole2_slack_len=", str(Scene_pole2_slack_len, 0, 3), "\n"
    )
    
    // Pole 3 and catenary
    //
    #ifndef(Scene_pole3_location) #declare Scene_pole3_location = Scene_pole2_location + <Math_Scale(SCALE_CM, 25), 0, 0>; #end
    #ifndef(Scene_pole3_height) #declare Scene_pole3_height = 1.25*Scene_pole2_height; #end
    #ifndef(Scene_pole3_radius) #declare Scene_pole3_radius = Scene_pole2_radius; #end
    #ifndef(Scene_pole3_catenary_len) #declare Scene_pole3_catenary_len = 1.25*vlength(Scene_pole3_location - Scene_pole2_location); #end

    #debug concat(
        "Scene_pole3_location=<", vstr(3, Scene_pole3_location, ",", 0, 3), ">\n",
        "Scene_pole3_height=", str(Scene_pole3_height, 0, 3), "\n",
        "Scene_pole3_radius=", str(Scene_pole3_radius, 0, 3), "\n",
        "Scene_pole3_catenary_len=", str(Scene_pole3_catenary_len, 0, 3), "\n"
    )
    
#end

// End Scene_variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_chain_coil_objects()
//      Create the coil cylinder, the segment from the chain start to the coil
//      start, the coil links, and the segment from the coil end to the floor
//
#macro Scene_chain_coil_objects()
    #local _coil_objects    = union {
        cylinder {
            Scene_cylinder_start,
            Scene_cylinder_end,
            Scene_cylinder_radius
            material {
                texture {
                    pigment { color rgb <1, 1, 0> }
                    normal { bumps 0.1 scale 0.1 }
                    finish {
                        fresnel
                        specular albedo 1.0
                        roughness 0.1
                        diffuse albedo 1.0
                    }
                }
                interior { ior 1.5 }
            }            
        }
    }
    
    #local _coil_axis   = vnormalize(Scene_cylinder_end - Scene_cylinder_start);
    #local _r               = Scene_chain_spec.resting_link_angle;
    #local _coil_zrot_fn    = function(N,x,y,z) {
        (0.5 - mod(N,2))*_r
    }
    #local _coil_options    = Chain_layout_options_create(,,,,_coil_zrot_fn)
    Chain_layout_add_segment(Scene_chain_layout, Scene_chain_coil_start,)
    Chain_layout_add_coil_segment(Scene_chain_layout,_coil_axis,Scene_cylinder_start,Scene_chain_coil_revolutions,Scene_chain_coil_offset,_coil_options)
    
    #declare Scene_chain_coil_end   = Scene_chain_layout.links[Scene_chain_layout.num_links-1].link_front;
    #declare Scene_chain_coil_drop_end  = <Scene_chain_coil_end.x, 0, Scene_chain_coil_end.z>;
    Chain_layout_add_segment(Scene_chain_layout, Scene_chain_coil_drop_end,)
    #if (Scene_chain_layout.links[Scene_chain_layout.num_links-1].link_front.y < 0)
        #declare Scene_chain_layout.num_links   = Scene_chain_layout.num_links-1;
    #end     
    
    _coil_objects
#end

// End Scene_chain_coil_objects
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_chain_spline_create()
//
#macro Scene_chain_spline_create()
    #local _floor_offset    = -Scene_chain_spec.resting_link_pair_min.y;
    #local _last_link       = Scene_chain_layout.links[Scene_chain_layout.num_links-1].link_front;
    #local _spline_dir      = vnormalize(Scene_spline_end - _last_link); 
    #local _spline_len      = vlength(Scene_spline_end - _last_link);
    #local _spline_xz       = _spline_dir*<1,0,1>;
    
    #local _raw_spline_len  = 0;
    #local _raw_spline_pt   = _last_link;
    #local _raw_spline  = spline {
        quadratic_spline
        _raw_spline_len, _raw_spline_pt,
        
        #if (_last_link.y > _floor_offset)
            #local _v   = _last_link.y - Scene_chain_link_size.y;
            #local _l   = Scene_chain_spec.link_specs[0].link_inner.y;
            #local _h   = sqrt(_l*_l - _v*_v);
            #local _xz  = _spline_xz*_h;
            #local _pt  = <_xz.x, _floor_offset, _xz.z>;
            #local _raw_spline_len  = _raw_spline_len + vlength(_pt - _raw_spline_pt);
            _raw_spline_len, _pt,
            #local _raw_spline_pt   = _pt;
        #end
        
        #local _mid_pt  = _raw_spline_pt + (_spline_dir*0.5*_spline_len);
        #local _pt      = <_mid_pt.x, _floor_offset, _mid_pt.z + 0.25*_spline_len>;
        #local _raw_spline_len  = _raw_spline_len + vlength(_pt - _raw_spline_pt);
        _raw_spline_len, _pt,
        #local _raw_spline_pt   = _pt;
        
        #local _raw_spline_len  = _raw_spline_len + vlength(Scene_spline_end - _raw_spline_pt);
        _raw_spline_len, Scene_spline_end
    }
    
    #local _dd1     = 0;
    #local _dspline = Spline_create_distance_spline(_raw_spline,"quadratic_spline",0,_raw_spline_len,0.25*Scene_chain_link_size.z,,,, _dd1)

    #local _r               = Scene_chain_spec.resting_link_angle;
    #local _spline_zrot_fn  = function(N,x,y,z) {
        (0.5 - mod(N,2))*_r
    }
    #local _spline_options    = Chain_layout_options_create(,,,,_spline_zrot_fn)
    
    Chain_layout_add_spline_segment(Scene_chain_layout, _dspline, _dd1, _spline_options)
    
#end

// End Scene_chain_spline_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Find_pole_intersection(PoleObject,GroundPt)
//
//      Finds the point at the top of the pole where a chain link would hit
//  if a straight line were drawn from the GroundPt to the top of the pole
//  object.
//
#macro Find_pole_intersection(PoleObject,GroundPt)
    #local _pole_min        = min_extent(PoleObject);
    #local _pole_max        = max_extent(PoleObject);
    #local _pole_c          = (_pole_min + _pole_max)/2;
    #local _pole_ground_c   = _pole_c*<1,0,1> + <0, GroundPt.y, 0>;
    #local _ground_pole_dir = (_pole_ground_c - GroundPt);
    #local _ground_pole_rot = degrees(atan2(_ground_pole_dir.x,_ground_pole_dir.z));
    #local _nrm             = <0,0,0>;

    #debug concat("GroundPt=<", vstr(3, GroundPt, ",", 0, 3), ">\n")
    #debug concat("_pole_c=<", vstr(3, _pole_c, ",", 0, 3), ">\n")
    #debug concat("_ground_pole_rot=", str(_ground_pole_rot, 0, 3), "\n")
    #debug concat("_ground_pole_dir=<", vstr(3, _ground_pole_dir, ",", 0, 3), ">\n")
    
    #local _step            = 1;
    #local _cur_a           = 90;
    #local _ipt             = <0,0,0>;
    #while (_cur_a > -90 & vlength(_nrm) = 0)
        #local _a1      = vrotate(z,<-_cur_a,0,0>);
        #local _tdir    = vnormalize(vrotate(_a1,<0, _ground_pole_rot, 0>));
        #local _hit     = trace(PoleObject, GroundPt, _tdir, _nrm);
        #local _cur_a   = _cur_a - _step;
    #end                                                           
    
    #if (vlength(_nrm) = 0)
        #error concat("No intersection found with pole from <", vstr(3, GroundPt, ",", 0, 3), ">\n")
    #end
    
    _hit    
    
#end

// End Find_pole_intersection
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_chain_pole1_objects()
//
#macro Scene_chain_pole1_objects()
    #local _objects = union {
        cylinder {
            Scene_pole1_location,
            Scene_pole1_location + <0, Scene_pole1_height, 0>,
            Scene_pole1_radius
            material {
                texture {
                    pigment { color rgb <1, 0, 0> }
                    finish {
                        fresnel
                        specular albedo 1.0
                        roughness 0.01
                        diffuse albedo 1.0
                    }
                }
                interior { ior 1.5 }        
            }
        }    
    }
    #local _last_pt     = Scene_chain_layout.links[Scene_chain_layout.num_links-1].link_front;
    
    // This part is easier than what Find_pole_intersection does...
    //
    #local _chain_pole_disp     = (Scene_pole1_location*<1,0,1> - _last_pt*<1,0,1>);
    #local _chain_pole_dist     = vlength(_chain_pole_disp);
    #local _chain_pole_dir      = vnormalize(_chain_pole_disp);
    
    #local _chain_pole_hdisp    = (Scene_pole1_location.y + Scene_pole1_height - _last_pt.y);
    #local _chain_pole_angle    = degrees(atan2(_chain_pole_hdisp,vlength(_chain_pole_disp)));
    #local _chain_pole_pt       = _last_pt + _chain_pole_dir*(_chain_pole_dist - Scene_pole1_radius) + <0, _chain_pole_hdisp, 0>;
    #local _chain_pole_axis     = vnormalize(_chain_pole_pt - _last_pt);
    #local _chain_pole_right    = VPerp_To_Plane(y, _chain_pole_axis);
    #local _chain_pole_up       = VPerp_To_Plane(_chain_pole_axis, _chain_pole_right);
    
    #debug concat("_chain_pole_pt=<", vstr(3, _chain_pole_pt, ",", 0, 3), ">\n")
    #local _chain_end           = _chain_pole_pt + 0.5*Scene_chain_link_size.x*_chain_pole_up;
    
    Chain_layout_add_segment(Scene_chain_layout, _chain_end,)
    
    // Find the axis between the last chain link and the top of pole 2
    //
    #local _last_pt             = Scene_chain_layout.links[Scene_chain_layout.num_links-1].link_front;
    #local _pole2_top           = Scene_pole2_location + <0, Scene_pole2_height, 0>;
    #local _pole1_top           = Scene_pole1_location + <0, Scene_pole1_height, 0>;
    #local _chain_pole1_disp    = _pole1_top*<1,0,1> - _last_pt*<1,0,1>;
    #local _chain_pole1_dist    = vlength(_chain_pole1_disp);
    #local _chain_pole1_dir     = vnormalize(_chain_pole1_disp);
    
    #local _chain_pole2_disp    = _pole2_top*<1,0,1> - _last_pt*<1,0,1>;
    #local _chain_pole2_dist    = vlength(_chain_pole2_disp);
    #local _chain_pole2_dir     = vnormalize(_chain_pole2_disp);
    
    #local _a   = acos(vdot(_chain_pole1_dir,_chain_pole2_dir));
    #local _c   = Scene_pole1_radius*sin(pi - 2*_a)/sin(_a);
    #local _chain_pole1_pt      = _last_pt*<1,0,1> + _chain_pole2_dir*_c + <0, Scene_pole2_height, 0>;
    #local _chain_pole1_disp    = _chain_pole1_pt - _last_pt;
    #local _chain_pole1_dir     = vnormalize(_chain_pole1_disp);
    #local _chain_pole1_right   = VPerp_To_Plane(y,_chain_pole1_dir);
    #local _chain_pole1_up      = VPerp_To_Plane(_chain_pole1_dir,_chain_pole1_right);
    #local _chain_end           = _chain_pole1_pt + 0.5*Scene_chain_link_size.x*_chain_pole1_up;
    
    Chain_layout_add_segment(Scene_chain_layout, _chain_end,) 
    
    _objects
#end 

// End Scene_chain_pole1_objects
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_chain_pole2_objects()
//
#macro Scene_chain_pole2_objects()
    #local _objects = union {
        cylinder {
            Scene_pole2_location,
            Scene_pole2_location + <0, Scene_pole2_height, 0>,
            Scene_pole2_radius
            material {
                texture {
                    pigment { color rgb <1, 0, 0> }
                    finish {
                        fresnel
                        specular albedo 1.0
                        roughness 0.01
                        diffuse albedo 1.0
                    }
                }
                interior { ior 1.5 }        
            }
        }    
    }
    
    #local _last_pt             = Scene_chain_layout.links[Scene_chain_layout.num_links-1].link_front;
    #local _chain_pole2_disp    = Scene_pole2_location*<1,0,1> - _last_pt*<1,0,1>;
    #local _chain_pole2_dist    = vlength(_chain_pole2_disp);
    #local _chain_pole2_dir     = vnormalize(_chain_pole2_disp);
    #local _chain_pole2_pt      = _last_pt*<1,0,1> + _chain_pole2_dir*(_chain_pole2_dist - Scene_pole1_radius) + <0, Scene_pole2_height, 0>;
    #local _chain_pole2_axis    = vnormalize(_chain_pole2_pt - _last_pt);
    #local _chain_pole2_right   = VPerp_To_Plane(y, _chain_pole2_axis);
    #local _chain_pole2_up      = VPerp_To_Plane(_chain_pole2_axis, _chain_pole2_right);
    #debug concat("_chain_pole2_pt=<", vstr(3, _chain_pole2_pt, ",", 0, 3), ">\n")
    #local _chain_end           = _chain_pole2_pt + 0.5*Scene_chain_link_size.x*_chain_pole2_up;

    Chain_layout_add_slack_segment(Scene_chain_layout, _chain_end,Scene_pole2_slack_len,)
    
    // Cross the pole again
    //
    #local _last_pt             = Scene_chain_layout.links[Scene_chain_layout.num_links-1].link_front;
    #local _pole3_top           = Scene_pole3_location + <0, Scene_pole3_height, 0>;
    #local _pole2_top           = Scene_pole2_location + <0, Scene_pole2_height, 0>;
    #local _chain_pole2_disp    = _pole2_top*<1,0,1> - _last_pt*<1,0,1>;
    #local _chain_pole2_dist    = vlength(_chain_pole2_disp);
    #local _chain_pole2_dir     = vnormalize(_chain_pole2_disp);
    
    #local _chain_pole3_disp    = _pole3_top*<1,0,1> - _last_pt*<1,0,1>;
    #local _chain_pole3_dist    = vlength(_chain_pole3_disp);
    #local _chain_pole3_dir     = vnormalize(_chain_pole3_disp);
    
    #local _a   = acos(vdot(_chain_pole2_dir,_chain_pole3_dir));
    #local _c   = Scene_pole2_radius*sin(pi - 2*_a)/sin(_a);
    #local _chain_pole2_pt      = _last_pt*<1,0,1> + _chain_pole3_dir*_c + <0, Scene_pole2_height, 0>;
    #local _chain_pole2_disp    = _chain_pole2_pt - _last_pt;
    #local _chain_pole2_dir     = vnormalize(_chain_pole2_disp);
    #local _chain_pole2_right   = VPerp_To_Plane(y,_chain_pole2_dir);
    #local _chain_pole2_up      = VPerp_To_Plane(_chain_pole2_dir,_chain_pole2_right);
    #local _chain_end           = _chain_pole2_pt + 0.5*Scene_chain_link_size.x*_chain_pole2_up;
    
    Chain_layout_add_segment(Scene_chain_layout, _chain_end,) 
    
    
    _objects
#end

// End Scene_chain_pole2_objects
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_chain_pole3_objects()
//
#macro Scene_chain_pole3_objects()
    #local _objects = union {
        cylinder {
            Scene_pole3_location,
            Scene_pole3_location + <0, Scene_pole3_height, 0>,
            Scene_pole3_radius
            material {
                texture {
                    pigment { color rgb <1, 0, 0> }
                    finish {
                        fresnel
                        specular albedo 1.0
                        roughness 0.01
                        diffuse albedo 1.0
                    }
                }
                interior { ior 1.5 }        
            }
        }    
    }
    #local _last_pt             = Scene_chain_layout.links[Scene_chain_layout.num_links-1].link_front;
    #local _chain_pole3_disp    = Scene_pole3_location*<1,0,1> - _last_pt*<1,0,1>;
    #local _chain_pole3_dist    = vlength(_chain_pole3_disp);
    #local _chain_pole3_dir     = vnormalize(_chain_pole3_disp);
    #local _chain_pole3_pt      = _last_pt*<1,0,1> + _chain_pole3_dir*(_chain_pole3_dist - Scene_pole1_radius) + <0, Scene_pole3_height, 0>;
    #local _chain_pole3_axis    = vnormalize(_chain_pole3_pt - _last_pt);
    #local _chain_pole3_right   = VPerp_To_Plane(y, _chain_pole3_axis);
    #local _chain_pole3_up      = VPerp_To_Plane(_chain_pole3_axis, _chain_pole3_right);
    #debug concat("_chain_pole3_pt=<", vstr(3, _chain_pole3_pt, ",", 0, 3), ">\n")
    #local _chain_end           = _chain_pole3_pt + 0.5*Scene_chain_link_size.x*_chain_pole3_up;

    Chain_layout_add_catenary_segment2(Scene_chain_layout, _chain_end,Scene_pole3_catenary_len,,,)
    
    _objects
#end

// End Scene_chain_pole3_objects
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_chain_objects()
//
//  This macro returns a union{} that consists of the various objects
//  the chain travels around/over/near and the chain itself.
//
#macro Scene_chain_objects()
    // Ensure all variables are set, and the Scene_chain_layout has been created
    Scene_variables()                                                           
    
    #local _objects = union {
        object { Scene_chain_coil_objects() }
        Scene_chain_spline_create()
        object { Scene_chain_pole1_objects() }
        object { Scene_chain_pole2_objects() }
        object { Scene_chain_pole3_objects() }
        
        object { Chain_layout_render(Scene_chain_layout) }
    }
    
    _objects
#end

// End Scene_chain_objects
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
                scale 4
            }    
        }
        #ifndef (Scene_chain_objects_result) #declare Scene_chain_objects_result = object { Scene_chain_objects() }; #end
        object { Scene_chain_objects_result }
        #if(Scene_use_camera_light)    
        object { Scene_camera_light() }
        #end
    }
    
    _scene
#end

// End Scene_object_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Scene_auto_set_camera(Obj)
//
#macro Scene_auto_set_camera(Obj)
    #local _obj_min = min_extent(Obj);
    #local _obj_max = max_extent(Obj);
    #local _obj_c   = (_obj_min + _obj_max)/2;
    #local _obj_sz  = _obj_max - _obj_min;
    
    #declare camera_lookat      = <_obj_c.x, _obj_c.y, _obj_c.z>;
    #local _max_dim             = max(_obj_sz.x,_obj_sz.y,_obj_sz.z);
    #declare camera_location    = <camera_lookat.x, _obj_max.y, camera_lookat.z -1.5*_max_dim>;
    
    #debug concat(
        "camera_lookat=<", vstr(3, camera_lookat, ",", 0, 3), ">\n",
        "camera_location=<", vstr(3, camera_location, ",", 0, 3), ">\n"
    ) 
#end

// End Scene_auto_set_camera
//-----------------------------------------------------------------------------

// End Scene object declarations
//=============================================================================

//=============================================================================
// View-specific configuration overrides
//

#switch(val(str(Scene_View,0,2)))
    #case(1.1)
        #declare Scene_use_camera_light = true;
        #declare Scene_chain_objects_result   = object { Scene_chain_objects() }
        Scene_auto_set_camera(Scene_chain_objects_result)
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
    