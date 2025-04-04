#include "iGraphics.h"
#include "Windows.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <string>
#include <cstring>
#include"bitmap_loader.h"

using namespace std;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Idraw Here::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
#define screenwidth 1200
#define screenheight 600

bool musicOn = false;
int limit = 10;
int k = 3;
int j = 0;
int image[66];
int img2[20];
int img3[23];
int img4[30];
int menuIndex = 1;
int img[45];
int i = 0;
int highS = 0;
int highL;
int shipX = screenwidth-200;
int shipY = screenheight/2;
int eShipdx = 140;
int eShipdy = 140;
int shipdx = 140;
int shipdy = 140;
int bulletX = 0;
int bulletY = 0;
int shipHealth = 10;
int enemyIndex = 0;
bool gameOver = false;
int hitCount = 0; 
int bulletIndex = 0;
int shootIndex = 0;
int playerLife = 20;
int renI = 0;
int renE = 0;

bool ex = false;
int bossX = -100;
int bossY = (screenheight/2)-100;
int bossdx = 200;
int bossdy = 100;
int by = (screenheight / 2) - 100;
int bx = 170;
bool start = false;
int bossHealth = 100;
int speed = 0;

char scoreText[20];
char scoreHigh[20];
void iDrawStart();
void iDrawHome();
void iPlay();
void drawHealthBar();
void initEnemies();
void spawnEnemy();
void drawEnemies();
void drawBullet();
void spawnBullet();
void initBullet();
void coliCheck();
void initShoot();
void drawShoot();
void spawnShoot();
void musicToggle();
void highScore(int m);
void renShip();
void renEShip();
void resetState(int n);
void iPlay2();
bool overLap();
void bossA();
void drawBossA();
void drawRedShipHealthBar();

struct Enemy{
	int x, y;
	bool active;
}enemies[100];

struct Bullet{
	int x, y;
	bool active;
}bullet[100];

struct Shoot{
	int x, y;
	bool active;
}shoot[100];

void iDraw()
{
	if (menuIndex == 1){
		iDrawStart();
	}
	else if (menuIndex == 2){
		iDrawHome();
		musicOn = false;
		musicToggle();
	}
	else if (menuIndex == 3)
	{
		if (!ex){
			iPlay();
			if (!musicOn){
				musicOn = true;
				musicToggle();
			}
		}
		else{
			iPlay2();
			if (!musicOn){
				musicOn = true;
				musicToggle();
			}
		}
	}
	else if (menuIndex == 4)
	{
		iShowImage(0, 0, screenwidth, screenheight, image[55]);
	}
	else if (menuIndex == 5)
	{
		iShowImage(0, 0, screenwidth, screenheight, img2[4]);
	}
	else if (menuIndex == 6){
		iShowImage(0, 0, screenwidth, screenheight, img2[3]);
	}
}

void iPlay(){
	iClear();
	iShowImage(0, 0, screenwidth, screenheight, img[i]);
	iShowImage(450,screenheight- 140, 400, 300, img[40]);
	iSetColor(0, 0, 0);
	sprintf_s(scoreHigh, "Highest Score: %d", highL);
	sprintf_s(scoreText, "Score: %d", highS);
	iText(40,screenheight-70 , scoreText);
	iText(40, screenheight - 90, scoreHigh);
	iShowImage(shipX, shipY, shipdx, shipdy, img2[8+renI]);
	drawEnemies();
	drawBullet();
	drawShoot();
	coliCheck();
	drawHealthBar();
	i++;
	if (highL < highS){
		highScore(highS);
	}
	if (highS == limit && playerLife>0){

		iText(screenwidth / 2, screenheight / 2, "Winner", GLUT_BITMAP_HELVETICA_18);
		resetState(3);
		ex = true;
	}
	if (highS > limit / 2){
		speed = 3;
	}
	if (playerLife <= 0){
		resetState(2);
	}
	iDelayMS(30);
	if (i == 40) {
		i = 0;
	}
}

