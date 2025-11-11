#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -----------------------------------------------------------------
// --- DEFINICIONES DE HARDWARE Y PATRONES ---
// -----------------------------------------------------------------

unsigned char PORT_ROW[8] = {1, 2, 4, 8, 16, 32, 64, 128};

// ENTRADAS (Botones con Pull-up)
#define BOTON_MOVER_DER PINC0
#define BOTON_MOVER_IZQ PINC1
#define BOTON_DIFICULTAD_1 PINC1
#define BOTON_DIFICULTAD_2 PINC2
#define BOTON_DIFICULTAD_3 PINC3

// SALIDAS (Señales de Resultado)
#define PIN_GANADA PINC4
#define PIN_PERDIDA PINC5

// --- PATRÓN FIJO DE OBSTÁCULOS (2x2) ---
const unsigned char OBSTACULOS_PATRON[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
    0x00,0x00,
    0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,
    0x00,0x00,
    0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,
    0x00,0x00,0x00,0x00,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
const int PATRON_SIZE = sizeof(OBSTACULOS_PATRON);

// --- MATRICES DE NÚMEROS AUMENTADAS Y CORREGIDAS ---
const unsigned char MSG_WELCOME[8] = {0x00,0x00,0x00,0x3C,0x42,0x3C,0x00,0x00};

// NUM_1 CORREGIDO
const unsigned char NUM_1[8] = {
   0b00000000,0b00010000,0b00011000,0b00010000,0b00010000,0b00010000,0b00111000,0b00000000
};

// NUM_2
const unsigned char NUM_2[8] = {
    0b00000000,0b00111110,0b01000010,0b00100000,0b00011000,0b00000010,0b01111111,0b00000000
};

// NUM_3 CORREGIDO
const unsigned char NUM_3[8] = {
    0b00000000,0b00111100,0b01000000,0b00111000,0b01000000,0b01000000,0b00111100,0b00000000
};

// -----------------------------------------------------------------
// --- PROTOTIPOS DE FUNCIONES ---
// -----------------------------------------------------------------
void init_perifericos();
void juego_esquivar(int dificultad);
void mostrar_matriz(unsigned char matriz[8], int retardo_ms);
void transpose_matrix(const unsigned char source[8], unsigned char destination[8]);

// -----------------------------------------------------------------
// --- IMPLEMENTACIÓN DE FUNCIONES AUXILIARES ---
// -----------------------------------------------------------------
void init_perifericos() {
    DDRB = 0xFF;
    DDRD = 0xFF;
    DDRC |= (1 << PIN_GANADA);
    DDRC |= (1 << PIN_PERDIDA);
    DDRC &= ~(1 << PINC0);
    DDRC &= ~(1 << PINC1);
    DDRC &= ~(1 << PINC2);
    DDRC &= ~(1 << PINC3);
    DDRC &= ~(1 << PINC6);
    PORTC |= (1 << PINC0);
    PORTC |= (1 << PINC1);
    PORTC |= (1 << PINC2);
    PORTC |= (1 << PINC3);
    PORTC &= ~(1 << PIN_GANADA);
    PORTC &= ~(1 << PIN_PERDIDA);
    PORTC |= (1 << PINC6);
}

void mostrar_matriz(unsigned char matriz[8], int retardo_ms) {
    int ciclos = retardo_ms / 6;
    for (int k = 0; k < ciclos; k++) {
        for (int j = 0; j < 8; j++) {
            PORTD = PORT_ROW[j];
            PORTB = ~matriz[j];
            _delay_ms(0.08);
        }
    }
    PORTD = 0x00;
    PORTB = 0xFF;
}

void transpose_matrix(const unsigned char source[8], unsigned char destination[8]) {
    for (int i = 0; i < 8; i++) {
        destination[i] = 0;
        for (int j = 0; j < 8; j++) {
            if (source[j] & (1 << i)) destination[i] |= (1 << j);
        }
    }
}

// -----------------------------------------------------------------
// --- IMPLEMENTACIÓN DE LA LÓGICA DEL JUEGO ---
// -----------------------------------------------------------------
void juego_esquivar(int dificultad) {
    unsigned char game_data[8] = {0};
    unsigned char transposed_display[8] = {0};
    unsigned char player_pos = 0x1C;
    game_data[7] = player_pos;
    int step_delay_ms;
    switch (dificultad) {
        case 1: step_delay_ms = 400; break;
        case 2: step_delay_ms = 200; break;
        case 3: step_delay_ms = 100; break;
        default: step_delay_ms = 300; break;
    }
    int obstaculo_indice = 0;
    int juego_activo = 1;
    long frame_counter = 0;
    while (juego_activo) {
        if (!(PINC & (1 << BOTON_MOVER_IZQ))) {
            if (!(player_pos & 0x80)) player_pos <<= 1;
            _delay_ms(50);
        }
        if (!(PINC & (1 << BOTON_MOVER_DER))) {
            if (!(player_pos & 0x01)) player_pos >>= 1;
            _delay_ms(50);
        }
        game_data[7] = player_pos;
        if (frame_counter % (step_delay_ms / 10) == 0) {
            if (game_data[6] & player_pos) {
                if (game_data[6] == 0xFF) {
                    juego_activo = 0;
                    PORTC |= (1 << PIN_GANADA);
                    break;
                }
                juego_activo = 0;
                PORTC |= (1 << PIN_PERDIDA);
                break;
            }
            for (int i = 7; i > 0; i--) game_data[i] = game_data[i - 1];
            if (obstaculo_indice < PATRON_SIZE) game_data[0] = OBSTACULOS_PATRON[obstaculo_indice++];
            else game_data[0] = 0x00;
        }
        transpose_matrix(game_data, transposed_display);
        mostrar_matriz(transposed_display, 10);
        frame_counter++;
    }
    unsigned char end_game_screen[8] = {0x00};
    if (PORTC & (1 << PIN_PERDIDA)) {
      end_game_screen[0] = 0x81; end_game_screen[1] = 0x42; end_game_screen[2] = 0x24; end_game_screen[3] = 0x18;
      end_game_screen[7] = 0x81; end_game_screen[6] = 0x42; end_game_screen[5] = 0x24; end_game_screen[4] = 0x18;
    } else if (PORTC & (1 << PIN_GANADA)) {
        end_game_screen[2] = 0x81; end_game_screen[3] = 0x42; end_game_screen[4] = 0x24; end_game_screen[5] = 0x18;
    }
    transpose_matrix(end_game_screen, transposed_display);
    mostrar_matriz(transposed_display, 3000);
    PORTD = 0x00;
    PORTB = 0xFF;
}

// -----------------------------------------------------------------
// --- FUNCIÓN PRINCIPAL (MAIN) ---
// -----------------------------------------------------------------
int main(void) {
    init_perifericos();
    srand(100);
    while (1) {
        PORTC &= ~((1 << PIN_GANADA) | (1 << PIN_PERDIDA));
        int dificultad = 0;
        unsigned char temp_display[8];
        while (dificultad == 0) {
            transpose_matrix(MSG_WELCOME, temp_display);
            mostrar_matriz(temp_display, 100);
            if (!(PINC & (1 << BOTON_DIFICULTAD_1))) dificultad = 1;
            else if (!(PINC & (1 << BOTON_DIFICULTAD_2))) dificultad = 2;
            else if (!(PINC & (1 << BOTON_DIFICULTAD_3))) dificultad = 3;
        }
        const unsigned char *num_pattern;
        switch(dificultad) {
            case 1: num_pattern = NUM_1; break;
            case 2: num_pattern = NUM_2; break;
            case 3: num_pattern = NUM_3; break;
            default: num_pattern = NUM_1; break;
        }
        transpose_matrix(num_pattern, temp_display);
        mostrar_matriz(temp_display, 1500);
        if (dificultad > 0) juego_esquivar(dificultad);
    }
}
