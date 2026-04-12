/* R2D2 Project | STETH 2025*/ 
#include "DFRobot_DF2301Q.h"       // Voice recognition
#include <Servo.h>                 // Servo
#include <SoftwareSerial.h>        // Software serial
#include "DFRobotDFPlayerMini.h"   // MP3 player module (DFPlayer Mini)
#define Led 13 // Pin του LED

// Pins επικοινωνίας με DFPlayer Mini
static const uint8_t PIN_MP3_TX = 11; // Ard TX -> mp3 RX
static const uint8_t PIN_MP3_RX = 10; // Ard RX <- mp3 TX

// Timer variables
unsigned long lastActionTime = 0; // Αποθηκεύει το timestamp του τελευταίου action (millis)
const unsigned long idleTime = 20000; // Time required to go idle (30sec)

// Δημιουργία αντικειμένων
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;
Servo myservo;
DFRobot_DF2301Q_I2C asr;

// Πίνακας με τις διάρκειες των MP3 σε milliseconds (ms)
// Η θέση στον πίνακα αντιστοιχεί στο όνομα του αρχείου .mp3
const unsigned long mp3Durations[] = {
  0,      // Θέση 0 (δεν χρησιμοποιείται)
  10000,   // 1. Αγαπημένο τραγούδι
  11300,  // 2. Από πού ήρθες (80%)
  12300,  // 3. Από πού ήρθες (20%)
  3900,   // 4. Καλύτερος Jedi
  3000,   // 5. Τι γνώμη έχεις για τον Darth;
  7368,   // 6. Joke 1
  5808,   // 7. Joke 2
  11184,  // 8. Joke 3
  7776,   // 9. Joke 4
  5664,   // 10. Joke 5
  6744,   // 11. Joke 6
  6648,   // 12. Joke 7
  8112,   // 13. Joke 8
  22704,  // 14. Καλύτερος πιλότος
  18192,  // 15. Αληθινός R2D2;
  9360,   // 16. Μπορείς να χορέψεις;
  5400,   // 17. Αγαπημένος droid
  5928,   // 18. Millennium Falcon
  4008,   // 19. Πες μου μια ιστορία
  2232,   // 20. Αγαπημένη φράση
  2650,   // 21. Πριγκίπισσα Λία
  1368,   // 22. Δείξε μου ένα κόλπο
  3600,   // 23. Πιο τρομακτικός Sith
  3984,   // 24. Stormtroopers
  2800,   // 25. Τι ώρα είναι;
  0,	  // 26. Νο file
  0,	  // 27. Νο file
  0,	  // 28. Νο file
  0,	  // 29. Νο file
  2500,   // 30. idle Urgent Warn
  3500,   // 31. idle squeeks
  1500,   // 32. idle short
  2500,   // 33. idle scream-wreaw
  2500,   // 34. idle saber
  4500,   // 35. idle native
  3500,   // 36. idle native
  1500,   // 37. idle native
  1500,   // 38. idle native
  1500,   // 39. idle native
  2500,   // 40. idle native
  14500,  // 41. idle modem
  14500,  // 42. idle patrick
  5500,   // 43. idle vader saber
  3500,   // 44. idle laugh
  2500    // 45. idle detect
};

// SETUP
void setup() {
  Serial.begin(115200); // Έναρξη σειριακής επικοινωνίας για debugging (μέσω USB)
  randomSeed(analogRead(A0));  // <-- αυτό χρειάζεσαι
  softwareSerial.begin(9600); // Έναρξη σειριακής επικοινωνίας με το DFPlayer Mini
  pinMode(Led, OUTPUT); // Ορισμός του pin LED ως έξοδος
  digitalWrite(Led, LOW); // Βεβαιωνόμαστε ότι το LED είναι αρχικά σβηστό
  myservo.attach(6); // Δηλώνουμε το servo στο pin 6

  while (!(asr.begin())) { // Έναρξη αισθητήρα αναγνώρισης φωνής
    Serial.println("Αποτυχία σύνδεσης με τον αισθητήρα αναγνώρισης φωνής");
    delay(3000);
  }
  Serial.println("Ο αισθητήρας αναγνώρισης φωνής είναι έτοιμος");
  
  // Voice recog. settings
  asr.setVolume(4); // Ρύθμιση έντασης (1-7)
  asr.setMuteMode(0); // 1 = σίγαση
  asr.setWakeTime(255); // Διάρκεια αφύπνισης (0-255)
  Serial.print("Voice Recogn. wake time set to: ");
  Serial.println(asr.getWakeTime());

  // Έναρξη του DFPlayer Mini μία φορά κατά την εκκίνηση
  if (player.begin(softwareSerial)) {
    Serial.println("Το DFPlayer Mini συνδέθηκε επιτυχώς");
    player.volume(30); // Ρύθμιση έντασης (0-30)
  } else {
    Serial.println("Αποτυχία σύνδεσης με το DFPlayer Mini");
  }

  lastActionTime = millis(); // Σώζουμε το πρώτο timestamp μετά το boot
  Serial.print("Χρόνος μέχρι το boot (Σε ms): ");
  Serial.println(lastActionTime);
}