void iPlay2()
{
	iClear();
	iShowImage(0, 0, screenwidth, screenheight, img[i]);
	iShowImage(450, screenheight - 140, 400, 300, img[40]);
	iSetColor(0, 0, 0);
	sprintf_s(scoreHigh, "Highest Score: %d", highL);
	sprintf_s(scoreText, "Score: %d", highS);
	iText(40, screenheight - 70, scoreText);
	iText(40, screenheight - 90, scoreHigh);
	iShowImage(shipX, shipY, shipdx, shipdy, img2[8 + renI]);
	drawBullet();
	//body
	if (1){
		iShowImage(bossX, bossY, bossdx, bossdy, img4[28]);
		drawRedShipHealthBar();
		if (start){
			iShowImage(bx, by, 50, 50, img4[21]);
			bx += (10+speed);
			if (bx > screenwidth){
				bx = 200;
				by = (screenheight / 2) - 70;
			}
		}
		if (bossX < 200){
			bossX += 2;
		}
		if (bossX >= 200){
			start = true;
		}
	}
	if (bossHealth < 15){
		speed = 3;
	}
	if (bossHealth < 0){
		iText(screenwidth / 2, screenheight / 2, "Winner", GLUT_BITMAP_HELVETICA_18);
		resetState(2);
	}
	drawEnemies();
	drawShoot();
	//rest
	coliCheck();
	drawHealthBar();
	i++;
	if (highL < highS){
		highScore(highS);
	}
	if (playerLife <= 0){
		resetState(2);
	}
	iDelayMS(30);
	if (i == 40) {
		i = 0;
	}
}

void drawRedShipHealthBar() {
	int barWidth = 3;
	int barHeight = 10;
	int startX = screenwidth - 310;
	int startY = screenheight - 50;

	for (int j = 0; j < 100; j++) {
		if (j < bossHealth) {
			iSetColor(255, 0, 0);
		}
		else {
			iSetColor(100, 100, 100);
		}
		iFilledRectangle(startX + j * (barWidth), startY, barWidth, barHeight);
	}

	iSetColor(255, 255, 255);
	iRectangle(startX - 2, startY - 2, 100 * barWidth + 4, barHeight + 4);
}

void initEnemies()
{
	for (int k = 0; k < limit; k++){
		enemies[k].active = false;
	}
}

void drawEnemies()
{
	for (int k = 0; k < limit; k++){
		if (enemies[k].active){
			iShowImage(enemies[k].x,enemies[k].y,eShipdx,eShipdy,img4[0+renE]);
			enemies[k].x += (1+speed);
			if (enemies[k].x > screenwidth){
				enemies[k].active = false;
				shoot[k].active = false;

			}
		}
	}
}

void spawnEnemy()
{
	if (!overLap()){
		enemies[enemyIndex].x = -100;
		enemies[enemyIndex].y = rand() % (screenheight-300);
		enemies[enemyIndex].active = true;
		enemyIndex++;
		spawnShoot();
	}
}

void initBullet()
{
	for (int k = 0; k < limit; k++){
		bullet[k].active = false;
	}

}

void drawBullet()
{
	for (int k = 0; k < limit; k++){
		if (bullet[k].active){
			iFilledCircle(bullet[k].x, bullet[k].y, 5, 1000);
			bullet[k].x -= 10;
			if (bulletX < 0){
				bullet[k].active = false;
			}
		}
	}
}

void spawnBullet()
{
	bullet[bulletIndex].active = true;
	bullet[bulletIndex].x = shipX;
	bullet[bulletIndex].y = shipY+20;
	if (bulletIndex < limit-2){
		bulletIndex++;
	}
	else if (bulletIndex == limit-1){
		bulletIndex = 0;
	}
}

void initShoot()
{
	for (int k = 0; k <= limit; k++){
		shoot[k].active = false;
	}
}

void drawShoot()
{
	for (int k = 0; k < limit; k++){
		if (shoot[k].active){
			iSetColor(250,0,0);
			iFilledCircle(shoot[k].x, shoot[k].y, 5, 1000);
			shoot[k].x += 15;
			if (shoot[k].x > screenwidth){
				shoot[k].active = false;
				spawnShoot();
			}
		}
	}
}

