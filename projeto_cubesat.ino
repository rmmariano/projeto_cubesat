/*
 * Projeto Final
 * Software de bordo do Arduino Cubesat
 * Nome: Rodrigo M. Mariano
 */

#include <Thermistor.h>

// sensores
int sensor_luminosidade = A1;  // LDR      
Thermistor sensor_termico(A5); // NTC
int sensor_bateria = A3;       // Potenciômetro

// indicadores (LEDs)
int indicador_power = 2;          // branco
int indicador_tx_telemetria = 3;  // azul
int indicador_temp_high = 4;      // amarelo
int indicador_sombra = 5;         // verde
int indicador_sobrecarga = 6;     // vermelho

// quantidade de tempo em ms que o indicador de power vai piscar
int tempo_indicador_power_ms = 500;

// telemetria
int telemetria = 9; // buzzer (pwn)
int tempo_repetir_telemetria_ms = 20000; // 20000ms = 20s

// tempo da última ocorrência (inicializa com o tempo de agora)
int tempo_ultima_ocorrencia = millis();

// tempo (ms) do delay para cada iteracao da função loop()
int delay_iteracao = 1000;


void piscar_power_tres_vezes () {
  delay(1000);

  // (a) piscar o indicador de power (branco) 3 vezes
  for (int x=0; x<3; x++) {
    digitalWrite(indicador_power, HIGH);
    delay(tempo_indicador_power_ms);
    digitalWrite(indicador_power, LOW);
    delay(tempo_indicador_power_ms);
  }  
}

void verificar_sensor_luminosidade () {
  // (b.1) enviar sensor de luminosidade usando a fórmula abaixo
  long tensao_ldr = analogRead(sensor_luminosidade);
  long lux = tensao_ldr * 1000;
  Serial.print("Lux: ");
  Serial.println(lux);

  // (e) se a luminosidade for menor do que 100.000 Lux, 
  // liga o led sombra (verde)
  if (lux < 100000) {
    digitalWrite(indicador_sombra, HIGH);
    Serial.println("Luminosidade menor do que 100000 Lux...");
  } else {
    digitalWrite(indicador_sombra, LOW);
  }

  Serial.println();
}

void verificar_sensor_temperatura () {
  // (b.2) enviar o valor da temperatura em °C e °K
  // pega a temperatura do sensor em °C e converte para °K
  int graus_celsius = sensor_termico.getTemp();
  int graus_kelvin = graus_celsius + 273.15;

  Serial.print("Temperatura: ");
  Serial.print(graus_celsius);
  Serial.print("°C - ");
  Serial.print(graus_kelvin);
  Serial.println("°K");

  // (d) se a temperatura for maior do que 26°C, 
  // liga o led de temperatura (amarelo)
  if (graus_celsius > 26) {
    digitalWrite(indicador_temp_high, HIGH);
    Serial.println("Temperatura maior do que 26°C, cuidado...");
  } else {
    digitalWrite(indicador_temp_high, LOW);
  }

  Serial.println();
}

void verificar_sensor_bateria () {
  // (b.3) enviar o consumo da bateria (potenciomêtro)
  int consumo_bateria = analogRead(sensor_bateria);

  // mapeia de 0 de 1023 (sensor) para 0 a 200mA
  consumo_bateria = map(consumo_bateria, 0, 1023, 0, 200);

  Serial.print("Consumo da bateria: ");
  Serial.print(consumo_bateria);
  Serial.println("mA");

  // (f) se o consumo de bateria for maior do que 150mA, 
  // liga o led sobrecarga (vermelho) e enviar um aviso
  if (consumo_bateria > 150) {
    digitalWrite(indicador_sobrecarga, HIGH);
    Serial.println("Sobrecarga no consumo de bateria, cuidado...");
  } else {
    digitalWrite(indicador_sobrecarga, LOW);
  }

  Serial.println();
}

void enviar_sinal_telemetria () {
  // (c) transmitir a cada 20s um sinal de telemetria
  // tempo atual no arduino
  int tempo_agora = millis();

  // tempo passado desde o último sinal
  int tempo_passado = tempo_agora - tempo_ultima_ocorrencia;

  // verifica se já transcorreram 'tempo_ultima_ocorrencia'ms deste a última alteração
  if (tempo_passado >= tempo_repetir_telemetria_ms){
    // Serial.print("Tempo passado desde o último sinal de telemetria: ");
    // Serial.print(tempo_passado);
    // Serial.println("ms");

    // envia o sinal de telemetria (ativa o som do buzzer)
    tone(telemetria, 800);
    // ativar o led de telemetria (azul)
    digitalWrite(indicador_tx_telemetria, HIGH);
    // enviar um aviso pela porta serial
    Serial.println("Enviando um sinal de telemetria...");
    
    delay(500);

    // desliga o som e o led    
    noTone(telemetria);
    digitalWrite(indicador_tx_telemetria, LOW);    

    // salva o tempo da ultima ocorrencia de telemetria
    tempo_ultima_ocorrencia = tempo_agora;

    Serial.println();
  }
}

void setup () {
  Serial.begin(9600);

  // inicializa os indicadores (LEDs) como saída
  pinMode(indicador_power, OUTPUT);
  pinMode(indicador_tx_telemetria, OUTPUT);
  pinMode(indicador_temp_high, OUTPUT);
  pinMode(indicador_sombra, OUTPUT);
  pinMode(indicador_sobrecarga, OUTPUT);

  // inicializa a telemetria (buzzer) como saída
  pinMode(telemetria, OUTPUT);

  piscar_power_tres_vezes ();  
}

void loop () {
  Serial.println("**************************************************\n");

  verificar_sensor_luminosidade ();
  verificar_sensor_temperatura ();
  verificar_sensor_bateria ();
  enviar_sinal_telemetria ();

  delay(delay_iteracao);
}
