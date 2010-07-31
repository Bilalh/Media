#include "time_test.h"

// int tm_sec;     /* seconds (0 - 60) */
// int tm_min;     /* minutes (0 - 59) */
// int tm_hour;    /* hours (0 - 23) */
// int tm_mday;    /* day of month (1 - 31) */
// int tm_mon;     /* month of year (0 - 11) */
// int tm_year;    /* year - 1900 */
// int tm_wday;    /* day of week (Sunday = 0) */
// int tm_yday;    /* day of year (0 - 365) */
// int tm_isdst;   /* is summer time in effect? */
// char *tm_zone;  /* abbreviation of timezone name */
// long tm_gmtoff; /* offset from UTC in seconds */

int main (void) {
	TimeSetup
	
printTitle("R time Basic");
	TestTime("37 minutes ago",
	{
		tm->tm_min -= 37;
	})
	TestTime("2 hours after",
	{
		tm->tm_hour += 2;
	})
	TestTime("1 days ago",
	{
		tm->tm_mday -= 1;
	})
	TestTime("1337 minutes after",
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

printTitle("Rel time Medium");
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

printTitle("Rel time with at nn:nn");	
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
TimeEndSection

printTitle("Invalid inputs");
	TestTime("37 minuts ago" ,{})
	TestTime("2 hors after"  ,{})
	TestTime("1 dys ago"     ,{})
	TestTime("1 day next"    ,{})
	TestTime("five days ago" ,{})
	TestTime("5 days ago at five" ,{
		tm->tm_mday -= 5;
	})

TimeEndSection

printTitle("Compound rel time");	
	TestTime("17 minutes 3 hours ago",
	{
		tm->tm_min  -= 17;
		tm->tm_hour -= 3;
	
	})
	TestTime("67 minutes 32 days ago", {
		tm->tm_min  -= 67;
		tm->tm_mday -= 32;

	})
	TestTime("142 hours   1 minutes after", {
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




return;
	
printTitle("Rel time English");
	TestTime("an hour ago",
	{
		tm->tm_hour -= 1;
	})
	TestTime("an hour and 5 minutes ago",
	{
		tm->tm_hour -= 1;
		tm->tm_min  -= 5;
	})
	TestTime("5 mins ago",
	{
		tm->tm_min -= 5;
	})
TimeEndSection	

}