void spawnShoot()
{
	for (int k = 0; k < limit; k++){
		if (enemies[k].active){
			shoot[k].x = enemies[k].x + eShipdx;
			shoot[k].y = enemies[k].y+10;
			shoot[k].active = true;
		}
	}
}

void coliCheck()
{
	if (bx >= shipX && bx <= shipX + shipdx && by >= shipY && by <= shipY+shipdy){
		playerLife--;
		bx = 200;
	}

	for (int k = 0; k < limit; k++){
		if (bullet[k].active){
			if (bullet[k].x >= bossX && bullet[k].x <= bossX + bossdx && bullet[k].y >= bossY && bullet[k].y <= bossY + bossdy){
				bossHealth-=5;
				bullet[k].active = false;
				highS++;
			}
		}
	}

	for (int k = 0; k < limit; k++){
		if (enemies[k].active){
			for (int m = 0; m < limit; m++){
				if (bullet[m].active){
					if (bullet[m].x >= enemies[k].x && bullet[m].x <= enemies[k].x + (eShipdx-20) && bullet[m].y >= enemies[k].y && bullet[m].y <= enemies[k].y + (eShipdy-20)){
						bullet[m].active = false;
						enemies[k].active = false;
						shoot[k].active = false;
						highS++;
					}
				}
			}
		}
	}

	for (int k = 0; k < limit; k++){
		if (enemies[k].active){
			if ((shipX < enemies[k].x + eShipdx) && (shipX + shipdx > enemies[k].x) && (shipY < enemies[k].y + eShipdy) && (shipY + shipdy > enemies[k].y)){
				enemies[k].active = false;
				shoot[k].active = false;
				playerLife--;
			}
		}
	}


	for (int k = 0; k < limit; k++){
		if (shoot[k].active){
			if (shoot[k].x >= shipX && shoot[k].x <= shipX + shipdx && shoot[k].y >= shipY && shoot[k].y <= shipY + shipdy){
				shoot[k].active = false;
				playerLife--;
			}
		}
	}
}

void drawHealthBar() {
	int barWidth = 10;
	int barHeight = 20;
	int startX = 20;
	int startY = screenheight - 50;

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

bool overLap(){
	if (enemyIndex > 2){
		return enemies[enemyIndex - 2].active;
	}
	else{
		return false;
	}
}

void highScore( int m)
{
	std::ofstream file("highscores.txt");
	if (file.is_open())
	{
		file << m << std::endl;
		file.close();
		highL = m;
	}
}

void musicToggle()
{
	if (musicOn){
		PlaySound("Music\\M1.wav", NULL, SND_LOOP | SND_ASYNC);
	}
	else{
		PlaySound(0, 0, 0);
	}
}

void renShip()
{
	if (renI < 10){
		renI++;
	}
	else{
		renI = 0;
	}
}

void renEShip()
{
	if (renE < 19){
		renE++;
	}
	else{
		renE = 0;
	}
}

void resetState(int n)
{
	musicOn = false;
	limit = 10;
	k = 3;
	j = 0;
	menuIndex = n;
	i = 0;
	highS = 0;
	shipX = screenwidth - 200;
	shipY = screenheight / 2;
	bulletX = 0;
	bulletY = 0;
	shipHealth = 10;
	enemyIndex = 0;
	gameOver = false;
	hitCount = 0;
	bulletIndex = 0;
	shootIndex = 0;
	playerLife = 20;
	renI = 0;
	renE = 0;
	initEnemies();
	initBullet();
	initShoot();
	ex = false;
	bossX = -100;
	bossY = (screenheight / 2) - 100;
	bossdx = 200;
	bossdy = 100;
	by = (screenheight / 2) - 100;
	bx = 170;
	start = false;
	bossHealth = 100;
	speed = 0;
}

void iMouseMove(int mx, int my)
{

}


void iPassiveMouseMove(int mx, int my)
{

}

void iMouse(int button, int state, int mx, int my)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (menuIndex == 2)
		{
			if ((mx >= 50 && mx <= 250) && (my >= 350 && my <= 400)){
				menuIndex = 3;
			}
			if ((mx >= 50 && mx <= 250) && (my >= 290 && my <= 340)){
				menuIndex = 5;
			}
			if (mx >= 50 && mx < 250 && my >= 170 && my <= 220){

				menuIndex = 6;
			}
			if (mx >= 50 && mx <= 250 && my >= 230 && my <= 280){
				if (musicOn){
					musicOn = false;
				}
				else {
					musicOn = true;
				}
			}
		}

	}


	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

	}
}



