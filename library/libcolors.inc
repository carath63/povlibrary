/******************************************************************************
 * File: libcolors.inc
 * Description:
 *      FILL IN DESCRIPTION HERE
 ******************************************************************************/

#ifndef(Lib_colors_Inc_Temp)

#declare Lib_colors_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libcolors.inc\n"
#end                     

//=============================================================================
// Required Include Files
//
// All files required for the library to work should be included here.  There
// should be no assumption that any given include file is included prior to
// the library.inc file.
//
// Further, to preserve this library as something that can be packaged up to
// export to news.povray.org, only include standard POV-Ray include files or
// other files in this library directory.
//
#include "colors.inc"

// End Required Include Files
//=============================================================================

#declare N_DefaultGamma = 2.1;
    

// Macro RGB256G(<RGB>, Gamma)
// Description:
//      Converts an RGB triple vector in the 0-255 integer space to POV-RAY RGB in the 0.0 - 1.0 space.
//
// Parameters:
//      RGB - rgb triple vector with values in the range 0-255
//      Gamma - gamma value to use in converting
//
#macro RGB256G(RGB, Gamma)
    #local R    = pow(RGB.red/256, Gamma);
    #local G    = pow(RGB.green/256, Gamma);
    #local B    = pow(RGB.blue/256,Gamma);
    <R, G, B>
#end

// Macro RGB256(<RGB>)
// Description:
//      Converts an RGB triple vector in the 0-255 integer space to POV-RAY RGB in the 0.0 - 1.0 space
//      using the default gamm.
//
// Parameters:
//      RGB - rgb triple vector with the values in the range 0-255
//
#macro RGB256(RGB)
    RGB256G(RGB,N_DefaultGamma)
#end

#declare RGB_Black  = RGB256(<0,0,0>);
#declare RGB_White  = RGB256(<255,255,255>);
#declare RGB_Red    = RGB256(<255,0,0>);
#declare RGB_Lime   = RGB256(<0,255,0>);
#declare RGB_Blue   = RGB256(<0,0,255>);
#declare RGB_Yellow = RGB256(<255,255,0>);
#declare RGB_Cyan   = RGB256(<0,255,255>);
#declare RGB_Magenta    = RGB256(<255,0,255>);
#declare RGB_Silver = RGB256(<192,192,192>);
#declare RGB_Gray   = RGB256(<128,128,128>);
#declare RGB_Maroon = RGB256(<128,0,0>);
#declare RGB_Olive  = RGB256(<128,128,0>);
#declare RGB_Green  = RGB256(<0,128,0>);
#declare RGB_Purple = RGB256(<128,0,128>);
#declare RGB_Teal   = RGB256(<0,128,128>);
#declare RGB_Navy   = RGB256(<0,0,128>);

