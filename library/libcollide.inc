/******************************************************************************
 * File: libcollide.inc
 * Description:
 *      A library of macros for detecting object collisions and finding optimal
 *      placement for abutting objects
 ******************************************************************************/

#ifndef(Libcollide_Inc_Temp)

#declare Libcollide_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libcollide.inc\n"
#end                     

//=============================================================================
// Includes
//

//-----------------------------------------------------------------------------
// Standard Includes
//

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
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
// Col_object_map_create(Object,RadialResolution,VerticalResolution)
//
//      Creates a dictionary object that stores the distance from the center
//  of an object to its nearest edge in the XZ plane, scanning from the bottom
//  to the top of the object along the Y axis.
//
//  Object: The object{} to scan
//  RadialResolution: The arc degrees for each scan point, (i.e. 60 would mean 6 points per plane)
//  Vertical Resolution:    The distance between scanning planes
//
//  The resulting dictionary contains:
//      .mapped_object: Object
//      .min_ext:       min_extent(Object)
//      .max_ext:       max_extent(Object)
//      .center:        (min_ext + max_ext)/2
//      .sz:            (max_ext - min_ext)
//      .rsteps:        Number of scan points in each plane
//      .vsteps:        Number of vertical planes 
//      .maxrad:        Maximum radius across the mesh for the object
//      .radii:         matrix[vsteps][rsteps] containing the distance from the center.XZ to the edge in the given plane

//-----------------------------------------------------------------------------
// Col_object_map_dump(ObjectMap)
//
#macro Col_object_map_dump(ObjectMap)
    #debug concat("ObjectMap = {\n")
    #debug concat("   .min_ext: <", vstr(3, ObjectMap.min_ext, ",", 0, 3), ">,\n")
    #debug concat("   .max_ext: <", vstr(3, ObjectMap.max_ext, ",", 0, 3), ">,\n")
    #debug concat("   .center: <", vstr(3, ObjectMap.center, ",", 0, 3), ">,\n")
    #debug concat("   .sz: <", vstr(3, ObjectMap.sz, ",", 0, 3), ">,\n")
    #debug concat("   .rsteps:", str(ObjectMap.rsteps, 0, 0), ",\n")
    #debug concat("   .rres:", str(ObjectMap.rres, 0, 3), ",\n")
    #debug concat("   .vsteps:", str(ObjectMap.vsteps, 0, 0), "\n")
    #debug concat("   .vres:", str(ObjectMap.vres, 0, 3), "\n")
    #debug concat("   .maxrad:", str(ObjectMap.maxrad, 0, 3), "\n")
    #debug concat("   .radii: {\n")
    #for (i, 0, ObjectMap.vsteps-1, 1)
        #for (j, 0, ObjectMap.rsteps-1, 1)
            #debug concat(str(ObjectMap.radii[i][j],0,3), ",")
        #end
        #debug "\n"
    #end    
    #debug concat("   }\n")
    
    #debug concat("}\n")
#end

// end Col_object_map_dump
//-----------------------------------------------------------------------------