//Την καλούμε όταν θέλουμε να ανανεώσουμε το timestamp για το τελευταίο action
void updateTimer() {
  lastActionTime = millis();
}

void playMp3(int fileNumber) {
  // Έλεγχος ότι το fileNumber βρίσκεται εντός έγκυρου εύρους του πίνακα mp3Durations
if (fileNumber > 0 && fileNumber < (sizeof(mp3Durations) / sizeof(mp3Durations[0]))) {
    unsigned long duration = mp3Durations[fileNumber];
    
    digitalWrite(Led, HIGH);
    player.playMp3Folder(fileNumber);
    Serial.print("Αναπαραγωγή: ");
    Serial.print(fileNumber);
    Serial.print(" | Duration: ");
    Serial.println(duration);
    
    delay(duration);
    digitalWrite(Led, LOW);
    player.pause(); // Καλύτερα player.stop(); λέει το gpt
    updateTimer();
  }
}

//Main Loop
void loop() {
  int CMDID = asr.getCMDID(); // Get Command ID 

  switch (CMDID) {

    //BUILD IN COMMANDS
  case 103: // Αν η εντολή είναι... "Turn on the light"
    digitalWrite(Led, HIGH);
    Serial.println("Το φως άναψε (CMD 103)");
    updateTimer();
    break;

  case 104: // Αν η εντολή είναι.... "Turn off the light"
    digitalWrite(Led, LOW);
    Serial.println("Το φως έσβησε (CMD 104)");
    updateTimer();
    break;

    //CUSTOM COMMANDS
  
  // Αν η εντολή είναι... "Ποιο είναι το αγαπημένο σου τραγούδι;"
  case 5: playMp3(1); break;

  // Αν η εντολή είναι... "Από πού ήρθες;"
  case 6:  playMp3(random(1, 6) <= 4 ? 2 : 3); break;   //80% παίζει το 2, 20% το 3
  
  //"Ποιος είναι ο καλύτερος Jedi;"
  case 7: playMp3(4); break;

  //"Τι γνώμη έχεις για τον Darth;"
  case 8: playMp3(5); break;

  //Μπορείς να μου πείς ένα αστείο
  case 9: playMp3(random(6, 14)); break;

  // "Ποιος είναι ο καλύτερος πιλότος;"
  case 10: playMp3(14); break;

  //"Είσαι ο αληθινός R2D2;"
  case 11: playMp3(15); break;

  case 12: //"Μπορείς να χορέψεις;"
    Serial.println("Εντολή: Χορός / Κίνηση Κεφαλιού");
	myservo.write(120);
    delay(500);
    myservo.write(60);
    delay(500);
    myservo.write(90);
    playMp3(16);
    break;

  //"Ποιος είναι ο αγαπημένος σου droid;"
  case 13: playMp3(17); break;

  //"Τι γνώμη έχεις για το Millennium Falcon;"
  case 14: playMp3(18); break;

  //"Μπορείς να μου πεις μια ιστορία;"   			-->Note: Na ginei pes mou mia istoria
  case 15: playMp3(19); break;
   
   //"Ποια είναι η αγαπημένη σου φράση;"
  case 16: playMp3(20); break;

  //"Τι γνώμη έχεις για την Πριγκίπισσα Λία;"   	-->Note: Να αφαιρεθεί
  case 17: playMp3(21); break;

  // "Μπορείς να μου δείξεις ένα κόλπο;"   			-->Note: Να γίνει Δείξε μου ένα κολπο
  case 18: playMp3(22); break;
	
  //"Ποιος είναι ο πιο τρομακτικός Sith;"
  case 19: playMp3(23); break;

  //"Τι γνώμη έχεις για τους Stormtroopers;"
  case 20: playMp3(24); break;

  //"Τι ώρα είναι;"
  case 21: playMp3(25); break;
  
  //Dokimasame to stop playing CMID 93 me  player.pause(); den paizei logo delay  

  default:
    if (CMDID != 0) {
      Serial.print("Ελήφθη command που δεν χρησιμοποιείτε: command ID: ");
      Serial.println(CMDID);
    }
  }
  if (millis() - lastActionTime > idleTime) {
    // Perform idle action (e.g., play a sound, move, etc.)
    Serial.println("Idle για 30 δεύτερα. Performing idle action.");

    // ... your idle action code goes here ...
    playMp3(random(30, 46)); // //player.playMp3Folder(30);
    // ... your idle action code goes here..

    lastActionTime = millis(); // Reset timer after idle action
  }
  delay(400);
}