#declare RGB_DarkRed    = RGB256(<139,0,0>);
#declare RGB_Brown      = RGB256(<165,42,42>);
#declare RGB_FireBrick  = RGB256(<178,34,34>);
#declare RGB_Crimson    = RGB256(<220,20,60>);
#declare RGB_Tomato     = RGB256(<255,99,71>);
#declare RGB_Coral      = RGB256(<255,127,80>);
#declare RGB_IndianRed  = RGB256(<205,92,92>);
#declare RGB_LightCoral = RGB256(<240,128,128>);
#declare RGB_DarkSalmon = RGB256(<233,150,122>);
#declare RGB_Salmon     = RGB256(<250,128,114>);
#declare RGB_LightSalmon    = RGB256(<255,160,122>);
#declare RGB_OrangeRed  = RGB256(<255,69,0>);
#declare RGB_DarkOrange = RGB256(<255,140,0>);
#declare RGB_Orange     = RGB256(<255,165,0>);
#declare RGB_Gold       = RGB256(<255,215,0>);
#declare RGB_DarkGoldenrod  = RGB256(<184,134,11>);
#declare RGB_Goldenrod      = RGB256(<218,165,32>);
#declare RGB_PaleGoldrod    = RGB256(<238,232,170>);
#declare RGB_DarkKhaki      = RGB256(<189,183,107>);
#declare RGB_Khaki          = RGB256(<240,230,140>);
#declare RGB_YellowGreen    = RGB256(<154,205,50>);
#declare RGB_DarkOliveGreen = RGB256(<85,107,47>);
#declare RGB_OliveDrab      = RGB256(<107,142,35>);
#declare RGB_LawnGreen      = RGB256(<124,252,0>);
#declare RGB_Chartreuse     = RGB256(<127,255,0>);
#declare RGB_GreenYellow    = RGB256(<173,255,47>);
#declare RGB_DarkGreen      = RGB256(<0,100,0>);
#declare RGB_ForestGreen    = RGB256(<34,139,34>);
#declare RGB_LimeGreen      = RGB256(<50,205,50>);
#declare RGB_LightGreen     = RGB256(<144,238,144>);
#declare RGB_PaleGreen      = RGB256(<152,251,152>);
#declare RGB_DarkSeaGreen   = RGB256(<143,188,143>);
#declare RGB_MediumSpringGreen  = RGB256(<0,250,154>);
#declare RGB_SpringGreen    = RGB256(<0, 255, 127>);
#declare RGB_SeaGreen       = RGB256(<46,139,87>);
#declare RGB_MediumAquamarine   = RGB256(<102,205,170>);
#declare RGB_MediumSeaGreen = RGB256(<60,179,113>);
#declare RGB_LightSeaGreen  = RGB256(<32,178,170>);
#declare RGB_DarkSlateGray  = RGB256(<47,79,79>);
#declare RGB_DarkCyan       = RGB256(<0,139,139>);
#declare RGB_Aqua           = RGB256(<0,255,255>);
#declare RGB_LightCyan      = RGB256(<224,255,255>);
#declare RGB_DarkTurquoise  = RGB256(<0,206,209>);
#declare RGB_Turquoise      = RGB256(<64,224,208>);
#declare RGB_MediumTurquoise    = RGB256(<72,209,204>);
#declare RGB_PaleTurquoise  = RGB256(<175,238,238>);
#declare RGB_Aquamarine     = RGB256(<127,255,212>);
#declare RGB_PowderBlue     = RGB256(<176,224,230>);
#declare RGB_CadetBlue      = RGB256(<95,158,160>);
#declare RGB_SteelBlue      = RGB256(<70,130,180>);
#declare RGB_CornflowerBlue = RGB256(<100,149,237>);
#declare RGB_DeepSkyBlue    = RGB256(<0,191,255>);
#declare RGB_DodgerBlue     = RGB256(<30,144,255>);
#declare RGB_LightSkyBlue   = RGB256(<173,216,230>);
#declare RGB_SkyBlue        = RGB256(<135,206,235>);
#declare RGB_LightSkyBlue   = RGB256(<135,206,250>);
#declare RGB_MidnightBlue   = RGB256(<25,25,112>);
#declare RGB_DarkBlue       = RGB256(<0,0,139>);
#declare RGB_MediumBlue     = RGB256(<0,0,205>);
#declare RGB_RoyalBlue      = RGB256(<65,105,225>);
#declare RGB_BlueViolet     = RGB256(<138,43,226>);
#declare RGB_Indigo         = RGB256(<75,0,130>);
#declare RGB_DarkSlateBlue  = RGB256(<72,61,139>);
#declare RGB_MediumSlateBlue    = RGB256(<123,104,238>);
#declare RGB_MediumPurple   = RGB256(<147,112,219>);
#declare RGB_DarkMagenta    = RGB256(<139,0,139>);
#declare RGB_DarkViolet     = RGB256(<148,0,211>);
#declare RGB_DarkOrchid     = RGB256(<153,50,204>);
#declare RGB_MediumOrchid   = RGB256(<186,85,211>);
#declare RGB_Thistle        = RGB256(<216,191,216>);
#declare RGB_Plum           = RGB256(<221,160,221>);
#declare RGB_Violet         = RGB256(<238,130,238>);
#declare RGB_Orchid         = RGB256(<218,112,214>);
#declare RGB_MediumVioletRed    = RGB256(<199,21,133>);
#declare RGB_PaleVioletRed  = RGB256(<219,112,147>);
#declare RGB_DeepPink       = RGB256(<255,20,147>);
#declare RGB_HotPink        = RGB256(<255,105,180>);
#declare RGB_LightPink      = RGB256(<255,182,193>);
#declare RGB_Pink           = RGB256(<255,192,203>);
#declare RGB_AntiqueWhite   = RGB256(<250,235,215>);
#declare RGB_Beige          = RGB256(<245,245,220>);
#declare RGB_Bisque         = RGB256(<255,228,196>);
#declare RGB_BlanchedAlmond = RGB256(<255,235,205>);
#declare RGB_Wheat          = RGB256(<245,222,179>);
#declare RGB_CornSilk       = RGB256(<255,248,220>);
#declare RGB_LemonChiffon   = RGB256(<255,250,205>);
#declare RGB_LightGoldenrodYellow   = RGB256(<250,250,210>);
#declare RGB_LightYellow    = RGB256(<255,255,224>); 
#declare RGB_SaddleBrown    = RGB256(<139,69,19>);
#declare RGB_Sienna         = RGB256(<160,82,45>);
#declare RGB_Chocolate      = RGB256(<210,105,30>);
#declare RGB_Peru           = RGB256(<205,133,63>);
#declare RGB_SandyBrown     = RGB256(<244,164,96>);
#declare RGB_BurlyWood      = RGB256(<222,184,135>);
#declare RGB_Tan            = RGB256(<210,180,140>);
#declare RGB_RosyBrown      = RGB256(<188,143,143>);
#declare RGB_Moccasin       = RGB256(<255,228,181>);
#declare RGB_NavajoWhite    = RGB256(<255,222,173>);
#declare RGB_PeachPuff      = RGB256(<255,218,185>);
#declare RGB_MistyRose      = RGB256(<255,228,225>);
#declare RGB_LavenderBlush  = RGB256(<255,240,245>);
#declare RGB_Linen          = RGB256(<250,240,230>);
#declare RGB_OldLace        = RGB256(<253,245,230>);
#declare RGB_PapayaWhip     = RGB256(<255,239,213>);
#declare RGB_Seashell       = RGB256(<255,245,238>);
#declare RGB_MintCream      = RGB256(<245,255,250>);
#declare RGB_SlateGray      = RGB256(<112,128,144>);
#declare RGB_LightSlateGray = RGB256(<119,136,153>);
#declare RGB_LightSteelBlue = RGB256(<176,196,222>);
#declare RGB_Lavender       = RGB256(<230,230,250>);
#declare RGB_FloralWhite    = RGB256(<255,250,240>);
#declare RGB_AliceBlue      = RGB256(<240,248,255>);
#declare RGB_GhostWhite     = RGB256(<248,248,255>);
#declare RGB_HoneyDew       = RGB256(<240,255,240>);
#declare RGB_Ivory          = RGB256(<255,255,240>);
#declare RGB_Azure          = RGB256(<240,255,255>);
#declare RGB_Snow           = RGB256(<255,250,250>);
#declare RGB_DimGray        = RGB256(<105,105,105>);
#declare RGB_DarkGray       = RGB256(<169,169,169>);
#declare RGB_LightGray      = RGB256(<211,211,211>);
#declare RGB_Gainsboro      = RGB256(<220,220,220>);
#declare RGB_WhiteSmoke     = RGB256(<245,245,245>);

