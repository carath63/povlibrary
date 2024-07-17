/******************************************************************************
 * File: libtime.inc
 * Description:
 *      This library contains constants and macros for creating a structure
 * that represents a date/time and operations on that structure.
 ******************************************************************************/

#ifndef(Lib_time_Inc_Temp)

#declare Lib_time_Inc_Temp = version;
#version 3.8;

#ifdef(View_POV_Include_Stack)
    #debug "including libtime.inc\n"
#end                     

//=============================================================================
// TimeZone Declarations
//
// Declaration of time zone constants and lookup tables
//
#declare TimeZone_UCT       = 0;

// US-based timezones
#declare TimeZone_US_AST    = -4;
#declare TimeZone_US_EST    = -5;
#declare TimeZone_US_CST    = -6;
#declare TimeZone_US_MST    = -7;
#declare TimeZone_US_PST    = -8;
#declare TimeZone_US_AKST   = -9;
#declare TimeZone_US_HST    = -11;

#declare TimeZone_US_ADT    = -3;
#declare TimeZone_US_EDT    = -4;
#declare TimeZone_US_CDT    = -5;
#declare TimeZone_US_MDT    = -6;
#declare TimeZone_US_PDT    = -7;
#declare TimeZone_US_AKDT   = -8;

// UK-based timezones
#declare TimeZone_UK_PST    = -8;
#declare TimeZone_UK_EST    = -5;
#declare TimeZone_UK_AST    = -4;
#declare TimeZone_UK_EDT    = -4;
#declare TimeZone_UK_ADT    = -3;
#declare TimeZone_UK_FKST   = -3;
#declare TimeZone_UK_GST    = -2;
#declare TimeZone_UK_GMT    = 0;
#declare TimeZone_UK_BST    = 1;
#declare TimeZone_UK_CEST   = 2;
#declare TimeZone_UK_IOT    = 6;

//-----------------------------------------------------------------------------
// TimeZone_US_DST_start(YYYY,TZ)
// TimeZone_US_DST_end(YYYY,TZ)
//
// Utility functions for computing the beginning and ending date (YYYYMMDD) for
// daylight savings time in the given year
//
#macro TimeZone_US_DST_start(YYYY,TZ)
    #if (YYYY < 1966 | TZ = TimeZone_US_HST)
        #local _result  = -1;
    #else
        #if (YYYY < 1986)
            #local _april30     = DateTime_create(YYYY,4,30,0,0,0,0);
            #local _april30_dow = DateTime_dayofweek(_april30);
            #local _result      = 30 - _april30_dow;
        #elseif (YYYY < 2007)
            // First Sunday of April
            #local _april1      = DateTime_create(YYYY,4,1,0,0,0,0);
            #local _april1_dow  = DateTime_dayofweek(_april1);
            #if (_april1_dow = 0)
                #local _result  = 1;
            #else                   
                #local _result  = 8 - _april1_dow;
            #end
            #local _result  = YYYY*10000 + 400 + _result;
        #else
            // Second Sunday of March    
            #local _march1  = DateTime_create(YYYY,3,1,0,0,0,0);
            #local _march1_dow  = DateTime_dayofweek(_march1);
            #if (_march1_dow = 0)
                #local _result  = 8;
            #else                   
                #local _result  = 15 - _march1_dow;
            #end
            #local _result  = YYYY*10000 + 300 + _result;
        #end 
    #end    
_result       
#end

#macro TimeZone_US_DST_end(YYYY,TZ)
    #if (YYYY < 1966 | TZ = TimeZone_US_HST)
        #local  _result = -1;
    #else
        #if (YYYY < 2007)
            // Last Sunday of October
            #local _october31       = DateTime_create(YYYY,10,31,0,0,0,0);
            #local _october31_dow   = DateTime_dayofweek(_october31);
            #local _result          = 31 - _october31_dow;
        #else 
            // First Sunday of November   
            #local _november1       = DateTime_create(YYYY,11,1,0,0,0,TZ);
            #local _november1_dow   = DateTime_dayofweek(_november1);
            #if (_november1_dow = 0)
                #local _result  = 1;
            #else                   
                #local _result  = 8 - _november1_dow;
            #end
            #local _result  = YYYY*10000 + 1100 + _result;
        #end 
    #end    
