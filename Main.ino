//Пин аппаратного прерывания
constexpr uint8_t INTERRUPT_PIN = 2;

//Время начала импульса
volatile uint32_t sTime;
//Время окончания импульса
volatile uint32_t oTime;
//Флаг сообщающий что импульс произошёл
volatile bool interruptHappened = false;

//Быстрый аналог digitalRead() для Atmega328
bool digitalReadFast(uint8_t pin) {
  if (pin < 8) {
    return bitRead(PIND, pin);
  } else if (pin < 14) {
    return bitRead(PINB, pin - 8);
  } else if (pin < 20) {
    return bitRead(PINC, pin - 14);
  }
}

//Handler для прерываний
void isrHandler() {
  //теперь сигнал высокий
  if (digitalReadFast(INTERRUPT_PIN)) {
    sTime = micros();
  }
  //Теперь сигнал низкий
  else {
    oTime = micros() - sTime;
    interruptHappened = true;
  }
}

void setup() {
  //Запуск UART
  Serial.begin(115200);

  //Пин 2 теперь вход
  pinMode(INTERRUPT_PIN, INPUT);

  //Подключение прерывания
  attachInterrupt(
    digitalPinToInterrupt(INTERRUPT_PIN),
    isrHandler,
    CHANGE
  );
}

void loop() {
  //Импульс произошёл
  if (interruptHappened) {
    //Сбрасываем флаг
    interruptHappened = false;

    //Расчёт реального времени
    uint32_t t = oTime;

    //Вывод данных
    {
      Serial.print("Microseconds: ");
      Serial.println(t);
    }
  }
}
