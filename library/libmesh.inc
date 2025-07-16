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
#include "transforms.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
#include "libstringify.inc"

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
                    #local _vuv  = Mesh2.vertices[i].uv;
                #else
                    #local _vuv  = <0,0>;
                #end
                #if (i < Mesh2.vcount-1)
                    _vuv,
                #else
                    _vuv
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
                Mesh2.smooth_count,
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

//-----------------------------------------------------------------------------
// Mesh2_write(Mesh2,optional VarName)
//
// Writes the SDL for a mesh2 object from the given Mesh2 dictionary to a
// file opened with SFY_file_open()
//
#macro Mesh2_write(Mesh2,optional VarName)
    #ifdef(VarName)
        #write(SFY_file_handle, "#declare ", VarName, " = ")
    #end
    
    #write(SFY_file_handle, "mesh2 {\n")
    
    #write(SFY_file_handle, "    vertex_vectors {\n")
    #write(SFY_file_handle, "        ", str(Mesh2.vcount, 0, 0), ",")
    #for (i, 0, Mesh2.vcount-1, 1)
        #if (mod(i, 10) = 0)
            #write(SFY_file_handle, "\n        ")
        #end    
        #write(SFY_file_handle, Mesh2.vertices[i].point)
        #if (i < Mesh2.vcount-1)
            #write(SFY_file_handle, ",")
        #end
    #end
    #write(SFY_file_handle, "\n    }\n")
    
    #ifdef (Mesh2.smooth_faces)
        #write(SFY_file_handle, "    normal_vectors {\n")
        #write(SFY_file_handle, "        ", str(Mesh2.vcount, 0, 0), ",")
        #for (i, 0, Mesh2.vcount-1, 1)
            #if (mod(i, 10) = 0)
                #write(SFY_file_handle, "\n        ")
            #end    
            #ifdef (Mesh2.vertices[i].norm)
                #local _nrm = Mesh2.vertices[i].norm;
            #else
                #local _nrm = Mesh2.defaultNormal;
            #end
            #write(SFY_file_handle, _nrm)
            #if (i < Mesh2.vcount-1)
                #write(SFY_file_handle, ",")
            #end
        #end
        #write(SFY_file_handle, "\n    }\n")
    #end
    
    #if (Mesh2.uvmapping)
        #write(SFY_file_handle, "    uv_vectors {\n")
        #write(SFY_file_handle, "        ", str(Mesh2.vcount, 0, 0), ",")
        #for (i, 0, Mesh2.vcount-1, 1)
            #if (mod(i, 10) = 0)
                #write(SFY_file_handle, "\n        ")
            #end    
            #ifdef (Mesh2.vertices[i].uv)
                #local _vuv = Mesh2.vertices[i].uv;
            #else
                #local _vuv = <0,0>;
            #end
            #write(SFY_file_handle, _vuv)
            #if (i < Mesh2.vcount-1)
                #write(SFY_file_handle, ",")
            #end
        #end
        #write(SFY_file_handle, "\n    }\n")
    #end
    
    #local _num_faces   = 0;
    #ifdef (Mesh2.smooth_faces)
        #local _num_faces   = _num_faces + Mesh2.smooth_count;
    #end
    #ifdef (Mesh2.flat_faces)
        #local _num_faces   = _num_faces + Mesh2.flat_count;
    #end
            
    #write(SFY_file_handle, "    face_indices {\n")
    #write(SFY_file_handle, "        ", str(_num_faces, 0, 0), ",")
    #ifdef (Mesh2.smooth_faces)
        #for (i, 0, Mesh2.smooth_count-1, 1)
            #if (mod(i, 10) = 0)
                #write(SFY_file_handle, "\n        ")
            #end
            #write(SFY_file_handle, Mesh2.smooth_faces[i].vindices)    
            #if (i < _num_faces-1)
                #write(SFY_file_handle, ",")
            #end    
        #end
        #write(SFY_file_handle, "\n        ")
    #end
    #ifdef (Mesh2.flat_faces)
        #for (i, 0, Mesh2.flat_count-1, 1)
            #if (mod(i, 10) = 0)
                #write(SFY_file_handle, "\n        ")
            #end
            #write(SFY_file_handle, Mesh2.flat_faces[i].vindices)
            #if (i < _num_faces-1)
                #write(SFY_file_handle, ",")
            #end    
        #end
    #end
    #write(SFY_file_handle, "\n")
    #write(SFY_file_handle, "    }\n")
    
    #write(SFY_file_handle, "}\n")                                       

    #ifdef (Mesh2.flat_faces)
        #ifdef (Mesh2.smooth_faces)
        #write(SFY_file_handle, "    normal_indices {\n")
        #write(SFY_file_handle, "        ", str(Mesh2.smooth_count, 0, 0), ",")
        #for (i, 0, Mesh2.smooth_count-1, 1)
            #if (mod(i, 10) = 0)
                #write(SFY_file_handle, "\n        ")
            #end
            #write(SFY_file_handle, Mesh2.smooth_faces[i].vindices)
            #if (i < Mesh2.smooth_count-1)
                #write(SFY_file_handle, ",")
            #end
        #end
        #write(SFY_file_handle, "    }\n")
        #end
    #end
    
    #ifdef(VarName)
        #write(SFY_file_handle, ";\n")
    #end    
