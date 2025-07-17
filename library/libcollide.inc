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
#include "transforms.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
#include "libfn.inc"
#include "libstringify.inc"
#include "libmesh.inc"

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

//=============================================================================
// Col_object_bottom_map_generate(Object,InitRadius,RadiusResolution,TriangleResolution,RStream)
//
//      Creates a dictionary {} object for the given Object that contains concentric
// sets of triangles consisting of points on the bottom surface of a given object
//

//-----------------------------------------------------------------------------
// Col_object_bottom_radius_set_create(Radius)
//
//      Creates a dictionary {} object consisting of a set of triangle dictionaries
// generated by Mesh_triangle_create() generated from a given radius
// from the center of the bottom surface of an object.
//
#macro Col_object_bottom_radius_set_create(Radius)
    #local _rset    = dictionary {
        .set_radius:    Radius,
        .triangles:     array,
        .num_triangles: 0
    }
    ;
    
    _rset
#end

// End Col_object_bottom_radius_set_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Col_object_bottom_radius_set_add(RadiusSet,Triangle)
//
//      Adds a triangle dictionary to the given RadiusSet dictionary
//
#macro Col_object_bottom_radius_set_add(RadiusSet,Triangle)
    #local RadiusSet.triangles[RadiusSet.num_triangles] = Triangle;
    #local RadiusSet.num_triangles  = RadiusSet.num_triangles + 1;
#end

// End Col_object_bottom_radius_set_add
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Col_object_bottom_map_create(Object,RadiusResolution,TriangleResolution)
//
//      Creates a dictionary{} that contains bounds, size, and center information
// for the given Object, along with the requested resolution information and
// an empty array of radius set dictionaries.
//
#macro Col_object_bottom_map_create(Object,RadiusResolution,TriangleResolution)
    #local _obj_lbounds = min_extent(Object);
    #local _obj_ubounds = max_extent(Object);
    #local _obj_center  = <1,0,1>*(_obj_lbounds + _obj_ubounds)/2;
    #local _obj_size    = <1,0,1>*(_obj_ubounds - _obj_lbounds);
    #local _max_r       = sqrt(pow(_obj_size.x/2,2)+pow(_obj_size.z/2,2));
    #local _min_y       = _obj_ubounds.y + 1;

    #local _map = dictionary {
        .obj_lbounds:   _obj_lbounds,
        .obj_ubounds:   _obj_ubounds,
        .obj_center:    _obj_center,
        .obj_size:      _obj_size,
        .max_radius:    _max_r,
        .min_y:         _min_y,
        .radius_res:    RadiusResolution,
        .triangle_res:  TriangleResolution,
        .num_radius_sets:   0,
        .radius_sets:   array
    }
    ;
    
    _map
#end

// End Col_object_bottom_map_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Col_object_bottom_map_add(Map,RadiusSet)
//
//      Adds a new RadiusSet dictionary to the given Map dictionary
//
#macro Col_object_bottom_map_add(Map,RadiusSet)
    #local Map.radius_sets[Map.num_radius_sets] = RadiusSet;
    #local Map.num_radius_sets  = Map.num_radius_sets + 1;
#end

