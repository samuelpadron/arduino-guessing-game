Game Implementation
-------------------

The game implemented here is a guessing game where the player must guess a direction. This direction is chosen randomly at the start and after
the player makes a successful guess. On the webpage, the player can make the go for either 5, 10, or 15 rounds, with 5 rounds set as the default. The player's score can also be reset for continous play. The player can keep playing after they reach the target score, but will only be rewarded when he reaches the target score and no more (no rewards for bonus fun).

The player can only guess four directions in this version: "UP", "RIGHT", "DOWN", "LEFT", as the hand gesture sensor did not prove to be very consistent for the others, and provided a very poor and irritating user experience.

Once the player reaches the target score, a signal is sent to reward the player, which in this case is just moving the motor.

Shields and Connections
-----------------------

The shields that areused in this project are the OLED Display shield, the Wemos Motor shield version 2.0.0, and the PAJ7620 hand gesture recognition sensor along with a breadboard and some jumper wires to connect them to the D1 Mini. A WiFi connection is used in order to access the webpage.