// Color manipulation macros
//
#macro Fade(Color,Amt)
    #local R    = min(Color.red + Amt, 1.0);
    #local G    = min(Color.green + Amt, 1.0);
    #local B    = min(Color.blue + Amt, 1.0);
    <R,G,B>
#end

#macro Clip(Color)
    #local R    = max(min(Color.red,1.0), 0.0);
    #local G    = max(min(Color.green,1), 0.0);
    #local B    = max(min(Color.blue,1), 0.0);
    <R,G,B>
#end

#declare N_ColorChipSize        = 1.0;
#declare N_ColorChipThickness   = 0.25;
#declare O_ColorChip = box {
    <0,0,0>,
    <N_ColorChipSize,N_ColorChipThickness,N_ColorChipSize>
}

#macro M_ColorChip(P)
    object {
        O_ColorChip
        texture {
            pigment {
                color P
            }
        }
    }            
#end

#macro M_ColorChips(PA,PN)
#local Width    = 0.0;
union {
#for (i, 0, PN-1)
    object { 
        M_ColorChip(PA[i])
        translate <Width,0,0>
    }
    #local Width    = Width + N_ColorChipSize + .05;    
#end
    translate <-Width/2.0,0.0,0.0>
}
#end

#macro M_ColorSlice_RG(R,G,BStart,BEnd,BStep)
#local Width    = 0.0;
#local B        = BStart;
union {
    #while (B <= BEnd)
        #local C = color rgb RGB256(<R,G,B>);
        object {
            M_ColorChip(C)
            translate <Width,0,0>
        }
        #local Width    = Width + N_ColorChipSize + 0.05;
        #local B        = B + BStep;    
    #end
    translate <-Width/2.0,0.0,0.0>
}
#end

