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
    #debug concat("   .vsteps:", str(ObjectMap.vsteps, 0, 0), "\n")
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
    
    #local _matrix  = array[_vsteps][_rsteps];
    #for (i, 0, _vsteps-1, 1)
        #local _to_point    = <_center.x, _min_ext.y + i*VerticalResolution, _center.z>;
        #for (j, 0, _rsteps-1, 1)
            #local _from_point  = _to_point + vrotate(<0, 0, _max_r>, <0, j*RadialResolution, 0>);
            #local _from_dir    = vnormalize(_to_point - _from_point);
            #local _nrm         = <0,0,0>;
            #local _hit         = trace(Object, _from_point, _from_dir, _nrm);
            #if (vlength(_nrm) = 0)
                #local _hit = _to_point;
            #end
            #local _rad = vlength(_hit - _to_point);
            #local _matrix[i][j]    = _rad;    
        #end
    #end 
    
    #local _map = dictionary {
        .mapped_object: Object,
        .min_ext:       _min_ext,
        .max_ext:       _max_ext,
        .center:        _center,
        .sz:            _size,   
        .rsteps:        _rsteps,
        .vsteps:        _vsteps,
        .radii:         _matrix
    }
    ;
    
    _map   
    
#end

// End Col_object_map_create
//=============================================================================

#version Libcollide_Inc_Temp;

#end // ifndef (Libcollide_Inc_Temp)
