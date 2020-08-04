#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define ssid 	 "Nombre de tu red"
#define password "Contraseña      "

int Pines[] =               {12,13,15,3}; // GPIO 12, luz roja, 13 luz blanca, 15 luz de planta, 3 hidroponia.
const char* nombrePines[] = {"Luz roja","Luz blanca","Luz de planta","Hidroponia"};
const int NumPines = sizeof(Pines) / sizeof(int);
// Configuración de la conexión wifi.
WiFiServer server(80);
String header;
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
#define timeoutTime 2000
// Variables para realtime
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.south-america.pool.ntp.org",-18000,6000);
unsigned long updateTime = 0;

void setup() {
  Serial.begin(115200);
  for(int i=0;i<NumPines;i++)
  {
	pinMode(Pines[i], OUTPUT);
	digitalWrite(Pines[i], LOW);
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  timeClient.begin(); 
}

void loop(){
  WiFiClient client = server.available();

  if(millis()-updateTime >= 1000)
  {
    timeClient.update();
    updateTime = millis();
  }

  if (client) 
  {
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) 
	{
      currentTime = millis();         
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            for(int i=0;i<NumPines;i++)
	    {
		String busqueda = "GET /" + (String)Pines[i] + "/on";
		if (header.indexOf(busqueda) >= 0) 
       		{
                	EncenderPinRele(Pines[i]);
                	break;
              	}
      		busqueda = "GET /" + (String)Pines[i] + "/off";
              	if (header.indexOf(busqueda) >= 0) 
              	{
                	ApagarPinRele(Pines[i]);
                	break;
      		}
	    }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            client.println("<body style=\"background-color: #DCDCDC;\"><h1>NodeMCU Web Server</h1>");
            client.println("<p>Hora: " + timeClient.getFormattedTime() + "</p>");
            
        	for(int i=0;i<NumPines;i++)
		{
	            client.println("<p>"+(String)nombrePines[i]+" - estatus " + StatusPinReleColoured(StatusPinRele(Pines[i])) + "</p>");
                    
	            if (StatusPinRele(Pines[i]) == "apagado")
	              client.println("<p><a href=\"/"+String(Pines[i])+"/on\"><button class=\"button\">ON</button></a></p>");
	            else
	              client.println("<p><a href=\"/"+String(Pines[i])+"/off\"><button class=\"button button2\">OFF</button></a></p>");
          	}


            client.println("</body></html>");
            client.println();
            break;
          } 
          else 
          { 
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
  }
}

void EncenderPinRele(int pin)
{
	Serial.println("GPIO "+ String(pin)+ " pin rele is on");
	digitalWrite(pin, LOW);
}

void ApagarPinRele(int pin)
{
	Serial.println("GPIO "+ String(pin)+ " pin rele is off");
	digitalWrite(pin, HIGH);
}

String StatusPinRele(int pin)
{
	if(digitalRead(pin) == LOW)
		return "encendido";
	return "apagado";
}

String StatusPinReleColoured(String pin)
{
  if(pin == "apagado")
    return "<font color=\"red\">apagado</font>";
  return "<font color=\"green\">encendido</font>";
}