#end

// End Mesh2_write
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

//=============================================================================
// 3D Grid Meshes:  This is a specialization of the Mesh2_dict that includes
// 2 Plys of a rectangular grid of vertices that construct faces for the bottom
// and top grids as a Mesh2_grid, but also include faces that tie the two
// faces together.
//

//-----------------------------------------------------------------------------
// Mesh2_3dgrid_create(GridSize, UVMapping, optional SheetSize, optional Smooth, optional DefaultNormal)
//
// SheetSize is now 3D, with the Y component representing the thickness of the sheet
//
#macro Mesh2_3dgrid_create(GridSize, UVMapping, optional SheetSize, optional Smooth, optional DefaultNormal)
    #ifdef (local.DefaultNormal)
        #local _nrm = DefaultNormal;
    #else
        #local _nrm = <0,1,0>;
    #end
    
    #local _num_ply_vertices    = GridSize.x * GridSize.y;
    #local _num_vertices        = 2 * _num_ply_vertices;        
            
    #local _dict    = dictionary {
        .uvmapping: UVMapping,
        .vertices:  array[_num_vertices],
        .vcount:    _num_vertices,
        .defaultNormal: _nrm,
        .grid_size: GridSize,
        .ply_offset:    _num_ply_vertices,
        
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
        
        #local _box_size    = <SheetSize.x,SheetSize.z>/(GridSize - 1);
        #local _box_size_3d = <_box_size.x, 0, _box_size.y>;
        
        #for (i, 0, GridSize.y-1, 1)
            #for (j, 0, GridSize.x-1, 1)
                #local _index   = i*GridSize.x + j;
                #if (UVMapping)
                    #local _uv  = <i,j>/(GridSize - 1);
                #end
                #local _pt  = <i,0,j>*_box_size_3d;
                #local _dict.vertices[_index]   = Mesh2_vertex_create(_pt, _uv,);
                
                #local _index   = _num_ply_vertices + i*GridSize.x + j;
                #local _pt  = _pt + <0, SheetSize.y, 0>;
                #local _dict.vertices[_index]   = Mesh2_vertex_create(_pt, _uv,);
            #end
        #end    
    
    #end
    
    // Add in faces based on the grid
    //
    #for (i, 0, GridSize.y-2, 1)
        #for (j, 0, GridSize.x-2, 1)
            // Top ply faces
            //
            #local _fi  = <_num_ply_vertices + i*GridSize.x + j, _num_ply_vertices + i*GridSize.x + j + 1, _num_ply_vertices + (i+1)*GridSize.x + j + 1>;
            Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
            #local _fi  = <_num_ply_vertices + (i+1)*GridSize.x + j + 1, _num_ply_vertices + (i+1)*GridSize.x + j, _num_ply_vertices + i*GridSize.x + j>;
            Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
            
            // Bottom ply faces
            //
            #local _fi  = <(i+1)*GridSize.x + j + 1, i*GridSize.x + j + 1, i*GridSize.x + j>;
            Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
            #local _fi  = <i*GridSize.x + j, (i+1)*GridSize.x + j, (i+1)*GridSize.x + j + 1>;
            Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
        #end
    #end
    
    // Add in the side tying faces
    //
    #local _top_row_offset  = GridSize.x*(GridSize.y-1);
    #for (j, 0, GridSize.x-2, 1)
        #local _fi  = <j, j+1, _num_ply_vertices + j + 1>;
        Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
        #local _fi  = <_num_ply_vertices + j + 1, _num_ply_vertices + j, j>;
        Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
        
        #local _fi  = <_top_row_offset + _num_ply_vertices + j + 1, _top_row_offset + j + 1, _top_row_offset + j>;
        Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
        #local _fi  = <_top_row_offset + j, _top_row_offset + _num_ply_vertices + j, _top_row_offset + _num_ply_vertices + j + 1>;
        Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
    #end
    
    #local _right_col_offset    = GridSize.x-1;
    #for (i, 0, GridSize.y-2, 1)
        #local _fi  = <i*GridSize.x, _top_row_offset + i*GridSize.x, (i+1)*GridSize.x>;
        Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
        #local _fi  = <_top_row_offset + i*GridSize.x, _top_row_offset + (i+1)*GridSize.x, (i+1)*GridSize.x>;
        Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
        
        #local _fi  = <i*GridSize.x + _right_col_offset, _top_row_offset + i*GridSize.x + _right_col_offset, (i+1)*GridSize.x + _right_col_offset>;
        Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
        #local _fi  = <_top_row_offset + i*GridSize.x + _right_col_offset, _top_row_offset + (i+1)*GridSize.x + _right_col_offset, (i+1)*GridSize.x + _right_col_offset>;
        Mesh2_dict_add_face(_dict, Mesh2_face_create(_fi,_smooth))
    #end
    
    
    _dict
