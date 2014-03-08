#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define MAX_TEMP 45000
#define PIN_TEMPERATURA 14
long readNumberFile (char *filePath);
void TemperaturaMonitor();
int main ()
{
	wiringPiSetup(); //setup IO
	int pid;
	pid = fork();
	if (pid < 0)
	{
		printf("Fork incompleto");
	}
	if (pid = 0)
	{
		while (1)
		{
			TemperaturaMonitor();			
		}
	}
	return 0;
}

long readNumberFile(char *filePath)
{
	FILE *fd;
	char buff[2048];
	fd = fopen(filePath, "r");
	if (fd == NULL)
	{
		printf("Erro ao Abrir o Arquivo");
		exit(0);
	}
	while (fgets(buff, sizeof(buff), fd))
	{
		printf("Buffer lido: %s", buff);
		return (atol(buff));
	}
	return -1;
}

void TemperaturaMonitor()
{
	pinMode(PIN_TEMPERATURA, OUTPUT); //SETUP PIN
	long temperatura;
	temperatura = readNumberFile("/sys/class/thermal/thermal_zone0/temp");
	printf("Temperatura: %ld", temperatura);
	if (temperatura != -1)
	{
		if (temperatura >= MAX_TEMP)
		{
			digitalWrite(PIN_TEMPERATURA, HIGH);
		}
	}
}