#macro M_ColorSlice_RB(R,B,GStart,GEnd,GStep)
#local Width    = 0.0;
#local G        = GStart;
union {
    #while (G <= GEnd)
        #local C = color rgb RGB256(<R,G,B>);
        object {
            M_ColorChip(C)
            translate <Width,0,0>
        }
        #local Width    = Width + N_ColorChipSize + 0.05;
        #local G        = G + GStep;    
    #end
    translate <-Width/2.0,0.0,0.0>
}
#end

#macro M_ColorSlice_GB(G,B,RStart,REnd,RStep)
#local Width    = 0.0;
#local R        = RStart;
union {
    #while (R <= REnd)
        #local C = color rgb RGB256(<R,G,B>);
        object {
            M_ColorChip(C)
            translate <Width,0,0>
        }
        #local Width    = Width + N_ColorChipSize + 0.05;
        #local R        = R + RStep;    
    #end
    translate <-Width/2.0,0.0,0.0>
}
#end 

#declare O_Marker = sphere {
    0, 0.1
    pigment { Black }
    translate <N_ColorChipSize/2.0, N_ColorChipThickness + 0.05, N_ColorChipSize/2.0>
}

#macro M_ColorGrid_R(R,GStart,GEnd,GStep,BStart,BEnd,BStep)
#local MaxWidth = 0.0;
#local Length   = 0.0;
#local G        = GStart;
union {
    object { O_Marker }
    #while (G <= GEnd)
        #local Width    = 0.0;
        #local B        = BStart;
        #while (B <= BEnd)
            #local C    = color rgb RGB256(<R,G,B>);
            object {
                M_ColorChip(C)
                translate <Width,0,Length>
            }    
            #local Width    = Width + N_ColorChipSize + 0.05;
            #local B        = B + BStep;
        #end
        #if (Width > MaxWidth)
            #local MaxWidth = Width;
        #end    
        #local Length   = Length + N_ColorChipSize + 0.05;
        #local G        = G + GStep;
    #end
    translate <-MaxWidth/2.0,0,-Length/2.0>
}
#end

#macro M_ColorGrid_G(G,RStart,REnd,RStep,BStart,BEnd,BStep)
#local MaxWidth = 0.0;
#local Length   = 0.0;
#local R        = RStart;
union {
    object { O_Marker }
    #while (R <= REnd)
        #local Width    = 0.0;
        #local B        = BStart;
        #while (B <= BEnd)
            #local C    = color rgb RGB256(<R,G,B>);
            object {
                M_ColorChip(C)
                translate <Width,0,Length>
            }    
            #local Width    = Width + N_ColorChipSize + 0.05;
            #local B        = B + BStep;
        #end
        #if (Width > MaxWidth)
            #local MaxWidth = Width;
        #end    
        #local Length   = Length + N_ColorChipSize + 0.05;
        #local R        = R + RStep;
    #end
    translate <-MaxWidth/2.0,0,-Length/2.0>
}
#end

#macro M_ColorGrid_B(B,RStart,REnd,RStep,GStart,GEnd,GStep)
#local MaxWidth = 0.0;
#local Length   = 0.0;
#local R        = RStart;
union {
    object { O_Marker }
    #while (R <= REnd)
        #local Width    = 0.0;
        #local G        = GStart;
        #while (G <= GEnd)
            #local C    = color rgb RGB256(<R,G,B>);
            object {
                M_ColorChip(C)
                translate <Width,0,Length>
            }    
            #local Width    = Width + N_ColorChipSize + 0.05;
            #local G        = G + GStep;
        #end
        #if (Width > MaxWidth)
            #local MaxWidth = Width;
        #end    
        #local Length   = Length + N_ColorChipSize + 0.05;
        #local R        = R + RStep;
    #end
    translate <-MaxWidth/2.0,0,-Length/2.0>
}
#end

