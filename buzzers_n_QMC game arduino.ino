#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialisation de l'écran LCD avec adresse I2C 0x27, 16 colonnes et 2 lignes
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Définition des broches pour les boutons, potentiomètres et LEDs
const int button_pins[]   = {13, 12, 11, 10};  // Boutons des joueurs
const int potentio_pins[] = {A0, A1, A2, A3};  // Potentiomètres
const int led_pins[]      = {9, 8, 7, 6};      // LEDs des joueurs
const int nombre_players  = 4;                 // Nombre de joueurs

int potentio_values[] = {0,0,0,0};             // Valeurs des potentiomètres

// Tableau pour gérer les joueurs éliminés dans le jeu des buzzers
int buzzer_game_eleminated_players[] = {0,0,0,0}; 

// Broches pour les boutons de contrôle et le buzzer
const int buzzer = 3; 
const int ok_button = 5;
const int select_btn = 4;
const int back_btn = 2;

int previous = -1;        // Variable pour suivre le menu précédent

// Variables pour la navigation dans les menus
int cursor = 0;           // Curseur principal
int child_cursor = 0;     // Curseur secondaire
int grand_child_cursor = 0; // Curseur tertiaire

int a_game_is_on = 0;     // Indicateur de jeu en cours
int random_number = 0;    // Nombre aléatoire pour le jeu des buzzers
unsigned long previous_millis = 0; // Pour mesurer les intervalles de temps

int hearing = 0;          // Mode écoute pour le jeu QUIZ
int index_last_presser = -1; // Index du dernier joueur ayant appuyé

void setup() {
  // Configuration des entrées/sorties
  for (int i = 0; i < nombre_players; i++) {
    pinMode(button_pins[i], INPUT_PULLUP); // Boutons en entrée avec pull-up
  }
  pinMode(ok_button, INPUT_PULLUP);
  pinMode(select_btn, INPUT_PULLUP);
  pinMode(back_btn, INPUT_PULLUP);

  for (int i = 0; i < nombre_players; i++) {
    pinMode(led_pins[i], OUTPUT);         // LEDs en sortie
    digitalWrite(led_pins[i], LOW);       // LEDs éteintes au démarrage
  }
  pinMode(buzzer, OUTPUT);               // Buzzer en sortie

  // Initialisation de l'écran LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  randomSeed(analogRead(A0));             // Initialisation du générateur aléatoire

  show_main_menu();                       // Affichage du menu principal
}

void loop() {
  // Gestion de la navigation dans les menus
  if (digitalRead(select_btn) == LOW && previous == -1) {
    move_cursor(); // Déplacement du curseur principal
  }
  
  if (digitalRead(back_btn) == LOW) {
    go_back(); // Retour au menu précédent
  }

  // Sélection d'une option ou gestion du sous-menu actif
  if (digitalRead(ok_button) == LOW || previous != -1) {
    switch (cursor) {
      case 0: // Jeu des buzzers
          if (previous == -1) {
            show_buzzers_menu(); // Affiche le sous-menu des buzzers
            previous = 1;
          }
          buzzers_game(); // Lance le jeu des buzzers
          break;
      case 1: // Jeu QCM
          if (previous == -1) {
            show_QCM_menu(); // Affiche le sous-menu QCM
            previous = 1;
          }
          QCM_game(); // Lance le jeu QCM
          break;
    }
  }
}

// Fonctions pour le jeu QCM
void QCM_game() {
  if (digitalRead(select_btn) == LOW && !a_game_is_on) {
    move_child_cursor(); // Déplacement dans le sous-menu QCM
  }
  
  if (digitalRead(ok_button) == LOW || a_game_is_on) {
    previous = 11;
    switch (child_cursor) {
      case 0: // Mode LIVE
          if(!a_game_is_on) {
            show_QCM_display_setup();
          }
          a_game_is_on = 1;
          LIVE_QCM(); // Lance le QCM en temps réel
          break;
      case 1: // Mode TIMER
          a_game_is_on = 1;
          TIMER_QCM(); // Lance le QCM avec timer
          break;
    }
  }
}

// Affichage de la configuration du QCM
void show_QCM_display_setup() {
  lcd.clear();
  lcd.print("plyr1:  plyr2:  ");
  lcd.setCursor(0,1);
  lcd.print("plyr3:  plyr4:  ");
}