_result       
#end

// End TimeZone_US_DST_Start/TimeZone_US_DST_End
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TimeZone_US_is_dst(Date,TZ)
//
// Determine if the given Date falls within the range of DST for the given
// timezone.
//
#macro TimeZone_US_is_dst(Date,TZ)
    #local _dst_start   = TimeZone_US_DST_start(floor(Date/10000),TZ);
    #local _dst_end     = TimeZone_US_DST_end(floor(Date/10000),TZ);
    #if (_dst_start > 0 & _dst_start <= Date & _dst_end > 0 & _dst_end >= Date)
        #local _result  = true;
    #else
        #local _result  = false;
    #end
_result        
#end

// End TimeZone_US_is_dst
//-----------------------------------------------------------------------------

// End TimeZone Declaration
//=============================================================================

//=============================================================================
// DateTime Declaration
//
// To make it less verbose to carry around date and time information, we declare
// an array that carries the year (YYYY), month (MM), and day (DD) as well as
// hour (24-hour format), minute (MM), and second (SS.fraction), and a timezone
// offset (-12 to +12) relative to GMT.
//
// DateTime can also be used as a representation of duration by ignoring the
// timezone.
//

#declare DateTime_Year      = 0;
#declare DateTime_Month     = 1;
#declare DateTime_Day       = 2;
#declare DateTime_Hour      = 3;
#declare DateTime_Minute    = 4;
#declare DateTime_Second    = 5;
#declare DateTime_TZ        = 6;

//-----------------------------------------------------------------------------
// DateTime_create(YYYY,MM,DD,HH,MN,SS,TZ)
//
// Create a DateTime object with the given values
//
#macro DateTime_create(YYYY,MM,DD,HH,MN,SS,TZ)
    #local _result  = array[7]{
        YYYY,
        MM,
        DD,
        HH,
        MN,
        SS,
        TZ
    };     
_result
#end

// End DateTime_create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_copy(DateTime)
//
// Creates a new DateTime object that is an exact copy of the given DateTime.
//
#macro DateTime_copy(DateTime)
    #local _result  = DateTime_create(
        DateTime[DateTime_Year],
        DateTime[DateTime_Month],
        DateTime[DateTime_Day],
        DateTime[DateTime_Hour],
        DateTime[DateTime_Minute],
        DateTime[DateTime_Second],
        DateTime[DateTime_TZ]
    );                       
_result
#end