void iKeyboard(unsigned char key)
{
	if (key == 'e')
	{
		resetState(2);
		menuIndex = 2;
	}
	else if (key == 'm')
	{
		if (musicOn){
			musicOn = false;
		}
		else{
			musicOn = true;
		}
	}
	if (key == ' ') {
		spawnBullet();
	}

}


void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_UP && shipY < screenheight-200) {
		shipY += 10;
	}
	if (key == GLUT_KEY_RIGHT && shipX < screenwidth-200) {
		shipX += 10;
		renShip();
	}
	if (key == GLUT_KEY_LEFT && shipX > 0) {
		shipX -= 10;
		renShip();
	}
	if (key == GLUT_KEY_DOWN && shipY > 0) {
		shipY -= 10;
	}
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::iDraw Function::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
void iDrawStart()
{
		iShowImage(0, 0, screenwidth, screenheight, image[k]);
		if (k < 51)
		{
			k++;
		}
		else{
			menuIndex = 2;
		}
		iDelayMS(50);
	
}
void iDrawHome()
{
	iShowImage(0, 0, screenwidth, screenheight, img2[6]);
	iShowImage(50, 350, 200, 50,  img2[0]);
	iShowImage(50, 290, 200, 50, img2[1]);
	iShowImage(50, 230, 200, 50, img2[2]);
	iShowImage(50, 170, 200, 50, img2[5]);
}
void oImage() {
	img[0] = iLoadImage("img/0001.png");
	img[1] = iLoadImage("img/0002.png");
	img[2] = iLoadImage("img/0003.png");
	img[3] = iLoadImage("img/0004.png");
	img[4] = iLoadImage("img/0005.png");
	img[5] = iLoadImage("img/0006.png");
	img[6] = iLoadImage("img/0007.png");
	img[7] = iLoadImage("img/0008.png");
	img[8] = iLoadImage("img/0009.png");
	img[9] = iLoadImage("img/0010.png");
	img[10] = iLoadImage("img/0011.png");
	img[11] = iLoadImage("img/0012.png");
	img[12] = iLoadImage("img/0013.png");
	img[13] = iLoadImage("img/0014.png");
	img[14] = iLoadImage("img/0015.png");
	img[15] = iLoadImage("img/0016.png");
	img[16] = iLoadImage("img/0017.png");
	img[17] = iLoadImage("img/0018.png");
	img[18] = iLoadImage("img/0019.png");
	img[19] = iLoadImage("img/0020.png");
	img[20] = iLoadImage("img/0021.png");
	img[21] = iLoadImage("img/0022.png");
	img[22] = iLoadImage("img/0023.png");
	img[23] = iLoadImage("img/0024.png");
	img[24] = iLoadImage("img/0025.png");
	img[25] = iLoadImage("img/0026.png");
	img[26] = iLoadImage("img/0027.png");
	img[27] = iLoadImage("img/0028.png");
	img[28] = iLoadImage("img/0029.png");
	img[29] = iLoadImage("img/0030.png");
	img[30] = iLoadImage("img/0031.png");
	img[31] = iLoadImage("img/0032.png");
	img[32] = iLoadImage("img/0033.png");
	img[33] = iLoadImage("img/0034.png");
	img[34] = iLoadImage("img/0035.png");
	img[35] = iLoadImage("img/0036.png");
	img[36] = iLoadImage("img/0037.png");
	img[37] = iLoadImage("img/0038.png");
	img[38] = iLoadImage("img/0039.png");
	img[39] = iLoadImage("img/0040.png");
	img[40] = iLoadImage("img/island1.png");
	img[41] = iLoadImage("img/0006ship.png");
	img[42] = iLoadImage("img/enemyShip1.png");
	img[43] = iLoadImage("img/next.png");
	img[44] = iLoadImage("bc/eship/ship0001.png");
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Image loader::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
void loadImage(){


	image[3] = iLoadImage("bc/151223-800210508_003.png");
	image[4] = iLoadImage("bc/151223-800210508_004.png");
	image[5] = iLoadImage("bc/151223-800210508_005.png");
	image[6] = iLoadImage("bc/151223-800210508_006.png");
	image[7] = iLoadImage("bc/151223-800210508_007.png");
	image[8] = iLoadImage("bc/151223-800210508_008.png");
	image[9] = iLoadImage("bc/151223-800210508_009.png");
	image[10] = iLoadImage("bc/151223-800210508_010.png");
	image[11] = iLoadImage("bc/151223-800210508_011.png");
	image[12] = iLoadImage("bc/151223-800210508_012.png");
	image[13] = iLoadImage("bc/151223-800210508_013.png");
	image[14] = iLoadImage("bc/151223-800210508_014.png");
	image[15] = iLoadImage("bc/151223-800210508_015.png");
	image[16] = iLoadImage("bc/151223-800210508_016.png");
	image[17] = iLoadImage("bc/151223-800210508_017.png");
	image[18] = iLoadImage("bc/151223-800210508_018.png");
	image[19] = iLoadImage("bc/151223-800210508_019.png");
	image[20] = iLoadImage("bc/151223-800210508_020.png");
	image[21] = iLoadImage("bc/151223-800210508_021.png");
	image[22] = iLoadImage("bc/151223-800210508_022.png");
	image[23] = iLoadImage("bc/151223-800210508_023.png");
	image[24] = iLoadImage("bc/151223-800210508_024.png");
	image[25] = iLoadImage("bc/151223-800210508_025.png");
	image[26] = iLoadImage("bc/151223-800210508_026.png");
	image[27] = iLoadImage("bc/151223-800210508_027.png");
	image[28] = iLoadImage("bc/151223-800210508_028.png");
	image[29] = iLoadImage("bc/151223-800210508_029.png");
	image[30] = iLoadImage("bc/151223-800210508_030.png");
	image[31] = iLoadImage("bc/151223-800210508_031.png");
	image[32] = iLoadImage("bc/151223-800210508_032.png");
	image[33] = iLoadImage("bc/151223-800210508_033.png");
	image[34] = iLoadImage("bc/151223-800210508_034.png");
	image[35] = iLoadImage("bc/151223-800210508_035.png");
	image[36] = iLoadImage("bc/151223-800210508_036.png");
	image[37] = iLoadImage("bc/151223-800210508_037.png");
	image[38] = iLoadImage("bc/151223-800210508_038.png");
	image[39] = iLoadImage("bc/151223-800210508_039.png");
	image[40] = iLoadImage("bc/151223-800210508_040.png");
	image[41] = iLoadImage("bc/151223-800210508_041.png");
	image[42] = iLoadImage("bc/151223-800210508_042.png");
	image[43] = iLoadImage("bc/151223-800210508_043.png");
	image[44] = iLoadImage("bc/151223-800210508_044.png");
	image[45] = iLoadImage("bc/151223-800210508_045.png");
	image[46] = iLoadImage("bc/151223-800210508_046.png");
	image[47] = iLoadImage("bc/151223-800210508_047.png");
	image[48] = iLoadImage("bc/151223-800210508_048.png");
	image[49] = iLoadImage("bc/151223-800210508_049.png");
	image[50] = iLoadImage("bc/151223-800210508_050.png");
	image[51] = iLoadImage("bc/151223-800210508_051.png");
	image[52] = iLoadImage("bc/home/eship.png");
	image[53] = iLoadImage("bc/home/sky.jpg");
	image[54] = iLoadImage("bc/home/shoot.png");
	image[55] = iLoadImage("bc/home/over.jpg");
	image[56] = iLoadImage("bc/home/shoot2.png");

}

void homeLoader()
{
	img2[0] = iLoadImage("bc/home/homeplay.jpg");
	img2[1] = iLoadImage("bc/home/homecontrol.jpg");
	img2[2] = iLoadImage("bc/home/homesound.jpg");
	img2[3] = iLoadImage("bc/home/us.jpg");
	img2[4] = iLoadImage("bc/home/control.jpg");
	img2[5] = iLoadImage("bc/home/homecr.jpg");
	img2[6] = iLoadImage("bc/home/bg.jpg");
	img2[7] = iLoadImage("bc/home/back.jpg");
	img2[8] = iLoadImage("bc/home/0001.png");
	img2[9] = iLoadImage("bc/home/0002.png");
	img2[10] = iLoadImage("bc/home/0003.png");
	img2[11] = iLoadImage("bc/home/0004.png");
	img2[12] = iLoadImage("bc/home/0005.png");
	img2[13] = iLoadImage("bc/home/0006.png");
	img2[14] = iLoadImage("bc/home/0007.png");
	img2[15] = iLoadImage("bc/home/0008.png");
	img2[16] = iLoadImage("bc/home/0009.png");
	img2[17] = iLoadImage("bc/home/0010.png");
	img2[18] = iLoadImage("bc/home/0011.png");
	img2[19] = iLoadImage("bc/home/0012.png");

}

void eLoader()
{
	img4[0] = iLoadImage("bc/eship/ship0001.png");
	img4[1] = iLoadImage("bc/eship/ship0002.png");
	img4[2] = iLoadImage("bc/eship/ship0003.png");
	img4[3] = iLoadImage("bc/eship/ship0004.png");
	img4[4] = iLoadImage("bc/eship/ship0005.png");
	img4[5] = iLoadImage("bc/eship/ship0006.png");
	img4[6] = iLoadImage("bc/eship/ship0007.png");
	img4[7] = iLoadImage("bc/eship/ship0008.png");
	img4[8] = iLoadImage("bc/eship/ship0009.png");
	img4[9] = iLoadImage("bc/eship/ship0010.png");
	img4[10] = iLoadImage("bc/eship/ship0011.png");
	img4[11] = iLoadImage("bc/eship/ship0012.png");
	img4[12] = iLoadImage("bc/eship/ship0013.png");
	img4[13] = iLoadImage("bc/eship/ship0014.png");
	img4[14] = iLoadImage("bc/eship/ship0015.png");
	img4[15] = iLoadImage("bc/eship/ship0016.png");
	img4[16] = iLoadImage("bc/eship/ship0017.png");
	img4[17] = iLoadImage("bc/eship/ship0018.png");
	img4[18] = iLoadImage("bc/eship/ship0019.png");
	img4[19] = iLoadImage("bc/eship/ship0020.png");
	img4[20] = iLoadImage("bc/eship/p1.png");
	img4[21] = iLoadImage("bc/eship/p2.png");
	img4[22] = iLoadImage("bc/eship/p3.png");
	img4[23] = iLoadImage("bc/eship/p4.png");
	img4[24] = iLoadImage("bc/eship/p5.png");
	img4[25] = iLoadImage("bc/eship/p6.png");
	img4[26] = iLoadImage("bc/eship/p7.png");
	img4[27] = iLoadImage("bc/eship/p8.png");
	img4[28] = iLoadImage("bc/eship/red.png");
	img4[29] = iLoadImage("bc/eship/redre.png");



}


int main()
{
	iInitialize(screenwidth, screenheight, "The Silver Skull");
	std::ifstream inputFile("highscores.txt");

	if (inputFile.is_open()) {
		inputFile >> highL;
		inputFile.close();
	}
	homeLoader();
	oImage();
	loadImage();
	eLoader();
	initEnemies();
	initBullet();
	initShoot();
	iSetTimer(2000, spawnEnemy);
	iSetTimer(100, renEShip);
	iStart();
	return 0;
}