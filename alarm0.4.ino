#include <LiquidCrystal.h>
#include <Keypad.h>

//Board components
const int backlight = 8;
const int notif = 10;
const int alarm = 9;

//Sensors
const int doorA = 42;
const int doorB = 43;
const int doorC = 44;
const int doorD = 45;

const int window1 = 46;
const int window2 = 47;
const int window3 = 48;
const int window4 = 49;
const int window5 = 50;
const int window6 = 51;
const int window7 = 52;
const int window8 = 53;

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns

char keypressed;
char keymap[ROWS][COLS]=
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};
//Code that shows the the keypad connections to the arduino terminals
byte rowPins[ROWS] = {38, 36, 34, 32}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {39, 37, 35, 33}; //connect to the column pinouts of the kpd         
//initializes an instance of the Keypad class
Keypad kpd = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);


LiquidCrystal lcd(12, 11, 22, 23, 24, 25);

/*-------------------------------VARIABLES------------------------------------*/
String password="2580"; //Variable to store the current password
String tempPassword=""; //Variable to store the input password
int doublecheck;    //Check twice the new passoword
boolean armed = false;  //Variable for system state (armed:true / unarmed:false)
boolean input_pass;   //Variable for input password (correct:true / wrong:false)
boolean storedPassword = true;
boolean changedPassword = false;
boolean checkPassword = false;
int i = 1; //variable to index an array
/*----------------------------------------------------------------------------*/

int door = 0;
int onoff = 0;    
int count = 0;
int valv = 5000;
int Val = 0;
int fa = 1;
int co = 0;
int bone = 0;
int alarms = 0;
int pos = 0;
int waiting = 0;
int systemArm = 0;
int passchange = false;
int Position = 0;
int doorop = false;


long interval = 20000;
long previousMillis = 0; 


void setup() {

  pinMode(backlight, OUTPUT);
 // digitalWrite(backlight, HIGH);
  pinMode(doorA, INPUT);
  digitalWrite(doorA, HIGH);

  delay(2000);
  lcd.begin(16, 2);  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("  Initializing  ");
  delay(2000);

  
  tone(notif, 3500);
  delay(150);
  noTone(notif);

  lcd.clear();
  
  changePasscode();

  

}

void loop() {
  door = digitalRead(doorA);
  keypressed = kpd.getKey();


  if(systemArm == 1){
    lcd.setCursor(0,0);
    lcd.write("ECE DEPT   ARMED");
    
     if(door == HIGH){
      systemAlarm(); 
     }
    
  }
  if(systemArm == 0){
    lcd.setCursor(0,0);
    lcd.write("ECE DEPT   READY");
    lcd.setCursor(0,1);
    
    if(door == LOW){
      doorclosed();
     }
     if(door == HIGH){
      dooropen();   
     }
     
  }


  
  
  
  
  if(keypressed != NO_KEY){
    lcd.setCursor(0,1);
    //lcd.print(systemArm);
   // delay(3000);
    digitalWrite(backlight, HIGH);
    tone(notif,300,75);
    switch(keypressed){
       case('A'):
        if(systemArm == 1){
          lcd.setCursor(0,0);
          lcd.write("   Cannot Arm   ");
          lcd.setCursor(0,1);
          lcd.write(" Already  Armed ");
          delay(2000);
        }
        else if (systemArm == 0){
       
          arm();
        }
        break;
        
       case('B'):
        if(systemArm == 1){
          panic();
        }
        else if (systemArm == 0){
 
          panic();
        }
        break;
        
       case('C'):
        if(systemArm == 1){
          lcd.setCursor(0,0);
          lcd.write(" System   Armed ");
          lcd.setCursor(0,1);
          lcd.write("Disarm to Modify");
          delay(2000);
        }
        else if (systemArm == 0){
      
          changePasscode();
        }
        break;
        
       case('D'):
        if(systemArm == 0){
          lcd.setCursor(0,0);
          lcd.write(" Cannot  Disarm ");
          lcd.setCursor(0,1);
          lcd.write("System not Armed");
          delay(2000);
        }
        else if (systemArm == 1){
  
          disarm();
        }
        break;
       default:
        break;
    }
    
  }
  digitalWrite(backlight, LOW);
   
  
 
  

}

