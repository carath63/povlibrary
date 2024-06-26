/******************************************************************************
 * File: libsun.inc
 * Description:
 *      This library contains a number of macros for computing where the sun
 * should be located based on lat/long and date/time on Earth.
 *
 ******************************************************************************/

#ifndef(Lib_sun_Inc_Temp)

#declare Lib_sun_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libsun.inc\n"
#end                     

#include "liblights.inc"
#include "libtime.inc"
#include "libskydome.inc"

//=============================================================================
// Sun-related constants
//

#declare N_Sun_Zenith           = 96.0; // civil sun zenith angle
#declare N_Sun_Angular_Diameter = 31/60 + 27/3600;

// End Sun-related constants
//=============================================================================

//=============================================================================
// Sun_axis(DateTime,LAT,LONG)
//
// Computes the axis of rotation for the sun on the given DateTime as viewed
// from the given LAT/LONG coordinates with the +z axis facing north
// and returns it as a 3-vector with the altitude expressed as the x-rotation
// and the azimuth expressed as the y-rotation
//
// The calculations are copied and edited from sunpos.inc
//
#macro Sun_axis(DateTime,LAT,LONG)
    #local utc  = DateTime_utc(DateTime);
    
    // Amount of days to, or from, the year 2000
    #local d    = DateTime_y2k_days(DateTime);
    
    // Longitude of perihelion (w), eccentricity (e)
    #local w    = 282.9404+4.70935E-5*d;
    #local e    = 0.016709-1.151E-9*d;
    
    // Mean anomaly (M), sun's mean longitude (L)
    #local M    = 356.0470+0.9856002585*d;
    #if (0<M<360)
    #local M    = M-floor(M/360)*360;
    #end
    #local L    = w+M;
    #if (0<L<360)
    #local L    = L-floor(L/360)*360;
    #end
    
    // Obliquity of the ecliptic, eccentric anomaly (E)
    #local oblecl   = 23.4393-3.563E-7*d;
    #local E        = M+(180/pi)*e*sin(radians(M))*(1+e*cos(radians(M)));
    
    // Sun's rectangular coordinates in the plane of ecliptic (A,B)
    #local A    = cos(radians(E))-e;
    #local B    = sin(radians(E))*sqrt(1-e*e);
    
    // Distance (r), true anomaly (V), longitude of the sun (lon)
    #local r    = sqrt(A*A+B*B);
    #local V    = degrees(atan2(radians(B),radians(A)));
    #local lon  = V+w;
    #if (0<lon<360)
    #local lon  = lon-floor(lon/360)*360;
    #end
    
    // Calculate declination (Decl) and right ascension (RA)
    #local Decl = degrees(asin(sin(radians(oblecl))*sin(radians(lon))));
    #local RA   = degrees(atan2(sin(radians(lon))*cos(radians(oblecl)),cos(radians(lon))))/15;
    
    // Greenwich meridian siderial time at 00:00 (GMST0),siderial time (SIDTIME), hour angle (HA)
    #local UT       = DateTime_hourofday(utc);
    #local GMST0    = L/15+12;
    #local SIDTIME  = GMST0+UT+LONG/15;
    #local HA       = (SIDTIME-RA)*15;
    
    // This is what we're looking for: Altitude & Azimuth
    #local Al       = degrees(asin(sin(radians(LAT))*sin(radians(Decl))+cos(radians(LAT))*cos(radians(Decl))*cos(radians(HA))));
    #local Az       = degrees(atan2(sin(radians(HA)),cos(radians(HA))*sin(radians(LAT))-tan(radians(Decl))*cos(radians(LAT))))+180;
    #local _result  = <-Al,Az,0>;
_result  
#end

// End Sun_axis
//=============================================================================

//=============================================================================
// Sun_position(DateTime,LAT,LONG,Distance)
//
// Computes the position of the center of a sun-based object/light_source
// with an apparent distance of Distance on the given DateTime as viewed from
// the given LAT/LONG coordinates with the +z axis facing north and returns
// it as an xyz coordinate vector.
//
#macro Sun_position(DateTime,LAT,LONG,Distance)
    #local _axis    = Sun_axis(DateTime,LAT,LONG);
    #local _result  = vrotate(<0, 0, Distance>, _axis);
_result
#end

// End Sun_position
//=============================================================================

//=============================================================================
// Sun_rise(DateTime,LAT,LONG)
// Sun_set(DateTime,LAT,LONG)
//
// Computes the sunrise/sunset times for the given DateTime as viewed from the
// given LAT/LONG coordinates.
//

