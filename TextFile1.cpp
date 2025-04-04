#include "iGraphics.h"
#include <math.h> // for sqrt

// Constants for screen size
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;

// Images
int img[15];  // increased size for animation frames
int i = 4;    // starting frame index of fire animation

// Player ship
int shipX = 100;
int shipY = 100;

// Enemy red ship
int redShipX = 50;
int redShipY = 50;

int sharkSpeed = 2;
int minDistance = 50;

// Bullet variables
bool bulletFired = false;
double bulletX, bulletY;
double bulletSpeed = 5;
double bulletDx, bulletDy;

// Attack area variables
bool areaAttackActive = false;
int attackRadius = 100; // size of the attack area

// Health system
int playerLife = 20; // starts with 20 life bars

// Flags to prevent repeated life reduction in the same area attack
bool alreadyDamagedByArea = false;

// Slave ships
struct SlaveShip {
	int x, y;
	bool bulletFired;
	double bulletX, bulletY;
	double bulletDx, bulletDy;
};
SlaveShip slaveShips[100]; // Array to store slave ships

int slaveShipCount = 0;  // Counter to keep track of how many slave ships are present


// Function to make red ship follow player

void followShip() {
	double dx = shipX - redShipX;
	double dy = shipY - redShipY;
	double distance = sqrt(dx * dx + dy * dy);

	if (distance > minDistance) {
		double stepX = (dx / distance) * sharkSpeed;
		double stepY = (dy / distance) * sharkSpeed;

		redShipX += stepX;
		redShipY += stepY;
	}

	// Move slave ships and make them fire bullets
	for (int j = 0; j < slaveShipCount; j++) { // Changed 'i' to 'j' karon error astesilo 
		double dx = shipX - slaveShips[j].x;
		double dy = shipY - slaveShips[j].y;
		double distance = sqrt(dx * dx + dy * dy);

		if (distance > minDistance) {
			double stepX = (dx / distance) * sharkSpeed;
			double stepY = (dy / distance) * sharkSpeed;

			slaveShips[j].x += stepX;
			slaveShips[j].y += stepY;
		}

		// Fire bullets
		if (!slaveShips[j].bulletFired) {
			double dx = shipX - slaveShips[j].x;
			double dy = shipY - slaveShips[j].y;
			double distance = sqrt(dx * dx + dy * dy);

			if (distance > 0) { // Ensure distance is not zero
				slaveShips[j].bulletX = slaveShips[j].x + 50;
				slaveShips[j].bulletY = slaveShips[j].y + 25;

				slaveShips[j].bulletDx = (dx / distance) * bulletSpeed;
				slaveShips[j].bulletDy = (dy / distance) * bulletSpeed;
				slaveShips[j].bulletFired = true;
			}
		}
	}
}


// Function to spawn slave ships

void spawnSlaveShip() {
	if (slaveShipCount < 100) {  // Limit the number of slave ships to 100 (joto dibo toto ashte thekbe )
		// Spawn outside the screen
		int xPos;
		if (rand() % 2 == 0) {
			xPos = -100; // Assign -100 if the random number is even
		}
		else {
			xPos = SCREEN_WIDTH + 100; // Assign SCREEN_WIDTH + 100 if the random number is odd
		}

		int yPos = rand() % SCREEN_HEIGHT; // Random vertical position

		slaveShips[slaveShipCount].x = xPos;
		slaveShips[slaveShipCount].y = yPos;
		slaveShips[slaveShipCount].bulletFired = false;
		slaveShipCount++;
	}
}


// Function to fire a bullet from red ship toward player

void fireBullet() {
	if (!bulletFired) {
		bulletFired = true;

		bulletX = redShipX + 50;
		bulletY = redShipY + 25;

		double dx = (shipX + 50) - bulletX;
		double dy = (shipY + 25) - bulletY;

		double distance = sqrt(dx * dx + dy * dy);
		if (distance == 0) distance = 1; // prevent division by zero

		bulletDx = (dx / distance) * bulletSpeed;
		bulletDy = (dy / distance) * bulletSpeed;
	}
}


// Move bullet forward toward target

void moveBullet() {
	if (bulletFired) {
		bulletX += bulletDx;
		bulletY += bulletDy;

		// Check for hit with player ship
		if (bulletX >= shipX && bulletX <= shipX + 100 &&
			bulletY >= shipY && bulletY <= shipY + 50) {

			bulletFired = false;

			if (playerLife > 0) {
				playerLife -= 1;
				printf("Bullet Hit! Life Remaining: %d\n", playerLife);
			}
		}

		// Out of screen bounds
		if (bulletX < 0 || bulletX > SCREEN_WIDTH || bulletY < 0 || bulletY > SCREEN_HEIGHT) {
			bulletFired = false;
		}
	}

	// Move bullets fired by slave ships
	for (int j = 0; j < slaveShipCount; j++) { // Changed 'i' to 'j'
		if (slaveShips[j].bulletFired) {
			slaveShips[j].bulletX += slaveShips[j].bulletDx;
			slaveShips[j].bulletY += slaveShips[j].bulletDy;

			// Check for hit with player ship
			if (slaveShips[j].bulletX >= shipX && slaveShips[j].bulletX <= shipX + 100 &&
				slaveShips[j].bulletY >= shipY && slaveShips[j].bulletY <= shipY + 50) {

				slaveShips[j].bulletFired = false;

				if (playerLife > 0) {
					playerLife -= 1;
					printf("Bullet Hit from Slave Ship! Life Remaining: %d\n", playerLife);
				}
			}

			// Out of screen bounds
			if (slaveShips[j].bulletX < 0 || slaveShips[j].bulletX > SCREEN_WIDTH || slaveShips[j].bulletY < 0 || slaveShips[j].bulletY > SCREEN_HEIGHT) {
				slaveShips[j].bulletFired = false;
			}
		}
	}
}


