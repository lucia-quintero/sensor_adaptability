#include "DHT.h"
#include "MPU6050.h"
#include "Wire.h"
#include "I2Cdev.h"

DHT dht(2, DHT11);

float ruido = 0.0;
int humedad = 0;
float luz = 0.0;
float luz_2 = 0.0;
float emg = 0.0;
int temperatura = 0;
bool sel = HIGH;
int count = 0;
int count2 = 0;
int b = 0;
int a = 0;
float Res0=6.85;
String Salida = "";
int contador = 0;
int pasado = 0;

volatile float x[7]={0,0,0,0,0,0,0};
volatile float y;
float w;
float z;

const int mpuAddress = 0x68;  // Puede ser 0x68 o 0x69
MPU6050 mpu(mpuAddress);
 
int ax, ay, az;
int gx, gy, gz;
 
long tiempo_prev, dt;
float girosc_ang_x, girosc_ang_y;
float girosc_ang_x_prev, girosc_ang_y_prev;
 
void updateGiro()
{
   dt = millis() - tiempo_prev;
   tiempo_prev = millis();
 
   //girosc_ang_x = (gx / 131)*dt / 1000.0 + girosc_ang_x_prev;
   //girosc_ang_y = (gy / 131)*dt / 1000.0 + girosc_ang_y_prev;
 
   //girosc_ang_x_prev = girosc_ang_x;
   //girosc_ang_y_prev = girosc_ang_y;
   girosc_ang_x = 105;
   girosc_ang_y = 96;
}

void setup() {
  
  Serial.begin(115200);
  dht.begin();
  Wire.begin();
  mpu.initialize();
}

void loop() {
  
  humedad = dht.readHumidity();
//  humedad = contador;
//  contador++;
  temperatura = dht.readTemperature();
  //temperatura = analogRead(A0)*15;
  ruido = (20 * log(10))*(analogRead(A2)/1023.0);
  luz = analogRead(A1)*0.0048828125;
  luz_2 = 500/(Res0*((5-luz)/luz));


  mpu.getRotation(&gx, &gy, &gz);
 
   updateGiro();
  
  if (sel==LOW){
    int ahora = millis();
    int cambio = ahora - pasado;
    if(cambio >= 1000){
      count2++;
      b = count2%60;
      if(b == 1){
        Salida = String("0") + "," + String(humedad) + "," + String(temperatura) + "," + String(ruido) + "," + String(luz_2);
        Serial.println(Salida);
      }
      pasado = ahora;
    }

  }else{
    int ahora = millis();
    int cambio = ahora - pasado;
    emg = analogRead(A0);
      x[0]=((emg)/4);    // arreglo  para pasar de 10 bits a 8 bits con DAC de resistencias ponderadas
  
      y = x[0]-x[6]; // Ecuacion del filtro
      w = y*120/1024; //Calibración según músculo
    
      if(w<0){
        z = abs(w);
      } else{
        z = w;
      }

      x[6] = (x[5]); 
      x[5] = (x[4]); 
      x[4] = (x[3]);   // Actualizamos los valores del buffer
      x[3] = (x[2]);
      x[2] = (x[1]);
      x[1] = (x[0]);
      
      emg = z;
      
    if(cambio >= 200){
      count++;
      a = count%300;
      Salida = String("1") + "," + "-" + "," + "-" + "," + "-" + "," + "-" + "," + String(emg) + "," + String(girosc_ang_x) + "," + String(girosc_ang_y);
      if (a == 1){
        Salida = String("1") + "," + String(humedad) + "," + String(temperatura) + "," + String(ruido) + "," + String(luz_2) + "," + String(emg) + "," + String(girosc_ang_x) + "," + String(girosc_ang_y);
      }
      pasado = ahora;
      Serial.println(Salida);
    }

 }

}
