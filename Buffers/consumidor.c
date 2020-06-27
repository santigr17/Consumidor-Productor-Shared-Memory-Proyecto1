#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include "buffercircular.h"
#include "mensaje.h"
#include <ncurses.h>

//Función para capturar teclas

int kbhit(void)
{
	int ch = getch();

	if (ch != ERR)
	{
		ungetch(ch);
		return 1;
	}
	else
	{
		return 0;
	}
}

//Función especial para finalizar
void finalizar()
{
}

//Función para finalizar con número mágico
void finalizar_magico()
{
}

int main(int argc, char *argv[])
{
	int err = 0;
	int ret = 0;
	// unsigned int sec = 0;
	char scberrormsgcreate[TAMAX_MSGERROR + 1] = {'\0'};
	buffer ctx;
	errores scberr = SCB_OK;

	check_error(scberr, ret, scberrormsgcreate);
	printf("%s", scberrormsgcreate);

	//Se verifica entradas

	if (argc != 4)
	{
		printf("ERROR: Por favor indique nombre del buffer y el tipo de consumidor m (manual) o a (automatico) \n");
		printf("Argumento 0: [%s]\n", argv[0]);
		printf("Argumento 1: [%s]\n", argv[1]);
		printf("Argumento 2: [%s]\n", argv[2]);
		return (1);
	}

	scberr = get_buffer(&ctx, argv[1], &err);
	scberr = add_consumidor(&ctx, argv[1], &err);

	//Funciones para la captura de teclas
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	scrollok(stdscr, TRUE);
	//SE LIGA PRODUCTOR CON BUFFER USANDO EL ARGUMENTO PASADO POR CONSOLA
	SCB_SAMPLE_CHECK_ERROR(SCB_OK, scberr, ret, 1);

	if (!strcmp(argv[2], "manual"))
	{
		while (1)
		{

			mensaje msj;
			buffer ctx;
			system("clear");
			// se pide el buffer a memoria compartida
			scberr = get_buffer(&ctx, argv[1], &err);
			if (kbhit())
			{
				int enter = getch();
				//se pide info del controller

				int err = 0;
				int semlleno = 0;
				int semvacio = 0;
				int semcon_carrera = 0;
				buffer_control inf;
				errores scberr2;

				/* errores get_info(char *name, buffer_control *inf, int *semlleno, int *semvacio, int *semcon_carrera,int *semconsumidores,int *semproductores, int *err);*/
				scberr2 = get_info_buffer(argv[1], &inf, &semlleno, &semvacio, &semcon_carrera, &err);
				//Se verifica algun error
				SCB_SAMPLE_CHECK_ERROR(SCB_OK, scberr2, err, 1);
				printf("Finalizador.........: [%u] \n\r", inf.finalizar);

				//Se tiene que verificar finalizador

				if (enter == 10)
				{

					scberr = get_msg(&ctx, &msj, copyMessage, BLOCK, &ret);
					printw("Mi buffer es: %s \n\r", argv[1]);
					printw("Numero mágico: %u \n\r", msj.numero_magico);
					printw("Escrito por: %u \n\r", msj.pid);
					struct tm *info;
					info = localtime(&(msj.time));
					printw("Hora %s \n\r", asctime(info));
					sleep(4);
				}
			}
			else
			{

				printw("Por favor presionar ENTER para intentar consumir mensaje \n");

				sleep(2);
			}
		}
	}
	else if (!strcmp(argv[2], "automatico"))
	{

		while (1)
		{
			//se pide info del controller

			int err = 0;
			int semlleno = 0;
			int semvacio = 0;
			int semcon_carrera = 0;
			buffer_control inf;
			errores scberr2;

			/* errores get_info(char *name, buffer_control *inf, int *semlleno, int *semvacio, int *semcon_carrera,int *semconsumidores,int *semproductores, int *err);*/
			scberr2 = get_info_buffer(argv[1], &inf, &semlleno, &semvacio, &semcon_carrera, &err);
			//Se verifica algun error
			SCB_SAMPLE_CHECK_ERROR(SCB_OK, scberr2, err, 1);
			printf("Finalizador.........: [%u]\n", inf.finalizar);

			//Se tiene que verificar finalizador

			mensaje msj;
			buffer ctx;
			system("clear");
			// se pide el buffer a memoria compartida
			scberr = get_buffer(&ctx, argv[1], &err);
			//Se intenta leer
			scberr = get_msg(&ctx, &msj, copyMessage, BLOCK, &ret);
			printf("Mi buffer es: %s \n\r", argv[1]);
			printf("Numero mágico: %u \n\r", msj.numero_magico);
			printf("Escrito por: %u \n\r", msj.pid);
			struct tm *info;
			info = localtime(&(msj.time));
			printf("Hora: %s \n\r", asctime(info));
			sleep(4);
		}
	}
	else
	{
		printf("ERROR: Por favor indique nombre del buffer y el tipo de consumidor m (manual) o a (automatico) \n\r");
	}

	endwin();
	return 0;
}
