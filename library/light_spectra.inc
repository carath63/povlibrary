/******************************************************************************
 * File: light_spectra.inc
 * Description:
 *      Light spectrum splines and lumens values found in various places
 ******************************************************************************/

#ifndef(Light_spectra_Inc_Temp)

#declare Light_spectra_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including light_spectra.inc\n"
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
// Device: Cree XLamp XM-L2 Family
// Source: https://www.digikey.com/en/articles/defining-the-color-characteristics-of-white-leds
// Info:
//      Produce 310 lm at 700 mA
#declare Cree_xlamp_xml_l2_5000k_8000k_spectrum = spline { natural_spline
350, 0.0
380, 0.0
430, 0.4
445, 1.0
480, 0.075
500, 0.2
530, 0.38
540, 0.4
580, 0.35
630, 0.18
680, 0.05
730, 0.01
780, 0
800, 0.0
}
;

#declare Cree_xlamp_xml_l2_3700k_5000k_spectrum = spline { natural_spline
350, 0.0
380, 0.0
430, 0.4
445, 1.0
480, 0.095
500, 0.2
530, 0.475
555, 0.5
580, 0.5
630, 0.3
680, 0.1
730, 0.01
780, 0
800, 0.0
}
;

#declare Cree_xlamp_xml_l2_2600k_3700k_spectrum = spline { natural_spline
350, 0.0
380, 0.0
430, 0.1
445, 0.35
480, 0.1
500, 0.4
530, 0.5
540, 0.6
580, 0.9
605, 1.0
630, 0.85
680, 0.3
730, 0.05
780, 0
800, 0.0
}
;

#declare Cree_xlamp_xml_l2_lumens   = 310;

// End Device: Cree XLamp XM-L2 Family
//=============================================================================

#version Light_spectra_Inc_Temp;

#end // ifndef (Light_spectra_Inc_Temp)
