#include <LiquidCrystal.h>
#include <Wire.h>
#include <math.h>

String real_hour = "22"; 
String real_minute = "32"; 
String real_period = "PM"; 


String alarm_hour = "22"; 
String alarm_minute = "33"; 
String alarm_period = "PM"; 

#define button1 2
#define button2 3
#define button3 8
#define button4 9
#define one_minute 10000
#define buzzer 10
#define temperature_pin A0

float temperature_data;
int temperature_celcius;
int temperature_fahrenheit;
float voltage;
bool show_temperature_celcius = true;

bool pPeriod = true;
bool alarm_set = false;
bool alarm_bell = false;
unsigned long alarm_timing;
bool buzzerZ = false;

unsigned long snoozing;
int snooze_duration = 300000;
bool snoozeFunction = false;

unsigned long real_time; 

unsigned long b1_press;
bool b1_press_long = true;
bool b1_pressed = false;


bool b2_press_long = true;
bool b2_pressed = false;
unsigned long b2_press;


LiquidCrystal lcd(12, 11, 5, 4, 7, 6);

byte temperature_symbols[] = {
  0x04,
  0x0A,
  0x0A,
  0x0E,
  0x0E,
  0x1F,
  0x1F,
  0x0E
};

byte block_symbol[] = {
  0x04,
  0x04,
  0x04,
  0x04,
  0x04,
  0x04,
  0x04,
  0x04
};

byte alarm_on[] = {
  0x04,
  0x0E,
  0x0E,
  0x0E,
  0x1F,
  0x1F,
  0x00,
  0x04
};

byte alarm_off[] = {
  0x04,
  0x0A,
  0x0A,
  0x0A,
  0x11,
  0x1F,
  0x00,
  0x04
};

byte symbol[] = {
  0x05,
  0x02,
  0x05,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};





void setup()
{
  lcd.begin(16, 2);

  Serial.begin(9600);
  Serial.println("Real time alarm clock starting!");

  lcd.setCursor(0, 0);
  delay(300);
  lcd.clear();

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  real_time = millis(); 
}

