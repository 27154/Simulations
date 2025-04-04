#include <stdio.h>
#include <math.h>
#include <string.h>
#ifdef _WiN32
#include <unistd.h>
#else
#include<Windows.h>
void usleep(__int64 usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec);

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}
#endif

//global
float A = 0, B = 0, C = 0;
float cubeWidth = 20;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int BackgroudASCIICode = '.';
int distanceFromCam = 100;
float horizontalOffset;
float K1 = 40;
float IncrementSpeed = 0.3; // +smoother --  -Faster
float x, y, z;
float ooz;
float xp, yp;
int idx;

//coordinates
float calculateX(int i, int j, int k) 
{
	return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
		   j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);	       
}
float calculateY(int i, int j, int k) 
{
	return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
		j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
		i * cos(B) * sin(C);
}
float calculateZ(int i, int j, int k) 
{
	return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);

}

//surface
void calculateSurface(float cubeX, float cubeY, float cubeZ, int ch) 
{
	x = calculateX(cubeX, cubeY, cubeZ);
	y = calculateY(cubeX, cubeY, cubeZ);
	z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

	ooz = 1 / z;

	xp = (int)(width / 2 + horizontalOffset + K1 * ooz * x * 2);
	yp = (int)(height / 2 + K1 * ooz * y);

	idx = xp + yp * width;
	if (idx >= 0 && idx < width * height) 
	{

		if (ooz > zBuffer[idx])
		{
			zBuffer[idx] = ooz;
			buffer[idx] = ch;
		}
	}
}

int main()
{
	printf("\x1lb[2J");
	while (1)
	{
		//buffers
		memset(buffer, BackgroudASCIICode, width * height);
		memset(zBuffer, 0, width * height * 4);

		cubeWidth = 20;
		horizontalOffset = -2 * cubeWidth;
		for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += IncrementSpeed)
		{
			for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += IncrementSpeed) {
				calculateSurface(cubeX, cubeY, -cubeWidth, '@');
				calculateSurface(cubeWidth, cubeY, cubeX, '$');
				calculateSurface(-cubeWidth, cubeY, -cubeX, '~');
				calculateSurface(-cubeX, cubeY, cubeWidth, '#');
				calculateSurface(cubeX, -cubeWidth, -cubeY, ';');
				calculateSurface(cubeX, cubeWidth, cubeY, '+');
			}

	    }

		cubeWidth = 10; //cube 2
		horizontalOffset = 1 * cubeWidth;
		for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += IncrementSpeed) {
			for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += IncrementSpeed) {
				calculateSurface(cubeX, cubeY, -cubeWidth, '@');
				calculateSurface(cubeWidth, cubeY, cubeX, '$');
				calculateSurface(-cubeWidth, cubeY, -cubeX, '~');
				calculateSurface(-cubeX, cubeY, cubeWidth, '#');
				calculateSurface(cubeX, -cubeWidth, -cubeY, ';');
				calculateSurface(cubeX, cubeWidth, cubeY, '+');
			}
		}

		printf("\x1b[H");
		for (int k = 0; k < width * height; k++) {
			putchar(k % width ? buffer[k] : 10);

		}
		A += 0.03;
		B += 0.03;
		C += 0.01;

		usleep(8000);
	}
	return 0;
}