#end

// End Mesh2_3dgrid_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh2_3dgrid_get_vertex(Mesh2,I,J,P)
//
#macro Mesh2_3dgrid_get_vertex(Mesh2,I,J,P)
    #local _index   = Mesh2.ply_offset*P + I*Mesh2.grid_size.x + J;
    #local _v       = Mesh2.vertices[_index];
    
    _v
#end

// End Mesh2_3dgrid_get_vertex
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh2_3dgrid_set_vertex(Mesh2,I,J,P,V)
//
#macro Mesh2_3dgrid_set_vertex(Mesh2,I,J,P,V)
    #local _index   = Mesh2.ply_offset*P + I*Mesh2.grid_size.x + J;
    #if (_index >= Mesh2.vcount)
        #error concat("Mesh2_3dgrid_set_vertex(Mesh2,", str(I,0,0), ",", str(J,0,0), ",", str(P,0,0), ",V): Out of bounds\n")
    #end    
    #declare Mesh2.vertices[_index] = V;
#end

// End Mesh2_3dgrid_set_vertex
//-----------------------------------------------------------------------------


// End 3D Grid Meshes
//=============================================================================

//=============================================================================
// Mesh_plane Utilities
//

//-----------------------------------------------------------------------------
// Mesh_plane_create(Normal,optional Origin)
//
#macro Mesh_plane_create(Normal,optional Origin)
    #local _nrm     = vnormalize(Normal);
    #local _orig    = #ifdef(Origin) Origin; #else <0,0,0>; #end
    #local _plane   = dictionary {
        .pnorm: _nrm,
        .porig: _orig,
        .pd:    vdot(_nrm,_orig)
    }
    ;
    
    _plane
#end