// Toggle the area attack on/off continuously

void toggleAreaAttack() {
	areaAttackActive = !areaAttackActive;

	if (areaAttackActive) {
		printf("Area Attack Started!\n");
		alreadyDamagedByArea = false; // reset damage flag
	}
	else {
		printf("Area Attack Ended!\n");
	}
}


// Draw Health Bar

void drawHealthBar() {
	int barWidth = 10;
	int barHeight = 20;
	int startX = 20;
	int startY = SCREEN_HEIGHT - 50;

	for (int j = 0; j < 20; j++) {
		if (j < playerLife) {
			iSetColor(0, 255, 0); // Green for remaining life
		}
		else {
			iSetColor(100, 100, 100); // Gray for lost life
		}

		iFilledRectangle(startX + j * (barWidth), startY, barWidth, barHeight);
	}

	// Draw border
	iSetColor(255, 255, 255);
	iRectangle(startX - 2, startY - 2, 20 * barWidth + 4, barHeight + 4);
}


// Main Drawing Function

void iDraw() {
	iClear();

	// Draw player ship
	iShowImage(shipX, shipY, 100, 50, img[0]);

	// Draw red enemy ship
	iShowImage(redShipX, redShipY, 100, 50, img[2]);

	// Draw slave ships
	for (int j = 0; j < slaveShipCount; j++) { // Changed 'i' to 'j'
		iShowImage(slaveShips[j].x, slaveShips[j].y, 100, 50, img[11]);
	}

	// Draw bullet if fired
	if (bulletFired) {
		iSetColor(255, 0, 0);
		iFilledCircle(bulletX, bulletY, 5);
	}

	// Draw bullets from slave ships
	for (int j = 0; j < slaveShipCount; j++) { // Changed 'i' to 'j'
		if (slaveShips[j].bulletFired) {
			iSetColor(255, 0, 0);
			iFilledCircle(slaveShips[j].bulletX, slaveShips[j].bulletY, 5);
		}
	}

	// Draw Area Attack circle and animation
	if (areaAttackActive) {
		int centerX = redShipX + 50;
		int centerY = redShipY + 25;

		// Draw the circular area
		iSetColor(255, 100, 0);
		iCircle(centerX, centerY, attackRadius);

		// Play fire animation inside the circle
		iShowImage(centerX - attackRadius, centerY - attackRadius, attackRadius * 2, attackRadius * 2, img[i]);

		// Loop fire frames
		i++;
		if (i > 10) i = 4;

		// Check if player is inside attack area
		double dx = (shipX + 50) - centerX;
		double dy = (shipY + 25) - centerY;
		double distance = sqrt(dx * dx + dy * dy);

		if (distance <= attackRadius && !alreadyDamagedByArea) {
			if (playerLife > 0) {
				playerLife -= 3;
				if (playerLife < 0) playerLife = 0;
				printf("Player Hit by Area Attack! Life Remaining: %d\n", playerLife);
				alreadyDamagedByArea = true; // prevent repeated damage during the same attack
			}
		}
	}

	// Draw Health Bar
	drawHealthBar();
}

// -----------------------------------------
// Keyboard and Mouse Handling
// -----------------------------------------
void iMouseMove(int mx, int my) {


}
void iPassiveMouseMove(int mx, int my) {

}
void iMouse(int button, int state, int mx, int my) {

}

void iKeyboard(unsigned char key) {


}

void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_UP) {
		shipY += 10;
	}
	if (key == GLUT_KEY_RIGHT) {
		shipX += 10;
	}
	if (key == GLUT_KEY_LEFT) {
		shipX -= 10;
	}
	if (key == GLUT_KEY_DOWN) {
		shipY -= 10;
	}
}


// Load images for ships and animation

void loadimage() {
	img[0] = iLoadImage("img/0005ship.png");
	img[1] = iLoadImage("img/0006ship.png");
	img[2] = iLoadImage("img/red ship .png");
	img[3] = iLoadImage("img/red ship2 .png");

	// Fire animation frames
	img[4] = iLoadImage("img/flm1.png");
	img[5] = iLoadImage("img/flm2.png");
	img[6] = iLoadImage("img/flm3.png");
	img[7] = iLoadImage("img/flm4.png");
	img[8] = iLoadImage("img/flm5.png");
	img[9] = iLoadImage("img/flm6.png");
	img[10] = iLoadImage("img/flm7.png");
	img[11] = iLoadImage("img/enemyShip1.png");
}


// Main Function

int main() {
	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Enemy Fire Tracking Ship with Area Attack and Health Bar");

	loadimage();

	iSetTimer(40, followShip);           // Enemy follows player and slave ships follow the red ship
	iSetTimer(1000, fireBullet);         // Fires bullet every second
	iSetTimer(20, moveBullet);           // Moves bullet
	iSetTimer(2000, toggleAreaAttack);   // Toggle area attack every 2 seconds
	iSetTimer(3000, spawnSlaveShip);     // Spawn a new slave ship every 3 seconds

	iStart();
	return 0;
}