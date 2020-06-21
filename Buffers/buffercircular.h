#ifndef _BUFFERCIRCULAR_H
#define _BUFFERCIRCULAR_H

#include <stdint.h>
#include <semaphore.h>

//Se define tamaño máximo para el nombre y los errores

#define TAMAX_NOMBRE (30)
#define TAMAX_MSGERROR (100)

//Se crea struct para controlar el buffer y ver su estado

/*
+-------------+-----------+-----------+-----+-----------+
| Buffer control | espacio 1 | espacio 2 | ... | espacio n |
+-------------+-----------+-----------+-----+-----------+*/


//estructura para el control

typedef struct _buffer_control{
	uint16_t consumidores;
	uint16_t productores;
	sem_t vacio;
	sem_t lleno;
	sem_t con_carrera;
	uint16_t finalizar;
	uint16_t cabeza;
	uint16_t cola;
	uint16_t qtd;
	uint16_t capacidad;
	size_t   largo_mensaje; 
}buffer_control;


// estructura para el total del buffer

typedef struct _buffer{
	char name[TAMAX_NOMBRE];
	buffer_control *ctrl;
	void *mensajes;
}buffer;


// estructura para iterar (revisar)

typedef struct _iterar{
	uint16_t it;
	uint16_t qtd;
}iterar;

// enum para manejo de error



typedef enum{
	SCB_OK = 0,
	SCB_SHMEM,
	SCB_FTRUNC,
	SCB_SEMPH,
	SCB_MMAP,
	SCB_LLENO,
	SCB_VACIO,
	SCB_BLOQUEADO,
	SCB_ITER_FINAL
}errores;

//enum para bloquear el buffer

typedef enum{
	SCB_BLOQUEAR = 0,
	SCB_DESBLOQUEAR
}bloqueo;

//función para crear un buffer compartido nuevo

errores crear_buffer(char *name, uint16_t totalElementos, size_t tamElementos, buffer *ctx, int *err);

//función para ligar procesos a un buffer 

errores ligar_buffer(buffer *ctx, char *name, int *err,int tipo);


//función para obtener la información del buffer

errores get_info_buffer(char *name, buffer_control *inf, int *semlleno, int *semvacio, int *semcon_carrera, int *err);

//función para destruir el buffer

errores destruir_buffer(char *name, int *err);




//función para control de errores

void check_error(errores err, int ret, char *msg);




#endif