// End Mesh_plane_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_plane_create_triangle(Triangle)
//
#macro Mesh_plane_create_triangle(Triangle)
    #local _nrm = vcross(Triangle.verts[2] - Triangle.verts[0], Triangle.verts[1] - Triangle.verts[0]);
    #if (vlength(_nrm) > 0)
        #local _plane   = Mesh_plane_create(_nrm,Triangle.center);
    #else
        #error concat("Mesh_plane_create_triangle: Invalid Triangle:\n")
        Mesh_triangle_dump(Triangle,"    ")
    #end                                                          
    _plane
#end

// End Mesh_plane_create_triangle
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_plane_dump(Plane,IndentStr)
//
#macro Mesh_plane_dump(Plane,IndentStr)
    #debug concat(IndentStr, "dictionary {\n")
    #debug concat(IndentStr, "    .pnorm:    <", vstr(3,Plane.pnorm,",", 0, 6), ">,\n")
    #debug concat(IndentStr, "    .porig:    <", vstr(3,Plane.porig,",", 0, 6), ">,\n")
    #debug concat(IndentStr, "    .pd:       ", str(Plane.pd, 0, 6), "\n")
    #debug concat(IndentStr, "}\n")
#end

// End Mesh_plane_dump
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_plane_project(Plane,Point)
//
#macro Mesh_plane_project(Plane,Point)
    #local _k   = Plane.pd - vdot(Plane.pnorm,Point);
    (Point + _k*Plane.pnorm) 
#end

// End Mesh_plane_project
//-----------------------------------------------------------------------------

// End Mesh_plane Utilities
//=============================================================================

//=============================================================================
// Mesh_triangle Utilities
//

//-----------------------------------------------------------------------------
// Mesh_triangle_create(Points)
//
//      Creates a dictionary {} object consisting of a set of 3 points and
// an automatcally generate center in the XZ plane
//

#macro Mesh_triangle_create(Points)
    #local _triangle    = dictionary {
        .verts:     Points,
        .center:    (Points[0] + Points[1] + Points[2])/3
    }
    ;
    
    _triangle    
#end 

// End Mesh_triangle_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_update_vertex(Triangle,VIndex,NewValue)
//
#macro Mesh_triangle_update_vertex(Triangle,VIndex,NewValue)
    #local Triangle.verts[VIndex]   = NewValue;
    #local Triangle.center          = (Triangle.verts[0] + Triangle.verts[1] + Triangle.verts[2])/3;
#end

// End Mesh_triangle_update_vertex
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_transform(Triangle,Transform)
//
// Creates a new Mesh_triangle by transforming its vertices using the given
// Transform
//
#macro Mesh_triangle_transform(Triangle,Transform)
    #local _tpoints = array[3];
    #for (i, 0, 2, 1)
        #local _tpoints[i]  = vtransform(Triangle.verts[i],Transform);
    #end                                                              
    #local _mt  = Mesh_triangle_create(_tpoints);
    
    _mt
#end

// End Mesh_triangle_transform
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_project(Plane,Triangle)
//
#macro Mesh_triangle_project(Plane,Triangle)
    #local _pts = array[3];
    #for (i, 0, 2, 1)
        #local _pts[i]  = Mesh_plane_project(Plane,Triangle.verts[i]);
    #end                                                              
    #local _t   = Mesh_triangle_create(_pts);
    
    _t
#end

// End Mesh_triangle_project
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_dump(Triangle,IndentStr)
//
//  Writes a triangle dictionary to the debug stream at the given indent level
//
#macro Mesh_triangle_dump(Triangle,IndentStr)
    #debug concat(IndentStr,"dictionary {\n") 
    #debug concat(IndentStr,"    .verts:      array[3]{\n")
    #for (k, 0, 2, 1)
    #debug concat(IndentStr,"        <", vstr(3, Triangle.verts[k], ",", 0, 6), ">,\n")
    #end
    #debug concat(IndentStr,"    },\n")
    #debug concat(IndentStr,"    .center:     <", vstr(3, Triangle.center, ",", 0, 6), ">\n")
    #debug concat(IndentStr,"},\n")
#end

