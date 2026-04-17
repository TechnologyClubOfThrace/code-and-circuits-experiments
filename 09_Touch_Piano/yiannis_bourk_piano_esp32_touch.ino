#include <Arduino.h> 

#define BUZZER_PIN 25 //  pin buzzer

int threshold = 40;  // Όριο ευαισθησίας για τις capacitive touch εισόδους

volatile bool do_note_down  = false;
volatile bool re_note_down  = false;
volatile bool mi_note_down  = false;
volatile bool fa_note_down  = false;
volatile bool sol_note_down = false;
volatile bool la_note_down  = false;
volatile bool si_note_down  = false;

void do_note_interrupt() { 
  if(do_note_down == true) return;
  do_note_down = true;
  tone(BUZZER_PIN, 262 , 0);
}

void re_note_interrupt() { 
  if(re_note_down == true) return;
  re_note_down = true;
  tone(BUZZER_PIN, 294 , 0);
}

void mi_note_interrupt() { 
  if(mi_note_down == true) return;
  mi_note_down = true;
  tone(BUZZER_PIN, 330 , 0);
}

void fa_note_interrupt() { 
  if(fa_note_down == true) return;
  fa_note_down = true;
  tone(BUZZER_PIN, 349 , 0);
}

void sol_note_interrupt() { 
  if(sol_note_down == true) return;
  sol_note_down = true;
  tone(BUZZER_PIN, 392 , 0);
}

void la_note_interrupt() { 
  if(la_note_down == true) return;
  la_note_down = true;
  tone(BUZZER_PIN, 440 , 0);
}

void si_note_interrupt() { 
  if(si_note_down == true) return;
  si_note_down = true;
  tone(BUZZER_PIN, 494 , 0);
}




void set_all_notes_down(){
  noTone(BUZZER_PIN);
  do_note_down  = false;
  re_note_down  = false;
  mi_note_down  = false;
  fa_note_down  = false;
  sol_note_down = false;
  la_note_down  = false;
  si_note_down  = false;
}

void setup()
{
    Serial.begin(115200); 
    delay(1000); 
    pinMode(BUZZER_PIN, OUTPUT);
    Serial.println("ESP32 Touch Test\n");

    touchAttachInterrupt(T0, do_note_interrupt,  threshold);   
    touchAttachInterrupt(T3, re_note_interrupt,  threshold);
    touchAttachInterrupt(T4, mi_note_interrupt,  threshold);   
    touchAttachInterrupt(T5, fa_note_interrupt,  threshold);  
    touchAttachInterrupt(T6, sol_note_interrupt, threshold);  
    touchAttachInterrupt(T7, la_note_interrupt,  threshold);  
    touchAttachInterrupt(T8, si_note_interrupt,  threshold);  
}
void loop()
{
  if(
    (do_note_down  == true && touchRead(T0) > 50) || 
    (re_note_down  == true && touchRead(T3) > 50) ||
    (mi_note_down  == true && touchRead(T4) > 50) ||
    (fa_note_down  == true && touchRead(T5) > 50) ||
    (sol_note_down == true && touchRead(T6) > 50) ||
    (la_note_down  == true && touchRead(T7) > 50) ||
    (si_note_down  == true && touchRead(T8) > 50) 
    )
   {
    set_all_notes_down();
    //noTone(BUZZER_PIN);
    //do_note_down = false;
  }


    //noTone(BUZZER_PIN);

}