// End Col_object_bottom_map_add
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Col_object_bottom_radius_set_generate(Map,Object,Radius,RadiusResolution,TriangleResolution,RStream)
//
//  Generates a new RadiusSet for the given Map, Object, Radius, and TriangleResolution
// by determining the maximum number of triangles to generate by rotating an equilateral
// triangle at the given radius.
//
#macro Col_object_bottom_radius_set_generate(Map,Object,Radius,RadiusResolution,TriangleResolution,RStream)
    #local _radius_set  = Col_object_bottom_radius_set_create(Radius);
    
    #local _start_angle     = 0; //360*rand(RStream);
    #local _end_angle       = _start_angle + 120;
    #local _cur_angle       = _start_angle;
    #local _sector_len      = 2*pi*Radius/3;
    #local _sector_steps    = _sector_len/TriangleResolution;
    #local _step_angle      = 120/_sector_steps;
    
    #macro _clip_to_bounds(PT)
        <min(max(PT.x,Map.obj_lbounds.x),Map.obj_ubounds.x), 0, min(max(PT.z,Map.obj_lbounds.z),Map.obj_ubounds.z)>
    #end
    
    #while (_cur_angle < _end_angle)
        #local _pts         = array[3];
        #local _npts        = 0;
        #for (i, 0, 2, 1)
            #local _pt_temp = _clip_to_bounds(Map.obj_center + vrotate(<0, 0, Radius>, <0, _cur_angle - i*120, 0>));
            #local _pt_nrm  = <0,0,0>;
            #local _pt_hit  = trace(Object, _pt_temp*<1,0,1>+y*(Map.obj_lbounds.y-1), y, _pt_nrm);
            #if (vlength(_pt_nrm) = 0)
                #local _rmin    = RadiusResolution;
                #local _r2      = Radius - RadiusResolution/10;
                #while (_r2 >= _rmin & vlength(_pt_nrm) = 0)
                    #local _pt_temp = _clip_to_bounds(Map.obj_center + vrotate(<0, 0, _r2>, <0, _cur_angle - i*120, 0>));
                    #local _pt_nrm  = <0,0,0>;
                    #local _pt_hit  = trace(Object, _pt_temp*<1,0,1>+y*(Map.obj_lbounds.y-1), y, _pt_nrm);
                    #local _r2      = _r2 - RadiusResolution/10;
                #end
            #end     
            #if (vlength(_pt_nrm) > 0)
                #local _pts[i]  = _pt_hit;
                #if (_pts[i].y < Map.min_y)
                    #local Map.min_y    = _pts[i].y;
                #end
                #local _npts    = _npts + 1;
            #end 
        #end
        
        #if (_npts = 3)
            #local _triangle    = Mesh_triangle_create(_pts);
            Col_object_bottom_radius_set_add(_radius_set, _triangle)
        #end
        #local _cur_angle   = _cur_angle + _step_angle;
    #end
    
    _radius_set
#end

// End Col_object_bottom_radius_set_generate
//-----------------------------------------------------------------------------


#macro Col_object_bottom_map_generate(Object,InitRadius,RadiusResolution,TriangleResolution,RStream)
    #local _map = Col_object_bottom_map_create(Object,RadiusResolution,TriangleResolution);
    
    #local _cur_radius  = InitRadius;
    #while (_cur_radius < _map.max_radius)
        #local _radius_set  = Col_object_bottom_radius_set_generate(_map,Object,_cur_radius,RadiusResolution,TriangleResolution,RStream);
        #if (_radius_set.num_triangles > 0)
            Col_object_bottom_map_add(_map,_radius_set)
        #end                                           
        #local _cur_radius  = _cur_radius + RadiusResolution;
    #end
    
    _map
#end

// End Col_object_bottom_map_create
//-----------------------------------------------------------------------------

// End Col_object_bottom_map_generate
//=============================================================================

//=============================================================================
// Col_object_bottom_map_dump(Map)
//
//  Writes the given Map to the debug stream
//

//-----------------------------------------------------------------------------
// Col_object_bottom_radius_set_dump(RadiusSet,IndentStr)
//
//  Writes a RadiusSet dictionary to the debug stream at the given indent level
//
#macro Col_object_bottom_radius_set_dump(RadiusSet,IndentStr)
    #debug concat(IndentStr,"dictionary {\n")
    #debug concat(IndentStr,"    .set_radius:       ", str(RadiusSet.set_radius, 0, 6), ",\n")
    #debug concat(IndentStr,"    .num_triangles:    ", str(RadiusSet.num_triangles, 0, 0), ",\n")
    #debug concat(IndentStr,"    .triangles:        array[", str(RadiusSet.num_triangles, 0, 0), "]{\n")
    #for (j, 0, RadiusSet.num_triangles-1, 1)
    Mesh_triangle_dump(RadiusSet.triangles[j],concat(IndentStr,"        "))
    #end
    #debug concat(IndentStr,"    },\n")
    #debug concat(IndentStr,"},\n")
