#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "decript.h"
#include "funciones_auxiliares.h"
#include "generador_de_clave_de_encriptacion.h"
#include "inversor_de_clave_de_encriptacion.h"
#include "encriptador.h"


int desencriptar(char *data_file , char *user_key ,int zn, int lvl_seguridad)
{

	printf("\tdesencriptacion<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	int tam_buffer;//numero de elementos que tiene el fichero
	int size=pow((lvl_seguridad+1),2);
	int vector[size];//vector de enciptacion
	int size_matriz=(int)(pow((lvl_seguridad+1),2));

	//abre el documento para calcular su volumen
	FILE* f;
	if ( (f = fopen (data_file,"r")) == NULL )
	{
		printf("error, el fichero no se pudo abrir\n");
	}
	//consulta el tamaño del fichero
	fseek(f,0,SEEK_END);
	tam_buffer = ftell(f)+1;

	printf("\tuser key <%s>\n",user_key );
	printf("\ttam_buffer <%d>\n",tam_buffer );
	printf("\tsize <%d>\n", size);
	printf("\tlvl_seguridad <%d>\n", lvl_seguridad);


	//se declaran y reservan los buffers de datos
	int *buffer_int;
	char *buffer_char;
	//int *buffer_int_temp;

	//buffer_int_temp = (int*) malloc ((tam_buffer)*sizeof(int));
	buffer_int = (int*) malloc ((tam_buffer)*sizeof(int));
	buffer_char = (char*) malloc ((tam_buffer+size+1)*sizeof(char));

	//se cierra el fichero
	fclose(f);


	/*----------------------------------------------------------------*/


	//carga el fichero en buffer (char)

	if ( (f = fopen (data_file,"r")) == NULL )
	{
		printf("error, el fichero no se pudo abrir\n");
	}

	fgets(buffer_char,tam_buffer,f);
	//printf("\tbuffer_char 1 <%s>\n",buffer_char );

	fclose(f);



	/*----------------------------------------------------------------*/

	//pasa el buffer de char a int
	printf("\tbuffer_char ");
	for (int i = 0; i < tam_buffer-1; ++i)
	{
		printf("<%c>",buffer_char[i] );
	}
	printf("\n");

	for (int i = 0; i < tam_buffer-1; ++i)
	{
		buffer_int[i]=buffer_int[i]%zn;
		buffer_int[i] = define_abecedario(buffer_char[i]);
	}
	printf("\tbuffer_int ");
	for (int i = 0; i < tam_buffer-1; ++i)
	{
		printf("<%d>",buffer_int[i] );
	}
	printf("\n");


	/*----------------------------------------------------------------*/

	//encripta

	int **clave;//clave de cript
	int k;


	for (int pasada = 1; pasada >=0; pasada--)
	{
		printf("<<<<<<<<<<<<<< paso %d >>>>>>>>>>>>\n",pasada);
		clave=reservar_matriz(size);
		clave=creacion_clave(user_key, zn, lvl_seguridad, pasada);
		//printf("sin invertir\n");
		recalcular_matriz(clave, size, zn);
		//imprimir_matriz(clave,size);
		//printf("\ninvertida\n");
		calcular_inversa_matriz(clave, size, zn);
		recalcular_matriz(clave, size, zn);
		//imprimir_matriz(clave,size);
		int k=0;
		//multiplica y guarda en el vector
		for (int i = 0; i < tam_buffer-1;)
		{
			for (int j = 0; j < size; ++j)
			{
				vector[j]=buffer_int[i];
				i++;
			}
			multiplicar(clave, vector, size, zn);
			//imprimir_vector(vector, size);
			
			//asigna el valor del vector de vuelta al buffer
			for (int i = 0; i < size; ++i)
			{
				buffer_int[k]=vector[i];
				//printf("\tk=<%d> i=<%d>\n",k,i );
				k++;
			}

		}
		printf("\tbuffer_int ");
		for (int i = 0; i < tam_buffer-1; ++i)
		{
			printf("<%d>",buffer_int[i] );
		}
		printf("\n");
		liberar_matriz(&clave, size);
	}
	


	//pasa el buffer de int a char
	for (int i = 0; i < tam_buffer-1; ++i)
	{
		buffer_int[i]=buffer_int[i]%zn;
		buffer_char[i] = redefine_abecedario(buffer_int[i]);
	}
	printf("\tbuffer_char ");
	for (int i = 0; i < tam_buffer-1; ++i)
	{
		printf("<%c>",buffer_char[i] );
	}
	printf("\n");

	// elimina los caracteres de relleno
	//VALOR_DE_RELLENO definida en funciones_auxiliares
	int i=tam_buffer-size;
	while ( buffer_char[i] != VALOR_DE_RELLENO && i<tam_buffer )
	{
		i++;
	}
	if (buffer_char[i] == VALOR_DE_RELLENO )
	{
		buffer_char[i] = '\0';
	}



	//escribe el fichero
	f = fopen("decript_data.txt","w");

	fputs(buffer_char,f);

	fflush(f);

	fclose(f);

}