// End Mesh_triangle_dump
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_write(Triangle,IndentStr)
//
//  Writes a triangle dictionary to the debug stream at the given indent level
//
#macro Mesh_triangle_write(Triangle,IndentStr)
    #write(SFY_file_handle,IndentStr,"dictionary {\n") 
    #write(SFY_file_handle,IndentStr,"    .verts:      array[3]{\n")
    #for (k, 0, 2, 1)
    #write(SFY_file_handle,IndentStr,"        ", Triangle.verts[k], ",\n")
    #end
    #write(SFY_file_handle,IndentStr,"    },\n")
    #write(SFY_file_handle,IndentStr,"    .center:     ", Triangle.center, "\n")
    #write(SFY_file_handle,IndentStr,"},\n")
#end

// End Mesh_triangle_write
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_set_create()
// 
#macro Mesh_triangle_set_create()
    #local _set = dictionary {
        .num_triangles: 0,
        .triangles: array
    }
    ;
    
    _set
#end

// End Mesh_triangle_set_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_set_add(Set,Triangle)
//
#macro Mesh_triangle_set_add(Set,Triangle)
    #local Set.triangles[Set.num_triangles] = Triangle;
    #local Set.num_triangles                = Set.num_triangles + 1;
#end

// End Mesh_triangle_set_add
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_set_append(TriangleSet,TriangleSet2) 
//
#macro Mesh_triangle_set_append(TriangleSet,TriangleSet2)
    #for (i, 0, TriangleSet2.num_triangles-1, 1)
        Mesh_triangle_set_add(TriangleSet, TriangleSet2.triangles[i])
    #end
#end

// End Mesh_triangle_set_append
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_set_dump(Set,IndentStr)
//
#macro Mesh_triangle_set_dump(Set,IndentStr)
    #debug concat(IndentStr,"dictionary {\n")
    #debug concat(IndentStr,"    .num_triangles:    ", str(Set.num_triangles, 0, 0), ",\n")
    #if (Set.num_triangles > 0)
        #debug concat(IndentStr, "    .triangles:   array[", str(Set.num_triangles, 0, 0), "]{\n")
        #local _indent  = concat(IndentStr, "        ");
        #for (i, 0, Set.num_triangles-1, 1)
            Mesh_triangle_dump(Set.triangles[i],_indent)
        #end
        #debug concat(IndentStr, "    }\n")
    #else
        #debug concat(IndentStr, "    .triangles:   array\n")
    #end
    #debug concat(IndentStr,"}\n")
#end

// End Mesh_triangle_set_dump
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_set_write(Set,IndentStr)
//
#macro Mesh_triangle_set_write(Set,IndentStr)
    #write(SFY_file_handle, IndentStr, "dictionary {\n")
    #write(SFY_file_handle, IndentStr,"    .num_triangles:    ", str(Set.num_triangles, 0, 0), ",\n")
    #if (Set.num_triangles > 0)
        #write(SFY_file_handle, IndentStr, "    .triangles:   array[", str(Set.num_triangles, 0, 0), "]{\n")
        #local _indent  = concat(IndentStr, "        ");
        #for (i, 0, Set.num_triangles-1, 1)
            Mesh_triangle_write(Set.triangles[i],_indent)
        #end
        #write(SFY_file_handle, IndentStr, "    }\n")
    #else
        #write(SFY_file_handle, IndentStr, "    .triangles:   array\n")
    #end
    #write(SFY_file_handle, IndentStr,"}\n")
#end

// End Mesh_triangle_set_write
//-----------------------------------------------------------------------------

// End Mesh_triangle Utilities
//=============================================================================

//=============================================================================
// Mesh Triangle Surface Generation
//
// These are macros for generating Mesh_triangle_sets in various ways by
// scanning an object from the top or bottom (y/-y) directions along the
// bounding box in the XZ plane.
//

//-----------------------------------------------------------------------------
// Mesh_bb_create(LBounds,UBounds)
//
#macro Mesh_bb_create(LBounds,UBounds)
    #local _bb  = dictionary {
        .lbounds:   LBounds,
        .ubounds:   UBounds,
        .center:    (LBounds + UBounds)/2,
        .bbsize:    (UBounds - LBounds)
    }
    ;
    
    _bb
