#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2); 


const int button_pins[]   = {13, 12, 11, 10};
const int potentio_pins[] ={A0,A1,A2,A3};
const int led_pins[]      = {9, 8, 7, 6};
const int nombre_players  = 4;


int potentio_values[]={0,0,0,0};

int buzzer_game_eleminated_players[]={0,0,0,0};

const int buzzer=3;

const int ok_button = 5;
const int select_btn=4;

const int back_btn=2;
int previous=-1;

int cursor =0;
int child_cursor=0;
int grand_child_cursor=0;

int a_game_is_on=0;

int random_number=0;
unsigned long previous_millis = 0;


int hearing=0;
int index_last_presser=-1;

void setup() {
  
  Serial.begin(9600);
  
 
  for (int i = 0; i < nombre_players; i++) {
    pinMode(button_pins[i], INPUT_PULLUP);
  }
  pinMode(ok_button, INPUT_PULLUP);
  pinMode(select_btn, INPUT_PULLUP);
  pinMode(back_btn, INPUT_PULLUP);

  for (int i = 0; i < nombre_players; i++) {
    pinMode(led_pins[i], OUTPUT);
    digitalWrite(led_pins[i], LOW); 
  }
  pinMode(buzzer, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  randomSeed(analogRead(A0));

  show_main_menu();
  

}

void loop() {
  if (digitalRead(select_btn) == LOW && previous==-1 ) 
  {
    move_cursor();
  }
    if (digitalRead(back_btn) == LOW)
  {
    go_back();
  }


  if (digitalRead(ok_button) == LOW || previous!=-1) {
    switch (cursor) {  
      case 0:
          if (previous==-1)
          {
            show_buzzers_menu();
            previous=1;
          }
           // main menu was the previous 
          buzzers_game();
        break;
      case 1:

          if (previous==-1)
          {
            show_QCM_menu();
            previous=1;
          }
          
           // main menu was the previous 
          QCM_game();
        break;
   
    }
  }
   
}



void QCM_game()
{
  
    if (digitalRead(select_btn) == LOW && !a_game_is_on) {
    move_child_cursor();
    }
      if (digitalRead(ok_button) == LOW || a_game_is_on) {
        previous=11;
    switch (child_cursor) {  
      case 0:

          if(!a_game_is_on)
          {
            show_QCM_display_setup();
          }
          a_game_is_on=1;
          LIVE_QCM();
          
        break;
      case 1:
          a_game_is_on=1;
          TIMER_QCM();
        break;
   
    }
  }

}

void show_QCM_display_setup()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("plyr1:  ");
  lcd.print("plyr2:  ");
  lcd.setCursor(0,1);
  lcd.print("plyr3:  ");
  lcd.print("plyr4:  ");
}

void LIVE_QCM()
{
  for (int i=0;i<nombre_players;i++)
  {
    potentio_values[i]=analogRead(potentio_pins[i]);
    potentio_values[i]=map(potentio_values[i],0,1023,1,6);

  }
  
  lcd.setCursor(0,0);
  lcd.setCursor(6,0);lcd.print(potentio_values[0]);
  lcd.setCursor(14,0);lcd.print(potentio_values[1]);
  lcd.setCursor(0,1);
  lcd.setCursor(6,1);lcd.print(potentio_values[2]);
  lcd.setCursor(14,1);lcd.print(potentio_values[3]);
  delay(50);
  
}


void TIMER_QCM()
{


  
    lcd.clear();
    lcd.print("results in :");
    lcd.setCursor(14,0);
    lcd.print("s");
    for (int i=0;i<10;i++)
    {
      lcd.setCursor(12,0);
      lcd.print("  ");
      lcd.setCursor(12,0);
      lcd.print(10-i);
      delay(1000);
      
    }
    show_QCM_display_setup();

    for (int i=0;i<nombre_players;i++)
  {
    potentio_values[i]=analogRead(potentio_pins[i]);
    potentio_values[i]=map(potentio_values[i],0,1023,1,6);

  }
  lcd.setCursor(0,0);
  lcd.setCursor(6,0);lcd.print(potentio_values[0]);
  lcd.setCursor(14,0);lcd.print(potentio_values[1]);
  lcd.setCursor(0,1);
  lcd.setCursor(6,1);lcd.print(potentio_values[2]);
  lcd.setCursor(14,1);lcd.print(potentio_values[3]);
  a_game_is_on=0;

}








void buzzers_game()
{
      if (digitalRead(select_btn) == LOW && !a_game_is_on) {
    move_child_cursor();
    }
      if (digitalRead(ok_button) == LOW || a_game_is_on) {
        previous=12;
        switch (child_cursor) {  
          case 0:

              REFLEX_buzzers();
              
            break;
          case 1:
              
              QUIZ_buzzers();
            break;
      
        }
  }
}




void show_main_menu()
{
  cursor=0;
  lcd.clear();
  lcd.print("> buzzer");
  lcd.setCursor(0, 1);
  lcd.print("  QCM");
}
void show_QCM_menu()
{

  child_cursor=0;
  lcd.clear();
  lcd.print("> LIVE");
  lcd.setCursor(0, 1);
  lcd.print("  TIMER");

}
void show_buzzers_menu()
{

  child_cursor=0;
  lcd.clear();
  lcd.print("> REFLEX");
  lcd.setCursor(0, 1);
  lcd.print("  QUIZ");

}




