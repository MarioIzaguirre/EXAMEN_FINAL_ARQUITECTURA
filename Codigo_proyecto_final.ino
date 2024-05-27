//librerías que se usan para manejar la pantalla lcd
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//se declara la pantalla lcd
LiquidCrystal_I2C lcd(39,16,2);
//Variable que corresponde al pin al que está conectado el sensor (así se obtiene la medición)
int sensorPin = A0;
//Variable que guarda el porcentaje de humedad medido por el sensor
int humedad = 0;
//Variable que guarda los datos que envía la aplicación
int estado;

void setup() {
  //Configuración de la pantalla LCD
  lcd.init();
  lcd.backlight();
  //Configuración del serial para que se pueda leer los imputs enviados por la aplicación
  Serial.begin(9600);
  //Pin que maneja la bomba de agua (El cable rojo de la bomba debería ir en el pin que está puesto allí, en este caso es en el 12, pero pueden cambiarlo si quieren ponerolo en otro pin)
  pinMode(12, OUTPUT);
  //en un inicio el motor va a estar apagado hasta que se haga una lectura de humedad mayor al 50%
  digitalWrite(12, LOW);
  //el estado inicia en 0, para que desde un inicio el sistema de irrigación se haga de manera automática (o sea que según la medición de humedad es que se activa o no la bomba)
  estado = '0';
}

void loop() {
  //Se guarda el valor medido por el sensor
  int sensorValue = analogRead(sensorPin);
  //este valor se convierte a porcentaje
  humedad = map(sensorValue, 0, 1023, 100, 0); 
  //Se leen los imputs enviados por la app
  if(Serial.available() > 0)
  {
    estado = Serial.read();
  }
  //Si no se toca la app o si se envía un "0", el sistema funciona de manera automática, o sea según el valor medido por el sensor
  if(estado == '0')
  {
    //Si la humedad es menor a 50%, la bomba se debería activar (si creen que el valor de 50% es muy alto, lo pueden cambiar si gustan)
    if (humedad < 50) 
    {
      //se pone "High" al pin al que está conectado la bomba, si el cable rojo de la bomba está conectado en otro pin que no sea el 12, cambien allí el valor de 12, por el del pin al que está la bomba
      digitalWrite(12, HIGH);
    } 
    //ahora si la humedad es mayor a 50% (nuevamente depende de ustedes si creen que 50% es muy alto, lo pueden cambiar), la bomba no se debería activar
    else if(humedad > 50)
    {
      //se pone "Low" al pin al que está conectado la bomba, si el cable rojo de la bomba está conectado en otro pin que no sea el 12, cambien allí el valor de 12, por el del pin al que está la bomba
      digitalWrite(12, LOW);
    }
  }
  else
  {
    //Ahora si se le envía mediante la app un 1, la bomba independientemente de la medición, se va a encender (si se quisiera volver al modo automático, van a tener que enviar un "0")
    if (estado == '1')   
    {
      //se pone "High" al pin al que está conectado la bomba, si el cable rojo de la bomba está conectado en otro pin que no sea el 12, cambien allí el valor de 12, por el del pin al que está la bomba
      digitalWrite(12,HIGH);
    }
    else if (estado == '2') 
    {
      //se pone "Low" al pin al que está conectado la bomba, si el cable rojo de la bomba está conectado en otro pin que no sea el 12, cambien allí el valor de 12, por el del pin al que está la bomba
      digitalWrite(12,LOW);
    }
  }
  //por último, siempre se va a estar imprimendo en pantalla el nivel de humedad medido por el sensor (No se si esto es lo que tiene que hacer la pantalla, voy a ver el enunciado por que no estoy seguro de si esto es lo que tiene que hacer LÑASDJKFALÑJK)
  lcd.setCursor(0, 0);
  lcd.print("Humedad");
  lcd.setCursor(0, 1);
  lcd.print(humedad);
  //se hace un delay de 1 segundo para hacer la siguiente medición
  delay(1000);
}