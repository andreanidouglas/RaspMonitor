#include <wiringPi.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#define MAX_TEMP 45000
#define PIN_TEMPERATURA 7
long readNumberFile (char *filePath);
void TemperaturaMonitor();
void resetPins();
int main ()
{
	wiringPiSetup(); //setup IO
	resetPins();
	int pid, sid;
	pid = fork();
	if (pid < 0)
	{
		exit (0);
		//printf("Fork incompleto");
	}
	if (pid > 0)
	{
		exit(1);
	}
	umask(0); //set file mode mask
	sid = setsid(); //set session id
	if (sid<0)
	{
		exit(0);
	}

	if ((chdir("/"))<0) //change working directory
	{
		exit(0);
	}
	//close standards io
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	
	while (1)
	{
		TemperaturaMonitor();			
		sleep(30);
	}
	
	return 0;
}

long readNumberFile(char *filePath)
{
	FILE *fd;
	int returnValue= -1;
	char buff[2048];
	fd = fopen(filePath, "r");
	if (fd == NULL)
	{
		while (fd == NULL)
		{
			printf("Erro ao Abrir Arquivo");
			exit(1);
		}
	}
	while (fgets(buff, sizeof(buff), fd))
	{
		//printf("Buffer lido: %s", buff);
		returnValue = atol(buff);
		
	}
	fclose(fd);
	return returnValue;
}

void TemperaturaMonitor()
{
	pinMode(PIN_TEMPERATURA, OUTPUT); //SETUP PIN
	long temperatura;
	temperatura = readNumberFile("/sys/class/thermal/thermal_zone0/temp");
	//printf("Temperatura: %ld", temperatura);
	if (temperatura != -1)
	{
		if (temperatura >= MAX_TEMP)
		{
			digitalWrite(PIN_TEMPERATURA, HIGH);
			delay(300);
			digitalWrite(PIN_TEMPERATURA, LOW);
		}
	}
}

void resetPins()
{
	int i;
	for (i=0;i<=7;i++)
	{
		pinMode(i, OUTPUT);
		digitalWrite(i, HIGH);
		delay(50);
		digitalWrite(i, LOW);
	}
}
