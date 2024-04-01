/******************************************************************************
 * File: libscale.inc
 * Description:
 *      Macros for converting from one measurement system to another and setting
 * a standard scale to map to the POV standard unit.
 *
 ******************************************************************************/

#ifndef(Libscale_Inc_Temp)

#declare Libscale_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libscale.inc\n"
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

// End Required Include Files
//=============================================================================

//=============================================================================
// Scale conversion factors
//
// The basic, default, scale is 1 unit = 1 cm.  Other scales are provided in
// terms of the basic scale.
//
#declare SCALE_CM   = 1.0;
#declare SCALE_DM   = 0.1 * SCALE_CM;
#declare SCALE_MM   = 10 * SCALE_CM;
#declare SCALE_UM   = 1000 * SCALE_MM;
#declare SCALE_M    = .01 * SCALE_CM;
#declare SCALE_KM   = .001 * SCALE_M;

#declare SCALE_INCH = SCALE_CM/2.54;
#declare SCALE_FEET = SCALE_INCH/12;
#declare SCALE_YARD = SCALE_FEET/3;
#declare SCALE_MILE = SCALE_FEET/5280;

#ifndef (Math_Default_Scale)
#declare Math_Default_Scale = SCALE_CM;
#end

//-----------------------------------------------------------------------------
// Math_Convert(ScaleFrom,ScaleTo,Units)
//
// Converts Units in the given ScaleFrom to units in the given ScaleTo.
//
#macro Math_Convert(ScaleFrom,ScaleTo,Units)
(Units*(ScaleTo/ScaleFrom))
#end

// End Math_Convert
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Math_Scale(Scale,Units)
//
// Converts Units in the given Scale to units in the default scale as specified
// by the Math_Default_Scale.  For example, if the default scale is SCALE_CM
// and the supplied scale is SCALE_M, Units is multiplied by 100.
//
#macro Math_Scale(Scale,Units)
Math_Convert(Scale,Math_Default_Scale,Units)
#end

// End Math_Scale
//-----------------------------------------------------------------------------

// End Scale conversion factors
//=============================================================================

//=============================================================================
// Visual Acuity
//
#declare Visual_acuity_angle_degrees    = 0.008;
#declare Visual_acuity_angle_radians    = radians(Visual_acuity_angle_degrees);

#macro Visual_acuity(Distance)
    (Visual_acuity_angle_radians*Distance)
#end

#declare f_visual_acuity    = function(d) {
    (Visual_acuity_angle_radians*d)
}

// End Visual Acuity
//=============================================================================

#version Libscale_Inc_Temp;
#end // ifndef(Libscale_Inc_Temp)