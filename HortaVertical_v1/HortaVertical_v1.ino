//Programa: Monitoracao de quadro verde usando Arduino - Oficina SESC Carmo - SP
//Autor: Paulo Barbeiro - adaptação de FILIPEFLOP
/**/
 
#define higroA0 A0
#define higroA1 A1
#define higroA2 A2
#define higroA3 A3
#define higroA4 A4


#define pino_led_vermelho 3
#define pino_led_amarelo 2
#define pino_led_verde 1
 
int valor_higroA0, valor_higroA1, valor_higroA2, valor_higroA3, valor_higroA4, valor_higroA5;
int media;

// Medir a variação de corrente em solo seco, moderado e umido, e configurar os valores abaixo:
int limite_seco = 800;      // maior que 800 é seco
int limite_moderado = 400;  // maior que 400 e menor que limite_seco, é moderado
int limite_umido = 0;       // maior que 0 e menor que limite_moderado, é umido


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
  
}
 
void loop()
{
  //Le o valor de umidade dos sensores: pinos de A0 a A4
  valor_higroA0 = analogRead(higroA0);
  valor_higroA1 = analogRead(higroA1);
  valor_higroA2 = analogRead(higroA2);
  valor_higroA3 = analogRead(higroA3);
  valor_higroA4 = analogRead(higroA4);

  //Calcula média de umidade entre todos os sensores
  media = valor_higroA0 + valor_higroA1 + valor_higroA2 + valor_higroA3 + valor_higroA4;
  media = media / 5;
 
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

  // VERIFICAÇÃO DA UMIDADE
  // Definir o intervalo de verificações. Deixar apenas uma das opções descomentada.
  // 1- a cada segundo
  // delay(1000);
  
  // 2- a cada minuto
  //delay(60000);
  
  // 3- a cada hora
  delay(60000*60);
  
  // 4- uma vez ao dia:
  //delay(86400*1000);
}
 
void apagaleds()
{
  digitalWrite(pino_led_vermelho, LOW);
  digitalWrite(pino_led_amarelo, LOW);
  digitalWrite(pino_led_verde, LOW);
}