void doorclosed(){
    noTone(notif);
    lcd.setCursor(0,1);
    lcd.write("                ");
    co = 0;
}
void dooropen(){
      digitalWrite(backlight, HIGH);
      lcd.setCursor(0,1);
      lcd.write("   Door  Ajar   ");

      
      
         if (co == 0){
          tone(notif, 3500);
          delay(125);
          noTone(notif);
          delay(75);
          tone(notif, 3500);
          delay(125);
          noTone(notif);
          delay(75);
          tone(notif, 3500);
          delay(125);
          noTone(notif);
          delay(75);
          co++;
         }
         digitalWrite(backlight, LOW);
         return;
  }

//Change current password
void changePasscode(){
  unsigned long currentMillis = millis();
  previousMillis = currentMillis;  
  //long interval = 10000;
  
  retry: //label for goto
  tempPassword="";
  lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Current Password");
    
  i=2;
  while(!changedPassword){
    unsigned long currentMillis = millis();
    //Waiting for current password
    keypressed = kpd.getKey();   //Read pressed keys
    lcd.setCursor(0,0);
    lcd.print("Current Password");
    lcd.setCursor(0,1);
    lcd.print(">");
   
    if (keypressed != NO_KEY){
      previousMillis = currentMillis;
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ){
        tempPassword += keypressed;
        lcd.setCursor(i,1);
        lcd.print("*");
        i++;
        tone(notif,300,75);      
      }
      else if (keypressed=='#'){
        break;
      }
      else if (keypressed == '*'){
        i=1;
        if (password==tempPassword){
          storedPassword=false;
          tone(notif,500,200);
          newPassword();          //Password is correct, so call the newPassword function
          break;
        }
        else{               //Try again
          tempPassword="";
          tone(notif,500,200);
          delay(300);
          tone(notif,500,200);
          delay(300);
          goto retry;
        }
      }
    }
    else if(currentMillis - previousMillis > interval){
          lcd.setCursor(0,1);
          lcd.write("                ");
          return;
        }
  }
}
String firstpass;
//Setup new password
void newPassword(){
  
  long interval = 10000;
  unsigned long currentMillis = millis();
  previousMillis = currentMillis; 
  
  tempPassword="";
  changedPassword=false;
  lcd.clear();
  i=2;
  while(!storedPassword){
    
    keypressed = kpd.getKey();   //Read pressed keys
    if (doublecheck==0){
      lcd.setCursor(0,0);
      lcd.print("SET NEW PASSWORD");
      lcd.setCursor(0,1);
      lcd.print(">");
    }
    else{
      lcd.setCursor(0,0);
      lcd.print("One more time...");
      lcd.setCursor(0,1);
      lcd.print(">");
    }
    if (keypressed != NO_KEY){
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ){
        tempPassword += keypressed;
        lcd.setCursor(i,1);
        lcd.print("*");
        i++;
          tone(notif,300,75);
      }
      else if (keypressed=='#'){
        break;
      }
      else if(currentMillis - previousMillis > interval){
          lcd.setCursor(0,1);
          lcd.write("                ");
          break;
        }
      else if (keypressed == '*'){
        if (doublecheck == 0){
          firstpass=tempPassword;
          doublecheck=1;
          newPassword();
        }
        if (doublecheck==1){
          doublecheck=0;
          if (firstpass==tempPassword){
            i=1;
            firstpass="";
            password = tempPassword; // New password saved
            tempPassword="";//erase temp password
            lcd.setCursor(0,0);
            lcd.print("PASSWORD CHANGED");
            lcd.setCursor(0,1);
            lcd.print("----------------");
              storedPassword=true;
              tone(notif,500,200);
              delay(2000);
              lcd.clear();
              break;
          }
          else{
            firstpass="";
            newPassword();
          }
        }
      } 
    }
  }
}

