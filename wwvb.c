#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// Time difference with New York time zone
// 6 hours for Central European Time
#define HOUR_DIFF 6

int last = 0;
int i;

// Wait "t" milliseconds to pass since the last call
void waitnext(int t) {
  int next = last + t; 
  do {
    delay(1);
  } while (millis() < next); 
  last = next; 
}

// Generate marker
void marker() {
  pwmWrite (18, 2);
  waitnext(800);
  pwmWrite (18, 80);
  waitnext(200);
//  printf("%d Marker\n", i);
}

// Generate "One"
void one() {
  pwmWrite (18, 2);
  waitnext(500);
  pwmWrite (18, 80);
  waitnext(500);
//  printf("%d   1\n", i);
}

// Generate "Zero"
void zero() {
  pwmWrite (18, 2);
  waitnext(200);
  pwmWrite (18, 80);
  waitnext(800);
//  printf("%d   0\n", i);
}

int main (void)
{

  int hour;
  int minute;
  int year; 
  int day;
  int isdst; 

  time_t current = time(NULL);
  struct tm *tm_struct = gmtime(&current);
  minute = tm_struct->tm_min;
  printf("Waiting for next minute...\n");

  // Wait for the next minute to start
  do {
    delay(10);
    current = time(NULL);
    tm_struct = gmtime(&current);
  } while(tm_struct->tm_min == minute);

  minute = tm_struct->tm_min;
  hour = tm_struct->tm_hour;
  day = tm_struct->tm_yday+1;
  year = tm_struct->tm_year-100;
  isdst = tm_struct->tm_isdst; 

  hour += HOUR_DIFF;
  if (hour > 23) {
    hour -= 24;
    day += 1; 
  }

  // DST flag does not work for some reason...
  isdst = 0; 

  // Initialise WiringPi GPIO
  if (wiringPiSetupGpio() == -1) exit(1);

  printf("Initializing PWM...\n");

  // Initialise PWM on pin 18
  pinMode(18,PWM_OUTPUT);
  pwmSetMode(PWM_MODE_MS);
  pwmSetClock(2);
  pwmSetRange(160);

  printf("Starting WWVB... Year %2.2d, day %2.2d\n", year, day);

  // Run WWVB sequence 10 times (10 minutes)
  for (i=0; i<10; i++) {
    printf("%2.2d:%2.2d UTC (+%d)\n", hour, minute, isdst);

    // Transmit 60 bits of WWVB code
    // See https://en.wikipedia.org/wiki/WWVB

    marker();  // 0
    (((minute/10)>>2) & 1) ? one() : zero();  // 1
    (((minute/10)>>1) & 1) ? one() : zero();  // 2
    (((minute/10)>>0) & 1) ? one() : zero();  // 3
    zero();    // 4
    (((minute%10)>>3) & 1) ? one() : zero();  // 5
    (((minute%10)>>2) & 1) ? one() : zero();  // 6
    (((minute%10)>>1) & 1) ? one() : zero();  // 7
    (((minute%10)>>0) & 1) ? one() : zero();  // 8
    marker(); // 9
    zero();   // 10
    zero();   // 11
    (((hour/10)>>1) & 1) ? one() : zero(); // 12
    (((hour/10)>>0) & 1) ? one() : zero(); // 13
    zero();   // 14
    (((hour%10)>>3) & 1) ? one() : zero(); // 15
    (((hour%10)>>2) & 1) ? one() : zero(); // 16
    (((hour%10)>>1) & 1) ? one() : zero(); // 17
    (((hour%10)>>0) & 1) ? one() : zero(); // 18
    marker(); // 19
    zero();   // 20
    zero();   // 21
    (((day/100)>>1) & 1) ? one() : zero(); // 22
    (((day/100)>>0) & 1) ? one() : zero(); // 23
    zero();   // 24
    ((((day/10)%10)>>3) & 1) ? one() : zero(); // 25
    ((((day/10)%10)>>2) & 1) ? one() : zero(); // 26
    ((((day/10)%10)>>1) & 1) ? one() : zero(); // 27
    ((((day/10)%10)>>0) & 1) ? one() : zero(); // 28
    marker(); // 29
    (((day%10)>>3) & 1) ? one() : zero(); // 30
    (((day%10)>>2) & 1) ? one() : zero(); // 31
    (((day%10)>>1) & 1) ? one() : zero(); // 32
    (((day%10)>>0) & 1) ? one() : zero(); // 33
    zero();   // 34
    zero();   // 35
    zero();   // 36
    one();    // 37
    zero();   // 38
    marker(); // 39
    zero();   // 40
    zero();   // 41
    zero();   // 42
    zero();   // 43
    zero();   // 44
    (((year/10)>>3) & 1) ? one() : zero(); // 45
    (((year/10)>>2) & 1) ? one() : zero(); // 46
    (((year/10)>>1) & 1) ? one() : zero(); // 47
    (((year/10)>>0) & 1) ? one() : zero(); // 48
    marker(); // 49
    (((year%10)>>3) & 1) ? one() : zero(); // 50
    (((year%10)>>2) & 1) ? one() : zero(); // 51
    (((year%10)>>1) & 1) ? one() : zero(); // 52
    (((year%10)>>0) & 1) ? one() : zero(); // 53
    zero();   // 54
    (year%4 == 0) ? one() : zero(); // 55
    zero();   // 56
    isdst==1 ? one() : zero(); // 57
    isdst==1 ? one() : zero(); // 58
    marker(); // 59

    minute ++;
    if (minute == 60) {
      hour ++;
      minute = 0;
    }
  }

  // Turn off PWM
  pwmWrite (18, 0);
}
