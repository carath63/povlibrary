/******************************************************************************
 * File: sky.inc
 * Description:
 *      Sky model
 ******************************************************************************/

#ifndef(Sky_Inc_Temp)

#declare Sky_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including sky.inc\n"
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
#include "libscale.inc"
#include "libskydome.inc"
#include "libnoise.inc"
#include "liblights.inc"
#include "libtime.inc"

CIE_ColorSystemWhitepoint(sRGB_ColSys,Illuminant_D55)

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
// Fluffy_cloud(NSpheres,Scale,NSeed)
//

//-----------------------------------------------------------------------------
// Fluffy_cloud_shape(NSpheres,SPR,NSeed)
//
#macro Fluffy_cloud_shape(NSpheres,SPR,NSeed)
    #local _shape   = union {
        #for (i, 1, NSpheres, 1)
            #local _cr  = SPR*(0.01+0.19*rand(NSeed));
            sphere {
                <0, 0, 0>, _cr
                translate -0.5*rand(NSeed)*z
                rotate 180*rand(NSeed)*x
                rotate 360*rand(NSeed)*y
            }
        #end
    }
    
    _shape
#end

// End Fluffy_cloud_shape
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Fluffy_cloud_density(CloudShape,NSeed)
//
#macro Fluffy_cloud_density(CloudShape,NSeed)
    #local _density = density{
        pigment_pattern{
            object{CloudShape color rgb 0 color rgb 1}
            #local _s   = 1;
            scale _s warp{turbulence 0.4 lambda 0.6 omega 0.2} scale 1/_s
            rotate 360*rand(NSeed)
            #local _s   = 8;
            scale _s warp{turbulence 0.2 lambda 1.2 omega 0.4} scale 1/_s
            rotate 360*rand(NSeed)
            #local _s   = 50;
            scale _s warp { turbulence 0.8 lambda 4 omega 0.5 } scale 1/_s
        }
    }
    
    _density
#end

// End Fluffy_cloud_density
//-----------------------------------------------------------------------------

#macro Fluffy_cloud(NSpheres,Scale,SPR,NSeed)
    #local _cloud_shape     = object { Fluffy_cloud_shape(NSpheres,SPR,NSeed) }
    #local _cloud_density   = density { Fluffy_cloud_density(_cloud_shape,NSeed) }
    #local _scale           = vlength(<1,1,1>*Scale);
    
    #local _cloud   = sphere {
        <0, 0, 0>, 1
        hollow
        texture {
            pigment {
                color rgbt 1
            }
            finish {
                specular 0
                diffuse 0
            }
        }
        interior {
            media {
                absorption 0.33/_scale
                density {
                    _cloud_density
                }    
            }    
            media {
                scattering { 1, 1/_scale }
                density {
                    _cloud_density
                }
            }        
        }
        scale Scale
    }
    
    _cloud                
#end

// End Fluffy_cloud
//=============================================================================

//=============================================================================
// Cumulus_cloud(Nodes,Density,Scale,NSeed)
//

//-----------------------------------------------------------------------------
// Cumulus_node(Density,Scale,NSeed)
//
#macro Cumulus_node(Density,Scale,NSeed)
    #local _turb    = 0.6*(1 + 0.25*srand(NSeed));
    #local _lambda  = 2 + srand(NSeed);
    
    #local _node    = sphere {
        <0,0,0>, 1
        hollow
        material {
            texture {
                pigment { color rgbt 1 }
            }
            interior {
                media {
                    scattering { 2, Density/Scale }
                    absorption 0.2*Density/Scale
                    density {
                        spherical
                        color_map {
                            [0.0 rgb 0]
                            [1.0 rgb 1]
                        }
                        warp {
                            turbulence _turb
                            lambda _lambda
                        }
                        scale 0.85    
                    }    
                }
            }    
        } 
        scale Scale
    }
    
    _node
#end

// End Cumulus_node
//-----------------------------------------------------------------------------

