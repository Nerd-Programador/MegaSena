#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// Definir a resolução do display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Inicializar o objeto do display OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definir o pino do botão
const int buttonPin = 1;

// Definir os limites do sorteio
const int minNumber = 1;
const int maxNumber = 60;
const int numNumbers = 6;

void setup() {
  // Configurar o pino do botão como entrada com resistor de pull-up interno
  pinMode(buttonPin, INPUT_PULLUP);

  // Iniciar a comunicação I2C (protocolo usado pelo display OLED) com os pinos SDA (4 D2) e SCL (5 D1) do NodeMCU
  //Wire.begin(4, 5);

  // Inicializar o display OLED com endereço 0x3C e modo de tensão de alimentação SSD1306_SWITCHCAPVCC
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Limpar a tela e exibir uma mensagem inicial
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Pressione o botao");
  display.display();
}

// Função para verificar se um número já foi sorteado anteriormente
bool isDuplicate(int number, int* numbers, int size) {
  for (int i = 0; i < size; i++) {
    if (number == numbers[i]) {
      return true;
    }
  }
  return false;
}

void loop() {
  // Verificar se o botão foi pressionado (o pino está em nível baixo devido ao resistor de pull-up)
  if (digitalRead(buttonPin) == LOW) {
    // Limpar a tela e exibir uma mensagem de sorteio em andamento
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Sorteando...");
    display.display();
    delay(1000);

    // Inicializar um array para armazenar as dezenas sorteadas
    int numbers[numNumbers];

    // Usar o tempo atual em microssegundos como semente para o gerador de números aleatórios
    randomSeed(micros());

    // Realizar o sorteio das dezenas
    for (int i = 0; i < numNumbers; i++) {
      int num;
      do {
        // Gerar um número aleatório entre minNumber e maxNumber (incluindo os valores 01 e 60)
        num = random(minNumber, maxNumber + 1);
      } while (isDuplicate(num, numbers, i)); // Verificar se o número já foi sorteado anteriormente

      // Armazenar a dezena sorteada no array
      numbers[i] = num;
    }

    // Limpar a tela e exibir as dezenas sorteadas
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("Dezenas sorteadas:");
    display.setCursor(0, 30);
    for (int i = 0; i < numNumbers; i++) {
      display.print(numbers[i]);
      display.print(" ");
    }
    display.display();
  }
}
