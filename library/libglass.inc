/******************************************************************************
 * File: libglass.inc
 * Description:
 *      Libglass ".inc" file
 ******************************************************************************/

#ifndef(Libglass_Inc_Temp)

#declare Libglass_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libglass.inc\n"
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
// Glass Materials
//
#declare Glass_ior                  = 1.5;
#declare Glass_soda_lime_ior        = 1.511;
#declare Glass_albite_ior           = 1.489;
#declare Glass_arsenic_ior          = 2.040;
#declare Glass_crown_ior            = 1.52;
#declare Glass_crown_zinc_ior       = 1.517;
#declare Glass_crystal_ior          = 2.0;
#declare Glass_flint_29lead_ior     = 1.569;
#declare Glass_flint_55lead_ior     = 1.669;
#declare Glass_flint_71d_ior        = 1.805;
#declare Glass_flint_dense_ior      = 1.660;
#declare Glass_flint_heaviest_ior   = 1.890;
#declare Glass_flint_heavy_ior      = 1.655;
#declare Glass_flint_lanthanum_ior  = 1.800;
#declare Glass_flint_light_ior      = 1.580;
#declare Glass_flint_medium_ior     = 1.627;
#declare Glass_fused_silica_ior     = 1.459;
#declare Glass_plexiglass_ior       = 1.51;
#declare Glass_porcelain_ior        = 1.504;
#declare Glass_pyrex_ior            = 1.474;

//-----------------------------------------------------------------------------
// Glass_clean_texture(Roughness,Reflectivity)
//
#macro Glass_clean_texture(Roughness,Reflectivity)
    #local _t   = texture {
        pigment { color rgbf 1 }
        finish {
            fresnel Reflectivity
            specular albedo 1.0
            roughness Roughness
            diffuse albedo 0.0
            reflection {
                Reflectivity
            }
            conserve_energy
        }        
    }
    ;
    
    _t
#end

// End Glass_clean_texture
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Glass_interior(IOR,TintColor,TintDensity)
//
#macro Glass_interior(IOR,TintColor,TintDensity)
    #local _i   = interior {
        ior IOR
        #if (TintDensity > 0)
        fade_color TintColor
        fade_distance TintDensity
        fade_power 1002
        #end
        caustics 2.0
    }
    
    _i
#end

// End Glass_interior(IOR,TintColor)
//-----------------------------------------------------------------------------

// End Glass Materials
//=============================================================================

#version Libglass_Inc_Temp;

#end // ifndef (Libglass_Inc_Temp)
