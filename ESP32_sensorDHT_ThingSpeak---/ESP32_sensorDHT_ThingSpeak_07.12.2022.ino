#include <WiFi.h>
#include "ThingSpeak.h"

//trecho código DHT11-----------------------------------------------------------------------
#include "DHT.h"

#define pino_dht 4 // pino onde o sensor DHT está conectado
float temperatura; // variável para armazenar o valor de temperatura
float umidade; // variável para armazenar o valor de umidade
DHT dht(pino_dht, DHT11); // define o pino e o tipo de DHT
//fim do trecho DHT11-----------------------------------------------------------------------

// variaveis para alocar senha e nome da rede
const char* nome_da_rede = "GEFIE";
const char* senha = "@F4bul0s0@$";

// tipo de interação com Wifi
WiFiClient  client;

// variaveis para alocar ID do canal e chaves
unsigned long ID_canal = 1947618;
const char *Key_Write = "TCALHUNZ8WQV8HFE";

// minhas funções
void conectar_Wifi(const char* nome_da_rede, const char* senha);
void imprime_retorno(int valor);


// área das variáveis globais comuns
int leitura = 0; // ?????????????????????????????????????????????????????????

#define DHTPIN 4 //??????????????????????????????????????????????????????????

void setup() {
  //trecho de programa Rudolf________________________________________________I
  Serial.begin(115200); // iniciando Serial
  ThingSpeak.begin(client); // iniciando ThingSpeak
  //Fim trecho Rudolf________________________________________________________F

  //trecho código DHT11-----------------------------------------------------------------------
  dht.begin(); // inicializa o sensor DHT
  //fim do trecho DHT11-----------------------------------------------------------------------
}

void loop() {

  int resposta_escrita;
  
  conectar_Wifi(nome_da_rede, senha); // "função" de conexão
 
  //trecho código DHT11-----------------------------------------------------------------------
  //delay(2000); // 2 segundos (Datasheet)

  // A leitura da temperatura ou umidade pode levar 250ms
  // O atraso do sensor pode chegar a 2 segundos
  temperatura = dht.readTemperature(); // lê a temperatura em Celsius
  umidade = dht.readHumidity(); // lê a umidade

  // Se ocorreu alguma falha durante a leitura
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha na leitura do Sensor DHT!");
  }
  else { // Se não
    // Imprime o valor de temperatura
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" *C ");

    Serial.print("\t"); // tabulação

    // Imprime o valor de umidade
    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.print(" %\t");

    Serial.println(); // nova linha
    //fim do trecho-------------------------------------------------------------------------------

    resposta_escrita = ThingSpeak.writeField(ID_canal, 1, temperatura, Key_Write);
    Serial.println("Retorno da escrita da temperatura no canal:");
    imprime_retorno(resposta_escrita);
    resposta_escrita = ThingSpeak.writeField(ID_canal, 2, umidade, Key_Write);
    Serial.println("Retorno da escrita da umidade no canal:");
    imprime_retorno(resposta_escrita);
  }
}

// bloco de conexão Wifi
void conectar_Wifi(const char* nome_da_rede, const char* senha) {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Tentando se conectar a ");
    Serial.println(nome_da_rede);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(nome_da_rede, senha);
      delay(5000);
    }
    Serial.print("\n Conectado a ");
    Serial.println(nome_da_rede);
  }
}


// bloco de verificação, erros ThingSpeak
void imprime_retorno(int valor) {
   if (valor == 200) {
    Serial.println("Envio feito!!");
  }
  else if (valor == 404) {
    Serial.println("tive um problema!! Chave de API incorreta");
  }
  else if (valor == -101) {
    Serial.println("tive um problema!! O valor está fora de alcance ou a sequência é muito longa");
  }
  else if (valor == -201) {
    Serial.println("tive um problema!! Número de campo inválido");
  }
  else if (valor == -210) {
    Serial.println("tive um problema!! setField() não foi chamado antes de writeFields()");
  }
  else if (valor == -301) {
    Serial.println("tive um problema!! Falha ao conectar-se ao ThingSpeak");
  }
  else if (valor == -302) {
    Serial.println("tive um problema!! Falha inesperada durante a gravação para o ThingSpeak");
  }
  else if (valor == -303) {
    Serial.println("tive um problema!!Incapaz de analisar a resposta");
  }
  else if (valor == -304) {
    Serial.println("tive um problema!!Tempo limite, esperando que o servidor responda");
  }
  else if (valor == -401) {
    Serial.println("tive um problema!! Nada aconteu , os dados não foram envidos");
  }
  else {
    Serial.println("tive um problema!! HTTP codigo de erro " + String(valor));
  }
}
