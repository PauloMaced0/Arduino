#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const byte WIDTH = 16; // LCD width
const byte HEIGHT = 2; // LCD height

// LCD has 32 matrices 5:8 
const byte MATRIX_WIDTH = 5; 
const byte MATRIX_HEIGHT = 8;

// Snake properties
const byte MAX_LENGTH = 28; // Maximum length of the snake
byte snakeX[MAX_LENGTH][2]; // X's-coordinate of each segment of the snake
byte snakeY[MAX_LENGTH][2]; // Y's-coordinate of each segment of the snake
byte snakeLength = 1; // Initial length of the snake
byte snakeDirection = 0; // Direction of the snake (0: right, 1: down, 2: left, 3: up)
int snakeSpeed = 500;
bool gameOver = false;
bool gameWon = false;

typedef byte matrix[MATRIX_HEIGHT];

struct coordinates{
  byte lcd_x; 
  byte lcd_y; 
  byte matrix_x; 
  byte matrix_y; 
};

coordinates food;

matrix gameMatrix[2][16];

void setup() {
  lcd.begin(WIDTH, HEIGHT);
  lcd.clear();
  //randomSeed(analogRead(A0)); // Seed the random number generator
  resetGame();
}

void loop() {
  if (!gameOver && !gameWon) {
    handleInput();
    moveSnake();
    checkCollision();
    drawGame();
    delay(snakeSpeed); // Snake speed
  }
}

void resetGame() {
  snakeLength = 1;
  snakeDirection = 0;

  // Initial snake X's coordinates 
  snakeX[0][0] = 0;
  snakeX[0][1] = 0;
  // Initial snake Y's coordinates 
  snakeY[0][0] = 0;
  snakeY[0][1] = 0;

  generateFood();
  gameOver = false;
}

void generateFood() {
  food.lcd_x = random(0, WIDTH);
  food.matrix_x = random(0, MATRIX_WIDTH);
  food.lcd_y = random(0, HEIGHT);
  food.matrix_y = random(0, MATRIX_HEIGHT);
}

void resetGameMatrix() {
  // Constant reset operation (256 op)
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 16; j++) {
      for (int k = 0; k < 8; k++) {
        gameMatrix[i][j][k] = 0;
      }
    }
  }
}

void buildGameMatrices(){
  resetGameMatrix();
  gameMatrix[food.lcd_y][food.lcd_x][food.matrix_y] |= B10000 >> food.matrix_x;

  for(int i = 0; i < snakeLength; i++){
    byte* m = gameMatrix[snakeY[i][0]][snakeX[i][0]];
    m[snakeY[i][1]] |= B10000 >> snakeX[i][1];
  }
}

void handleInput() {
  // Input handling code here
  // snakeDirection = random(0, 4); Direction of the snake (0: right, 1: down, 2: left, 3: up)
  snakeDirection = 0;
}

void moveSnake() {
  // Move the snake
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i][0] = snakeX[i - 1][0];
    snakeX[i][1] = snakeX[i - 1][1];
    snakeY[i][0] = snakeY[i - 1][0];
    snakeY[i][1] = snakeY[i - 1][1];
  }
  
  // Update the snake's head position based on the direction
  switch (snakeDirection) {
    case 0: // Right
      if (snakeX[0][0] >= 15 && snakeX[0][1] >= 4) { // x limit reached
        snakeX[0][0] = 0;
        snakeX[0][1] = 0;
      } else if (snakeX[0][1] >= 4) { // x cell limit reached
        snakeX[0][0] += 1;
        snakeX[0][1] = 0;
      } else {
        snakeX[0][1] += 1;
      }
      break;
    case 1: // Down
      if (snakeY[0][0] >= 1 && snakeY[0][1] >= 7) { // x limit reached
        snakeY[0][0] = 0;
        snakeY[0][1] = 0;
      } else if (snakeY[0][1] >= 7) { // x cell limit reached
        snakeY[0][0] += 1;
        snakeY[0][1] = 0;
      } else {
        snakeY[0][1] += 1;
      }
      break;
    case 2: // Left
      if (snakeX[0][0] <= 0 && snakeX[0][1] <= 0) { // x limit reached
        snakeX[0][0] = 15;
        snakeX[0][1] = 4;
      } else if (snakeX[0][1] <= 0) { // x cell limit reached
        snakeX[0][0] -= 1;
        snakeX[0][1] = 4;
      } else {
        snakeX[0][1] -= 1;
      }
      break;
    case 3: // Up
      if (snakeY[0][0] <= 0 && snakeY[0][1] <= 0) { // x limit reached
        snakeY[0][0] = 1;
        snakeY[0][1] = 7;
      } else if (snakeY[0][1] <= 0) { // x cell limit reached
        snakeY[0][0] -= 1;
        snakeY[0][1] = 7;
      } else {
        snakeY[0][1] -= 1;
      }
      break;
  }
}

void checkCollision() {
  // Check if the snake hits itself
  for (int i = 1; i < snakeLength && gameOver != true; i++) {
    if (snakeX[i][0] == snakeX[0][0] && snakeX[i][1] == snakeX[0][1] && snakeY[i][0] == snakeY[0][0] && snakeY[i][1] == snakeY[0][1]) {
      gameOver = true;
    }
  }
  // Check if the snake eats the food
  if (snakeX[0][0] == food.lcd_x && snakeX[0][1] == food.matrix_x && snakeY[0][0] == food.lcd_y && snakeY[0][1] == food.matrix_y) {
    // Increase the snake's length
    snakeLength++;
    if(snakeLegth >= MAX_LENGTH){
      gameWon = true;
    }
    // Generate new food
    generateFood();
  }
}

void drawGame() {
  lcd.clear();

  // Sets every matrix with snake coordinates and food 
  buildGameMatrices();

  byte* m;
  // Draw food
  m = gameMatrix[food.lcd_y][food.lcd_x];
  lcd.createChar(7, m);
  lcd.setCursor(food.lcd_x, food.lcd_y);
  lcd.write((byte)7);
  
  // Draw snake's head
  m = gameMatrix[snakeY[0][0]][snakeX[0][0]];
  lcd.createChar(0, m);
  lcd.setCursor(snakeX[0][0], snakeY[0][0]);
  lcd.write((byte)0);

  byte reg = 6;
  byte lastCell[2] = {snakeY[0][0],snakeX[0][0]};

  for(int i = 1; i < snakeLength && reg >= 1; i++){
    if (lastCell[0] != snakeY[i][0] || lastCell[1] != snakeX[i][0]) {
      m = gameMatrix[snakeY[i][0]][snakeX[i][0]];
      lcd.createChar(reg, m);
      lcd.setCursor(snakeX[i][0], snakeY[i][0]);
      lcd.write((byte)reg);
      reg--;
      lastCell[0] = snakeY[i][0];
      lastCell[1] = snakeX[i][0];
      if(reg == 0){
        gameWon = true; // when is out of register to assign lcd cells(hardware restriction)
      }
    }
  }

  // Check if the game is over
  if (gameOver) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over!");
  } else if (gameWon) {
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Game Won!");
  }
}