#end

// End Mesh_bb_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_bb_dump(BB)
//
#macro Mesh_bb_dump(BB)
    #debug concat("dictionary {\n")
    #debug concat("    .lbounds:    ", vstr(3, BB.lbounds, ",", 0, 6), ",\n")
    #debug concat("    .ubounds:    ", vstr(3, BB.ubounds, ",", 0, 6), ",\n")
    #debug concat("    .center:    ", vstr(3, BB.center, ",", 0, 6), ",\n")
    #debug concat("    .bbsize:    ", vstr(3, BB.bbsize, ",", 0, 6), "\n")
    #debug concat("}\n")
#end

// End Mesh_bb_dump
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_bb_subdivide(BB,DimensionSplits)
//
#macro Mesh_bb_subdivide(BB,DimensionSplits)
    #local _splits  = DimensionSplits*<1,1,1>;
    #local _xdivs   = max(1, _splits.x);
    #local _ydivs   = max(1, _splits.y);
    #local _zdivs   = max(1, _splits.z);
    
    #local _result  = array[_xdivs*_ydivs*_zdivs];
    #local _idx     = 0;
    #for (_x, 0, _xdivs-1, 1)
        #for (_y, 0, _ydivs-1, 1)
            #for (_z, 0, _zdivs-1, 1)
                #local _lb  = <BB.lbounds.x + _x*(BB.bbsize.x/_xdivs), BB.lbounds.y + _y*(BB.bbsize.y/_ydivs), BB.lbounds.z + _z*(BB.bbsize.z/_zdivs)>;
                #local _ub  = <BB.lbounds.x + (_x+1)*(BB.bbsize.x/_xdivs), BB.lbounds.y + (_y+1)*(BB.bbsize.y/_ydivs), BB.lbounds.z + (_z+1)*(BB.bbsize.z/_zdivs)>;
                #local _result[_idx]    = Mesh_bb_create(_lb,_ub);
                #local _idx             = _idx + 1;
            #end
        #end
    #end
    
    _result
#end

// End Mesh_bb_subdivide
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_hbb_create(Level,NumSubLevels,BB)
//
//      Creates a dictionary{} representing the root of a tree with NumLevels-1
// children, all with empty triangle sets
//
#macro Mesh_triangle_hbb_create(Level,NumSubLevels,BB) 
    #local _tset    = Mesh_triangle_set_create();
    
    #local _hbb = dictionary {
        .level:         Level,
        .sub_levels:    NumSubLevels,
        .bb:            BB,
        .num_children:  0,
        .children:      array,
        .triangle_set:  _tset
    }
    ;
    
    #if (NumSubLevels > 0)
        #local _sub_bbs = Mesh_bb_subdivide(BB,<2,1,2>);
        #for (i, 0, 3, 1)
            Mesh_triangle_hbb_add_child(_hbb,Mesh_triangle_hbb_create(Level+1,NumSubLevels-1,_sub_bbs[i]))
        #end
    #end
    
    _hbb
#end

// End Mesh_triangle_hbb_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_hbb_add_child(HBB,ChildHBB)
//
#macro Mesh_triangle_hbb_add_child(HBB,ChildHBB)
    #local HBB.children[HBB.num_children]   = ChildHBB;
    #local HBB.num_children                 = HBB.num_children + 1;
#end