#macro Cumulus_cloud_old(Nodes,Density,Scale,NSeed)
    #local _cloud   = union {
        object { 
            Cumulus_node(Density,Scale,NSeed)
        }    
        #for (i, 2, Nodes, 1)
            #local _ndist   = Scale*(2 - 0.1*rand(NSeed));
            #local _nrot    = <-180*rand(NSeed),180*srand(NSeed),0>;
            object {
                Cumulus_node(Density*(1+0.2*srand(NSeed)),Scale*(1+0.2*srand(NSeed)),NSeed)
                translate <0, 0, _ndist>
                rotate _nrot
            }    
        #end
    }
    
    _cloud
#end

#macro Cumulus_cloud(Nodes,Density,Scale,NSeed)
    #local _cloud   = object { Cumulus_node(Density,Scale,NSeed) }
    #local _tdist   = 5*Nodes*Scale;
    
    #for (i, 2, Nodes,1)
        #local _size    = max_extent(_cloud) - min_extent(_cloud);
        #debug concat("_size=<", vstr(3, _size, ",", 0, 3), ">\n")
        #local _scale   = Scale*(1 + 0.25*srand(NSeed));
        #local _density = Density*(1 + 0.2*srand(NSeed));
        #local _nangle  = <-180*rand(NSeed), 180*srand(NSeed), 0>;
        #local _from    = vrotate(<0, 0, _tdist>, _nangle);
        #local _to      = <0, 0, 0>;
        #local _dir     = _to - _from;
        #local _nrm     = <0, 0, 0>;
        #local _isect   = trace(_cloud, _from, _dir, _nrm);
        #debug concat("_isect=<", vstr(3, _isect, ",", 0, 3), ">\n")
        #local _nrot    = <180*srand(NSeed),180*srand(NSeed),0>;
        #if (vlength(_nrm) > 0)
            #local _overlap = _scale*(0.5 + 0.5*rand(NSeed));
            #local _loc     = vnormalize(_isect) * (vlength(_isect) + _overlap);
            #local _dist    = vlength(_loc);
        #else
            #local _dist    = Scale*(2 + 0.1*srand(NSeed));
            #local _loc     = vrotate(<0, 0, _dist>, _nangle);
        #end 
        #debug concat("_loc=<", vstr(3, _loc, ",", 0, 3), ">\n")
        #local _cloud   = union {
            object { _cloud }
            object {
                Cumulus_node(_density,_scale,NSeed)
                rotate _nrot
                translate _loc
            }
        }        
    #end
    
    _cloud
#end

// End Cumulus_cloud
//=============================================================================

//=============================================================================
// Sky(NSeed)
//
#macro Sky(SunDate,NSeed)
    #ifndef(Sky_quality)
    #declare Sky_quality    = 2;
    #end
    #local _sky_config                                   = SkyConfig_create(); 
    #local _sky_config[SkyConfig_sun_light_intensity]    = 6;
    #local _sky_config[SkyConfig_sun_area_light]         = true;
    #local _sky_config[SkyConfig_turbidity]              = SkyConfig_Turbidity_Clear;
    #local _sky_config[SkyConfig_sun_parallel_light]     = true;
    #local _sky_config[SkyConfig_min_vertices]           = 14400;    // With a large sky dome, this removes the visible triangles
    
    #local _sun_dist    = Math_Scale(SCALE_M, 10000); 
    /* Edinburgh Scotland */
    #local _lat         = 55.9533;
    #local _lng         = -3.18;

    #local _sky = union {
        object {
            Sky_dome(_sky_config,SunDate,_sun_dist+2,_lat,_lng)
            translate <0, -1, 0>
        }
        object { Sun_light(_sky_config,SunDate,_lat,_lng,_sun_dist,45) }
        
        #if (Sky_quality > 0)
        #local _node_scale  = Math_Scale(SCALE_M, 250);
        #local _node_density    = 0.3;
        object {
            object { Cumulus_cloud(20,_node_density,_node_scale,NSeed) }
            translate <0, Math_Scale(SCALE_M, 1000), Math_Scale(SCALE_M, 1000)>
        }
        object {
            object { Cumulus_cloud(15,_node_density,_node_scale,NSeed) }
            translate <Math_Scale(SCALE_M, 600), Math_Scale(SCALE_M, 1500), Math_Scale(SCALE_M, 1500)>
        }                    
        #end
    }
    
    _sky
#end

// End Sky
//=============================================================================

#version Sky_Inc_Temp;

#end // ifndef (Sky_Inc_Temp)