// QCM en temps réel
void LIVE_QCM() {
  // Lecture et mapping des valeurs des potentiomètres
  for (int i=0; i<nombre_players; i++) {
    potentio_values[i] = analogRead(potentio_pins[i]);
    potentio_values[i] = map(potentio_values[i], 0, 1023, 1, 6);
  }
  
  // Affichage des valeurs
  lcd.setCursor(6,0); lcd.print(potentio_values[0]);
  lcd.setCursor(14,0); lcd.print(potentio_values[1]);
  lcd.setCursor(6,1); lcd.print(potentio_values[2]);
  lcd.setCursor(14,1); lcd.print(potentio_values[3]);
  delay(50);
}

// QCM avec timer
void TIMER_QCM() {
  lcd.clear();
  lcd.print("results in :");
  lcd.setCursor(14,0);
  lcd.print("s");
  
  // Compte à rebours
  for (int i=0; i<10; i++) {
    lcd.setCursor(12,0);
    lcd.print("  ");
    lcd.setCursor(12,0);
    lcd.print(10-i);
    delay(1000);
  }
  
  show_QCM_display_setup();
  
  // Lecture des valeurs après le timer
  for (int i=0; i<nombre_players; i++) {
    potentio_values[i] = analogRead(potentio_pins[i]);
    potentio_values[i] = map(potentio_values[i], 0, 1023, 1, 6);
  }
  
  // Affichage des résultats
  lcd.setCursor(6,0); lcd.print(potentio_values[0]);
  lcd.setCursor(14,0); lcd.print(potentio_values[1]);
  lcd.setCursor(6,1); lcd.print(potentio_values[2]);
  lcd.setCursor(14,1); lcd.print(potentio_values[3]);
  a_game_is_on = 0;
}

// Fonctions pour le jeu des buzzers
void buzzers_game() {
  if (digitalRead(select_btn) == LOW && !a_game_is_on) {
    move_child_cursor(); // Déplacement dans le sous-menu buzzers
  }
  
  if (digitalRead(ok_button) == LOW || a_game_is_on) {
    previous = 12;
    switch (child_cursor) {
      case 0: // Mode REFLEX
          REFLEX_buzzers();
          break;
      case 1: // Mode QUIZ
          QUIZ_buzzers();
          break;
    }
  }
}

// Affichage des menus
void show_main_menu() {
  cursor = 0;
  lcd.clear();
  lcd.print("> buzzer");
  lcd.setCursor(0, 1);
  lcd.print("  QCM");
}

void show_QCM_menu() {
  child_cursor = 0;
  lcd.clear();
  lcd.print("> LIVE");
  lcd.setCursor(0, 1);
  lcd.print("  TIMER");
}

void show_buzzers_menu() {
  child_cursor = 0;
  lcd.clear();
  lcd.print("> REFLEX");
  lcd.setCursor(0, 1);
  lcd.print("  QUIZ");
}

// Jeu REFLEX (le premier qui appuie gagne)
void REFLEX_buzzers() {
  if (!a_game_is_on) {
    // Initialisation du jeu
    tone(buzzer, 500, 150);
    delay(300);
    lcd.clear();
    lcd.print("waiting ...");
    lcd.setCursor(0, 1);
    lcd.print("OUT:");
    
    previous_millis = millis();
    random_number = (random(3)*random(3)+random(2)) * 987; // Génération d'un délai aléatoire
    
    a_game_is_on = 1;
    // Réinitialisation des joueurs éliminés et LEDs
    for (int j=0; j<4; j++) {
      buzzer_game_eleminated_players[j] = 0;
      digitalWrite(led_pins[j], LOW);
    }
  }
  else {
    if((millis()-previous_millis) > random_number) {
      if (previous_millis) {
        // Signal pour appuyer
        lcd.setCursor(0, 0);
        tone(buzzer, 1000, 50);
        lcd.print("WINNER : ?      ");
        previous_millis = 0;
      }
      
      // Détection du gagnant
      for(int i=0; i<4; i++) {
        if(digitalRead(button_pins[i]) == LOW) {
          if (buzzer_game_eleminated_players[i] != 1) {
            digitalWrite(led_pins[i], HIGH);
            a_game_is_on = 0;
            lcd.setCursor(9, 0); lcd.print(i+1);
            delay(2000);
          }
        }
      }
    }
    else {
      // Détection des joueurs qui appuient trop tôt
      for(int i=0; i<4; i++) {
        if(digitalRead(button_pins[i]) == LOW) {
          buzzer_game_eleminated_players[i] = 1;
          lcd.setCursor(5+i*2, 1); lcd.print(i+1);
          if (all_players_eliminated()) {
            GAME_OVER();
          }
        }
      }
    }
  }
}

