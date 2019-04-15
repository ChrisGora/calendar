/* This program takes a valid date in the calendar
	and returns the correct day of the week */

#include "calendar.h"

enum { Mon, Tue, Wed, Thu, Fri, Sat, Sun, Invalid };

int months[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool isLeap(int y) {
	bool leapA;
	bool leapB;
	bool leapC;
	if (y % 4 == 0) leapA = true;
	else leapA = false;
	if (y % 100 == 0) leapB = false;
	else leapB = true;
	if (y % 400 == 0) leapC = true;
	else leapC = false;
//	printf("isLeap:%d \n", leap);
	return leapA && (leapB || leapC);
}

int sumMonths(int d, int m, int y) {
	int sum = 0;
	if ((!(isLeap(y)) && (m >= 3))) sum = sum - 1;
	m = m - 2;
	while (m >= 0) {
		sum = sum + months[m];
		m = m - 1;
	}
//	printf("sumMonths: %d \n", sum);
	return sum;
}

long sumYearsPre1900(int y) {
	long sum = 0;
	while (y < 1899) {
		sum = sum + sumMonths(0, 13, (y + 1));
		y = y + 1;
	}
	return sum;
}

long sumYears(int y) {
	long sum = 0;
	if (y < 1900) {
		sum = sumYearsPre1900(y);
		return sum;
	}
	while (y > 1900) {
		sum = sum + sumMonths(0, 13, (y - 1));
		y = y - 1;
	}
//	printf("sumYears: %ld \n", sum);
	return sum;
}

int convert(const char date[], int a, int b) {
    int x;
	int y;
	int sum;
    x = date[a] - '0';
	y = date[b] - '0';
	sum = (x * 10) + y;
//	printf("convert: %d \n", sum);
    return sum;
}

int convertSingle(const char date[], int a) {
    int x;
    x = date[a] - '0';
//	printf("convertSingle: %d \n", x);
    return x;
}

bool dayRangeCheck(int d, int m){
	d = d + 1;
	if (d > months[m - 1]) {
//		printf("dayRangeCheck: False\n");
		return false;
	}
	else {
//		printf("dayRangeCheck: True\n");
		return true;
	}
}

bool monthRangeCheck(int m){
	if (m > 12 || m == 0) return false;
	else return true;
}

bool formatCheck(const char date[]){
//	printf("date 2 %d\n", date[2]);
//	printf("date 5 %d\n", date[5]);
	if ((date[2] == '/') && (date[5] == '/')) return true;
	else return false;
}

bool oneDigitCheck (char m) {
	if (('0' <= m) && ('9' >= m)) return true;
	else return false;
}

bool allDigitsCheck(const char date[]){
	int length = strlen(date);
	while (length > 0){
		if ((length == 3) || (length == 6)) length--;
		if (! oneDigitCheck(date[length - 1])) return false;
		else length--;
	}
	return true;
}

long calendar(int d, int m, int y) {
//	printf("d= %d , m= %d , y= %d \n", d, m, y);
	long sum = d + sumMonths(d, m, y);
	if (y >= 1900) sum = sum + sumYears(y);
	else if (y < 1900) sum = sumYears(y) + sumMonths(0, 13, y) - sum;
//	printf("calendar says: %ld \n", sum);
	return sum;
}

bool checkAll(int n, const char date[], int d, int m, int y) {
	bool checkA = dayRangeCheck(d, m);
	bool checkB = monthRangeCheck(m);
	bool checkC = formatCheck(date);
	bool checkD = allDigitsCheck(date);
	return (checkA && checkB && checkC && checkD);
}

bool checkParsed(int d, int m, int y) {
	bool checkA = dayRangeCheck(d, m);
	bool checkB = monthRangeCheck(m);
	return (checkA && checkB);
}

// The first 4 lines of this function have been adapted from https://www.gnu.org/software/libc/manual/html_node/Time-Functions-Example.html
long daysBetween(int d, int m, int y){
	time_t curtime;
	curtime = time(NULL);
	struct tm *loctime;
	loctime = localtime(&curtime);
	int dToday = (loctime->tm_mday) - 1;
	int mToday = (loctime->tm_mon) + 1 ;
	int yToday = (loctime->tm_year) + 1900;
//	printf("day %d month %d year %d \n", dToday, mToday, yToday);
	long days = calendar(d, m, y);
	long daysToToday = calendar(dToday, mToday, yToday);
	long daysBetween;
	bool A = ( y > yToday);
	bool B = (y == yToday) && (m > mToday);
	bool C = (y == yToday) && (m = mToday) && (d >= dToday);
	bool D = (y < 1900);
//	printf("days to today %ld \n", daysToToday);
//	printf("days %ld \n", days);
	if (A || B || C) daysBetween = days - daysToToday;
	else if (D) daysBetween = days + daysToToday;
	else daysBetween = daysToToday - days;
	return daysBetween;
}

int parseDay(int n, const char date[]) {
	return convert(date, 0, 1);
}

int parseMonth(int n, const char date[]) {
	return convert(date, 3, 4);
}

int parseYear(int n, const char date[]) {
	int y = 0;
	int multiplier = 1;
	int length = strlen(date);
	while (length > 6){
		int digit = convertSingle(date, (length - 1));
		y = y + (digit * multiplier);
		length--;
		multiplier = multiplier * 10;
	}
	return y;
}

int dayOfWeekParsed(int d, int m, int y) {
	d = d - 1;
	int day = Invalid;
	if (!checkParsed(d, m, y)) {
		return day = Invalid;
	}
	long whichDay = calendar(d, m, y) % 7;
	if (y < 1900) whichDay = 7 - whichDay;
	if (y < 1582) printf("Warning: this is likely to be incorrect\n");
	return whichDay;
}

int dayOfWeek(int n, const char date[]) {
	bool checkC = formatCheck(date);
	bool checkD = allDigitsCheck(date);
	if (!checkC || !checkD) {
		return Invalid;
	}
	int d = parseDay(n, date);
	int m = parseMonth(n, date);
	int y = parseYear(n, date);
	return dayOfWeekParsed(d, m, y);
}

int printDay(int d) {
	if (d == Invalid) printf("Invalid \nPlease use dd/mm/yyyy format\n");
	else if (d == Mon) printf("Monday \n");
	else if (d == Tue) printf("Tuesday \n");
	else if (d == Wed) printf("Wednesday \n");
	else if (d == Thu) printf("Thursday \n");
	else if (d == Fri) printf("Friday \n");
	else if (d == Sat) printf("Saturday \n");
	else if (d == Sun) printf("Sunday \n");
	return 0;
}

void post1900test() {
	assert(dayOfWeek(2, "01/01/1900") == Mon);
	assert(dayOfWeek(2, "06/06/1999") == Sun);
	assert(dayOfWeek(2, "14/10/2017") == Sat);
	assert(dayOfWeek(2, "13/10/2017") == Fri);
	assert(dayOfWeek(2, "12/01/8526") == Sat);
}

void testLeap() {
	assert(dayOfWeek(2, "28/02/1900") == Wed);
	assert(dayOfWeek(2, "01/03/1900") == Thu);
	assert(dayOfWeek(2, "29/02/2004") == Sun);
	assert(dayOfWeek(2, "01/03/2004") == Mon);

}

void testDays() {
	assert((dayOfWeek(2, "31/01/1900")) == Wed);
	assert((dayOfWeek(2, "32/01/1900")) == Invalid);
	assert((dayOfWeek(2, "33/01/1900")) == Invalid);
}

void testMonths() {
	assert(dayOfWeek(2, "01/13/1900") == Invalid);
	assert(dayOfWeek(2, "06/58/1999") == Invalid);
	assert(dayOfWeek(2, "13/15/2017") == Invalid);
	assert(dayOfWeek(2, "12/00/8526") == Invalid);
}

void testTooLarge() {
	assert(dayOfWeek(2, "369/02/1901") == Invalid);
	assert(dayOfWeek(2, "354/05/2017") == Invalid);
	assert(dayOfWeek(2, "14/1198/2017") == Invalid);
	assert(dayOfWeek(2, "145/05/2017") == Invalid);

}

void testLetters() {
	assert(dayOfWeek(2, "xy/10/2017") == Invalid);
	assert(dayOfWeek(2, "14/ad/2017") == Invalid);
	assert(dayOfWeek(2, "14/10/2o1e") == Invalid);
}

void post9999test() {
	assert(dayOfWeek(2, "01/01/10000") == Sat);
	assert(dayOfWeek(2, "02/01/10000") == Sun);
}

void pre1900test() {
	assert(dayOfWeek(2, "31/12/1899") == Sun);
	assert(dayOfWeek(2, "01/01/1899") == Sun);
	assert(dayOfWeek(2, "31/12/1898") == Sat);
	assert(dayOfWeek(2, "01/01/1600") == Sat);
	assert(dayOfWeek(2, "15/10/1582") == Fri);
}

void pre1900test2() {
	assert(dayOfWeek(2, "31/12/1899") == 6);
	assert(dayOfWeek(2, "01/01/1899") == 6);
	assert(dayOfWeek(2, "31/12/1898") == 5);
	assert(dayOfWeek(2, "01/01/1600") == 5);
	assert(dayOfWeek(2, "15/10/1582") == 4);
}

void pre1900testParsed() {
	assert(dayOfWeekParsed(31, 12, 1899) == 6);
	assert(dayOfWeekParsed(1, 1, 1899) == 6);
	assert(dayOfWeekParsed(31, 12, 1898) == 5);
	assert(dayOfWeekParsed(1, 1, 1600) == 5);
	assert(dayOfWeekParsed(15, 10, 1582) == 4);
}

void test() {
	post1900test();
	testLeap();
	testDays();
	testMonths();
	testTooLarge();
	testLetters();
	post9999test();
	pre1900test();
	pre1900test2();
	pre1900testParsed();
	printf("All tests passed\n");
}

int main(int n, char *args[n]) {
	if (n == 1) test();
	else if (n == 2) {
		int d = parseDay(n, args[1]);
		int m = parseMonth(n, args[1]);
		int y = parseYear(n, args[1]);
		printDay(dayOfWeekParsed(d, m, y));
		printf("Days between today and the date given: %ld \n", daysBetween(d, m, y));
	}
	else printf("Please input a date in fromat dd/mm/yyyy \n");

}