#macro Col_object_map_create(Object,RadialResolution,VerticalResolution)
    #local _min_ext = min_extent(Object);
    #local _max_ext = max_extent(Object);
    #local _center  = (_min_ext + _max_ext)/2;
    #local _size    = (_max_ext - _min_ext);
    #local _max_r   = max(_size.x,_size.z);
    
    #local _rsteps  = ceil(360/RadialResolution);
    #local _vsteps  = ceil(_size.y/VerticalResolution);
    #local _maxrad  = 0;
    
    #local _min_y   = _min_ext.y + 1e-6;
    #local _max_y   = _max_ext.y - 1e-6;
    
    #local _matrix  = array[_vsteps][_rsteps];
    #for (i, 0, _vsteps-1, 1)
        #local _to_y        = f_clamp(_min_y + i*VerticalResolution, _min_y, _max_y);
        #local _to_point    = <_center.x, _to_y, _center.z>;
        #for (j, 0, _rsteps-1, 1)
            #local _from_point  = _to_point + vrotate(<0, 0, _max_r>, <0, j*RadialResolution, 0>);
            #local _from_dir    = vnormalize(_to_point - _from_point);
            #local _nrm         = <0,0,0>;
            #local _hit         = trace(Object, _from_point, _from_dir, _nrm);
            #if (vlength(_nrm) = 0)
                #local _hit = _to_point;
            #end
            #local _hit_dist    = vlength(_from_point - _hit);
            #local _from_dist   = vlength(_from_point - _to_point);
            #if (_hit_dist > _from_dist)
                //#debug concat("_to_point=<", vstr(3, _to_point, ",", 0, 6), ">; _from_point=<", vstr(3, _from_point, ",", 0, 6), ">; _hit=<", vstr(3, _hit, ",", 0, 6), ">\n")
                //#debug concat("j=", str(j, 0, 0) "; _hit_dist=", str(_hit_dist, 0, 6), "; _from_dist=", str(_from_dist, 0, 6), "\n")
                #local _rad = -vlength(_hit - _to_point);
            #else
                #local _rad = vlength(_hit - _to_point);
            #end        
            #local _matrix[i][j]    = _rad;
            #if (_rad > _maxrad)
                #local _maxrad  = _rad;
            #end        
        #end
    #end 
    
    #local _map = dictionary {
        .mapped_object: Object,
        .min_ext:       _min_ext,
        .max_ext:       _max_ext,
        .center:        _center,
        .sz:            _size,
        .rres:          RadialResolution,   
        .rsteps:        _rsteps,         
        .vres:          VerticalResolution,
        .vsteps:        _vsteps,
        .radii:         _matrix,
        .maxrad:        _maxrad
    }
    ;
    
    _map   
    
#end

// End Col_object_map_create
//=============================================================================

//=============================================================================
// Object Abutting Macros
//

//-----------------------------------------------------------------------------
// Col_slide_map_to_object(FromObjectMap,FromPt,ToObject,optional ToPt,Gap,Tolerance)
//
//      Compute the point along the XZ vector between FromPt and the center of ToObject
//  where the given FromObjectMap can be slid such that it abuts ToObject (with a minimum Gap) but does
//  not overlap it within the Tolerance using the given map.
//
//  FromObjectMap: result of a Col_object_map_create() call
//  FromPt:        <x,z> coordinates of the starting point of the slide towards ToObject
//  ToObject:   object{} to abut with the object in FromObjectMap
//  Gap:        minimum distance between newly placed object and the ToObject
//  Tolerance:  Minimum delta between iterations moving the point before declaring success
//