// Vérifie si tous les joueurs sont éliminés
int all_players_eliminated() {
  for (int i=0; i<4; i++) {
    if (buzzer_game_eleminated_players[i] == 0) {
      return 0;
    }
  }
  
  // Réinitialisation si tous éliminés
  for (int i=0; i<4; i++) {
    buzzer_game_eleminated_players[i] = 0;
  }
  return 1;
}

// Jeu QUIZ (avec élimination)
void QUIZ_buzzers() {
  if (!a_game_is_on) {
    // Initialisation du jeu
    lcd.clear();
    lcd.print("waiting ...");
    a_game_is_on = 1;
    hearing = 1;
    for (int j=0; j<4; j++) {
      buzzer_game_eleminated_players[j] = 0;
      digitalWrite(led_pins[j], LOW);
    }
  }
  else {
    if (hearing) {
      // Détection du premier joueur à appuyer
      for(int i=0; i<4; i++) {
        if(digitalRead(button_pins[i]) == LOW) {
          if (buzzer_game_eleminated_players[i] != 1) {
            tone(buzzer,150,150);
            digitalWrite(led_pins[i], HIGH);
            index_last_presser = i;
            hearing = 0;
            show_keep_or_eliminate_menu(); // Menu pour garder/éliminer
          }
        }
      }
    }
    else {
      // Gestion du menu d'élimination
      if (digitalRead(select_btn) == LOW) {
        move_grand_child_cursor();
      }
      
      if (digitalRead(ok_button) == LOW) {
        digitalWrite(led_pins[index_last_presser], LOW);
        if(grand_child_cursor == 1) {
          // Élimination du joueur
          buzzer_game_eleminated_players[index_last_presser] = 1;
          if (all_players_eliminated()) {
            GAME_OVER();
          }
          else {
            lcd.clear();
            lcd.print("waiting ...");
            hearing = 1;
          }
        }
        else {
          // Le joueur est gardé
          lcd.clear();
          lcd.print("waiting ...");
          hearing = 1;
        }
      }
    }
  }
}

// Affichage du menu garder/éliminer
void show_keep_or_eliminate_menu() {
  grand_child_cursor = 0;
  lcd.clear();
  lcd.print(index_last_presser+1);
  lcd.print(" : > Keep");
  lcd.setCursor(0, 1);
  lcd.print("      Eliminate");
}

// Affichage du Game Over
void GAME_OVER() {
  a_game_is_on = 0;
  lcd.clear();
  lcd.print("GAME OVER !");
  lcd.setCursor(0, 1);
  lcd.print("ok: restart");
  delay(1000);
}

// Retour au menu précédent
void go_back() {
  switch (previous) {
    case 1: // Retour au menu principal
      previous = -1;
      show_main_menu();
      break;
    case 11: // Retour au menu QCM
      previous = 1;
      show_QCM_menu();
      break;
    case 12: // Retour au menu buzzers
      previous = 1;
      show_buzzers_menu();
      break;
  }
  
  // Réinitialisation des LEDs
  a_game_is_on = 0;
  for (int j=0; j<4; j++) {
    digitalWrite(led_pins[j], LOW);
  }
  delay(1000);
}

// Fonctions de déplacement des curseurs
void move_cursor() {
  lcd.setCursor(0,cursor);
  lcd.print(" ");
  cursor = 1-cursor;
  lcd.setCursor(0,cursor);
  lcd.print(">");
  delay(1000);
}

void move_child_cursor() {
  lcd.setCursor(0,child_cursor);
  lcd.print(" ");
  child_cursor = 1-child_cursor;
  lcd.setCursor(0,child_cursor);
  lcd.print(">");
  delay(1000);
}

void move_grand_child_cursor() {
  lcd.setCursor(4,grand_child_cursor);
  lcd.print(" ");
  grand_child_cursor = 1-grand_child_cursor;
  lcd.setCursor(4,grand_child_cursor);
  lcd.print(">");
  delay(1000);
}