//-----------------------------------------------------------------------------
// _Sun_long_hour(LONG)
//
// Computes the hour for the given longitude.
//                                                                             
#macro _Sun_long_hour(LONG)
    #local _result  = LONG/15;
_result;
#end

// End _Sun_long_hour
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// _Sun_mean_anomaly(DayOfYear)
//
// Computes the mean anomaly for the given DayOfYear with fractional days to
// the right of the decimal point.
//
#macro _Sun_mean_anomaly(DayOfYear)
    #local _result  = (0.9856 * DayOfYear) - 3.289;
_result    
#end

// End _Sun_mean_anomaly
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// _Sun_true_longitude(M)
//
#macro _Sun_true_longitude(M)
    #local _result  = M + (1.916 * sind(M)) + (0.020 * sin(2*M)) + 282.634;
    #local _result  = Trig_norm_degrees(_result);
_result    
#end   

// End _Sun_true_longitude
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// _Sun_right_ascension(L)
//
#macro _Sun_right_ascension(L)                                                
    #local RA       = atand(0.91764 * tand(L));
    #local RA       = Trig_norm_degrees(RA);
    #local LQ       = (floor(L/90))*90;
    #local RAQ      = (floor(RA/90))*90;
    #local RA       = RA + (LQ - RAQ);
    #local RA       = RA / 15;
RA    
#end

// End _Sun_right_ascension
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// _Sun_sin_declination(L)
//
#macro _Sun_sin_declination(L)
    #local sinDec = 0.39782 * sind(L);
sinDec    
#end

// End _Sun_sin_declination
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// _Sun_cos_declination(sinDec)
//
#macro _Sun_cos_declination(sinDec)
    #local cosDec   = cosd(asind(sinDec));
cosDec    
#end
// End _Sun_cos_declination
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// _Sun_local_hour_angle(sinDec,cosDec,LAT)
//
#macro _Sun_local_hour_angle(sinDec,cosDec,LAT)
    #local cosH = (cosd(N_Sun_Zenith) - (sinDec * sind(LAT))) / (cosDec * cosd(LAT));
cosH    
#end                                                                                
                                                                                
// End _Sun_local_hour_angle
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// _Sun_local_mean_time(Hour,RA,ApproxTime,LNGHour,LocalOffset)
//
#macro _Sun_local_mean_time(Hour,RA,ApproxTime,LNGHour,LocalOffset)
    #local T    = Hour + RA - (0.06571 * ApproxTime) - 6.622;
    #local T    = T - LNGHour + LocalOffset;
T    
#end                                                                               
// End _Sun_local_mean_time
//-----------------------------------------------------------------------------
                                                                               
#macro Sun_rise(DateTime,LAT,LNG)
    #local N            = DateTime_dayofyear(DateTime);
    #local lngHour      = _Sun_long_hour(LNG); 
    #local LocalOffset  = DateTime[DateTime_TZ];
    #local ta           = N + ((6 - lngHour)/24);   // approximate time for sunrise
    #local M            = _Sun_mean_anomaly(ta);
    #local L            = _Sun_true_longitude(M);
    #local RA           = _Sun_right_ascension(L);
    #local sinDec       = _Sun_sin_declination(L);
    #local cosDec       = _Sun_cos_declination(sinDec);
    #local cosH         = _Sun_local_hour_angle(sinDec,cosDec,LAT); 
    #if (cosH > 1)
        #warning concat("M_Sunrise: no sunrise on ",str(MM,0,0),"/",str(DD,0,0),"/",str(YYYY,0,0), " at <", str(LAT,0,3), ",", str(LNG,0,3),"> = ", str(cosH,0,3), "\n")
    #end
    #local H        = (360 - acosd(cosH))/15;
    #local T        = _Sun_local_mean_time(H,RA,ta,lngHour,LocalOffset);
    #local T        = HourOfDay_normalize(T);
T    
#end

#macro Sun_set(DateTime,LAT,LNG)
    #local N            = DateTime_dayofyear(DateTime);
    #local lngHour      = _Sun_long_hour(LNG); 
    #local LocalOffset  = DateTime[DateTime_TZ];
    #local ta           = N + ((18 - lngHour)/24);   // approximate time for sunset
    #local M            = _Sun_mean_anomaly(ta);
    #local L            = _Sun_true_longitude(M);
    #local RA           = _Sun_right_ascension(L);
    #local sinDec       = _Sun_sin_declination(L);
    #local cosDec       = _Sun_cos_declination(sinDec);
    #local cosH         = _Sun_local_hour_angle(sinDec,cosDec,LAT); 
    #if (cosH < -1)
        #warning concat("M_Sunset: no sunset on ",str(MM,0,0),"/",str(DD,0,0),"/",str(YYYY,0,0), " at <", str(LAT,0,3), ",", str(LNG,0,3),">\n")
    #end
    #local H        = acosd(cosH)/15;
    #local T        = _Sun_local_mean_time(H,RA,ta,lngHour,LocalOffset);
    #local T        = HourOfDay_normalize(T);