#macro Col_slide_map_to_object(FromObjectMap,FromPt,ToObject,optional ToPt,Gap,Tolerance)
    #local _to_min_ext  = min_extent(ToObject);
    #local _to_max_ext  = max_extent(ToObject);
    #local _to_sz       = _to_max_ext - _to_min_ext;
    #local _to_center   = (_to_min_ext + _to_max_ext)/2;
    
    #ifdef (ToPt)
        #local _to_pt   = <ToPt.x, _to_center.y, ToPt.y>;
    #else
        #local _to_pt   = _to_center;
    #end        
    
    #ifdef (Col_slide_verbose) #debug concat("_to_pt=<", vstr(3, _to_pt, ",", 0, 3), ">\n") #end
    
    #local _xz_dir          = vnormalize(<_to_pt.x,0,_to_pt.z> - <FromPt.x,0,FromPt.y>);
    
    #ifdef (Col_slide_verbose) #debug concat("_xz_dir=<", vstr(3,_xz_dir,",",0,3), ">\n") #end
    #local _from_obj_angle  = mod(360+degrees(atan2(_xz_dir.x,_xz_dir.z)),360);
    #local _from_obj_rstep  = mod(f_round(_from_obj_angle/FromObjectMap.rres),FromObjectMap.rsteps);
    
    #local _min_y       = FromObjectMap.min_ext.y + 1e-6;
    #local _max_y       = FromObjectMap.max_ext.y - 1e-6;
    
    #for (i, 0, FromObjectMap.vsteps-1, 1)
        #local _y       = f_clamp(_min_y + i*FromObjectMap.vres, _min_y, _max_y);
        #local _from    = <FromPt.x,_y,FromPt.y>;
        #local _to      = <_to_pt.x,_y,_to_pt.z>;
        //#debug concat("trace dir=<", vstr(3, (_to - _from), ",", 0, 3), ">\n")
        #local _nrm     = <0,0,0>;
        #local _hit     = trace(ToObject, _from, (_to - _from), _nrm);
        #if (vlength(_nrm) = 0)
            #local _obj_pt      = _to;
            #local _obj_dist    = vlength(_from - _to);
        #else
            #ifdef (Col_slide_verbose) #debug concat("_from=<", vstr(3, _from, ",", 0, 3), "; _hit=<", vstr(3, _hit, ",", 0, 3), ">\n") #end
            #local _or          = (Gap + FromObjectMap.radii[i][_from_obj_rstep]);
            //#debug concat("_or=", str(_or, 0, 3), "\n")
            #local _obj_pt      = _hit - _xz_dir*_or;
            //#debug concat("_obj_pt=<", vstr(3, _obj_pt, ",", 0, 3), ">\n")
            #local _obj_dist    = vlength(_from - _obj_pt);    
        #end
        #ifndef (_best_pt)
            #local _best_pt     = _obj_pt;
            #local _best_dist   = _obj_dist;
        #elseif (_obj_dist < _best_dist)
            #local _best_pt     = _obj_pt;
            #local _best_dist   = _obj_dist;
        #end
        
        //#debug concat("i=", str(i, 0, 3), "; _best_pt=<", vstr(3, _best_pt, ",", 0, 3), ">; _best_dist=", str(_best_dist, 0, 3), "\n")        
    #end
    
    //#debug concat("Pre-collision check: _best_pt=<", vstr(3, _best_pt, ",", 0, 3), ">\n")
    
    // The next step would be to check the edges on the other sides of the object to make sure they don't hit
    // anything else
    #local _delta   = 1e6;
    
    #while (_delta > Tolerance)
        #local _iter    = 0;
        #while (_iter < 4 & _delta > Tolerance)
            #local _delta       = 0;
            #local _iter        = _iter + 1;
            #local _prev_best   = <_best_pt.x, 0, _best_pt.z>;
            #for (j, 0, FromObjectMap.rsteps-1, 1)
                #for (i, 0, FromObjectMap.vsteps-1, 1)
                    #local _y       = FromObjectMap.min_ext.y + i*FromObjectMap.vres;
                    #local _from   = <_best_pt.x, _y, _best_pt.z>;
                    #local _dir     = vrotate(z, <0, j*FromObjectMap.rres, 0>);
                    #local _nrm     = <0,0,0>;
                    #local _hit     = trace(ToObject, _from, _dir, _nrm);
                    #if (vlength(_nrm) > 0)
                        #local _dist    = vlength(_hit - _from);
                        #local _overlap = (FromObjectMap.radii[i][j] + Gap) - _dist;
                        //#debug concat("_overlap=", str(_overlap, 0, 6), "\n")
                        #if (_overlap > Tolerance)
                            //#debug concat("_overlap=", str(_overlap, 0, 6), "\n")
                            //#debug concat("Bumped into the object from dir <", vstr(3, _dir, ",", 0, 3), "> with overlap ", str(_overlap, 0, 3), " at <", vstr(3, _hit, ",", 0, 3), ">\n")
                            #local _new_pt  = _hit - _dir*(Gap + FromObjectMap.radii[i][j]);
                            #local _best_pt = <_new_pt.x, 0, _new_pt.z>;
                            //#debug concat("   New best point is <", vstr(3, _best_pt, ",", 0, 3), ">\n")
                        #end
                    #end
                #end
            #end
            #local _delta   = vlength(_best_pt - _prev_best);
        #end
        #if (_delta > Tolerance)
            // Move the best point away from the center of the
            // cluster object by the max radius
            #local _push_dir    = vnormalize(_best_pt - <_to_pt.x, 0, _to_pt.z>);
            #ifdef (Col_slide_verbose) #debug concat("Pushing object from <", vstr(3, _best_pt, ",", 0, 3), "> ") #end
            #local _best_pt     = _best_pt - _push_dir*FromObjectMap.maxrad;          
            #ifdef (Col_slide_verbose) #debug concat("to <", vstr(3, _best_pt, ",", 0, 3), ">\n") #end
        #end
    #end        
    
    //#debug concat("Post-collision check: _best_pt=<", vstr(3, _best_pt, ",", 0, 3), ">\n")
    <_best_pt.x, 0, _best_pt.z>
