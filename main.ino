///////////////////////////////////////////////////////  НАСТРОЙКИ  //////////////////////////////////////////////////////////////////////////////////////////////////////************
#define TIME_MOTOR 140 ////  IN SEC
#define POWER 250      ////  100 - 255
bool JERK = 0;         //// (0-без рывков || 1-с рывками)  работает рывками по 1/4 из TIME_MOTOR по N раза и с силой POWER с паузами в 0.5 сек
#define N 4            //// количество рывков
///////////////////////
#define Hours 9        //// нужное время
#define HoursSECOND 19 //// второе время
#define minuts 0
#define secunds 0
#define motorPin 2
int work = TIME_MOTOR * 1000;

#include "RTClib.h"

RTC_DS1307 rtc;
char daysOfTheWeek[7][23] = {"Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};

void setup()
{
    //  rtc.adjust(DateTime(2020, 8, 25, 3, 8, 0));
    // rtc.adjust(DateTime(2020, 7, 21, 3, 10, 0));
    pinMode(motorPin, OUTPUT);
    analogWrite(motorPin, 0);
    Serial.begin(57600);

#ifndef ESP8266
    while (!Serial)
        ; // wait for serial port to connect. Needed for native USB
#endif

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        abort();
    }

    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
        //rtc.adjust(DateTime(2020, 7, 20, 23, 19, 0));
        //    rtc.adjust(DateTime(2021, 8, 25, 13, 23, 11));
    }
}

void loop()
{
    //  delay (2000);
    //  digitalWrite(motorPin, 1);
    //  delay (2000);
    //  digitalWrite(motorPin, 0);
    //  delay (2000);
    //  digitalWrite(motorPin, 1);

    //  digitalWrite(motorPin, HIGH);
    //  delay(500);
    //  digitalWrite(motorPin, LOW);
    //  delay(500);
    //  digitalWrite(motorPin, HIGH);
    //  delay(500);
    //  digitalWrite(motorPin, LOW);
    //  delay(500);
    DateTime now = rtc.now();
    int h = now.hour();
    int m = now.minute();
    int s = now.second();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    delay(1000);

    if (h == Hours && m == minuts && s == secunds)
    {
        feed();
    }
    if (h == HoursSECOND && m == minuts && s == secunds)
    {
        feed();
    }
    //  delay (1000);
}

void feed()
{
    if (JERK)
    {
        int jerk = work / 4;
        if (jerk < 500)
        {
            jerk = jerk * 2;
        }
        for (int i = 0; i < N; i++)
        {
            analogWrite(motorPin, POWER);
            delay(jerk);
            analogWrite(motorPin, 0);
            delay(jerk);
        }
    }
    if (!JERK)
    {
        Serial.println("eating time!");
        digitalWrite(motorPin, 1);
        delay(work);
        digitalWrite(motorPin, 0);
    }
}