#end 

// End Col_object_bottom_radius_set_dump
//-----------------------------------------------------------------------------

#macro Col_object_bottom_map_dump(Map)
    #debug concat("dictionary {\n") 
    #debug concat("    .obj_lbounds:        <", vstr(3, Map.obj_lbounds, ",", 0, 6), ">,\n")
    #debug concat("    .obj_ubounds:        <", vstr(3, Map.obj_ubounds, ",", 0, 6), ">,\n")
    #debug concat("    .obj_center:         <", vstr(3, Map.obj_center, ",", 0, 6), ">,\n")
    #debug concat("    .obj_size:           <", vstr(3, Map.obj_size, ",", 0, 6), ">,\n")
    #debug concat("    .max_radius:         ", str(Map.max_radius, 0, 6), ",\n")
    #debug concat("    .min_y:              ", str(Map.min_y, 0, 6), ",\n")
    #debug concat("    .radius_res:         ", str(Map.radius_res, 0, 6), ",\n")
    #debug concat("    .triangle_res:       ", str(Map.triangle_res, 0, 6), ",\n")
    #debug concat("    .num_radius_sets:    ", str(Map.num_radius_sets, 0, 0), ",\n")
    #debug concat("    .radius_sets:        array[", str(Map.num_radius_sets, 0, 0), "]{\n")
    #for (i, 0, Map.num_radius_sets-1, 1)
    Col_object_bottom_radius_set_dump(Map.radius_sets[i],"        ")
    #end
    #debug concat("    },\n")
    #debug concat("}\n")
#end

// End Col_object_bottom_map_dump
//=============================================================================


//=============================================================================
// Col_object_bottom_map_write(Map,optional VarName)
//
//  Writes the given Map to the currently open SFY_file_handle with an optional
// #declare VarName = statement
//


//-----------------------------------------------------------------------------
// Col_object_bottom_radius_set_write(RadiusSet,IndentStr)
//
//  Writes a RadiusSet dictionary to the currently open SFY_file_handle at the given indent level
//
#macro Col_object_bottom_radius_set_write(RadiusSet,IndentStr)
    #write(SFY_file_handle,IndentStr,"dictionary {\n")
    #write(SFY_file_handle,IndentStr,"    .set_radius:       ", str(RadiusSet.set_radius, 0, 6), ",\n")
    #write(SFY_file_handle,IndentStr,"    .num_triangles:    ", str(RadiusSet.num_triangles, 0, 0), ",\n")
    #write(SFY_file_handle,IndentStr,"    .triangles:        array[", str(RadiusSet.num_triangles, 0, 0), "]{\n")
    #for (j, 0, RadiusSet.num_triangles-1, 1)
    Mesh_triangle_write(RadiusSet.triangles[j],concat(IndentStr,"        "))
    #end
    #write(SFY_file_handle,IndentStr,"    },\n")
    #write(SFY_file_handle,IndentStr,"},\n")
#end 

// End Col_object_bottom_radius_set_write
//-----------------------------------------------------------------------------

#macro Col_object_bottom_map_write(Map,optional VarName)
    #ifdef (VarName)
    #write(SFY_file_handle,"#declare ", VarName, " = ")
    #end
    #write(SFY_file_handle,"dictionary {\n") 
    #write(SFY_file_handle,"    .obj_lbounds:        ", Map.obj_lbounds, ",\n")
    #write(SFY_file_handle,"    .obj_ubounds:        ", Map.obj_ubounds, ",\n")
    #write(SFY_file_handle,"    .obj_center:         ", Map.obj_center, ",\n")
    #write(SFY_file_handle,"    .obj_size:           ", Map.obj_size, ",\n")
    #write(SFY_file_handle,"    .max_radius:         ", Map.max_radius, ",\n")
    #write(SFY_file_handle,"    .min_y:              ", Map.min_y, ",\n")
    #write(SFY_file_handle,"    .radius_res:         ", Map.radius_res, ",\n")
    #write(SFY_file_handle,"    .triangle_res:       ", Map.triangle_res, ",\n")
    #write(SFY_file_handle,"    .num_radius_sets:    ", str(Map.num_radius_sets, 0, 0), ",\n")
    #write(SFY_file_handle,"    .radius_sets:        array[", str(Map.num_radius_sets, 0, 0), "]{\n")
    #for (i, 0, Map.num_radius_sets-1, 1)
    Col_object_bottom_radius_set_write(Map.radius_sets[i],"        ")
    #end
    #write(SFY_file_handle,"    },\n")
    #write(SFY_file_handle,"}\n")
    #ifdef (VarName)
    #write(SFY_file_handle,";\n")
    #end