#end

// End Col_slide_map_to_object
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Col_slide_object_to_object(FromObject,FromPt,ToObject,optional ToPt,RRes,HRes,Gap,Tolerance)
//
//      This is a utility macro that first computes the object map for the given FromObject
//  using the given radial and horizontal resolution values, and then slides the object
//  to the given ToObject
//
//  If Col_slide_verbose is defined, the object map is dumped before returning
//
#macro Col_slide_object_to_object(FromObject,FromPt,ToObject,optional ToPt,RRes,HRes,Gap,Tolerance)
    #local _map = Col_object_map_create(FromObject,RRes,HRes);
    #ifdef (Col_slide_verbose)
        Col_object_map_dump(_map)
    #end
    Col_slide_map_to_object(_map,FromPt,ToObject,ToPt,Gap,Tolerance)
#end

// End Col_slide_object_to_object
//-----------------------------------------------------------------------------


// End Object Abutting Macros
//=============================================================================

//=============================================================================
// Macros for setting objects on uneven surfaces
//

//-----------------------------------------------------------------------------
// Get_surface_bounds(SurfaceObject,LBounds,UBounds,Tolerance,optional RestingObject,optional ROHeight)
//
//      This is a utility macro that scans a surface object by shooting rays
//  from above the maximum height of the object in the -y direction and recording
//  the maximum and minimum height values.  If RestingObject is supplied,
//  only points that would be covered by the RestingObject are considered in
//  the scan.  The ROHeight should be the maximum height (y) of the Resting object
//  when resting on the y=0 plane.
//
#macro Get_surface_bounds(SurfaceObject,LBounds,UBounds,Tolerance,optional RestingObject,optional ROHeight)
    #local _min_pt  = UBounds;
    #local _max_pt  = LBounds;
    
    #ifdef(RestingObject)
        #local _ro_trace_y  = #ifdef(ROHeight) ROHeight+1 ; #else 1e6; #end
    #else
        #local _pt_included = true;
    #end        
    
    #for (_z, LBounds.z, UBounds.z, Tolerance)
        #for (_x LBounds.x, UBounds.x, Tolerance)
            #local _nrm = <0,0,0>;
            #local _try = <_x, UBounds.y, _z>;
            #local _hit = trace(SurfaceObject, _try, -y, _nrm);
            #if (vlength(_nrm) > 0)
                //#debug concat("_try=<", vstr(3, _try, ",", 0, 3), ">; _hit=<", vstr(3, _hit, ",", 0, 3), ">\n")
                #ifdef(RestingObject)
                    #local _ro_nrm  = <0,0,0>;
                    #local _ro_pt   = trace(RestingObject, <_x, _ro_trace_y, _z>, -y, _ro_nrm);
                    #if (vlength(_ro_nrm) > 0)
                        #local _pt_included = true;
                    #else
                        #local _pt_included = false;
                    #end        
                #end
                #if (_pt_included)
                    #if (_hit.y < _min_pt.y)
                        #local _min_pt  = _hit;
                    #end
                    #if (_hit.y > _max_pt.y)
                        #local _max_pt  = _hit;
                    #end 
                #end           
            #end
        #end
    #end
    
    #local _bounds  = dictionary {
        .min_pt:    _min_pt,
        .max_pt:    _max_pt
    }
    ;
    
    _bounds
#end  

// End Get_surface_bounds
//-----------------------------------------------------------------------------

// End Macros for setting objects on uneven surfaces
//=============================================================================

#version Libcollide_Inc_Temp;

#end // ifndef (Libcollide_Inc_Temp)