//Panic
void panic(){
  
  PanicBegin:
  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  int i = 0;
  int j=2;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("  PANIC ACTIVE  ");
  systemArm = 1;
  keypressed = kpd.getKey();
  do{
    valv = valv - fa;
    keypressed = kpd.getKey();
      //tone(alm, valv);
      tone(notif, valv);
      digitalWrite(backlight, HIGH);
      tone(notif, valv);
      if (valv <= 1500) {
        valv = 5000;
      }
  }while(keypressed != 'B');

 while(!checkPassword){
  unsigned long currentMillis = millis(); 
  
    lcd.setCursor(0,0);
    lcd.print(" Enter Password ");
    lcd.setCursor(0,1);
    lcd.print(">");
    keypressed = kpd.getKey();
   //Read pressed keys
    if (keypressed != NO_KEY){    //Accept only numbers and * from keypad
      previousMillis = currentMillis;  
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ){
        tempPassword += keypressed;
        lcd.setCursor(j,1);
        lcd.print("*");       //Put * on lcd
        j++;
        tone(notif,300,75);     //Button tone
      }
      else if (keypressed == 'A'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Disabled    ");
        delay(500);
        goto PanicBegin;
      }
      else if (keypressed == 'C'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Disabled    ");
        delay(500);
        goto PanicBegin;
      }
      else if (keypressed == 'D'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Disabled    ");
        delay(500);
        goto PanicBegin;
      }
      else if (keypressed=='#'){
        goto PanicBegin;
      }
      else if (keypressed == '*' || keypressed == 'B'){  //Check for password
        if (password==tempPassword){//If it's correct...
          lcd.clear();      
          lcd.setCursor(0,0);
          lcd.print("***   AKCR   ***");
          lcd.setCursor(0,1);
          lcd.print("                ");
          systemArm = 0;
          tone(notif, 500);    //Play a tone while door is unlocked
          //digitalWrite(lock, HIGH);//unlock the door for 5 seconds
          delay(5000);
          noTone(notif);
          break;
        }
        else{           //if it's false, retry
          tempPassword="";
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("   Try Again   ");
          delay(500);
          goto PanicBegin;
        }
      }
    }
     else if((unsigned long)(currentMillis - previousMillis) >= interval){
          lcd.setCursor(0,1);
          lcd.write("                ");
          systemArm = true;
          goto PanicBegin;
        }
  }
  
  noTone(notif);
  digitalWrite(backlight, LOW);
  return;
}

//Disarm
void disarm(){
  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  lockAgain: //goto label
  tempPassword="";
  i=2;
  
  //digitalWrite(lock, LOW);
  while(!checkPassword){
    unsigned long currentMillis = millis();
    lcd.setCursor(0,0);
    lcd.print(" Enter Password ");
    lcd.setCursor(0,1);
    lcd.print(">");
    
    keypressed = kpd.getKey();   //Read pressed keys
    if (keypressed != NO_KEY){    //Accept only numbers and * from keypad
      previousMillis = currentMillis;
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ){
        tempPassword += keypressed;
        lcd.setCursor(i,1);
        lcd.print("*");       //Put * on lcd
        i++;
        tone(notif,300,75);     //Button tone
      }
      else if (keypressed == 'A'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Disabled    ");
        delay(500);
        goto lockAgain;
      }

      else if (keypressed == 'C'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Disabled    ");
        delay(500);
        goto lockAgain;
      } 
      
      else if (keypressed=='#'){
        break;
      }
      else if (keypressed == '*' || keypressed == 'D'){  //Check for password
        if (password==tempPassword){//If it's correct...
          lcd.clear();      
          lcd.setCursor(0,0);
          lcd.print("*** DISARMED ***");
          lcd.setCursor(0,1);
          lcd.print("                ");
          tone(notif, 500);    //Play a tone while door is unlocked
          //digitalWrite(lock, HIGH);//unlock the door for 5 seconds
          delay(5000);
          noTone(notif);
          systemArm = 0;
          break;
        }
        else{           //if it's false, retry
          tempPassword="";
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("   Try Again   ");
          delay(500);
          goto lockAgain;
        }
      }
    }
     else if((unsigned long)(currentMillis - previousMillis) >= interval){
          lcd.setCursor(0,1);
          lcd.write("                ");
          systemArm = 1;
          return;
        }
  }
}

void DisAlarm(){
  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  lockAgain: //goto label
  tempPassword="";
  i=2;
  
  //digitalWrite(lock, LOW);
  while(!checkPassword){
    unsigned long currentMillis = millis();
    lcd.setCursor(0,0);
    lcd.print(" Enter Password ");
    lcd.setCursor(0,1);
    lcd.print(">");
    
    keypressed = kpd.getKey();   //Read pressed keys
    if (keypressed != NO_KEY){    //Accept only numbers and * from keypad
      previousMillis = currentMillis;
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ){
        tempPassword += keypressed;
        lcd.setCursor(i,1);
        lcd.print("*");       //Put * on lcd
        i++;
        tone(notif,300,75);     //Button tone
      }
      else if (keypressed == 'A'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Disabled    ");
        delay(500);
        systemAlarm();
      }

      else if (keypressed == 'C'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Disabled    ");
        delay(500);
        systemAlarm();
      } 
      
      else if (keypressed =='#'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Disabled    ");
        delay(500);
        systemAlarm();
      }
      else if (keypressed == '*' || keypressed == 'D'){  //Check for password
        if (password==tempPassword){//If it's correct...
          lcd.clear();      
          lcd.setCursor(0,0);
          lcd.print("*** DISARMED ***");
          lcd.setCursor(0,1);
          lcd.print("                ");
          tone(notif, 500);    //Play a tone while door is unlocked
          //digitalWrite(lock, HIGH);//unlock the door for 5 seconds
          delay(5000);
          noTone(notif);
          systemArm = 0;
          break;
        }
        else{           //if it's false, retry
          tempPassword="";
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write("  INVALID  KEY  ");
          delay(500);
          systemAlarm();
        }
      }
    }
  }
}