// End DateTime_copy
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_add(DateTime,ValueType,Value)
//
// Adds the given Value expressed as DateTime_Year, DateTime_Month, DateTime_Day
// DateTime_Hour, DateTime_Minute, or DateTime_Second to the given DateTime
//
#macro DateTime_add(DateTime,ValueType,Value)
    #if (Value < 0)
        #error concat("DateTime_add: only positive values are allowed. Use DateTime_sub for subtraction\n")
    #end
    
    #switch(ValueType)
        #case(DateTime_Year)
            #local DateTime[DateTime_Year]  = DateTime[DateTime_Year] + Value; 
        #break
        
        #case(DateTime_Month)
            #local DateTime[DateTime_Month] = DateTime[DateTime_Month] + Value;
            #while (DateTime[DateTime_Month] > 12)
                DateTime_add(DateTime,DateTime_Year,1)
                #local DateTime[DateTime_Month] = DateTime[DateTime_Month] - 12;
            #end
        #break
        
        #case(DateTime_Day)
            #local _monthdays               = DateTime_monthdays(DateTime);
            #local DateTime[DateTime_Day]   = DateTime[DateTime_Day] + Value;
            #while (DateTime[DateTime_Day] > _monthdays)
                DateTime_add(DateTime,DateTime_Month,1)
                #local DateTime[DateTime_Day]   = DateTime[DateTime_Day] - _monthdays;
                #local _monthdays               = DateTime_monthdays(DateTime);
            #end    
        #break
        
        #case(DateTime_Hour)
            #local DateTime[DateTime_Hour]  = DateTime[DateTime_Hour] + Value;
            #while (DateTime[DateTime_Hour] >= 24)
                DateTime_add(DateTime,DateTime_Day,1)
                #local DateTime[DateTime_Hour]  = DateTime[DateTime_Hour] - 24;
            #end
        #break
        
        #case(DateTime_Minute)
            #local DateTime[DateTime_Minute]    = DateTime[DateTime_Minute] + Value;
            #while (DateTime[DateTime_Minute] >= 60)
                DateTime_add(DateTime,DateTime_Hour,1)
                #local DateTime[DateTime_Minute]    = DateTime[DateTime_Minute] - 60;
            #end
        #break
        
        #case(DateTime_Second)
            #local DateTime[DateTime_Second]    = DateTime[DateTime_Second] + Value;
            #while (DateTime[DateTime_Second] >= 60)
                DateTime_add(DateTime,DateTime_Minute,1)
                #local DateTime[DateTime_Second]    = DateTime[DateTime_Second] - 60;
            #end
        #break
    #end    
#end

// End DateTime_add
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_sub(DateTime,ValueType,Value)
//
// Adds the given Value expressed as DateTime_Year, DateTime_Month, DateTime_Day
// DateTime_Hour, DateTime_Minute, or DateTime_Second to the given DateTime
//
#macro DateTime_sub(DateTime,ValueType,Value)
    #if (Value < 0)
        #error concat("DateTime_sub: only positive values are allowed. Use DateTime_add for addition\n")
    #end
    
    #switch(ValueType)
        #case(DateTime_Year)
            #local DateTime[DateTime_Year]  = DateTime[DateTime_Year] - Value; 
        #break
        
        #case(DateTime_Month)
            #local DateTime[DateTime_Month] = DateTime[DateTime_Month] - Value;
            #while (DateTime[DateTime_Month] < 1)
                DateTime_sub(DateTime,DateTime_Year,1)
                #local DateTime[DateTime_Month] = DateTime[DateTime_Month] + 12;
            #end
        #break
        
        #case(DateTime_Day)
            #local DateTime[DateTime_Day]   = DateTime[DateTime_Day] - Value;
            #while (DateTime[DateTime_Day] < 1)
                DateTime_sub(DateTime,DateTime_Month,1)
                #local _monthdays               = DateTime_monthdays(DateTime);
                #local DateTime[DateTime_Day]   = DateTime[DateTime_Day] + _monthdays;
            #end    
        #break
        
        #case(DateTime_Hour)
            #local DateTime[DateTime_Hour]  = DateTime[DateTime_Hour] - Value;
            #while (DateTime[DateTime_Hour] < 0)
                DateTime_sub(DateTime,DateTime_Day,1)
                #local DateTime[DateTime_Hour]  = DateTime[DateTime_Hour] + 24;
            #end
        #break
        
        #case(DateTime_Minute)
            #local DateTime[DateTime_Minute]    = DateTime[DateTime_Minute] - Value;
            #while (DateTime[DateTime_Minute] < 0)
                DateTime_sub(DateTime,DateTime_Hour,1)
                #local DateTime[DateTime_Minute]    = DateTime[DateTime_Minute] + 60;
            #end
        #break
        
        #case(DateTime_Second)
            #local DateTime[DateTime_Second]    = DateTime[DateTime_Second] - Value;
            #while (DateTime[DateTime_Second] < 0)
                DateTime_sub(DateTime,DateTime_Minute,1)
                #local DateTime[DateTime_Second]    = DateTime[DateTime_Second] + 60;
            #end
        #break
    #end    
