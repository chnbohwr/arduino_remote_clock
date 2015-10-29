
#include <IRremote.h>
#include <Wire.h>
#include <Sodaq_DS3231.h>
#include <LiquidCrystal_I2C.h>

int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
//make degree dign
uint8_t degree[8]  = {
  140, 146, 146, 140, 128, 128, 128, 128
};
//day
char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
//datetime define
DateTime now;

bool backlightStatu = false;
int nowmillis = 0;
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  Wire.begin();
  rtc.begin();
  now = rtc.now();
  lcd.init();
  lcd.createChar(0, degree);
  nowmillis = millis();
  showLove();
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value > 16800000) {

    } else if (results.value == 16732845) {
      // button 9
      changeBacklght();
    } else if (results.value == 16753245) {
      changeDatetime('Y', 1);
    } else if (results.value == 16736925) {
      changeDatetime('Y', -1);
    } else if (results.value == 16720605) {
      changeDatetime('M', 1);
    } else if (results.value == 16712445) {
      changeDatetime('M', -1);
    } else if (results.value == 16769055) {
      changeDatetime('D', 1);
    } else if (results.value == 16754775) {
      changeDatetime('D', -1);
    } else if (results.value == 16738455) {
      changeDatetime('H', 1);
    } else if (results.value == 16750695) {
      changeDatetime('H', -1);
    } else if (results.value == 16724175) {
      changeDatetime('m', 1);
    } else if (results.value == 16718055) {
      changeDatetime('m', -1);
    } else if (results.value == 16716015) {
      changeDatetime('d', 1);
    } else if (results.value == 16726215) {
      changeDatetime('d', -1);
    }
    irrecv.resume(); // Receive the next value
  }

  int mil = millis();
  if ((mil - nowmillis) > 6000) {
    nowmillis = mil;
    printInformation();
  } else if (mil < nowmillis) {
    nowmillis = 0;
  }

}

void changeBacklght() {
  if (backlightStatu == true) {
    backlightStatu = false;
    lcd.noBacklight();
  } else {
    backlightStatu = true;
    lcd.backlight();
  }
}
void printInformation() {
  now = rtc.now();
  printNowTime();
  printNowTemp();
  printNowDate();
  printNowDay();
}
void printNowTime() {
  lcd.setCursor(0, 1);
  String now_time = parseDateTime(now.hour()) + ":" + parseDateTime(now.minute());
  lcd.print( now_time);
}
void printNowTemp() {
  lcd.setCursor(9, 1);
  lcd.print(rtc.getTemperature(), 1);
  lcd.write(0);
  lcd.print("C");
}

void printNowDate() {
  lcd.setCursor(0, 0);
  String now_date = parseDateTime(now.year()) + "/" + parseDateTime(now.month()) + "/" + parseDateTime(now.date());
  lcd.print(now_date);
}

void printNowDay() {
  lcd.setCursor(11, 0);
  lcd.print( weekDay[now.dayOfWeek()]);
}

String parseDateTime(int number) {
  String rt;
  if (number < 10) {
    rt = "0" + String(number);
  } else {
    rt = String(number);
  }
  return rt;
}

void changeDatetime(char type, int number) {
  long timestamp;
  lcd.clear();
  switch (type) {
    case 'Y': {
        DateTime dt(now.year() + number, now.month(), now.date(), now.hour(), now.minute(), 0, now.dayOfWeek());
        timestamp = dt.getEpoch();
        rtc.setEpoch(timestamp);
      }
      break;
    case 'M': {
        int nextMonth = now.month() + number;
        DateTime ddt(now.year() , nextMonth, now.date(), now.hour(), now.minute(), 0, now.dayOfWeek());
        timestamp = ddt.getEpoch();
        rtc.setEpoch(timestamp);
      }
      break;
    case 'D': {
        int nextDate = now.date() + number;
        DateTime ddt(now.year() , now.month(), nextDate, now.hour(), now.minute(), 0, now.dayOfWeek());
        timestamp = ddt.getEpoch();
        rtc.setEpoch(timestamp);
      }
      break;
    case 'H': {
        int nexthour = now.hour() + number;
        DateTime ddt(now.year() , now.month(), now.date(), nexthour, now.minute(), 0, now.dayOfWeek());
        timestamp = ddt.getEpoch();
        rtc.setEpoch(timestamp);
      }
      break;
    case 'm': {
        int nextminute = now.minute() + number;
        DateTime ddt(now.year() , now.month(), now.date(), now.hour(), nextminute, 0, now.dayOfWeek());
        timestamp = ddt.getEpoch();
        rtc.setEpoch(timestamp);
      }
      break;
    case 'd': {
        int nextday = now.dayOfWeek() + number;
        if (nextday > 6 || nextday < 0) {
          nextday = 0;
        }
        Serial.println(nextday);
        DateTime ddt(now.year() , now.month(), now.date(), now.hour(), now.minute(), 0, nextday);
        rtc.setDateTime(ddt); //Adjust date-time as defined 'dt' above
      }
      break;
  }
  now = rtc.now();
  printInformation();
}

void changeYear(int y) {
  lcd.clear();
  Serial.println(y);
  DateTime dt(now.year() + y, now.month(), now.date(), now.hour(), now.minute(), 0, now.dayOfWeek());
  long timestamp = dt.getEpoch();
  rtc.setEpoch(timestamp);
  printInformation();
}

void showLove() {
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello, Raccoon");
  lcd.setCursor(0, 1);
  lcd.print("Pig Love You");
  delay(2000);
  lcd.noBacklight();
  lcd.clear();
  printInformation();
}