T    
#end

// End Sun_rise/Sun_set
//=============================================================================

//=============================================================================
// Sun_object(SkyConfig,DateTime,LAT,LNG,Distance,Angle)
//
// Creates an object that looks like the sun and can be used in a "looks_like"
// clause for a light source representing the sun.
//
#macro Sun_object(SkyConfig,DateTime,LAT,LNG,Distance,Angle)
    #local _sun_radius  = tan(radians(N_Sun_Angular_Diameter/2))*Distance;
    #local _sun_pos     = Sun_position(DateTime,LAT,LNG,Distance);
    #local _sun_color   = Sky_sun_color(SkyConfig,DateTime,LAT,LNG);
    #local _scale       = _sun_radius * (180/Angle);
    sphere {
        <0,0,0>,
        1*_scale
        hollow
        no_shadow
        texture {
            pigment { rgbt 0.99 }
        }
        interior {
            media {
                emission 1
                density {
                    spherical
                    color_map {
                        [0.00 rgb 0]
                        [0.10 rgb _sun_color * <0.9,0.5,0.5>]
                        [0.25 rgb _sun_color * 1]
                        [1.00 rgb _sun_color * 1.1]
                    }
                    turbulence 1.1
                    omega 0.75
                    lambda 8
                    scale _scale
                }    
            }
        }
        translate _sun_pos    
    }
#end

// End Sun_object
//=============================================================================

//=============================================================================
// Sun_light(SkyConfig,DateTime,LAT,LNG,Distance,Angle)
//
// Create a light source that looks like the sun
//
#macro Sun_light(SkyConfig,DateTime,LAT,LNG,Distance,Angle)
    #local _sun_radius  = tan(radians(N_Sun_Angular_Diameter/2))*Distance;
    #local _sun_pos     = Sun_position(DateTime,LAT,LNG,Distance);
    #local _sun_axis    = Sun_axis(DateTime,LAT,LNG);
    #local _sun_color   = Sky_sun_color(SkyConfig,DateTime,LAT,LNG);
    #local _scale       = _sun_radius * (180/Angle);
    #local _lc          = Light_Color(_sun_color, SkyConfig[SkyConfig_sun_light_intensity]);

    #debug concat("Sun_light: _sun_color=<", vstr(3, _sun_color, ",", 0, 3), ">\n")
    #debug concat("Sun_light: _sun_pos=<", vstr(3, _sun_pos, ",", 0, 3), ">\n") 
    light_source {
        <0,0,0>
        #ifdef (Use_Simple_Color)
        color rgb _sun_color     
        #else
            #local _sun_arc_radius  = 2*pi*Distance/288;    // was a 5 degree arc radius, reducing it to 2.5, now trying 1.25
        //#declare Lightsys_Scene_Scale=1000;
        _lc
            #if (SkyConfig[SkyConfig_sun_parallel_light])
            parallel
            point_at <0, 0, -Distance>
            #end
            #if (SkyConfig[SkyConfig_sun_area_light] > 1)
            area_light x*_sun_arc_radius,y*_sun_arc_radius,SkyConfig[SkyConfig_sun_area_light],SkyConfig[SkyConfig_sun_area_light]
            jitter
            adaptive 0
            orient circular
            #end
        #end
        looks_like {
            sphere {
                <0,0,0>,
                1.5*_scale
                hollow
                no_shadow
                texture {
                    pigment { rgbt 1 }
                }
                interior {
                    media {
                        emission 5*_lc/_scale
                        density {
                            spherical
                            color_map {
                                [0.00 rgb 0]
                                [0.05 rgb _sun_color * <0.9,0.5,0.5>]
                                [0.20 rgb _sun_color * 1]
                                [1.00 rgb _sun_color * 1.1]
                            }
                            #local _wscale  = 30;
                            scale _wscale
                            warp {
                                turbulence 4//0.05
                                omega 0.75
                                lambda 8
                            }    
                            scale _scale/_wscale
                        }    
                    }
                }
            } 
        }    
        //translate _sun_pos    
        translate <0, 0, Distance>
        rotate _sun_axis
    }    
#end

// End Sun_light
//=============================================================================

#version Lib_sun_Inc_Temp;
#end // ifndef(Lib_sun_Inc_Temp)