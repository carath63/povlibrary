/******************************************************************************
 * File: libwood.inc
 * Description:
 *      Utility functions for generating wood grain pigments and colormaps.
 *
 *      Individual wood grain instances are found in the "woodgrains" folder
 *      and include the image files used to generate colormaps and .inc files
 *      with the grain patterns.
 ******************************************************************************/

#ifndef(Libwood_Inc_Temp)

#declare Libwood_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libwood.inc\n"
#end                     

//=============================================================================
// Includes
//

//-----------------------------------------------------------------------------
// Standard Includes
//
#include "rand.inc"

// End Standard Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// My Includes
//
#include "libcmap.inc"
#include "libgeom.inc"

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
// Wood Grain Randomization
//

//-----------------------------------------------------------------------------
// Wood_grain_cut(GrainPigment,TrunkDiameter,TrunkLength,BoardSize,RStream)
//
#macro Wood_grain_cut(GrainPigment,TrunkDiameter,TrunkLength,BoardSize,RStream)
    // Determine the Y offset for the cross cut
    //
    #local _max_planks  = floor(0.5*TrunkDiameter/BoardSize.y);
    #local _plank       = int(_max_planks*rand(RStream));
    #local _max_width   = RadiusHeight2Chord(0.5*TrunkDiameter,(_plank+1)*BoardSize.y);
    #while (_plank > 0 & _max_width < BoardSize.x)
        #local _plank       = _plank - 1;
        #local _max_width   = RadiusHeight2Chord(0.5*TrunkDiameter,(_plank+1)*BoardSize.y); // Something wrong here...
    #end
    #local _ty  = _plank*BoardSize.y;
    
    #if (_max_width > BoardSize.x)
        #local _tx  = SRand(RStream)*(_max_width - BoardSize.x);
    #else
        #local _tx  = 0;
    #end
    
    #if (TrunkLength > BoardSize.z)
        #local _tz  = SRand(RStream)*(TrunkLength - BoardSize.z);
    #else
        #local _tz  = 0;
    #end                
    
    #local _cut = pigment {
        GrainPigment
        translate <_tx, _ty, _tz>
        #if (rand(RStream) <= 0.5)
            rotate <0, 0, 180>
        #end    
    }
    
    _cut    
#end

// End Wood_grain_cut
//-----------------------------------------------------------------------------


// End Wood Grain Randomization
//=============================================================================

//=============================================================================
// Wood Color Map Generation
//

//-----------------------------------------------------------------------------
// Wood_image_type(ImageFile)
//
#macro Wood_image_type(ImageFile)
    #local _len     = strlen(ImageFile);
    #local _cur     = _len;
    #local _dotpos  = -1;
    #while (_dotpos < 0 & _cur > 0)
        #if (substr(ImageFile,_cur,1) = ".")
            #local _dotpos  = _cur;
        #else
            #local _cur = _cur - 1;    
        #end
    #end
    
    #if (_dotpos < 0 | _dotpos >= _len)
        jpeg
    #else
        #local _ext = strlwr(substr(ImageFile,_dotpos+1,(_len - _dotpos)));
        //#debug concat("_ext=", _ext, "\n")
        #switch(true)
            #case(_ext = "gif")
                gif
            #break
            #case(_ext = "tga")
                tga
            #break
            #case(_ext = "iff")
                iff
            #break
            #case(_ext = "ppm")
                ppm
            #break
            #case(_ext = "pgm")
                pgm
            #break                                
            #case(_ext = "png")
                png
            #break
            #case(_ext = "jpg" | _ext = "jpeg")
                jpeg
            #break
            #case(_ext = "tiff")
                tiff
            #break
            #else
                jpeg
            #break    
        #end
    #end
            
#end

// End Wood_image_type
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Wood_image_map_create(FileName)
//
// Creates an image_map{} from the given FileName, parsing the extension to
// determine the bitmap type and using standard once and interpolation attributes
//
#macro Wood_image_map_create(FileName)
    #local _im  = pigment {
        image_map {
            Wood_image_type(FileName) FileName
            once
            interpolate 2    
        }
        rotate <90, 0, 0>
    }    
    
    _im
#end 
// End Wood_image_map_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Wood_cmap_create(FileName,Start,End,Entries)
//
#macro Wood_cmap_create(FileName,Start,End,Entries)
    #local _wood_pigment    = pigment {
        Wood_image_map_create(FileName)
    }
    
    #local _cmap    = Cmap_create_from_pigment(_wood_pigment,Color_type_rgb,<Start,0,0.5>,<End,0,0.5>,Entries)
    
    _cmap        
#end

// End Wood_cmap_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Wood_color_map_fade(Cmap,FadeColor,FadeWeight)
//
// Fades the entire color map by adding the FadeColor*FadeWeight to each color
// in the color map, and then normalizing by FadeWeight+1.
//
#macro Wood_color_map_fade(Cmap,FadeColor,FadeWeight)
    #for (i, 0, Cmap.num_entries-1, 1)
        #local Cmap.entries[i].value    = (Cmap.entries[i].value + FadeColor*FadeWeight)/(FadeWeight+1);
    #end
#end

// End Wood_color_map_fade
//-----------------------------------------------------------------------------

// End Wood Color Map Generation
//=============================================================================

//=============================================================================
// Wood Pigment Generation
//

//-----------------------------------------------------------------------------
// Wood_pigment_create(Grain,Cmap)
//
#macro Wood_pigment_create(Grain,Cmap)
    #local _p   = pigment {
        Grain
        color_map { Cmap_generate(Cmap) }
    }
    
    _p    
#end 

// End Wood_pigment_create
//-----------------------------------------------------------------------------

// End Wood Pigment Generation
//=============================================================================

#version Libwood_Inc_Temp;

#end // ifndef (Libwood_Inc_Temp)
