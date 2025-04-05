#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// corroboramos que se haya creado correctamente el config
	if(config == NULL){
		log_error(logger, "No se pudo leer el archivo de configuracion");
		log_destroy(logger);
		return EXIT_FAILURE;
	}

	// Loggeamos el valor de config
	char* valor_clave = config_get_string_value(config, "CLAVE");
	log_info(logger, "El valor de CLAVE es: %s", valor_clave);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor_clave, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log", "Cliente", 1, LOG_LEVEL_INFO); //crea el logger
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("cliente.config"); //crea el cliente
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	
	// realiza el readline y corrobora que la linea no este vacia con strcmp
	while ((leido = readline("> ")) && strcmp(leido, "") != 0) { 
    	log_info(logger, "Ingresaste: %s", leido);
    	free(leido);
	}

	free(leido);
}

void paquete(int conexion)
{
	char* leido;
	t_paquete* paquete = crear_paquete();

	while ((leido = readline("> ")) && strcmp(leido, "") != 0) {
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido); // liberar cada línea después de agregarla al paquete
	}

	free(leido); // liberar la última línea vacía

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