void REFLEX_buzzers()
{
  if (!a_game_is_on)
  {
    tone(buzzer, 500, 150);
    delay(300);
    lcd.clear();
    lcd.print("waiting ...");
    lcd.setCursor(0, 1);
    lcd.print("OUT:");

    previous_millis=millis();
    random_number=(  random(3)*random(3)+random(2)  )*987;


    a_game_is_on=1;
    for (int j=0;j<4;j++)
    {
      buzzer_game_eleminated_players[j]=0;
      digitalWrite(led_pins[j], LOW);
    }
  }
  else {

      if((millis()-previous_millis)>random_number)
      {
        if (previous_millis)
        {
          lcd.setCursor(0, 0);
          tone(buzzer, 1000, 50); // son unique
          lcd.print("WINNER : ?      ");
  
          previous_millis=0;
        }

        for(int i=0;i<4;i++)
        {
          if(digitalRead(button_pins[i])==LOW)
          {
            
            if (buzzer_game_eleminated_players[i]!=1)
            {
              digitalWrite(led_pins[i], HIGH);
              a_game_is_on=0;

              lcd.setCursor(9, 0);lcd.print(i+1);
              
              delay(2000);

            }
            
          }
        }

      }
      else {
      {
        for(int i=0;i<4;i++)
        {
          if(digitalRead(button_pins[i])==LOW)
          {
            buzzer_game_eleminated_players[i]=1;
            lcd.setCursor(5+i*2, 1);lcd.print(i+1);
            if (all_players_eliminated())
            {
              GAME_OVER();
            }
            
          }
        }

      }
      }




  }
}

int all_players_eliminated()
{
  for (int i=0;i<4;i++)
  {
    if (buzzer_game_eleminated_players[i]==0)
    {
      return 0;
    }
  }

  for (int i=0;i<4;i++)
  {
    buzzer_game_eleminated_players[i]=0;
  }
  return 1;
}

void QUIZ_buzzers()
{
  if (!a_game_is_on)
  {
    
    
    lcd.clear();
    lcd.print("waiting ...");
    a_game_is_on=1;
    hearing=1;
    for (int j=0;j<4;j++)
    {
      buzzer_game_eleminated_players[j]=0;
      digitalWrite(led_pins[j], LOW);
    }
  }
  else {
  
    if (hearing)
    { 
        for(int i=0;i<4;i++)
        {
          if(digitalRead(button_pins[i])==LOW)
          {
            
            if (buzzer_game_eleminated_players[i]!=1)
            {
              tone(buzzer,150,150);
              digitalWrite(led_pins[i], HIGH);
              index_last_presser=i;
              hearing=0;
              show_keep_or_eliminate_menu();
            }
            
          }
        }
    }
    else {
          if (digitalRead(select_btn) == LOW  ) 
          {
          move_grand_child_cursor();
          }
          if (digitalRead(ok_button) == LOW ) 
          {
            digitalWrite(led_pins[index_last_presser] , LOW);
            if(grand_child_cursor==1)
            {
              buzzer_game_eleminated_players[index_last_presser]=1;
              if (all_players_eliminated())
              {
                GAME_OVER();
              }
              else 
              {
                lcd.clear();
                lcd.print("waiting ...");
                hearing=1;
              }
              
            }
            else {
                lcd.clear();
                lcd.print("waiting ...");
                hearing=1;
            }
            

            

          }
    
    }





  
  }
}



void show_keep_or_eliminate_menu()
{
  grand_child_cursor=0;
  lcd.clear();
  lcd.print(index_last_presser+1);
  lcd.print(" : > Keep");
  lcd.setCursor(0, 1);
  lcd.print("      Eliminate");
}






void GAME_OVER()
{
  a_game_is_on=0;
  lcd.clear();
  lcd.print("GAME OVER !");
  lcd.setCursor(0, 1);
  lcd.print("ok: restart");
  delay(1000);
}

void go_back()
{
  switch (previous){
    case 1:
      previous=-1;
      show_main_menu();
      break;
    case 11:
      previous=1;
      show_QCM_menu();
      break;
    case 12:
      previous=1;
      show_buzzers_menu();
      break;
  }
  a_game_is_on=0;
  for (int j=0;j<4;j++)
  {
    
    digitalWrite(led_pins[j], LOW);
  }
  delay(1000);
}



void move_cursor()
{
  lcd.setCursor(0,cursor);
  lcd.print(" ");
  cursor=1-cursor;
  
  lcd.setCursor(0,cursor);
  lcd.print(">");
  delay(1000);
}
void move_child_cursor()
{
  lcd.setCursor(0,child_cursor);
  lcd.print(" ");
  child_cursor=1-child_cursor;
  
  lcd.setCursor(0,child_cursor);
  lcd.print(">");
  delay(1000);
}
void move_grand_child_cursor()
{
  lcd.setCursor(4,grand_child_cursor);
  lcd.print(" ");
  grand_child_cursor=1-grand_child_cursor;
  
  lcd.setCursor(4,grand_child_cursor);
  lcd.print(">");
  delay(1000);
}