//=============================================================================
// CIE_Skylight extract
//
// I have pulled a number of macros used in CIE_Skylight.inc from the LightSys IV
// download and copied them here so they can be used without including all of
// the other cruft in CIE_Skylight.inc that I don't need.                      

// ======================================================
// Quick and dirty color conversion macros
//
// No need to do xyz->LCH conversion via CIE.inc because 
// 1) for speed 
// 2) we are only interested in DeltaE (the color difference)
//    so no chromatic adaption is needed.
// The final xyz->RGB conversion is done by CIE.inc so
// all settings from there are used and should be applied
// BEFORE including CIE_Skylight.inc 
// -Ive
// ======================================================

#macro Yxy_to_XYZ (c3Yxy)    
 (<c3Yxy.y/c3Yxy.z*c3Yxy.x, c3Yxy.x, (1-c3Yxy.y-c3Yxy.z)/c3Yxy.z*c3Yxy.x>)
#end   

#declare EK  = 216/24389;
#declare LK  = 16/116;
#declare KK  = 24389/27;
#declare M13 = 1/3;
    
#macro XYZ2LCH(XYZ)
  #if (XYZ.y > EK)
    #local F = pow(XYZ.y,M13);
    #local L = 116*F-16;
  #else
    #local F = 7.787*XYZ.y+LK;
    #local L = KK*XYZ.y;
  #end 
  #local A = 500*((XYZ.x>EK ? pow(XYZ.x,M13) : 7.787*XYZ.x+LK) - F);
  #local B = 200*(F - (XYZ.z>EK ? pow(XYZ.z,M13) : 7.787*XYZ.z+LK));
  #local C = sqrt(A*A+B*B);
  #if (A>0) #local H = atan2(A,B); #else #local H=0;  #end
  <L,C,H>
#end

#macro DeltaE_CMC(col1,col2)
  #local LCH1=XYZ2LCH(col1);
  #local DeltaLCH=XYZ2LCH(col2)-LCH1;
  #local S_C=(0.0638*LCH1.y/(1+0.0131*LCH1.y)+0.638);
  #local S_C_f=sqrt(pow(LCH1.y,4)/(pow(LCH1.y,4)+1900));
  #local S_C_T=( (LCH1.z>radians(164))&(LCH1.z<radians(345)) ? 0.56+abs(0.2*cos(LCH1.z+168)) : 0.36+abs(0.4*cos(LCH1.z+35)) );
  #local S_LCH=<( (LCH1.x<16) ? 0.511 : 0.040975*LCH1.x/(1+0.01765*LCH1.x) ), S_C, S_C*(1+S_C_f*(S_C_T-1))>;
  vlength(DeltaLCH/S_LCH)
#end

// End CIE_Skylight extract
//=============================================================================

//=============================================================================
// Cosine Color Palette Creation
//

//-----------------------------------------------------------------------------
// Cosine_color(C1,C2,C3,C4,T)
//
// Calculates a color on a gradient pallette ranging from C1 - C2 to C1 + C2
// along a cosine ramp controlled by C3 and C4, with C3 controlling the
// frequency, and C4 providing an offset
//
// Original article: https://iquilezles.org/articles/palettes/
// Adapted from code created by kurtz le pirate: https://news.povray.org/povray.binaries.images/attachment/%3C667af29b%241%40news.povray.org%3E/utf-8
// 
#macro Cosine_color(C1,C2,C3,C4,T)
    #local _c   = 
        C1 + C2*<
            cos(2*pi*(T*C3.red+C4.red)),
            cos(2*pi*(T*C3.green+C4.green)),
            cos(2*pi*(T*C3.blue+C4.blue))
        >;
        
    _c        
#end

// End Cosine_Color
//-----------------------------------------------------------------------------


// End Cosine Color Palette Creation
//=============================================================================

#version Lib_colors_Inc_Temp;
#end // ifndef(Lib_colors_Inc_Temp)