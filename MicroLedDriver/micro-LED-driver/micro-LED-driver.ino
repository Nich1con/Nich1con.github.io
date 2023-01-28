#define PWM_PIN 0
#define BTN_PIN 2

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  pwm_init();
}

void loop() {
  static uint8_t mode = 0;
  static bool btn_state = false, btn_flag = false;
  static uint32_t btn_timer = millis(), timer = millis();

  if (millis() - btn_timer >= 64) {
    btn_timer = millis();
    btn_state = !digitalRead(BTN_PIN);
  }

  if (btn_state && !btn_flag) {
    btn_flag = true;
    timer = millis();
  }

  if (!btn_state && btn_flag) {
    btn_flag = false;
    if (++mode > 3) mode = 0; 
    switch (mode) {
      case 0: fadeOff(); break;
      case 1: fadeFromTo(0, 10);  break;
      case 2: fadeFromTo(10, 40); break;
      case 3: fadeFromTo(40, 70); break;
    }
  }
}

void fadeOff(void) {
  for (uint8_t i = 70; i; i--) {
    pwm_write(i);
    delay(10);
  } pwm_write(0);
}

void fadeFromTo(uint8_t from, uint8_t to) {
  for (uint8_t i = from; i < to + 1; i++) {
    pwm_write(i);
    delay(10);
  }
}

/* Функции для генерации ШИМ */
void pwm_init(void) {
  /*
      Режим Phase correct PWM с потолком ICR0
      Процентное разрешение ШИМ (0-100)
  */
  TCCR0A = (1 << WGM01) | (1 << COM0A1);  // WGM01 + ШИМ на канале A
  TCCR0B = (1 << WGM03) | 0b10;           // WGM03 + делитель /8
  ICR0 = 100;                             // Счёт до 100
  OCR0A = 50;                              // Заполнение 0
}

void pwm_write(uint8_t duty) {
  OCR0A = duty;
}
