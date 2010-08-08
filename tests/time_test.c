#include "time_test.h"
TimeVar
// int tm_sec;     /* seconds (0 - 60) */
// int tm_min;     /* minutes (0 - 59) */
// int tm_hour;    /* hours   (0 - 23) */
// int tm_mday;    /* day of month  (1 - 31) */
// int tm_mon;     /* month of year (0 - 11) */
// int tm_year;    /* year - 1900 */
// int tm_wday;    /* day of week (Sunday = 0) */
// int tm_yday;    /* day of year (0 - 365) */
// int tm_isdst;   /* is summer time in effect? */
// char *tm_zone;  /* abbreviation of timezone name */
// long tm_gmtoff; /* offset from UTC in seconds */

int time_test_main(int test_no) {
TimeSetup

Sections {
Section("Relative time Basic")    
		TestTime("37 minutes ago",
		{
			tm->tm_min -= 37;
		})
		TestTime("2 hours after",
		{
			tm->tm_hour += 2;
		})
		TestTime("1 day ago",
		{
			tm->tm_mday -= 1;
		})
		TestTime("1337 mins after",
		{
			tm->tm_min += 1337;
		})
		TestTime("978 hours ago",
		{
			tm->tm_hour -= 978;
		})
		TestTime("413 days after",
		{
			tm->tm_mday += 413;
		})
TimeEndSection

Section("Relative time Medium")
	TestTime("17 minutes ago 3 hours ago",
	{
		tm->tm_min  -= 17;
		tm->tm_hour -= 3;

	})
	TestTime("67 minutes ago 32 days ago", {
		tm->tm_min  -= 67;
		tm->tm_mday -= 32;

	})
	TestTime("142 hours ago 1 minutes after", {
		tm->tm_min  += 1;
		tm->tm_hour -= 142;

	})
	TestTime("127 days after 94 minutes after", {
		tm->tm_min   += 94;
		tm->tm_mday  += 127;

	})
	TestTime("14 hours ago 13 minutes ago 8 days ago", {
		tm->tm_min  -= 13;
		tm->tm_hour -= 14;
		tm->tm_mday -= 8;

	})
	TestTime("9 minutes after 8 days ago 4 hours ago", {
		tm->tm_min  += 9;
		tm->tm_hour -= 4;
		tm->tm_mday -= 8;

	})
TimeEndSection

Section("Relative time with at nn:nn")
	TestTime("3 days ago at 12:33",
	{
		tm->tm_min  = 33;
		tm->tm_hour = 12;
		tm->tm_mday -= 3;
	})
	TestTime("9 minutes ago at 23:12", {
		tm->tm_min  = 12;
		tm->tm_hour = 23;

	})
	TestTime("9 days after at 2:3", {
		tm->tm_min  = 3;
		tm->tm_hour = 2;
		tm->tm_mday += 9;
	})	
	TestTime("at 2:12", {
		tm->tm_min  = 12;
		tm->tm_hour = 2;
	})
TimeEndSection

Section("Invalid inputs")
	TestTime("37 mimuts ago" ,{})
	TestTime("2 houss after"  ,{})
	TestTime("1 dps ago"     ,{})
	TestTime("1 dax now"    ,{})
	TestTime("five days ago" ,{})
	TestTime("5 days ago at five" ,{
		tm->tm_mday -= 5;
	})

TimeEndSection

Section("Compound Relative time")
	TestTime("17 minutes 3 hours ago",
	{
		tm->tm_min  -= 17;
		tm->tm_hour -= 3;

	})
	TestTime("67 minutes 32 days ago", {
		tm->tm_min  -= 67;
		tm->tm_mday -= 32;

	})
	TestTime("142 hours   1 minute after", {
		tm->tm_min  += 1;
		tm->tm_hour += 142;

	})
	TestTime("127 days       94 minutes after", {
		tm->tm_min   += 94;
		tm->tm_mday  += 127;

	})
	TestTime("14 hours 13 minutes 8 days ago", {
		tm->tm_min  -= 13;
		tm->tm_hour -= 14;
		tm->tm_mday -= 8;

	})
	TestTime("9 minutes  8 days ago 4 hours ago", {
		tm->tm_min  -= 9;
		tm->tm_hour -= 4;
		tm->tm_mday -= 8;

	})
TimeEndSection

Section("Full Dates")
	TestTime("2008-12-30T05:21:45",
	{
		tm->tm_year = 108;
		tm->tm_mon  = 11;
		tm->tm_mday = 30;
		tm->tm_hour = 05;
		tm->tm_min  = 21;
		tm->tm_sec  = 45;
	})
	TestTime("2007-04-14",
	{
		tm->tm_year = 107;
		tm->tm_mon  = 3;
		tm->tm_mday = 14;
	})	
	TestTime("2005-09-14 at 5:33",
	{
		tm->tm_year = 105;
		tm->tm_mon  = 8;
		tm->tm_mday = 14;

		tm->tm_hour = 5;
		tm->tm_min  = 33;
	})	
TimeEndSection

Section("Months")
	TestTime("11th Jan", {
		tm->tm_mon  = 0;
		tm->tm_mday = 11;

	})
	TestTime("2nd Feb", {
		tm->tm_mon  = 1;
		tm->tm_mday = 2;

	})
	TestTime("3rd Mar", {
		tm->tm_mon  = 2;
		tm->tm_mday = 3;

	})
	TestTime("11th Apr", {
		tm->tm_mon  = 3;
		tm->tm_mday = 11;

	})
	TestTime("12th May 2010", {
		tm->tm_year = 110;
		tm->tm_mon  = 4;
		tm->tm_mday = 12;

	})
	TestTime("13rd Jun", {
		tm->tm_mon  = 5;
		tm->tm_mday = 13;

	})
	TestTime("15th Jul 2009", {
		tm->tm_year = 109;
		tm->tm_mon  = 6;
		tm->tm_mday = 15;

	})
	TestTime("22nd August", {
		tm->tm_mon  = 7;
		tm->tm_mday = 22;

	})
	TestTime("31th Sep 5 mins after", {
		tm->tm_mon  = 8;
		tm->tm_mday = 31;
		tm->tm_min += 5;

	})
	TestTime("7th Oct 2 days ago", {
		tm->tm_mon  = 9;
		tm->tm_mday = 5;

	})
	TestTime("19th Nov 1972", {
		tm->tm_year = 72;
		tm->tm_mon  = 10;
		tm->tm_mday = 19;

	})
	TestTime("1st Dec 2002 at 5:32", {
		tm->tm_year = 102;
		tm->tm_mon  = 11;
		tm->tm_mday = 1;
		tm->tm_hour = 5;
		tm->tm_min  = 32;

	})
TimeEndSection

Section("Days")
	TestTime("near thursday", {
		tm->tm_mday += day_diff(tm->tm_wday, THURSDAY);
	})
	TestTime("next tuesday", {
		tm->tm_mday += day_future(tm->tm_wday, TUESDAY);
	})
	TestTime("last monday", {
		tm->tm_mday += day_last(tm->tm_wday, MONDAY);
	})
TimeEndSection
};

TestRun
TimePrintTestResults
return TimeResult;
}

#ifndef ALL_TESTS
MakeMain(time)
#endif