#end

// End Col_object_bottom_map_write
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Col_object_bottom_triangle_transform(Triangle,Transform,UseYOffset)
//
#macro Col_object_bottom_triangle_transform(Triangle,Transform,UseVOffset)
    #local _pmult   = #if(UseVOffset) <1,1,1>; #else <1,0,1>; #end
    #local _tpoints = array[3];
    #for (i, 0, 2, 1)
        #local _tpoints[i]  = vtransform(Triangle.verts[i]*_pmult,Transform);
    #end                                                              
    #local _triangle    = Mesh_triangle_create(_tpoints);
    
    _triangle
#end

// End Col_object_bottom_triangle_transform
//-----------------------------------------------------------------------------

#macro Col_rest_map_on_surface_transform(Map,Surface,DropHeight,optional XZLocation,optional Floor)
    #local _xz_loc  = #ifdef(XZLocation) <1,0,1>*XZLocation; #else <0,0,0>; #end 
    #local _floor   = #ifdef(Floor) Floor; #else -1e6; #end
    #local _dlevel  = #ifdef(Col_rest_map_on_surface_debug) Col_rest_map_on_surface_debug; #else 0; #end
    
    // Standard translation to get the object 
    #local _map_translation = SFY_transform_instance_create(SFY_transform_type_translate, _xz_loc - y*Map.min_y);
    
    #local _cur_object_transform    = SFY_transform_create(_map_translation);
    #local _cur_object_trans        = transform { SFY_transform_sdl(_cur_object_transform) }
    
    #macro _get_surface_points(Triangle)
        #local _ps   = array[3];
        #for (k, 0, 2, 1)
            #local _nrm = <0,0,0>;
            #local _hit = trace(Surface, Triangle.verts[k]*<1,0,1> + y*DropHeight, -y, _nrm);
            #if (vlength(_nrm) = 0)
                #local _hit = Triangle.verts[k]*<1,0,1> + y*_floor;
            #end 
            #local _ps[k]    = _hit;   
        #end
        
        _ps
    #end
    
    // Need to look at this more closely.  Something is happening that is shifting the object in the xz-plane, even though the center
    // of each triangle is <0,0,0> + _loc.  Could be something with how the plane origin is being computed as it relates to
    // the center of the object.
    //
    
    #local _min_plane_y = -1e12;
    
    //#for (i, 0, Map.num_radius_sets-1, 1)
    #for (i, Map.num_radius_sets-1, 0, -1)
        #if (_dlevel > 0) #debug concat("Col_map_on_surface_transform: Radius set: ", str(Map.radius_sets[i].set_radius, 0, 6), "\n") #end
        #local _num_attempts    = 0;
        #local _num_shifts      = 1;
        #while (_num_shifts > 0 & _num_attempts < 10)
            #if (_num_attempts > 0 & _dlevel > 1)
                #debug concat("Col_map_on_surface_transform: Attempt #", str(_num_attempts, 0, 0), " after ", str(_num_shifts, 0, 0), " shifts\n")
            #end    
            #local _num_shifts      = 0;
            #local _num_attempts    = _num_attempts + 1;
            #for (j, 0, Map.radius_sets[i].num_triangles-1, 1)
                #local _trans_triangle  = Col_object_bottom_triangle_transform(Map.radius_sets[i].triangles[j],_cur_object_trans,true);
                #local _trans_base      = Col_object_bottom_triangle_transform(Map.radius_sets[i].triangles[j],_cur_object_trans,false);
                #local _surface_pts     = _get_surface_points(_trans_triangle);
                #local _plane_pts       = array[3];
                #local _num_below       = 0;
                #for (p, 0, 2, 1)
                    #if (_trans_triangle.verts[p].y < _surface_pts[p].y)
                        #local _num_below       = _num_below + 1;
                        #local _plane_pts[p]    = _surface_pts[p] - y*(_trans_triangle.verts[p].y - _trans_base.verts[p].y);
                    #else
                        #local _plane_pts[p]    = _trans_base.verts[p];    
                    #end
                #end
                
                #if (_num_below > 0)
                    #local _num_shifts      = _num_shifts + 1;
                    #local _plane_normal    = vcross(_plane_pts[2] - _plane_pts[0], _plane_pts[1] - _plane_pts[0]);
                    #if (vlength(_plane_normal) > 0)
                        #local _plane_normal    = vnormalize(_plane_normal);
                        #local _plane_origin    = (_plane_pts[0] + _plane_pts[1] + _plane_pts[2])/3;
                        /*
                        #if (_plane_origin.y < _min_plane_y)
                            #local _plane_origin    = <1,0,1>*_plane_origin + y*_min_plane_y;
                        #else
                            #local _min_plane_y = _plane_origin.y;
                        #end 
                        */           
                        
                        #if(_dlevel > 2) #debug concat("_plane_normal=<", vstr(3, _plane_normal, ",", 0, 9), ">; _plane_origin=<", vstr(3, _plane_origin, ",", 0, 9), ">\n") #end
                        #if(_dlevel > 3)
                        #for (p1, 0, 2, 1)
                            #debug concat("    _surface_pts[", str(p1, 0, 0), "]=<", vstr(3, _surface_pts[p1], ",", 0, 6), ">\n")
                            #debug concat("    _trans_triangle.verts[", str(p1, 0, 0), "]=<", vstr(3, _trans_triangle.verts[p1], ",", 0, 6), ">\n")
                            #debug concat("    _trans_base.verts[", str(p1, 0, 0), "]=<", vstr(3, _trans_base.verts[p1], ",", 0, 6), ">\n")
                            #debug concat("    _plane_pts[", str(p1, 0, 0), "]=<", vstr(3, _plane_pts[p1], ",", 0, 6), ">\n")
                        #end            
                        #end
                        
                        // Translate the bottom of the object so the center of the current triangle is at the origin
                        #local _cur_object_transform    = SFY_transform_create(SFY_transform_instance_create(SFY_transform_type_translate, -Map.radius_sets[i].triangles[j].center*<1,0,1> - y*Map.min_y));
                        
                        // Add in the matrix rotation to align the object with the plane
                        //
                        SFY_transform_add_reorient_trans(_cur_object_transform,y,_plane_normal)
                        
                        // Add in the translation to the plane origin
                        //
                        SFY_transform_add(_cur_object_transform,SFY_transform_instance_create(SFY_transform_type_translate, _plane_origin))
                        #local _cur_object_trans    = transform { SFY_transform_sdl(_cur_object_transform) }
                    #end    
                #end
            #end
        #end    
    #end
    
    _cur_object_transform
    
