#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definição de GPIOs
#define BUZZER_GPIO 21
#define BUTTON1_GPIO 16
#define BUTTON2_GPIO 17
#define BUTTON3_GPIO 18



// Frequências para diferentes sons
#define NOTE_C4 262  // Dó
#define NOTE_D4 294  // Ré
#define NOTE_E4 330  // Mi
#define NOTE_F4 349  // Fá
#define NOTE_G4 392  // Sol
#define NOTE_A4 440  // Lá
#define NOTE_B4 494  // Si


// Função para configurar o PWM no pino especificado
void pwm_set_frequency(uint slice_num, uint channel, float frequency) {
    if (frequency == 0) {
        // Desativa o PWM para parar o som
        pwm_set_enabled(slice_num, false);
    } else {
        uint32_t clock_freq = 125000000; // Frequência do clock do Pico (125 MHz)
        uint32_t divider = clock_freq / frequency / 4096; // Calcula o divisor
        if (divider < 1) divider = 1;
        if (divider > 255) divider = 255;

        pwm_set_clkdiv(slice_num, divider);
        uint32_t wrap = clock_freq / divider / frequency - 1;
        pwm_set_wrap(slice_num, wrap);
        pwm_set_chan_level(slice_num, channel, wrap / 2); // 50% duty cycle
        pwm_set_enabled(slice_num, true);
    }
}

int main() {
    // Inicializa os GPIOs dos botões e do buzzer
    gpio_init(BUTTON1_GPIO);
    gpio_set_dir(BUTTON1_GPIO, GPIO_IN);
    gpio_pull_up(BUTTON1_GPIO);

    gpio_init(BUTTON2_GPIO);
    gpio_set_dir(BUTTON2_GPIO, GPIO_IN);
    gpio_pull_up(BUTTON2_GPIO);

    gpio_init(BUTTON3_GPIO);
    gpio_set_dir(BUTTON3_GPIO, GPIO_IN);
    gpio_pull_up(BUTTON3_GPIO);

    gpio_set_function(BUZZER_GPIO, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_GPIO);
    uint channel = pwm_gpio_to_channel(BUZZER_GPIO);

    //implementação do botão A GPIO 5 Botão A
    const uint BUTTON_PIN_A = 5;
    gpio_init(BUTTON_PIN_A);
    gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_A);

    //implementação do botão B GPIO 5
    const uint BUTTON_PIN_B = 6;
    gpio_init(BUTTON_PIN_B);
    gpio_set_dir(BUTTON_PIN_B, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_B);


    while (true) {
        if (!gpio_get(BUTTON_PIN_A)) {
            pwm_set_frequency(slice_num, channel, NOTE_C4);
        }
         else if (!gpio_get(BUTTON_PIN_B)) {
            pwm_set_frequency(slice_num, channel, NOTE_D4);
        } else if (!gpio_get(BUTTON3_GPIO)) {
            pwm_set_frequency(slice_num, channel, NOTE_E4);
        
        } else {
            pwm_set_frequency(slice_num, channel, 0); // Sem som
        }
        sleep_ms(100); // Pequeno atraso para evitar polling excessivo
    }
}
