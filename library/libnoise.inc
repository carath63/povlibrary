/******************************************************************************
 * File: libnoise.inc
 * Description:
 *      This library contains macros for creating noise functions based on
 * any given internal noise or pattern function that automates the scaling and
 * size of the results of calling that function for a given point in <x,y,z>
 * space.
 ******************************************************************************/

#ifndef(Libnoise_Inc_Temp)

#declare Libnoise_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libnoise.inc\n"
#end

#include "libfn.inc"                    

//=============================================================================
// Noise_size_create(Size)
//
#macro Noise_size_create(Size)
    function(x,y,z) {
        Size
    }    
#end

// End Noise_size_create
//=============================================================================

//=============================================================================
// Noise Generator Functions
//

//-----------------------------------------------------------------------------
// White Noise Functions
//

#macro White_noise_2d(SeedV,SeedN)
    #local _seedv_x = SeedV.x;
    #local _seedv_y = SeedV.y;
    
    function(x,y) {
        f_frac(
            sin(
                f_dot_2d(sin(x),sin(y),_seedv_x,_seedv_y)
            ) * SeedN
        )    
    }
#end

#macro White_noise_3d(SeedV,SeedN)
    #local _seedv_x = SeedV.x;
    #local _seedv_y = SeedV.y;
    #local _seedv_z = SeedV.z;
    
    function(x,y,z) {
        f_frac(
            sin(
                f_dot_3d(sin(x),sin(y),sin(z),_seedv_x,_seedv_y,_seedv_z)
            ) * SeedN
        )    
    }
#end

// End White Noise Functions
//-----------------------------------------------------------------------------


// End Noise Generator Functions
//=============================================================================

//=============================================================================
// Noise_create(SourceOrigin,NoiseFn,NoiseOrigin,NoiseScale,NoiseSizeFn)
//
// Creates a function(x,y,z) that selects a noise value using the given noise
// function by scaling and translating the input point and multiplying by the
// given size function(x,y,z)
//
#macro Noise_create(SourceOrigin,NoiseFn,NoiseOrigin,NoiseScale,NoiseSizeFn)
    #local _scale   = <1,1,1>*NoiseScale;
    #local _xScale  = _scale.x;
    #local _yScale  = _scale.y;
    #local _zScale  = _scale.z;
    #local _xTrans  = -_xScale*SourceOrigin.x + NoiseOrigin.x;
    #local _yTrans  = -_yScale*SourceOrigin.y + NoiseOrigin.y;
    #local _zTrans  = -_zScale*SourceOrigin.z + NoiseOrigin.z;
    
    function(x,y,z) {
        NoiseFn(_xScale*x+_xTrans, _yScale*y+_yTrans, _zScale*z+_zTrans)*NoiseSizeFn(x,y,z)
    }
#end

#macro Noise_create2(SourceOrigin,NoiseFn,NoiseOrigin,NoiseScale,NoiseSize)
    #local _scale   = <1,1,1>*NoiseScale;
    #local _xScale  = _scale.x;
    #local _yScale  = _scale.y;
    #local _zScale  = _scale.z;
    #local _xTrans  = -_xScale*SourceOrigin.x + NoiseOrigin.x;
    #local _yTrans  = -_yScale*SourceOrigin.y + NoiseOrigin.y;
    #local _zTrans  = -_zScale*SourceOrigin.z + NoiseOrigin.z;
    
    function(x,y,z) {
        NoiseFn(_xScale*x+_xTrans, _yScale*y+_yTrans, _zScale*z+_zTrans)*NoiseSize
    }
#end

// End Noise_create
//=============================================================================

//=============================================================================
// Noise_turbulate(NoiseFn,Turbulence)
//
#macro Noise_turbulate(NoiseFn,Turbulence)
    #local _turbulence  = <1,1,1>*Turbulence;
    #local _turbx   = _turbulence.x;
    #local _turby   = _turbulence.y;
    #local _turbz   = _turbulence.z;
    
    function(x,y,z) {
        NoiseFn(x+f_snoise3d(x,y,z)*_turbx, y+f_snoise3d(x,y,z)*_turby, z+f_snoise3d(x,y,z)*_turbz)
    }
#end

#macro Noise_turbulate2(NoiseFn,Turbulence,Scale)
    #local _turbulence  = <1,1,1>*Turbulence;
    #local _turbx   = _turbulence.x;
    #local _turby   = _turbulence.y;
    #local _turbz   = _turbulence.z;
    
    #local _scale   = <1,1,1>*Scale;
    #local _scalex  = _scale.x;
    #local _scaley  = _scale.y;
    #local _scalez  = _scale.z;
    
    function(x,y,z) {
        NoiseFn(x+f_snoise3d(0,y*_scaley,z*_scalez)*_turbx, y+f_snoise3d(x*_scalex,0,z*_scalez)*_turby, z+f_snoise3d(x*_scalex,y*_scaley,0)*_turbz)
    }
#end

// End Noise_turbulate
//=============================================================================

//=============================================================================
// Blurred_texture(Texture,BlurAmount,BlurScale,BlurSamples,RStream)
//
#macro Blurred_texture(Texture,BlurAmount,BlurScale,BlurSamples,RStream)
    #local _t   = texture {
        average
        texture_map {
            #for (i, 1, BlurSamples, 1)
                #local _t_blurred   = texture {
                    Texture
                    normal {
                        bumps BlurAmount
                        translate BlurScale*<rand(RStream),rand(RStream),rand(RStream)>
                        scale BlurScale
                    }    
                }
                [1 _t_blurred]
            #end
        }
    }
    ;
    
    _t
#end

// End Blurred_texture
//=============================================================================

#version Libnoise_Inc_Temp;
#end // ifndef(Libnoise_Inc_Temp)