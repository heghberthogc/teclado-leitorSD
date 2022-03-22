/* 28-02-22
 *  Projeto Market Self
 *  Empresa: Start704
 *  Autor: Heghbertho G Costa - 85-992850808
 *  Melhorias no código, identificação de leds, uso de portas digitais
 *  Introdução de leitura pelo SD
 *########################### 02-03-22 
 * Incluir módulo Relógio - falha
  
*/
#include <Keypad.h> //INCLUSÃO DE BIBLIOTECA
#include <SD.h>
#include <SPI.h>




///##################################################################################################### Variáveis e constantes 

const byte qtdLinhas = 4; //QUANTIDADE DE LINHAS DO TECLADO
const byte qtdColunas = 3; //QUANTIDADE DE COLUNAS DO TECLADO
//CONSTRUÇÃO DA MATRIZ DE CARACTERES
char matriz_teclas[qtdLinhas][qtdColunas] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};



 
byte PinosqtdLinhas[qtdLinhas] = {3, 4, 5, 6}; //PINOS UTILIZADOS PELAS LINHAS
byte PinosqtdColunas[qtdColunas] = {8, 9, 7}; //PINOS UTILIZADOS PELAS COLUNAS (original pino 7 seria 10, porém arduino com porta defeituosa)
const int buzzer = A3;
const int RelePin = A4;
const int ledAmarelo = A0;
const int ledVermelho = A1;
const int ledVerde = A2;
const int pinoSS = 2;


String digitada;
int estado=0; 
String codigo; 
byte byteRead;
File myFile;
String buffer;
String listaArquivoSD = "id.txt";
int portaAberta = 5; // Aproximadamente 15 segundos (1000 + 700) miselimos
bool resultado;

//##################################################################################################### Funções 

void (*funcReset)() = 0;
  // Testa SD
  bool testaSD(){
    bool resultado=false;
    if (SD.begin()) {
      resultado=true;
    } else{
      resultado=false;
    }
   return resultado;
  }


bool FiltraID2(String arquivoSD, String id){
  myFile = SD.open(arquivoSD); // Abre o Arquivo
  bool busca = false;
  if (myFile) {
      while (myFile.available()) {
        buffer = myFile.readStringUntil('\n'); 
        if(buffer.indexOf(id) == 0 && buffer.length() -1 == id.length() ){
          return busca = true;
        }   
      }
      myFile.close();
  } 
  else {
    Serial.println("Erro na abertura do arquivo " + arquivoSD);
    beep3();
    funcReset();
  }
  return busca;
}


  

//INICIALIZAÇÃO DO TECLADO
Keypad meuteclado = Keypad( makeKeymap(matriz_teclas), PinosqtdLinhas, PinosqtdColunas, qtdLinhas, qtdColunas); 
 
void setup(){
  Serial.begin(9600); 
  pinMode(pinoSS, OUTPUT);
  pinMode(RelePin, OUTPUT); 
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);

  Serial.println(F("Aperte uma tecla...")); 
  Serial.println(); 
  digitalWrite(ledAmarelo, HIGH);

  if(testaSD()){
    Serial.println(F("SD Card pronto para uso.")); // Imprime na tela
    beep0();
  }else{
    Serial.println(F("Falha na inicialização do SD Card."));
    digitalWrite(ledVermelho, HIGH);
    beep3();    
  }

 
}


void loop(){

  
  char tecla = meuteclado.getKey(); //VERIFICA SE ALGUMA DAS TECLAS FOI PRESSIONADA - teclado 
  if(tecla != NO_KEY){
   digitalWrite(ledAmarelo, HIGH);
    
  Serial.println(tecla);
    estado=1;
    if(tecla=='#'){
      if(testaSD()){
        Serial.println("Ativado SD");
      } else {
        Serial.println("Erro SD");
        beep3();
      }
      
      
      beep0();
      digitalWrite(ledAmarelo, LOW);
      if(FiltraID2(listaArquivoSD, digitada) || digitada == "85992850808"){
        estado=3;
        leds(estado);
        estado=0;
      }else{
        estado=2;
        leds(estado);
        delay(500);
        estado=0;
      }
      digitada="";
    }else{
      digitada+=tecla;
    }
    leds(estado);
  }
  
}

void leds(int e){ //0=Aguardando, 1=Digitando, 2=Negado, 3=Aceito
  if(e==0){
  //  Serial.println(F("Aguardando"));
    digitalWrite(ledAmarelo, HIGH);
  }else if(e==1){
  //  Serial.println(F("Digitando "));
    tone(buzzer,449,100);
    delay(100);

  }else if(e==2){
    Serial.println(F("Negado"));
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(RelePin,LOW);
    beep2();
    delay(2000);
    digitalWrite(ledVermelho, LOW);
  }else if(e==3){
    Serial.println(F("Aceito"));
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVerde, HIGH);
    digitalWrite(RelePin,HIGH);
    beep4();
    digitalWrite(ledVerde, LOW);
    digitalWrite(RelePin,LOW);
    
  }
}



void beep0()
{
     tone(buzzer,2000);  
     delay(100); 
     noTone(buzzer);
}
void beep1()
{
     tone(buzzer,1500);  
     delay(1000); 
     noTone(buzzer);
}

void beep2()
{
     tone(buzzer,1000);  
     delay(500); 
     noTone(buzzer);
     delay(100);
     tone(buzzer,1000);  
     delay(500); 
     noTone(buzzer);
     delay(100);
     tone(buzzer,1000);  
     delay(500); 
     noTone(buzzer);
}


void beep3()
{
     tone(buzzer,500);  
     delay(300); 
     noTone(buzzer);
     delay(100);
     tone(buzzer,500);  
     delay(300); 
     noTone(buzzer);
     delay(100);
     tone(buzzer,500);  
     delay(300); 
     noTone(buzzer);
}

void beep4(){
  for(int i=0;i<portaAberta;i++){
    tone(buzzer,2500);  
    delay(700); 
    noTone(buzzer);
    delay(1000); 
  }
   for(int i=0;i<portaAberta-1;i++){
    tone(buzzer,2500);  
    delay(500); 
    noTone(buzzer);
    delay(700); 
  }
   for(int i=0;i<portaAberta;i++){
    tone(buzzer,2500);  
    delay(500); 
    noTone(buzzer);
    delay(200); 
  }
}
