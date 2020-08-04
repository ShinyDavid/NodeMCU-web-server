# NodeMCU-web-server
Servidor web para controlar pines de la placa NodeMCU basada en ESP8266.

https://es.wikipedia.org/wiki/NodeMCU

# Configuración
Nuestro entorno de arduino debe estar configurado de tal manera que la placa ya sea detectada por nuestra computadora con los drivers requeridos, antes de compilar el programa
debemos cambiar ciertos parametros para el correcto funcionamiento.

Primero que nada debemos saber que yo la programe para un modulo de reles el cual encienden cuando se tiene una salida en bajo (cero lógico) y se apaga cuando se tiene uno lógico (3.3 volts).
Si se requiere una salida normal (1 logico con 3.3 volts y 0 lógico con 0 volts) hay que invertir los parametros de salida.

Para conectarse a la red wifi primero se requiere cambiar el SSID (nombre de la red) y la contraseña descritos a continuación.
```
#define ssid 	 "Nombre de tu red"
#define password "Contraseña"
```
posteriormente tendremos que configurar los pines de salida deacuerdo al uso que le vayamos a brindar, aquí es donde nuestro programa se diferencia de muchos otros debido a que
no se requiere modificar tanta parte del codigo al agregar una nueva salida o retirar una, en mi caso uso cuatro pines de salida los cuales se modifican así:
```
int Pines[] =               {12,13,15,3}; // GPIO 12 luz roja,GPIO 13 luz blanca,GPIO 15 luz de planta,GPIO 3 hidroponia.
const char* nombrePines[] = {"Luz roja","Luz blanca","Luz de planta","Hidroponia"};
```
En mi caso los uso para la iluminación de mi casa y para una pequeña bomba con un proyecto de hidroponia, a mi parecer la explicación es muy sencilla:
```
int Pines[]               = {pines,GPIO,de,salida,separados,por,comas};
const char* nombrePines[] = {"nombres","de","cada","pin","gpio","descrito","anteriormente"};
```

Una vez que todos los parametros esten configurados correctamente procedemos a compilar y subir el archivo a nuestra placa, posteriormente nos conectamos a la ip que reporta a traves del serial de la computadora, o podemos
verificar la ip que se le asigna si tenemos accedo a nuestro modem/router y la ingresamos en nuestro navegador web, lo cual nos mostrara la pagina de nuestra placa, ya solamente pulsando los botones podemos activar los puertos GPIO o desactivarlos.
