/******************************************************************************
 * File: libmesh.inc
 * Description:
 *      This is a library of macros for constructing mesh2 objects
 ******************************************************************************/

#ifndef(Libmesh_Inc_Temp)

#declare Libmesh_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libmesh.inc\n"
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
// Mesh2 Dictionary
//
// The Mesh2 Dictionary is a container with all of the information needed
// to create a mesh2 object via macro calls.  It consists of vertex, uv, and
// normal lists, as well as face definitions (flat and smooth).
//

//-----------------------------------------------------------------------------
// Mesh2_vertex_create(Point,optional UV,optional NRM)
//
#macro Mesh2_vertex_create(Point,optional UV,optional NRM)
    #local _v   = dictionary {
        .point: Point,
        #ifdef (local.UV)
        .uv:    UV,
        #end
        #ifdef (local.NRM)
        .norm:  NRM
        #end
    }
    ;
    
    _v
#end

// End Mesh2_vertex_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh2_face_create(VIndices,optional isSmooth)
//
#macro Mesh2_face_create(VIndices, optional isSmooth)
    #ifndef (local.isSmooth)
        #local _sm  = false;
    #else
        #local _sm  = isSmooth;
    #end        
    #local _f   = dictionary {
        .vindices:  VIndices,
        .isSmooth:  _sm
    }
    ;
    
    _f
#end

// End Mesh2_face_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh2_face_normal(Mesh2,Face)
//
#macro Mesh2_face_normal(Mesh2,Face)
    #local _pa  = Mesh2.vertices[Face.vindices.x].point;
    #local _pb  = Mesh2.vertices[Face.vindices.y].point;
    #local _pc  = Mesh2.vertices[Face.vindices.z].point;
    
    (-vnormalize(vcross(_pb-_pa,_pc-_pa)))

#end

// End Mesh2_face_normal
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh2_dict_create(UVMapping,DefaultNormal)
//

#macro Mesh2_dict_create(UVMapping,optional DefaultNormal)
    #ifdef (local.DefaultNormal)
        #local _nrm = DefaultNormal;
    #else
        #local _nrm = <0,1,0>;
    #end
            
    #local _dict    = dictionary {
        .uvmapping: UVMapping,
        .vertices:  array,
        .vcount:    0,
        .defaultNormal: _nrm
        
        // Added by Mesh2_dict_add_face
        // .smooth_faces,
        // .smooth_count
        // .flat_faces,
        // .flat_count
    }
    ;
    
    _dict
#end

// End Mesh2_dict_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh2_dict_add_vertex(Mesh2,Vertex)
//
#macro Mesh2_dict_add_vertex(Mesh2,Vertex)
    #declare Mesh2.vertices[Mesh2.vcount]   = Vertex;
    #declare Mesh2.vcount                   = Mesh2.vcount + 1;
#end

// End Mesh2_dict_add_vertex
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh2_dict_add_face(Mesh2,Face)
//
#macro Mesh2_dict_add_face(Mesh2,Face)
    #if (Face.isSmooth)
        #ifndef (Mesh2.smooth_faces)
            #declare Mesh2.smooth_faces = array;
            #declare Mesh2.smooth_count = 0;
        #end                                
        #declare Mesh2.smooth_faces[Mesh2.smooth_count] = Face;
        #declare Mesh2.smooth_count                     = Mesh2.smooth_count + 1;
    #else
        #ifndef (Mesh2.flat_faces)
            #declare Mesh2.flat_faces = array;
            #declare Mesh2.flat_count = 0;
        #end
        #declare Mesh2.flat_faces[Mesh2.flat_count] = Face;
        #declare Mesh2.flat_count                   = Mesh2.flat_count + 1;
    #end
#end