void loop() {

  //print clk on lcd pPeriod
  lcd.setCursor(0, 0);
  lcd.print(real_hour);
  lcd.print(":");
  lcd.print(real_minute);
  lcd.print(" ");
  lcd.print(real_period);

  lcd.setCursor(0, 1);
  lcd.print(alarm_hour);
  lcd.print(":");
  lcd.print(alarm_minute);
  lcd.print(" ");
  lcd.print(alarm_period);



  if (alarm_set == false) {
    lcd.createChar(0, alarm_off);
    lcd.setCursor(15, 1);
    lcd.write(byte(0));
    delay(300);
  }

  else {
    lcd.createChar(1, alarm_on);
    lcd.setCursor(15, 1);
    lcd.write(byte(1));
    delay(300);

  }

//Temperature Sensor Part
  if (show_temperature_celcius == true) {

    temperature_data = analogRead(temperature_pin);
    voltage = temperature_data * (5000 / 1024.0);
    temperature_celcius = (voltage - 500) / 10;

    lcd.setCursor(12, 0);
    lcd.print(temperature_celcius);
    lcd.createChar(3, symbol);
    lcd.setCursor(14, 0);


    lcd.write(byte(3));
    lcd.setCursor(15, 0);
    lcd.print("C");
  }

  if (show_temperature_celcius == false) {

    temperature_data = analogRead(temperature_pin);
    voltage = temperature_data * (5000 / 1024.0);
    temperature_celcius = (voltage - 500) / 10;
    temperature_fahrenheit = (temperature_celcius * 1.8) + 32;

    lcd.setCursor(12, 0);
    lcd.print(temperature_fahrenheit);
    lcd.createChar(3, symbol);
    lcd.setCursor(14, 0);


    lcd.write(byte(3));
    lcd.setCursor(15, 0);

    lcd.print("F");
  }

  lcd.createChar(4, block_symbol);
  lcd.setCursor(9, 0);
  lcd.write(byte(4));
  lcd.setCursor(9, 1);
  lcd.write(byte(4));
  lcd.createChar(5, temperature_symbols);
  lcd.setCursor(10, 0);
  lcd.write(byte(5));


  if (digitalRead(button1) == 1) {


    if (b1_press_long == true) {
      b1_press_long = false;
      b1_pressed = true;
      b1_press = millis();
    }


  }

  if (digitalRead(button1) == 0 && b1_pressed == true) {

    
    if (millis() - b1_press < 5500) {
      Serial.println("Short press for button 1(Switch to 24h mode)");
      pPeriod = not pPeriod;
      bool first_state = true;
      bool second_state = true;

      if (real_period == "PM" && first_state) { 
        real_hour = String(real_hour.toInt() + 12);
        real_period = "  ";
        first_state = false;

      }

      if (real_period == "AM" && first_state) { 
        real_period = "  ";
        first_state = false;

      }


      if (real_period == "  " && first_state) {

        if (real_hour.toInt() < 12) { 
          real_period = "AM";
        }

        if (real_hour.toInt() >= 12) {
          real_hour = String(real_hour.toInt() - 12);

          if (real_hour.toInt() + 1 < 10) {
            real_hour = "0" + real_hour;
          }

          real_period = "PM";
        }


        first_state = false;

      }

      

      if (alarm_period == "PM" && second_state) { 
        alarm_hour = String(alarm_hour.toInt() + 12);
        alarm_period = "  ";
        second_state = false;

      }

      if (alarm_period == "AM" && second_state) { 
        alarm_period = "  ";
        second_state = false;

      }


      if (alarm_period == "  " && second_state) {

        if (alarm_hour.toInt() < 12) { 
          alarm_period = "AM";
        }

        if (alarm_hour.toInt() >= 12) {
          alarm_hour = String(alarm_hour.toInt() - 12);

          if (alarm_hour.toInt() + 1 < 10) {
            alarm_hour = "0" + alarm_hour;
          }

          alarm_period = "PM";
        }


        second_state = false;


      }


      delay(300);

    }
    
    else {
      int position_ = 1;

      while (true) {

        if (digitalRead(button1) == 1) {
          position_ = position_ + 1;

          if (position_ > 3 && pPeriod == true) {
            position_ = 1;
          }

          if (position_ > 2 && pPeriod == false) {
            position_ = 1;
          }

          if (position_ < 1) {
            position_ = 3;
          }

          delay(300);
        }

        if (digitalRead(button2) == 1) {
          if (position_ == 1) {
            real_hour = String(real_hour.toInt() - 1);

            if (real_hour.toInt() < 0) {
              real_hour = "0";
            }

            if (real_hour.toInt() < 10) {
              real_hour = "0" + real_hour;
            }

            delay(300);

          }

          if (position_ == 2) {
            real_minute = String(real_minute.toInt() - 1);

            if (real_minute.toInt() < 0) {
              real_minute = "0";
            }

            if (real_minute.toInt() < 10) {
              real_minute = "0" + real_minute;
            }

            delay(300);
          }

          if (position_ == 3) {
            for (int i = 0; i < 1; i++) {
              if (real_period == "AM") {
                real_period = "PM";
                break;
              }

              if (real_period == "PM") {
                real_period = "AM";
                break;
              }
            }
          }
        }


        if (digitalRead(button3) == 1) {
          if (position_ == 1) {
            real_hour = String(real_hour.toInt() + 1);

            if (pPeriod == true && real_hour.toInt() >= 12) {
              real_hour = "11";
            }

            if (real_hour.toInt() < 10) {
              real_hour = "0" + real_hour;
            }

            if (pPeriod == false && real_hour.toInt() >= 24) {
              real_hour = "24";
            }

            delay(300);
          }

          if (position_ == 2) {
            real_minute = String(real_minute.toInt() + 1);

            if (real_minute.toInt() >= 59) {
              real_minute = "59";
            }

            if (real_minute.toInt() < 10) {
              real_minute = "0" + real_minute;
            }

            delay(300);
          }

          if (position_ == 3) {
            for (int i = 0; i < 1; i++) {
              if (real_period == "AM") {
                real_period = "PM";
                break;
              }

              if (real_period == "PM") {
                real_period = "AM";
                break;
              }
            }
          }
        }


        if  (position_ == 1) {
          Serial.println("!!Real clock hour setting!!");
          lcd.setCursor(0, 0);
          lcd.print("  ");
          delay(300);
          lcd.setCursor(0, 0);
          lcd.print(real_hour);
        }

        if (position_ == 2) {
          Serial.println("!!Real clock minute setting!!");
          lcd.setCursor(3, 0);
          lcd.print("  ");
          delay(300);
          lcd.setCursor(3, 0);
          lcd.print(real_minute);
        }

        if (position_ == 3 ) {
          Serial.println("!!Real Clock time period!!");
          lcd.setCursor(6, 0);
          lcd.print("  ");
          delay(300);
          lcd.setCursor(6, 0);
          lcd.print(real_period);
        }

        delay(300);

        if (digitalRead(button4) == 1) {
          break;
          delay(300);
        }
      }

    }
    b1_pressed = false;
    b1_press_long = true;
  }

  

  if (digitalRead(button2) == 1) {

    if (b2_press_long == true) {
      b2_press_long = false;
      b2_pressed = true;
      b2_press = millis();
    }

  }

  if (digitalRead(button2) == 0 && b2_pressed == true) {

   
    if (millis() - b2_press < 3500) {
      Serial.println("Button2 is pressed(short)");
      alarm_set = not alarm_set;
      b2_press_long = true;
      b2_pressed = false;
      delay(300);

    }

    

    else {
      int position_ = 1;

      while (true) {

        if (digitalRead(button1) == 1) {
          position_ = position_ + 1;

          if (position_ > 3 && pPeriod == true) {
            position_ = 1;
          }

          if (position_ > 2 && pPeriod == false) {
            position_ = 1;
          }

          if (position_ < 1) {
            position_ = 3;
          }

          delay(300);
        }

        if (digitalRead(button2) == 1) {
          if (position_ == 1) {
            alarm_hour = String(alarm_hour.toInt() - 1);

            if (alarm_hour.toInt() < 0) {
              alarm_hour = "0";
            }

            if (alarm_hour.toInt() < 10) {
              alarm_hour = "0" + alarm_hour;
            }

            delay(300);

          }

          if (position_ == 2) {
            alarm_minute = String(alarm_minute.toInt() - 1);

            if (alarm_minute.toInt() < 0) {
              alarm_minute = "0";
            }

            if (alarm_minute.toInt() < 10) {
              alarm_minute = "0" + alarm_minute;
            }

            delay(300);
          }

          if (position_ == 3) {
            for (int i = 0; i < 1; i++) {
              if (alarm_period == "AM") {
                alarm_period = "PM";
                break;
              }

              if (alarm_period == "PM") {
                alarm_period = "AM";
                break;
              }
            }
          }
        }


        if (digitalRead(button3) == 1) {
          if (position_ == 1) {
            alarm_hour = String(alarm_hour.toInt() + 1);

            if (pPeriod == true && alarm_hour.toInt() >= 12) {
              alarm_hour = "11";
            }

            if (alarm_hour.toInt() < 10) {
              alarm_hour = "0" + alarm_hour;
            }

            if (pPeriod == false && alarm_hour.toInt() >= 24) {
              alarm_hour = "24";
            }

            delay(300);
          }

          if (position_ == 2) {
            alarm_minute = String(alarm_minute.toInt() + 1);

            if (alarm_minute.toInt() >= 59) {
              alarm_minute = "59";
            }

            if (alarm_minute.toInt() < 10) {
              alarm_minute = "0" + alarm_minute;
            }

            delay(300);
          }

          if (position_ == 3) {
            for (int i = 0; i < 1; i++) {
              if (alarm_period == "AM") {
                alarm_period = "PM";
                break;
              }

              if (alarm_period == "PM") {
                alarm_period = "AM";
                break;
              }
            }
          }
        }


        if  (position_ == 1) {
          Serial.println("Alarm Clock Hour Selection");
          lcd.setCursor(0, 1);
          lcd.print("  ");
          delay(300);
          lcd.setCursor(0, 1);
          lcd.print(alarm_hour);
        }

        if (position_ == 2) {
          Serial.println("Alarm Clock Minute Selection");
          lcd.setCursor(3, 1);
          lcd.print("  ");
          delay(300);
          lcd.setCursor(3, 1);
          lcd.print(alarm_minute);
        }

        if (position_ == 3 ) {
          Serial.println("Alarm Clock Period Selection");
          lcd.setCursor(6, 1);
          lcd.print("  ");
          delay(300);
          lcd.setCursor(6, 1);
          lcd.print(alarm_period);
        }

        delay(150);

        if (digitalRead(button4) == 1) {
          break;
          delay(300);
        }
      }

      b2_pressed = false;
      b2_press_long = true;
    }

  }


  if (digitalRead(button3) == 1) {
    
    Serial.println("Button3 is pressed");
    if (digitalRead(buzzer)==1) {
    
      digitalWrite(buzzer, LOW);
      alarm_bell=false;
      alarm_set=false;

    }

    else {
      show_temperature_celcius = not show_temperature_celcius;
   
    }
    delay(300);
  }

  
  if ((millis() - real_time) >= one_minute) { 

    real_time = millis();

    real_minute = String(real_minute.toInt() + 1);


    if (real_minute.toInt() < 10) {

      real_minute = "0" + real_minute;
      //Serial.println(real_minute);
    }

    if (real_minute.toInt() >= 60) { 
      real_minute = "00";

      real_hour = String(real_hour.toInt() + 1);

      if (real_hour.toInt()  < 10) {

        real_hour = "0" + real_hour;
      }
      //Serial.println(real_minute);


      
      if ((real_hour.toInt() >= 12) && (real_period == "AM")) {
        real_hour = "00";
        real_period = "PM";
      }

      if ((real_hour.toInt() >= 12) && (real_period == "PM")) {
        real_hour = "00";
        real_period = "AM";
      }

      if ((real_hour.toInt() >= 24) && (real_period == "  ")) {
        real_hour = "00";
      }
    }
    delay(100);
  }

  

  if (alarm_set == true && alarm_bell == false && snoozeFunction == false) {
    if (real_hour == alarm_hour && real_minute == alarm_minute) {
      if (pPeriod == false) {
        Serial.println("ALERT! ALARM!");
        alarm_bell = true;
        tone(buzzer,500,300);
        alarm_timing = millis();
        delay(100);
        buzzerZ = true;
        

      }

      else {
        if (real_period == alarm_period) {
          Serial.println("ALERT! ALARM!");
          alarm_bell = true;
          alarm_timing = millis();
          tone(buzzer,500,300);
          delay(100);


        }
      }
    }

  }

  if (alarm_bell == true && snoozeFunction == false) {
    if (millis() - alarm_timing > one_minute) {
      digitalWrite(buzzer, LOW);
      alarm_bell = false;
      snoozeFunction = false;
    }

    else {
      digitalWrite(buzzer, HIGH);
      if (digitalRead(button4) == 1) {
        Serial.println("Button 4 is pressed! Snooze Mode activated!");
        snoozeFunction = true;
        alarm_bell = false;
        snoozing = millis();
        digitalWrite(buzzer, LOW);
        delay(300);
      }

    }
  }

  if (snoozeFunction == true) {

    if (millis() - snoozing > snooze_duration) {
      alarm_timing = millis();
      delay(150);
      Serial.println("Alarm activated!");
      alarm_bell = true;
      tone(buzzer,500,300);
      snoozeFunction = false;
      delay(300);

    }
  }


}