#end

// End Col_rest_map_on_surface_transform
//=============================================================================

//=============================================================================
// Col_rest_triangle_set_on_surface_transform(TriangleSet,Surface,optional XZLocation,optional Floor)
//
//      Creates a new SFY_transform that attempts to rest the given TriangleSet on the given Surface such that all of the
// vertices of all triangles lie on or above the surface.                                                                
//

// Notes:
// The triangle set should represent the base of the object if it were resting on a flat surface at y=0.  If it has
// a bumpy bottom, there may be points that extend below the y=0 plane and they will be taken into account
//
// The goal is to compute a planar transformation to apply to the object such that no triangle points representing
// the base dip below the height of the surface.  To do this:
//
// Initialize the current plane to: Normal=y, origin=<0,0,0>
// Keep repeating this process until no triangles have points below the surface, or the maximum iterations is reached
//  for each Triangle:
//      Transform the Triangle using the current planar transform
//      Transform the Triangle projected onto the y=0 plane using the current planar transform; these are the current plane points
//      Repeat until all vertices are above the surface or the maximum retries are reached:
//          for each Vertex of the transformed Triangle:
//              Project the Vertex onto the surface
//              If the Vertex.y < surface.y:
//                  Compute how much the the vertex would need to be raised, (i.e. surface.y - Vertex.y) and update the matching transformed projected Vertex by adding that value to it
//                  Create a new planar transform based on the current set of points
//                  Retry the triangle
//              Else
//                  Continue to the next Vertex 
//
#macro Col_rest_triangle_set_on_surface_transform(TriangleSet,Surface,optional XZLocation,optional Floor)
    #local _xz_loc          = #ifdef(XZLocation) <1,0,1>*XZLocation; #else <0,0,0>; #end 
    #local _floor           = #ifdef(Floor) Floor; #else -1e6; #end
    #local _dlevel          = #ifdef(Col_rest_triangle_set_on_surface_debug) Col_rest_triangle_set_on_surface_debug; #else 0; #end
    #local _max_attempts    = #ifdef(Col_rest_triangle_set_on_surface_max_attempts) max(1,Col_rest_triangle_set_on_surface_max_attempts); #else 10; #end
    #local _drop_height     = max_extent(Surface).y + 10;
    
    #macro _get_surface_points(Triangle)
        #local _ps   = array[3];
        #for (k, 0, 2, 1)
            #local _nrm = <0,0,0>;
            #local _hit = trace(Surface, Triangle.verts[k]*<1,0,1> + y*_drop_height, -y, _nrm);
            #if (vlength(_nrm) = 0)
                #local _hit = Triangle.verts[k]*<1,0,1> + y*_floor;
            #end 
            #local _ps[k]    = _hit;   
        #end
        
        _ps
    #end
    
    #local _object_plane            = Mesh_plane_create(y,<0,0,0>);
    #local _object_sfy_transform    = SFY_transform_create(SFY_transform_instance_create(SFY_transform_type_translate, _xz_loc));
    #local _object_transform        = transform { SFY_transform_sdl(_object_sfy_transform) }
    #local _min_y_trans             = 0;
    #local _attempts                = 0;
    
    #local _keep_adjusting  = true;
    #while (_keep_adjusting)
        #local _cur_triangle        = 0;
        #local _attempts            = _attempts + 1;
        #local _num_updates         = 0;
        #while (_cur_triangle < TriangleSet.num_triangles)
            #local _trans_triangle      = Mesh_triangle_transform(TriangleSet.triangles[_cur_triangle],_object_transform);
            #local _trans_projection    = Mesh_triangle_transform(Mesh_triangle_project(_object_plane,TriangleSet.triangles[_cur_triangle]),_object_transform);
            #local _trans_center        = TriangleSet.triangles[_cur_triangle].center*<1,0,1>;
            #local _surface_pts         = _get_surface_points(_trans_triangle);
            #local _triangle_updates    = 0;
            #for (i, 0, 2, 1)
                #if (_surface_pts[i].y > _trans_triangle.verts[i].y)
                    #local _delta                       = _surface_pts[i].y - _trans_triangle.verts[i].y;
                    Mesh_triangle_update_vertex(_trans_projection,i,_trans_projection.verts[i] + y*_delta)
                    #local _new_plane                   = Mesh_plane_create_triangle(_trans_projection);
                    #if (_new_plane.porig.y > _min_y_trans)
                        #local _min_y_trans = _new_plane.porig.y;
                    #elseif (_new_plane.porig.y < _min_y_trans)
                        #local _new_plane = Mesh_plane_create(_new_plane.pnorm,<1,0,1>*_new_plane.porig + y*_min_y_trans);
                    #end 
                    #if (_dlevel > 2)
                        #debug concat("_trans_projection: \n")
                        Mesh_triangle_dump(_trans_projection, "    ")
                        #debug concat("_new_plane generated: \n")
                        Mesh_plane_dump(_new_plane, "    ")
                    #end
                    #local _object_sfy_transform        = SFY_transform_create(SFY_transform_instance_create(SFY_transform_type_translate, -_trans_center));
                    SFY_transform_add_reorient_trans(_object_sfy_transform, y, _new_plane.pnorm)
                    SFY_transform_add(_object_sfy_transform, SFY_transform_instance_create(SFY_transform_type_translate, _new_plane.porig))
                    #local _object_transform            = transform { SFY_transform_sdl(_object_sfy_transform) }
                    #local _trans_triangle              = Mesh_triangle_transform(TriangleSet.triangles[_cur_triangle],_object_transform);
                    #local _trans_projection            = Mesh_triangle_transform(Mesh_triangle_project(_object_plane,TriangleSet.triangles[_cur_triangle]),_object_transform);
                    #local _surface_pts                 = _get_surface_points(_trans_triangle);
                    #local _num_updates                 = _num_updates + 1;
                #end
            #end
            #local _cur_triangle    = _cur_triangle + 1;
        #end
        
        #if (_dlevel > 1)
            #debug concat("Col_rest_triangle_set_on_surface_transform: Attempt ", str(_attempts, 0, 0), " had ", str(_num_updates, 0, 0), " updates\n")
        #end
        
        #if (_attempts >= _max_attempts | _num_updates = 0)
            #if (_attempts >= _max_attempts)
                #warning concat("Col_rest_triangle_set_on_surface_transform: reached ", str(_max_attempts, 0, 0), "; stopping\n")
            #end    
            #local _keep_adjusting  = false;
        #end    
    #end
    
    // Make one last pass over the triangles of the object and find the 3 points that are closest to the surface.  If the distance
    // of at least one of those 3 points is > 0, do one more adjustment to rest the surface on those points.
    //
    #macro _triangle_distance(Triangle)
        #local _surface_points  = _get_surface_points(Triangle);
        #local _min_dist        = max(0,Triangle.verts[0].y - _surface_points[0].y);
        #for (i, 1, 2, 1)
            #local _d   = max(0, Triangle.verts[i].y - _surface_points[i].y);
            #if (_d < _min_dist)
                #local _min_dist    = _d;
            #end    
        #end    
        
        _min_dist
    #end
    
    #for (i, 0, TriangleSet.num_triangles-1, 1)
        #local _trans_triangle      = Mesh_triangle_transform(TriangleSet.triangles[i],_object_transform);
        #local _trans_projection    = Mesh_triangle_transform(Mesh_triangle_project(_object_plane,TriangleSet.triangles[i]),_object_transform);
        #local _trans_center        = TriangleSet.triangles[i].center*<1,0,1>;
        #local _triangle_d          = _triangle_distance(_trans_triangle);
        #ifndef (_min_triangle)
            #local _min_triangle        = _trans_triangle;
            #local _min_projection      = _trans_projection;
            #local _min_center          = _trans_center;
            #local _min_distance        = _triangle_d;
        #else
            #if (_triangle_d < _min_distance)
                #local _min_triangle    = _trans_triangle;
                #local _min_projection  = _trans_projection;
                #local _min_center      = _trans_center;
                #local _min_distance    = _triangle_d;
            #end    
        #end
    #end
    
    #if (_min_distance > 0)
        #if (_dlevel > 0) #debug concat("Col_rest_triangle_set_on_surface_transform: Adjusting to rest on the surface: ", str(_min_distance, 0, 6), "\n") #end
        SFY_transform_add(_object_sfy_transform, SFY_transform_instance_create(SFY_transform_type_translate, -y*_min_distance))
    #end
    
    #if (_dlevel > 0)
        #debug concat("Col_rest_triangle_set_on_surface_transform: Ran ", str(_attempts, 0, 0), " iterations\n")
    #end
    
    _object_sfy_transform
#end

// End Col_rest_triangle_set_on_surface_transform
//=============================================================================

#version Libcollide_Inc_Temp;

#end // ifndef (Libcollide_Inc_Temp)
