#include "include/time_test.h"
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



TestResult time_test_main(int test_no) {
TimeSetup

Sections {
Section("Relative time Basic"){
	TimeTest("37 minutes ago", {
		tm->tm_min -= 37;

	})
	TimeTest("2 hours after", {
		tm->tm_hour += 2;

	})
	TimeTest("1 day ago", {
		tm->tm_mday -= 1;

	})
	TimeTest("1337 mins after", {
		tm->tm_min += 1337;

	})
	TimeTest("978 hours ago", {
		tm->tm_hour -= 978;

	})
	TimeTest("413 days after", {
		tm->tm_mday += 413;

	})
}TimeEndSection

Section("Relative time Medium"){
	TimeTest("17 minutes ago 3 hours ago", {
		tm->tm_min  -= 17;
		tm->tm_hour -= 3;

	})
	TimeTest("67 minutes ago 32 days ago", {
		tm->tm_min  -= 67;
		tm->tm_mday -= 32;

	})
	TimeTest("142 hours ago 1 minutes after", {
		tm->tm_min  += 1;
		tm->tm_hour -= 142;

	})
	TimeTest("127 days after 94 minutes after", {
		tm->tm_min   += 94;
		tm->tm_mday  += 127;

	})
	TimeTest("14 hours ago 13 minutes ago 8 days ago", {
		tm->tm_min  -= 13;
		tm->tm_hour -= 14;
		tm->tm_mday -= 8;

	})
	TimeTest("9 minutes after 8 days ago 4 hours ago", {
		tm->tm_min  += 9;
		tm->tm_hour -= 4;
		tm->tm_mday -= 8;

	})
}TimeEndSection

Section("Relative time with at nn:nn"){
	TimeTest("3 days ago at 12:33", {
		tm->tm_min  = 33;
		tm->tm_hour = 12;
		tm->tm_mday -= 3;

	})
	TimeTest("9 minutes ago at 23:12", {
		tm->tm_min  = 12;
		tm->tm_hour = 23;

	})
	TimeTest("9 days after at 2:3", {
		tm->tm_min  = 3;
		tm->tm_hour = 2;
		tm->tm_mday += 9;

	})	
	TimeTest("at 2:12", {
		tm->tm_min  = 12;
		tm->tm_hour = 2;

	})
}TimeEndSection

Section("Invalid inputs"){
	TimeTest("37 mimuts ago" ,{})
	TimeTest("1 dps ago"     ,{})
	TimeTest("1 dax now"    ,{})
	TimeTest("five days ago" ,{})
	TimeTest("5 days ago at five" ,{
		tm->tm_mday -= 5;

	})

}TimeEndSection

Section("Compound Relative time"){
	TimeTest("17 minutes 3 hours ago", {
		tm->tm_min  -= 17;
		tm->tm_hour -= 3;

	})
	TimeTest("67 minutes 32 days ago", {
		tm->tm_min  -= 67;
		tm->tm_mday -= 32;

	})
	TimeTest("142 hours   1 minute after", {
		tm->tm_min  += 1;
		tm->tm_hour += 142;

	})
	TimeTest("127 days       94 minutes after", {
		tm->tm_min   += 94;
		tm->tm_mday  += 127;

	})
	TimeTest("14 hours 13 minutes 8 days ago", {
		tm->tm_min  -= 13;
		tm->tm_hour -= 14;
		tm->tm_mday -= 8;

	})
	TimeTest("9 minutes  8 days ago 4 hours ago", {
		tm->tm_min  -= 9;
		tm->tm_hour -= 4;
		tm->tm_mday -= 8;

	})
}TimeEndSection

Section("Full Dates"){
	TimeTest("2008-12-30T05:21:45", {
		tm->tm_year = 108;
		tm->tm_mon  = 11;
		tm->tm_mday = 30;
		tm->tm_hour = 05;
		tm->tm_min  = 21;
		tm->tm_sec  = 45;

	})
	TimeTest("2007-04-14", {
		tm->tm_year = 107;
		tm->tm_mon  = 3;
		tm->tm_mday = 14;

	})	
	TimeTest("2005-09-14 at 5:33", {
		tm->tm_year = 105;
		tm->tm_mon  = 8;
		tm->tm_mday = 14;

		tm->tm_hour = 5;
		tm->tm_min  = 33;

	})	
}TimeEndSection

Section("Months"){
	TimeTest("11th Jan", {
		tm->tm_mon  = 0;
		tm->tm_mday = 11;

	})
	TimeTest("2nd Feb", {
		tm->tm_mon  = 1;
		tm->tm_mday = 2;

	})
	TimeTest("3rd Mar", {
		tm->tm_mon  = 2;
		tm->tm_mday = 3;

	})
	TimeTest("11th Apr", {
		tm->tm_mon  = 3;
		tm->tm_mday = 11;

	})
	TimeTest("12th May 2010", {
		tm->tm_year = 110;
		tm->tm_mon  = 4;
		tm->tm_mday = 12;

	})
	TimeTest("13rd Jun", {
		tm->tm_mon  = 5;
		tm->tm_mday = 13;

	})
	TimeTest("15th Jul 2009", {
		tm->tm_year = 109;
		tm->tm_mon  = 6;
		tm->tm_mday = 15;

	})
	TimeTest("22nd August", {
		tm->tm_mon  = 7;
		tm->tm_mday = 22;

	})
	TimeTest("31th Sep 5 mins after", {
		tm->tm_mon  = 8;
		tm->tm_mday = 31;
		tm->tm_min += 5;

	})
	TimeTest("7th Oct 2 days ago", {
		tm->tm_mon  = 9;
		tm->tm_mday = 5;

	})
	TimeTest("19th Nov 1972", {
		tm->tm_year = 72;
		tm->tm_mon  = 10;
		tm->tm_mday = 19;

	})
	TimeTest("1st Dec 2002 at 5:32", {
		tm->tm_year = 102;
		tm->tm_mon  = 11;
		tm->tm_mday = 1;
		tm->tm_hour = 5;
		tm->tm_min  = 32;

	})
}TimeEndSection

Section("Days"){
	TimeTest("near thursday", {
		tm->tm_mday += day_diff(tm->tm_wday, THURSDAY);

	})
	TimeTest("next tuesday", {
		tm->tm_mday += day_future(tm->tm_wday, TUESDAY);

	})
	TimeTest("last monday", {
		tm->tm_mday += day_last(tm->tm_wday, MONDAY);

	})
	TimeTest("near sun", {
		tm->tm_mday += day_diff(tm->tm_wday, SUNDAY);

	})
	TimeTest("next Wed", {
		tm->tm_mday += day_future(tm->tm_wday, WEDNESDAY);

	})
	TimeTest("last Friday", {
		tm->tm_mday += day_last(tm->tm_wday, FRIDAY);

	})
	TimeTest("last Sun", {
		tm->tm_mday += day_last(tm->tm_wday, SUNDAY);

	})
}TimeEndSection


Section("day -> last day"){
	TimeTest("last sunday", {
		tm->tm_mday += day_last(tm->tm_wday, SUNDAY);

	})
	TimeTest("sunday", {
		tm->tm_mday += day_last(tm->tm_wday, SUNDAY);

	})
	TimeTest("Monday", {
		tm->tm_mday += day_last(tm->tm_wday, MONDAY);

	})
	TimeTest("tue", {
		tm->tm_mday += day_last(tm->tm_wday, TUESDAY);

	})
	TimeTest("Wed", {
		tm->tm_mday += day_last(tm->tm_wday, WEDNESDAY);

	})
	TimeTest("Thursday", {
		tm->tm_mday += day_last(tm->tm_wday, THURSDAY);

	})
	TimeTest("fri", {
		tm->tm_mday += day_last(tm->tm_wday, FRIDAY);

	})
	TimeTest("Sat", {
		tm->tm_mday += day_last(tm->tm_wday, SATURDAY);

	})
	
}TimeEndSection

Section("at am/pm"){
	TimeTest("at 8pm",{
		tm->tm_hour = 20;
		tm->tm_min  = 0;
	})
	TimeTest("at 5pm",{
		tm->tm_hour = 17;
		tm->tm_min  = 0;
	})
	TimeTest("at 1pm",{
		tm->tm_hour = 13;
		tm->tm_min  = 0;
	})

	TimeTest("at 8am",{
		tm->tm_hour = 8;
		tm->tm_min  = 0;
	})
	TimeTest("at 2am",{
		tm->tm_hour = 2;
		tm->tm_min  = 0;
	})
	TimeTest("at 10am",{
		tm->tm_hour = 10;
		tm->tm_min  = 0;
	})
	TimeTest("at 12pm",{
		tm->tm_hour = 12;
		tm->tm_min  = 0;
	})
	TimeTest("at 12am",{
		tm->tm_hour = 0;
		tm->tm_min  = 0;
	})
	TimeTest("at 0am",{
		tm->tm_hour = 0;
		tm->tm_min  = 0;
	})
}TimeEndSection



Section("at am/pm with minutes"){
	TimeTest("at 10:30pm",{
		tm->tm_hour = 22;
		tm->tm_min  = 30;
	})
	TimeTest("at 11:43pm",{
		tm->tm_hour = 23;
		tm->tm_min  = 43;
	})
	TimeTest("at 1:1pm",{
		tm->tm_hour = 13;
		tm->tm_min  = 1;
	})
	TimeTest("at 12:00pm",{
		tm->tm_hour = 12;
		tm->tm_min  = 00;
	})
	TimeTest("at 12:30am",{
		tm->tm_hour = 0;
		tm->tm_min  = 30;
	})
	TimeTest("at 11:3am",{
		tm->tm_hour = 11;
		tm->tm_min  = 3;
	})
	TimeTest("at 6:54am",{
		tm->tm_hour = 6;
		tm->tm_min  = 54;
	})
	TimeTest("at 1:21am",{
		tm->tm_hour = 1;
		tm->tm_min  = 21;
	})
}TimeEndSection



Section("last with with the at"){
	TimeTest("last sun at 8pm",{
		tm->tm_mday += day_last(tm->tm_wday, SUNDAY);
		tm->tm_hour = 20;
		tm->tm_min  = 0;
	})
	TimeTest("last sun 10:30pm",{
		tm->tm_mday += day_last(tm->tm_wday, SUNDAY);
		tm->tm_hour = 22;
		tm->tm_min  = 30;
	})
	TimeTest("last sun 22:30",{
		tm->tm_mday += day_last(tm->tm_wday, SUNDAY);
		tm->tm_hour = 22;
		tm->tm_min  = 30;
	})
	TimeTest("mon 9:30pm",{
		tm->tm_mday += day_last(tm->tm_wday, MONDAY);
		tm->tm_hour = 21;
		tm->tm_min  = 30;
	})
	TimeTest("mon 21:30",{
		tm->tm_mday += day_last(tm->tm_wday, MONDAY);
		tm->tm_hour = 21;
		tm->tm_min  = 30;
	})
}TimeEndSection


};

TestRun
TimePrintTestResults
TimeMakeResult;
return TimeResults;
}