// End Mesh_triangle_hbb_add_child
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_hbb_object_generate_triangles(HBB,Object,ScanDir)
//
#macro Mesh_triangle_hbb_object_generate_triangles(HBB,Object,ScanDir)
    #local _debug       = #ifdef (Mesh_triangle_hbb_debug) Mesh_triangle_hbb_debug; #else 0; #end
    #local _scan_res    = #ifdef (Mesh_triangle_hbb_scan_resolution) Mesh_triangle_hbb_scan_resolution; #else 0.01; #end
    #local _scan_size   = _scan_res * vlength(HBB.bb.bbsize);
    
    #if (_debug > 0)
        Mesh_bb_dump(HBB.bb)
    #end    
    
    // Generate triangles for this HBB's bounding box
    //
    #if (ScanDir > 0)
        #local _scan_dir    = -y;
        #local _scan_height = HBB.bb.ubounds.y + 1;
    #else
        #local _scan_dir    = y;
        #local _scan_height = HBB.bb.lbounds.y - 1;
    #end
    
    // Find a location for the center point that projects onto the object
    //
    
    #local _center_xz   = <1,0,1> * HBB.bb.center;
    #local _max_r       = vlength(<1,0,1>*HBB.bb.bbsize)/2;
    #local _cur_r       = 0;
    #local _nrm         = <0,0,0>;
    #local _center_pt   = trace(Object, _center_xz + y*_scan_height, _scan_dir, _nrm);
    #if (vlength(_nrm) = 0)
        #local _cur_r   = _cur_r + _scan_size;
        #while (_cur_r <= _max_r & vlength(_nrm) = 0)
            #local _cur_a   = 0;
            #local _delta_a = 360/(2*pi*_cur_r/_scan_size);
            #while (_cur_a < 360 & vlength(_nrm) = 0)
                #local _center_xz   = vrotate(<0, 0, _cur_r>, <0, _cur_a, 0>);
                #local _center_pt   = trace(Object, _center_xz + y*_scan_height, _scan_dir, _nrm);
            #end
        #end
    #end
    
    // If we find a center point, we can proceed with finding the vertices
    // for the corners of the bounding box, then generating up to 4 triangles,
    // and subdividing the bounding box to find child triangles.  Otherwise,
    // no triangles are added for the parent bb, and no children are generated
    // as they will not find a center point either.
    //
    // We do not worry, at this point, whether the line segment from each
    // of the corner vertices is completely contained within the object.  That
    // is probably too time-consuming for the added value, but we can explore it
    // later.
    #if (vlength(_nrm) > 0)
        #if (_debug > 1) #debug concat("_center_pt=<", vstr(3, _center_pt, ",", 0, 6), ">\n") #end
        #local _vertices    = array[4] {
            <HBB.bb.lbounds.x, 0, HBB.bb.lbounds.z>,
            <HBB.bb.ubounds.x, 0, HBB.bb.lbounds.z>,
            <HBB.bb.ubounds.x, 0, HBB.bb.ubounds.z>,
            <HBB.bb.lbounds.x, 0, HBB.bb.ubounds.z>
        }
        ;
        #for (i, 0, 3, 1)
            // Find a location for the vertex that projects onto the object
            // from the given location.
            //
            #local _vert_xz = <1,0,1>*_vertices[i];
            #local _max_d   = vlength(_vert_xz - <1,0,1>*_center_pt);
            #local _cur_d   = 0;
            #local _nrm     = <0,0,0>;
            #local _vert_pt = trace(Object, _vert_xz + y*_scan_height, _scan_dir, _nrm);
            #if (vlength(_nrm) = 0)
                #local _ddir    = vnormalize(<1,0,1>*_center_pt - _vert_xz);
                #while (_cur_d <= _max_d & vlength(_nrm) = 0)
                    #local _vert2_xz    = _vert_xz + _ddir*_cur_d;
                    #local _vert_pt     = trace(Object, _vert2_xz + y*_scan_height, _scan_dir, _nrm);
                    #if (vlength(_nrm) = 0)
                        #local _cur_d   = _cur_d + _scan_size;
                    #end    
                #end
            #end
            #if (vlength(_nrm) = 0)
                #local _vertices[i] = _center_pt;
            #else
                #local _vertices[i] = _vert_pt;
                #if (_debug > 1) #debug concat("_vertices[", str(i, 0, 0), "]=<", vstr(3, _vertices[i], ",", 0, 6), ">\n") #end    
            #end    
        #end
        
        // Compute and validate the triangle from the CenterPoint to vertex[i], to vertex[mod(i+1,4)]
        #for (i, 0, 3, 1)
            #local _tpoints = array[3] {
                _center_pt, _vertices[i], _vertices[mod(i+1,4)]
            }
            ;
            #if (vlength(_tpoints[0] - _tpoints[1]) > 0 & vlength(_tpoints[1] - _tpoints[2]) > 0 & vlength(_tpoints[2] - _tpoints[0]) > 0)
                Mesh_triangle_set_add(HBB.triangle_set, Mesh_triangle_create(_tpoints))
                #if (_debug > 2) Mesh_triangle_dump(HBB.triangle_set.triangles[HBB.triangle_set.num_triangles-1],"") #end
            #end
        #end
        
        #if (HBB.num_children > 0 & HBB.triangle_set.num_triangles > 0)
            #for (i, 0, HBB.num_children - 1, 1)
                Mesh_triangle_hbb_object_generate_triangles(HBB.children[i],Object,ScanDir)
            #end
        #end
    #else
        #warning concat("Mesh_triangle_hbb_object_generate_triangles: No center found\n")    
    #end