//ALARM
void systemAlarm(){
  
  //key = kpd.getKey();

   if (count == 20){
          lcd.clear();
          lcd.setCursor(0,0); 
          lcd.write("  System Alarm  ");
          digitalWrite(backlight, HIGH);
          alarms = 1;
          count++;
        }
    if (count == 21){
      keypressed = kpd.getKey();
      if(keypressed == 'D'){
        DisAlarm();
      }
      else{
        lcd.setCursor(0,1); 
        lcd.write(" INVALID OPTION ");
      }
      valv = valv - fa;
      tone(alarm, valv);
      if (valv <= 1500) {
        valv = 5000;
      }

    }

    else{

      Val = 9 - count/2;
      kpd.getKey();
     
      lcd.setCursor(0,0); 
      lcd.write("  System Alarm  ");
      //checkPass();
      lcd.setCursor(19,2);
      lcd.print(Val); 
      tone(notif, 3500);
      digitalWrite(backlight, HIGH);  
      delay(250);
      digitalWrite(backlight, LOW); 
      noTone(notif);
      delay(250);
      

      count++;
    }
    co = 0;
        
}

void arm(){
  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  lockAgain: //goto label
  
  tempPassword="";
  lcd.clear();
  i=2;

  //digitalWrite(lock, LOW);
  while(!checkPassword){
    unsigned long currentMillis = millis();
    
    lcd.setCursor(0,0);
    lcd.print("   Arm System   ");
    lcd.setCursor(0,1);
    lcd.print(">");
    keypressed = kpd.getKey();
   //Read pressed keys
    if (keypressed != NO_KEY){    //Accept only numbers and * from keypad
      previousMillis = currentMillis;
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ){
        tempPassword += keypressed;
        lcd.setCursor(i,1);
        lcd.print("*");       //Put * on lcd
        i++;
        tone(notif,300,75);     //Button tone
      }
    

      else if (keypressed == 'D'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Disabled    ");
        delay(500);
        goto lockAgain;
      }

      else if (keypressed == 'C'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Disabled    ");
        delay(500);
        goto lockAgain;
      }

      else if (keypressed == 'B'){
        panic();
      }
      
      else if (keypressed=='#'){
        break;
      }
      
      
      else if (keypressed == '*' || keypressed == 'A'){  //Check for password
        if (password==tempPassword){//If it's correct...
          do{
            lcd.clear();      
            lcd.setCursor(0,0);
            lcd.print("***  ARMING  ***");
              if(waiting > 19){
                tone(notif, 1000);
              }
              else{
               digitalWrite(backlight, HIGH);
               kpd.getKey(); 
               tone(notif, 500);
              }
             if(pos < 4 ){ 
              if(pos % 2 == 0){
                lcd.setCursor(0,1);
                lcd.write("    Exit Now    ");
              }
              else{
                lcd.setCursor(0,1);
                lcd.write("                ");    
              }
          lcd.setCursor((3 - pos),1);
          lcd.write("<");
          lcd.setCursor((12 + pos),1);
          lcd.write(">");
          kpd.getKey();
          delay(500);
          pos++;
            }
            else{
              pos = 0;
              lcd.setCursor((pos),1);
              lcd.write("   <");
              lcd.setCursor((12 + pos),1);
              lcd.write(">   ");
              lcd.setCursor(4,1);
              lcd.write("Exit Now");
              kpd.getKey(); 
              delay(500);
              pos++;
            }

          digitalWrite(backlight, LOW); 
          noTone(notif);
          
          delay(500);
          waiting++; 
      }while(waiting != 24);
          systemArm = 1;
          break;
      }
        else{           //if it's false, retry
          tempPassword="";
          tone(notif,500,200);
          delay(300);
          tone(notif,500,200);
          delay(300);
          systemArm = 0;
          goto lockAgain;
          
        }
      }
    }
    else if((unsigned long)(currentMillis - previousMillis) >= interval){
          lcd.setCursor(0,1);
          lcd.write("                ");
          systemArm = 0;
          return;
        }
  }
}