#end

// End DateTime_sub
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_utc(DateTime)
//
#macro DateTime_utc(DateTime)
    #local _result  = DateTime_copy(DateTime);
    #local _result[DateTime_TZ]     = TimeZone_UCT;
    #local _result[DateTime_Hour]   = _result[DateTime_Hour] - DateTime[DateTime_TZ];
    #if (_result[DateTime_Hour] < 0)                                
        #local _result[DateTime_Hour]   = _result[DateTime_Hour] + 24;
        #local _result[DateTime_Day]    = _result[DateTime_Day] - 1;
        #if (_result[DateTime_Day] = 0)
            #local _result[DateTime_Month]  = _result[DateTime_Month] - 1;
            #if (_result[DateTime_Month] = 0)
                #local _result[DateTime_Month]  = 12;
                #local _result[DateTime_Year]   = _result[DateTime_Year] - 1;
            #end
            #local _result[DateTime_Day]    = DateTime_monthdays(_result);
        #end
    #elseif (_result[DateTime_Hour] > 23)
        #local _result[DateTime_Hour]   = _result[DateTime_Hour] - 24;
        #local _result[DateTime_Day]    = _result[DateTime_Day] + 1;
        #if (_result[DateTime_Day] > DateTime_monthdays(_result))
            #local _result[DateTime_Month]  = _result[DateTime_Month] + 1;
            #if (_result[DateTime_Month] > 12)
                #local _result[DateTime_Month]  = _result[DateTime_Month] - 12;
                #local _result[DateTime_Year]   = _result[DateTime_Year] + 1;
            #end                                                             
            #local _result[DateTime_Day]    = 1;
        #end    
    #end
_result    
#end

// End DateTime_utc
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_leapyear(DateTime)
//
// Determines if the given DateTime falls within a leap year or not
//
#macro DateTime_leapyear(DateTime)
    #local _yyyy    = DateTime[DateTime_Year];
    #local _result  = false;
    #if (mod(_yyyy,4) = 0)
        #if (mod(_yyyy,100) = 0)
            #if (mod(_yyyy,400) = 0)
                #local _result  = true;
            #end
        #else
            #local _result  = true;
        #end
    #end
_result    
#end

// End DateTime_leapyear
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_monthdays(DateTime)
//
// Determines the number of days in the month for the given DateTime
//
#macro DateTime_monthdays(DateTime)
    #local _mm      = DateTime[DateTime_Month];
    #local _result  = 30;
    #if (_mm = 1 | _mm = 3 | _mm = 5 | _mm = 7 | _mm = 8 | _mm = 10 | _mm = 12)
        #local _result = 31;
    #elseif (_mm = 2)
        #if (DateTime_leapyear(DateTime))
            #local _result = 29;
        #else
            #local _result = 28;
        #end        
    #end
_result        
#end

// End DateTime_monthdays
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_dayofyear(DateTime)
//
// Computes the day of the year represented by the given DateTime.
// January 1st is 1, December 31 is 365 or 366
//
#macro DateTime_dayofyear(DateTime) 
    #local MM       = DateTime[DateTime_Month];
    #local YYYY     = DateTime[DateTime_Year];
    #local DD       = DateTime[DateTime_Day];
    #local N1       = floor(275*MM/9);
    #local N2       = floor((MM+9)/12);
    #local N3       = (1 + floor((YYYY - 4*floor(YYYY/4) + 2) / 3));
    #local _result  = N1 - (N2*N3) + DD - 30;
_result
#end

// End DateTime_dayofyear
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_dayofweek(DateTime)
//
// Computes the day of the week represented by the given DateTime with Sunday
// being 0, and Saturday being 6
//
#macro DateTime_dayofweek(DateTime)
    #local MM   = DateTime[DateTime_Month];
    #local YYYY = DateTime[DateTime_Year];
    #local DD   = DateTime[DateTime_Day];
    #if (MM < 3)
        #local YYYY = YYYY - 1;
        #local m    = MM + 10;
    #else                     
        #local m    = MM - 2;
    #end
    #local k    = DD;
    #local D    = mod(YYYY,100);
    #local C    = floor(YYYY/100);
    
    #local f    = k + floor((13*m - 1)/5) + D + floor(D/4) + floor(C/4) - 2*C;
    #local f    = mod(f,7);
    #while (f < 0)
        #local f    = f + 7;
    #end
