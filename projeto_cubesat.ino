/*
 * Projeto Final
 * Software de bordo do Arduino Cubesat
 * Nome: Rodrigo M. Mariano
 */

// sensores
int sensor_luminosidade = A1;  // LDR
int sensor_termico = A2;       // NTC
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
int tempo_repetir_telemetria_ms = 10000; // 30000ms = 30s

// tempo da última ocorrência
int tempo_ultima_ocorrencia = millis();


void piscar_power_tres_vezes () {
  // (a) piscar o indicador de power 3 vezes
  for (int x=0; x<3; x++) {
    digitalWrite(indicador_power, HIGH);
    delay(tempo_indicador_power_ms);
    digitalWrite(indicador_power, LOW);
    delay(tempo_indicador_power_ms);
  }  
}

void verificar_sensor_luminosidade () {
  // (b)
  // (b.1)
  int tensao_ldr = analogRead(sensor_luminosidade);
  int lux = tensao_ldr * 1000;
  Serial.print("Lux: ");
  Serial.println(lux);

  // (e) se a luminosidade for menor do que 1000 Lux, liga o led sombra
  if (lux < 1000) {
    digitalWrite(indicador_sombra, HIGH);
    Serial.println("Luminosidade menor do que 1000 Lux...");
  } else {
    digitalWrite(indicador_sombra, LOW);
  }

  Serial.println();
}

void verificar_sensor_temperatura () {
  // TODO: (b.2) enviar o valor da temperatura
  int graus_celsius = 0;
  int graus_kelvin = 0;
  Serial.println("Temperatura: ");
  Serial.print("Celsius: ");
  Serial.println(graus_celsius);
  Serial.print("Kelvin: ");
  Serial.println(graus_kelvin);

  // (d) se a temperatura for maior do que 30°C, liga o led de temperatura
  if (graus_celsius > 30) {
    digitalWrite(indicador_temp_high, HIGH);
    Serial.println("Temperatura maior do que 30°C, cuidado...");
  } else {
    digitalWrite(indicador_temp_high, LOW);
  }

  Serial.println();
}

void verificar_sensor_bateria () {
  // (b.3) enviar o consumo da bateria (potenciomêtro)
  int consumo_bateria = analogRead(sensor_bateria);
  Serial.println("Consumo da bateria: ");
  Serial.println(consumo_bateria);

  // (f) se o consumo de bateria for maior do que 150mA, liga o led
  // sobrecarga e enviar um aviso
  if (consumo_bateria > 150) {
    digitalWrite(indicador_sobrecarga, HIGH);
    Serial.println("Sobrecarga no consumo de bateria, cuidado...");
  } else {
    digitalWrite(indicador_sobrecarga, LOW);
  }

  Serial.println();
}

void enviar_sinal_telemetria () {
  // (c) transmitir a cada 30s um sinal de telemetria
  int tempo_agora = millis();

  // verifica se já transcorreram 'tempo_ultima_ocorrencia'ms deste a última alteração
  if ((tempo_agora - tempo_ultima_ocorrencia) >= tempo_repetir_telemetria_ms){ 
    tone(telemetria, 800);
    delay(500);
    noTone(telemetria);
    delay(500);
  
    // ativar o led de telemetria (azul) e enviar um aviso pela porta serial
    digitalWrite(indicador_tx_telemetria, HIGH);
    delay(1000);
    digitalWrite(indicador_tx_telemetria, LOW);
    delay(1000);
    Serial.println("Enviando um sinal de telemetria...");

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
  // verificar_sensor_luminosidade ();  // ok
  // verificar_sensor_temperatura ();
  // verificar_sensor_bateria ();       // ok
  // enviar_sinal_telemetria ();        // ok

  delay(500);
}