#end

// End Mesh_triangle_hbb_object_generate_triangles
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_hbb_object_generate(Object,NumLevels,ScanDir)
//
//      Creates a new Mesh_triangle_hbb starting with 4 triangles (max) for the bounding
// box of the object, and then recursively subdividing that bounding box NumLevels-1
// times and generating triangles for the sub-levels.  The triangles are computed
// scanning the object at the vertices of the bounding box from the given ScanDir,
// (-1/+1)*y.  If the ray doesn't intersect the object, the vertex is moved towards
// the center of the bounding box (stopping at the center and failing to create
// a triangle.
//
#macro Mesh_triangle_hbb_object_generate(Object,NumLevels,ScanDir)
    #local _root_bb = Mesh_bb_create(min_extent(Object),max_extent(Object));
    #local _root    = Mesh_triangle_hbb_create(0,NumLevels-1,_root_bb);
    
    Mesh_triangle_hbb_object_generate_triangles(_root,Object,ScanDir)
    
    _root
#end

// End Mesh_triangle_hbb_object_generate
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_hbb_get_leaf_triangles(HBB)
//
#macro Mesh_triangle_hbb_add_leaf_triangles(HBB,TriangleSet)
    #if (HBB.num_children = 0)
        #Mesh_triangle_set_append(TriangleSet, HBB.triangle_set)
    #else
        #for (i, 0, HBB.num_children-1, 1)
            Mesh_triangle_hbb_add_leaf_triangles(HBB.children[i],TriangleSet)
        #end
    #end
#end

#macro Mesh_triangle_hbb_get_leaf_triangles(HBB)
    #local _triangle_set    = Mesh_triangle_set_create();
    Mesh_triangle_hbb_add_leaf_triangles(HBB,_triangle_set)
    
    _triangle_set
#end

// End Mesh_triangle_hbb_get_leaf_triangles
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mesh_triangle_hbb_get_ordered_triangles(HBB)
//
#macro Mesh_triangle_hbb_add_level_n_triangles(HBB,TriangleSet,Level)
    #if (HBB.level = Level)
        Mesh_triangle_set_append(TriangleSet, HBB.triangle_set)
    #elseif (HBB.level < Level & HBB.num_children > 0)
        #for (i, 0, HBB.num_children - 1, 1)
            Mesh_triangle_hbb_add_level_n_triangles(HBB.children[i],TriangleSet,Level)
        #end
    #end
#end

#macro Mesh_triangle_hbb_get_ordered_triangles(HBB)
    #local _triangle_set    = Mesh_triangle_set_create();
    
    #for (i, 0, HBB.sub_levels, 1)
        Mesh_triangle_hbb_add_level_n_triangles(HBB,_triangle_set,i)
    #end
    _triangle_set
#end

// End Mesh_triangle_hbb_get_ordered_triangles
//-----------------------------------------------------------------------------

// End Mesh Triangle Surface Generation
//=============================================================================

#version Libmesh_Inc_Temp;

#end // ifndef (Libmesh_Inc_Temp)