f        
#end

// End DateTime_dayofweek
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_date(DateTime)
//
// Computes a numeric value for the date portion of the DateTime as an 8-digit
// number in the form: YYYYMMDD
//
#macro DateTime_date(DateTime)
    #local _result  = DateTime[DateTime_Year]*10000 + DateTime[DateTime_Month]*100 + DateTime[DateTime_Day];
_result
#end

// End DateTime_date
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_y2k_days(DateTime)
//
// Computes the number of days to or from 1/1/2000
//

// End DateTime_y2k_days
//-----------------------------------------------------------------------------
#macro DateTime_y2k_days(DateTime)
    #local _utc     = DateTime_utc(DateTime);
    #local Year     = _utc[DateTime_Year];
    #local Month    = _utc[DateTime_Month];
    #local Day      = _utc[DateTime_Day];
    #local UT       = DateTime_hourofday(_utc); 
    #local _result  = 367*Year-int((7*int((Year+int((Month+9))/12)))/4)+int((275*Month)/9)+Day-730530+UT/24;
_result
#end

//-----------------------------------------------------------------------------
// DateTime_time(DateTime)
//
// Computes a numeric value for the time portion of the DateTime as floating
// point number in the form: HHMMSS.SSS
//
#macro DateTime_time(DateTime)
    #local _result  = DateTime[DateTime_Hour]*10000 + DateTime[DateTime_Minute]*100 + DateTime[DateTime_Second];
_result    
#end

// End DateTime_time
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_hourofday(DateTime)
//
// Computes the hour of the day as an HourOfDay - a floating point number where the hour part
// is to the left of the decimal and the minutes/seconds/fractions are to the
// right of the decimal as a fraction of an hour.
//
#macro DateTime_hourofday(DateTime)
    #local _result  = DateTime[DateTime_Hour] + (DateTime[DateTime_Minute]/60) + (DateTime[DateTime_Second]/3600);
_result     
#end

// End DateTime_hourofday
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// HourOfDay_normalize(HourOfDay)
//
// Normalizes the given hour-of-day expressed as hour.fraction-part such that
// the hour part lies with the range 0-24
// 
#macro HourOfDay_normalize(HourOfDay)
    #local _result  = HourOfDay;
    #while (_result < 0)
        #local _result  = _result + 24;
    #end
    #while (_result >= 24)
        #local _result  = _result - 24;
    #end
_result        
#end

// End HourOfDay_normalize(HourOfDay)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DateTime_str(DateTime)
//
// Create a string representation of the given DateTime in the format:
//
//      YYYY/MM/DD HH:MM:SS GMT[+/-TZ]
//
#macro DateTime_str(DateTime)
    #if (DateTime[6] > 0)
        #local _tz_string   = concat("+", str(DateTime[6],0,0));
    #elseif (DateTime[6] < 0)
        #local _tz_string   = str(DateTime[6],0,0);
    #else
        #local _tz_string   = "";
    #end        
    #local _result = concat(
        str(DateTime[0],-4,0), "/",
        str(DateTime[1],-2,0), "/",
        str(DateTime[2],-2,0), " ",
        str(DateTime[3],-2,0), ":",
        str(DateTime[4],-2,0), ":",
        str(DateTime[5],-5,2), " GMT",
        _tz_string
    )
_result    
#end 

// End DateTime_str
//-----------------------------------------------------------------------------

// End DateTime Declaration
//=============================================================================

#version Lib_time_Inc_Temp;
#end // ifndef(Lib_time_Inc_Temp)