// End Mesh2_dict_add_face
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh2_dict_compute_vertex_normals_from_faces(Mesh2)
//
// Iterate over all of the faces and compute normals for the vertices that
// are attached to faces by averaging the normals of the faces they are attached
// to.
//
#macro Mesh2_dict_compute_vertex_normals_from_faces(Mesh2)
    #local _vnormals    = array[Mesh2.vcount];
    
    #ifdef (Mesh2.smooth_faces)
        #for (i, 0, Mesh2.smooth_count-1, 1)
            #local _fnorm   = Mesh2_face_normal(Mesh2,Mesh2.smooth_faces[i]);
            #local _vi      = Mesh2.smooth_faces[i].vindices.x;
            #ifndef (_vnormals[_vi])
                #local _vnormals[_vi]   = dictionary { .norm: _fnorm, .ncount: 1 };
            #else
                #local _vnormals[_vi].norm      = (_vnormals[_vi].ncount * _vnormals[_vi].norm)/(_vnormals[_vi].ncount + 1);
                #local _vnormals[_vi].ncount    = _vnormals[_vi].ncount + 1;
            #end
            #local _vi      = Mesh2.smooth_faces[i].vindices.y;
            #ifndef (_vnormals[_vi])
                #local _vnormals[_vi]   = dictionary { .norm: _fnorm, .ncount: 1 };
            #else
                #local _vnormals[_vi].norm      = (_vnormals[_vi].ncount * _vnormals[_vi].norm)/(_vnormals[_vi].ncount + 1);
                #local _vnormals[_vi].ncount    = _vnormals[_vi].ncount + 1;
            #end
            #local _vi      = Mesh2.smooth_faces[i].vindices.z;
            #ifndef (_vnormals[_vi])
                #local _vnormals[_vi]   = dictionary { .norm: _fnorm, .ncount: 1 };
            #else
                #local _vnormals[_vi].norm      = (_vnormals[_vi].ncount * _vnormals[_vi].norm)/(_vnormals[_vi].ncount + 1);
                #local _vnormals[_vi].ncount    = _vnormals[_vi].ncount + 1;
            #end
        #end
    #end
    #ifdef (Mesh2.flat_faces)
        #for (i, 0, Mesh2.flat_count-1, 1)
            #local _fnorm   = Mesh2_face_normal(Mesh2,Mesh2.flat_faces[i]);
            #local _vi      = Mesh2.flat_faces[i].vindices.x;
            #ifndef (_vnormals[_vi])
                #local _vnormals[_vi]   = dictionary { .norm: _fnorm, .ncount: 1 };
            #else
                #local _vnormals[_vi].norm      = (_vnormals[_vi].ncount * _vnormals[_vi].norm)/(_vnormals[_vi].ncount + 1);
                #local _vnormals[_vi].ncount    = _vnormals[_vi].ncount + 1;
            #end
            #local _vi      = Mesh2.flat_faces[i].vindices.y;
            #ifndef (_vnormals[_vi])
                #local _vnormals[_vi]   = dictionary { .norm: _fnorm, .ncount: 1 };
            #else
                #local _vnormals[_vi].norm      = (_vnormals[_vi].ncount * _vnormals[_vi].norm)/(_vnormals[_vi].ncount + 1);
                #local _vnormals[_vi].ncount    = _vnormals[_vi].ncount + 1;
            #end
            #local _vi      = Mesh2.flat_faces[i].vindices.z;
            #ifndef (_vnormals[_vi])
                #local _vnormals[_vi]   = dictionary { .norm: _fnorm, .ncount: 1 };
            #else
                #local _vnormals[_vi].norm      = (_vnormals[_vi].ncount * _vnormals[_vi].norm)/(_vnormals[_vi].ncount + 1);
                #local _vnormals[_vi].ncount    = _vnormals[_vi].ncount + 1;
            #end
        #end
    #end
    
    #for (i, 0, Mesh2.vcount-1, 1)
        #ifdef (_vnormals[i])
            #declare Mesh2.vertices[i].norm     = _vnormals[i].norm;
        #else
            #warning concat("Mesh2_dict_compute_vertex_normals_from_faces: No normal computed for vertex ", str(i, 0, 0), "\n")    
        #end
    #end
    
#end

// End Mesh2_dict_compute_vertex_normals_from_faces
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh2_generate(Mesh2)
//
#macro Mesh2_generate(Mesh2)
    #local _mesh2   = mesh2 {
        vertex_vectors {
            Mesh2.vcount,
            #for (i, 0, Mesh2.vcount-1, 1)
                #if (i < Mesh2.vcount-1)
                    Mesh2.vertices[i].point,
                #else
                    Mesh2.vertices[i].point
                #end        
            #end
        }
        
        #ifdef (Mesh2.smooth_faces)
        normal_vectors {
            Mesh2.vcount,
            #for (i, 0, Mesh2.vcount-1, 1)
                #ifdef (Mesh2.vertices[i].norm)
                    #local _nrm = Mesh2.vertices[i].norm;
                #else
                    #local _nrm = Mesh2.defaultNormal;
                #end
                #if (i < Mesh2.vcount-1)
                    _nrm,
                #else
                    _nrm
                #end                
            #end
        }
        #end
        
        #if (Mesh2.uvmapping)
        uv_vectors {
            Mesh2.vcount,
            #for (i, 0, Mesh2.vcount-1, 1)
                #ifdef (Mesh2.vertices[i].uv)
                    #local _uv  = Mesh2.vertices[i].uv;
                #else
                    #local _uv  = <0,0>;
                #end
                #if (i < Mesh2.vcount-1)
                    _uv,
                #else
                    _uv
                #end                
            #end
        }
        #end
        
        #local _num_faces   = 0;
        #ifdef (Mesh2.smooth_faces)
            #local _num_faces   = _num_faces + Mesh2.smooth_count;
        #end
        #ifdef (Mesh2.flat_faces)
            #local _num_faces   = _num_faces + Mesh2.flat_count;
        #end        
        face_indices {
            _num_faces,
            #ifdef (Mesh2.smooth_faces)
                #for (i, 0, Mesh2.smooth_count-1, 1)
                    #if (i < _num_faces-1)
                    Mesh2.smooth_faces[i].vindices,
                    #else
                    Mesh2.smooth_faces[i].vindices
                    #end
                #end
            #end
            #ifdef (Mesh2.flat_faces)
                #for (i, 0, Mesh2.flat_count-1, 1)
                    #if (i < Mesh2.flat_count-1)
                    Mesh2.flat_faces[i].vindices,
                    #else
                    Mesh2.flat_faces[i].vindices
                    #end        
                #end
            #end
        }
        
        #ifdef (Mesh2.flat_faces)
            #ifdef (Mesh2.smooth_faces)
            normal_indices {
                #for (i, 0, Mesh2.smooth_count-1, 1)
                    #if (i < Mesh2.smooth_count-1)
                    Mesh2.smooth_faces[i].vindices,
                    #else
                    Mesh2.smooth_faces[i].vindices
                    #end
                #end
            }
            #end
        #end
    }
    
    _mesh2
