//Programa: Monitoracao de quadro verde usando Arduino - Oficina SESC Carmo - SP
//Autor: Paulo Barbeiro - adaptação de FILIPEFLOP
/**/
 
#define higroA0 A0
#define higroA1 A1
#define higroA2 A2
#define higroA3 A3
#define higroA4 A4

#define valvula 4

#define pino_led_vermelho 3
#define pino_led_amarelo 2
#define pino_led_verde 1

#define buttonPin 5

int valor_higroA0, valor_higroA1, valor_higroA2, valor_higroA3, valor_higroA4, valor_higroA5;
int media;


// Medir a variação de corrente em solo seco, moderado e umido, e configurar os valores abaixo:
int limite_seco = 800;      // maior que 800 é seco
int limite_moderado = 400;  // maior que 400 e menor que limite_seco, é moderado
int limite_umido = 0;       // maior que 0 e menor que limite_moderado, é umido

//Sistema de irrigação tem capacidade de 4L/h
float litrosDesejados = 1/4.0; // 1.0 = 1litro  ::  1/2.0 = 500ml  ::  1/4.0 = 250ml
float tempoParaLitros = (litrosDesejados*4.0)*(3600000);

/*  UNIDADES DE TEMPO::
      1 segundo = 1000;
      1 minuto  = 60000;    -> 60*segundo
      1 hora    = 3600000;  -> 60*minuto
      12 horas  = 43200000; -> 12*hora
      24 horas  = 86400000; -> 24*hora
*/

// Intervalo para verificação de irrigação
unsigned long intervaloIrrigacao = 60000;
unsigned long tempoAtualIrrigacao;
unsigned long tempoAnteriorIrrigacao = 0;
unsigned long ultimaIrrigacao;
int estadoValvula = LOW;

// Intervalo para verificações de umidade
unsigned long intervaloVerificaUmidade = 60000;
unsigned long tempoAtualUmidade;
unsigned long ultimaVerificacaoUmidade;




void setup()
{
  //Mostra o valor da porta analogica no serial monitor. Usar apenas para configuração.
  Serial.begin(9600);
  
  pinMode(higroA0, INPUT);
  pinMode(higroA1, INPUT);
  pinMode(higroA2, INPUT);
  pinMode(higroA3, INPUT);
  pinMode(higroA4, INPUT);
  
  pinMode(pino_led_vermelho, OUTPUT);
  pinMode(pino_led_amarelo, OUTPUT);
  pinMode(pino_led_verde, OUTPUT);

  pinMode(valvula, OUTPUT);

  pinMode(buttonPin, INPUT);
  
}
 
void loop()
{

  int estadoBotao = digitalRead(buttonPin);

  if( estadoBotao == HIGH ){
    digitalWrite(13, HIGH);
    }
  else{
    digitalWrite(13, LOW);
    }
  
  tempoAtualUmidade = millis();
  

  //Verificação de umidade para leds
  if(tempoAtualUmidade - ultimaVerificacaoUmidade > intervaloVerificaUmidade) {
    
    // save the last time you blinked the LED 
    ultimaVerificacaoUmidade = tempoAtualUmidade;   

    //Calcula média de umidade entre todos os sensores
    media = calculaMedia();
    media = valor_higroA0*1.9;
 
    //Mostra o valor da porta analogica no serial monitor
    //Descomente as linhas abaixo para configurar o programa.
    //Comente as linhas abaixo para o uso final. Economia de energia.
    /*
    Serial.print("Medição dos sensores: A0: ");
    Serial.print(valor_higroA0);
    Serial.print(" - A1: ");
    Serial.print(valor_higroA1);
    Serial.print(" - A2: ");
    Serial.print(valor_higroA2);
    Serial.print(" - A3: ");
    Serial.print(valor_higroA3);
    Serial.print(" - A4: ");
    Serial.print(valor_higroA4);
    Serial.print(" - Média: ");
    Serial.println(media);//*/


    //Media :: Solo umido, acende o led verde
    if (media > limite_umido && media < limite_moderado){
      //Comando abaixo (Serial.println()) - Usar somente para configuração do sistema
      //Serial.println(" Status media: Solo umido");
      apagaleds();
      digitalWrite(pino_led_verde, HIGH);
    }
    else if (media > limite_moderado && media < limite_seco){
      //Comando abaixo (Serial.println()) - Usar somente para configuração do sistema
      //Serial.println(" Status media: Umidade moderada");
      apagaleds();
      digitalWrite(pino_led_amarelo, HIGH);
    }
    else if (media > limite_seco && media < 1024){
      //Comando abaixo (Serial.println()) - Usar somente para configuração do sistema
      //Serial.println(" Status media: Solo seco");
      apagaleds();
      digitalWrite(pino_led_vermelho, HIGH);
    }
    
  }//verificação da umidade */

  

  //Verificação do tempo para irrigação
  tempoAtualIrrigacao = millis();
  
  if( (estadoValvula == HIGH) && (tempoAtualIrrigacao-tempoAnteriorIrrigacao >= tempoParaLitros) ) {
      desligaValvula();
  }
  else if( (media > limite_seco && media < 1024) && (estadoValvula == LOW) ){
      ligaValvula();
  }
  else if( estadoBotao == HIGH ){
      ligaValvula();
  }

}

int calculaMedia(){
    //Le o valor de umidade dos sensores: pinos de A0 a A4
    valor_higroA0 = analogRead(higroA0);
    valor_higroA1 = analogRead(higroA1);
    valor_higroA2 = analogRead(higroA2);
    valor_higroA3 = analogRead(higroA3);
    valor_higroA4 = analogRead(higroA4);
    
    return (valor_higroA0 + valor_higroA1 + valor_higroA2 + valor_higroA3 + valor_higroA4)/5;
  }

void ligaValvula(){
    estadoValvula = HIGH;
    tempoAnteriorIrrigacao = tempoAtualIrrigacao;
    media = calculaMedia();
    digitalWrite(valvula, estadoValvula);
    Serial.print("Inicia Valvula. Duracao: "); Serial.println(tempoParaLitros);
  }

void desligaValvula(){
    estadoValvula = LOW;
    tempoAnteriorIrrigacao = tempoAtualIrrigacao;
    digitalWrite(valvula, estadoValvula);
    Serial.println("Finaliza Valvula");
  }

void apagaleds(){
  digitalWrite(pino_led_vermelho, LOW);
  digitalWrite(pino_led_amarelo, LOW);
  digitalWrite(pino_led_verde, LOW);
}
