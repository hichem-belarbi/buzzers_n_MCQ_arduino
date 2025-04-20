# 🎮 Arduino Buzzer & QCM Game  
👨‍💻 By HICHEM belarbi

## 🇬🇧 English Version

## 📌 Description
This Arduino-based project brings two interactive game modes: a **QCM game** using rotary selectors and a **Buzzer game** with real-time reflex and quiz mechanics.

### 🧠 QCM Game:
Players use rotary selectors to choose answers. Two game modes are supported:

- **Live Mode:** Player selections are read and displayed live based on the current position of each selector.
- **Timer Mode:** Players have 10 seconds to lock in their answer. After the countdown, the system freezes inputs and shows the results.

This mode is adaptable to both interactive quizzes and timed challenges to test player reactivity.

### 🚨 Buzzer Game:
The buzzer game includes two modes:

- **Reflex Mode:**  
  A buzzer sounds after a random delay. Pressing a button *before* the buzzer disqualifies the player. The first player to press their button *after* the sound wins.

- **Quiz Mode:**  
  The game master asks a question. The system listens for buzzes. The first player to buzz is displayed on screen and gets to answer. The master can accept the answer or eliminate the player.

---

## 🎛️ Features
- 🌀 Rotary encoders for QCM answer selection
- ⏱️ Countdown timer for timed QCM mode
- 🔘 Buzzers with elimination logic
- 📺 LCD/Display shows active player or selections
- 🎵 Sound feedback using buzzer module

---

## 🔧 Hardware Used
- Arduino Uno / Mega
- Rotary selectors (1 per player)
- Push buttons (buzzers)
- Buzzer (audio feedback)
- LCD / OLED / 7-segment display
-  LED indicators for each player

---

## 🚀 Installation and Usage
1. Upload the code to your Arduino using the Arduino IDE.
2. Connect components as described in the wiring diagram (see `pics/` folder).
3. Select the game mode using onboard buttons or a menu interface.
4. Follow the on-screen instructions to start playing!

---

## ⚖️ License
This project is open-source. Feel free to use, modify, and share it!

---

## 🇫🇷 Version Française

## 📌 Description
Ce projet basé sur Arduino propose deux jeux interactifs : un **QCM avec sélecteurs rotatifs** et un **jeu de buzzers**, intégrant des mécaniques de réflexe et de quiz.

### 🧠 Jeu QCM :
Chaque joueur utilise un sélecteur rotatif pour choisir sa réponse. Deux modes sont disponibles :

- **Mode Live :**  
  Les réponses sont lues en direct, en fonction de la position actuelle du sélecteur.

- **Mode Timer :**  
  Les joueurs disposent de 10 secondes pour sélectionner leur réponse. Une fois le temps écoulé, les réponses sont figées et les résultats affichés.

Un mode idéal pour des quiz animés ou des compétitions chronométrées.

### 🚨 Jeu Buzzers :
Deux modes de jeu sont inclus :

- **Mode Réflexe :**  
  Un bip est émis après un délai aléatoire. Si un joueur appuie *avant* le bip, il est éliminé et ne peut plus participer. Le premier à appuyer *après* le bip gagne.

- **Mode Quiz :**  
  Le maître du jeu pose une question. Le système attend un signal. Le premier joueur à appuyer est affiché à l’écran. Le maître peut alors l’autoriser à répondre ou l’éliminer.

---

## 🎛️ Fonctionnalités
- 🌀 Sélecteurs rotatifs pour réponses QCM
- ⏱️ Compte à rebours pour mode chronométré
- 🔘 Buzzers avec système d’élimination
- 📺 Affichage LCD pour le suivi des réponses
- 🎵 Feedback sonore via buzzer

---

## 🔧 Matériel Utilisé
- Arduino Uno / Mega
- Sélecteurs rotatifs (1 par joueur)
- Boutons-poussoirs (buzzers)
- Buzzer (son)
- Écran LCD / OLED / 7 segments
- LEDs  pour signalisation

---

## 🚀 Installation et Utilisation
1. Téléversez le code sur l’Arduino via l’IDE Arduino.
2. Connectez les composants selon le schéma disponible (`dossier pics`).
3. Sélectionnez le mode de jeu via boutons ou menu.
4. Suivez les instructions à l’écran pour jouer !

---

## ⚖️ Licence
Projet open-source. Vous pouvez l’utiliser et le modifier librement.