#end

// End Mesh2_generate
//-----------------------------------------------------------------------------


// End Mesh2 Dictionary
//=============================================================================

//=============================================================================
// 2D Grid Meshes: This is a specialization of the Mesh2 that makes it easier
// to represent 2D sheets with a rectangular grid of vertices whose 3D locations
// can be manipulated.  The point is to make it easier to generate faces based
// on the grid of point positions.
//

//-----------------------------------------------------------------------------
// Mesh2_grid_create(GridSize, UVMapping, optional SheetSize, optional Smooth, optional DefaultNormal)
// 
// Simple wrapper around Mesh2_dict_create() that adds in a grid of vertices.
//
// If the optional SheetSize is provided, the vertex list is initialized with
// points that cover the given 2D area defined by <0,0> to SheetSize
//
// If no SheetSize is provided, the vertex list is initialized to the appropriate
// size based on the GridSize, but no vertices are assigned.  They should be
// added using Mesh2_grid_set_vertex(I,J,Vertex).
//
// Faces are always immediately created with two triangles per grid box using
// the grid indices.  If no SheetSize is created, it is important that all vertices
// in the grid get set before doing any normal computations or generating
// the mesh2 object.
//
#macro Mesh2_grid_create(GridSize, UVMapping, optional SheetSize, optional Smooth, optional DefaultNormal)
    #ifdef (local.DefaultNormal)
        #local _nrm = DefaultNormal;
    #else
        #local _nrm = <0,1,0>;
    #end
    
    #local _num_vertices    = GridSize.x * GridSize.y;        
            
    #local _dict    = dictionary {
        .uvmapping: UVMapping,
        .vertices:  array[_num_vertices],
        .vcount:    _num_vertices,
        .defaultNormal: _nrm,
        .grid_size: GridSize,
        
        // Added by Mesh2_dict_add_face
        // .smooth_faces,
        // .smooth_count
        // .flat_faces,
        // .flat_count
    }
    ;
    
    // Add in initial vertex points based on the sheet size
    // if provided
    
    #ifdef (local.SheetSize)
        #ifdef (local.Smooth)
            #local _smooth  = Smooth;
        #else
            #local _smooth  = false;
        #end
        
        #local _dict.sheet_size     = SheetSize;
        #local _dict.smoothing      = _smooth;
        
        #local _box_size    = SheetSize/(GridSize - 1);
        #local _box_size_3d = <_box_size.x, 0, _box_size.y>;
        
        #for (i, 0, GridSize.y-1, 1)
            #for (j, 0, GridSize.x-1, 1)
                #local _index   = i*GridSize.x + j;
                #if (UVMapping)
                    #local _uv  = <i,j>/(GridSize - 1);
                #end
                #local _pt  = <i,0,j>*_box_size_3d;
                #local _dict.vertices[_index]   = Mesh2_vertex_create(_pt, _uv,);    
            #end
        #end    
    
    #end
    
    // Add in faces based on the grid
    //
    #for (i, 0, GridSize.y-2, 1)
        #for (j, 0, GridSize.x-2, 1)
            #local _fi  = <i*GridSize.x + j, i*GridSize.x + j + 1, (i+1)*GridSize.x + j + 1>;
            Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
            #local _fi  = <(i+1)*GridSize.x + j + 1, (i+1)*GridSize.x + j, i*GridSize.x + j>;
            Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
        #end
    #end
    
    
    _dict
#end

// End Mesh2_grid_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh2_grid_get_vertex(Mesh2,I,J)
//
#macro Mesh2_grid_get_vertex(Mesh2,I,J)
    #local _index   = I*Mesh2.grid_size.x + J;
    #local _v       = Mesh2.vertices[_index];
    
    _v
#end

// End Mesh2_grid_get_vertex
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh2_grid_set_vertex(Mesh2,I,J,V)
//
#macro Mesh2_grid_set_vertex(Mesh2,I,J,V)
    #local _index   = I*Mesh2.grid_size.x + J;
    #declare Mesh2.vertices[_index] = V;
#end

// End Mesh2_grid_set_vertex
//-----------------------------------------------------------------------------

// End 2D Grid Meshes
//=============================================================================

#version Libmesh_Inc_Temp;

#end // ifndef (Libmesh_Inc_Temp)
