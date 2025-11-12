

void Do_redonda(){
Sound_Play(523.25,400);

}


void Do_blanca(){
Sound_Play(523.25,200);
}

void Do_uno(){
Sound_Play(523.25,100);
}

void Do_medio(){
Sound_Play(523.25,50);
}

void Do_trescuartos(){
Sound_Play(523.25,75);
}

void Do_uncuarto(){
Sound_Play(523.25,25);
}

void Do_unoymedio(){
Sound_Play(523.25,150);
}

void Re_uno(){
Sound_Play(587.33,100);
}

void Re_medio(){
Sound_Play(587.33,50);
}

void Mi_medio(){
Sound_Play(659.25,50);
}

void Mi_uno(){
Sound_Play(659.25,100);
}

void Fa_medio(){
Sound_Play(698.45,50);
}

void Fa_uno(){
Sound_Play(698.45,100);
}

void Sol_medio(){
Sound_Play(783.99,50);
}

void Sol_uno(){
Sound_Play(783.99,100);
}

void Sol_unoymedio(){
Sound_Play(783.99,150);
}

void La_medio(){
Sound_Play(440,100);
}

void LaUno(){
Sound_Play(880,50);
}

void La_uno(){
Sound_Play(440,50);
}

void B_uno(){
Sound_Play(493.88,50);
}

void B_medio(){
Sound_Play(493.88,25);
}


void Silencio(){
Delay_ms(100);
}

void Silencio_cuatrotiempos(){
Delay_ms(400);
}

void Melodia(){

Sol_uno();Sol_uno();Mi_medio();Fa_medio();Sol_medio();La_medio();
Sol_uno();Fa_uno();Mi_uno();Re_uno();
Mi_uno();Mi_uno();Do_medio();Re_medio();Mi_medio();Fa_medio();
Mi_uno();Re_uno();Do_blanca();
Do_medio();Do_medio();Do_uno();La_uno;
Sol_unoymedio();Do_medio();Do_uno();Re_medio();Mi_medio();
Fa_uno();Mi_uno();Re_uno();Do_uno();
Re_uno();Silencio();Do_trescuartos();Do_uncuarto();Do_uno();
Do_redonda();
Silencio_cuatrotiempos();

}


/*
   [Mi_uno(); B_medio(); Do_medio(); Re_uno(); Do_medio(); B_medio();
  La_uno(); La_medio(); Do_medio(); Mi_uno(); Re_medio(); Do_medio(); B_uno(); B_medio();
  Do_medio(); Re_uno(); Mi_uno(); Do_uno(); La_uno(); La_uno();
  Fa_uno(); LaUno(); Sol_medio(); Fa_medio(); Mi_uno(); Do_medio(); Mi_uno(); Re_medio();
  Do_medio(); B_uno(); B_medio(); Do_medio(); Re_uno(); Mi_uno(); Do_uno(); La_uno(); La_uno();]
*/



juego = [Mi_uno(), B_medio(), Do_medio(), Re_uno(), Do_medio(), B_medio(),
  La_uno(), La_medio(), Do_medio(), Mi_uno(), Re_medio(), Do_medio(), B_uno(), B_medio(),
  Do_medio(), Re_uno(), Mi_uno(), Do_uno(), La_uno(), La_uno(),
  Fa_uno(), LaUno(), Sol_medio(), Fa_medio(), Mi_uno(), Do_medio(), Mi_uno(), Re_medio(),
  Do_medio(), B_uno(), B_medio(), Do_medio(), Re_uno(), Mi_uno(), Do_uno(), La_uno(), La_uno()];
  
  
void videojuego(){


  for(int j=0 ; j<2; j++ ){
  
           if(Button(&PORTB,4,1,1) | Button(&PORTB,5,1,1)){
         break;
         }
  }

}

// Sonido de victoria

void sol(int duration) {
    Sound_Play(392.00, duration);
}

void la_sharp(int duration) {
    Sound_Play(466.16, duration);
}

void do_prime(int duration) {
    Sound_Play(523.25, duration);
}

void re_prime(int duration) {
    Sound_Play(587.33, duration);
}

void re_sharp_prime(int duration) {
    Sound_Play(622.25, duration);
}

void fa_prime(int duration) {
    Sound_Play(698.46, duration);
}

void sol_prime(int duration) {
    Sound_Play(783.99, duration);
}

void la_prime(int duration) {
    Sound_Play(880.00, duration);
}


void Melody_Win() {
    int duration_short = 200;
    int duration_standard = 400;
    int duration_long = 600;

    do_prime(duration_standard);  // C
    la_prime(duration_short);      // A
    sol(duration_short);           // G
    la_prime(duration_standard);    // A
    do_prime(duration_long);       // C
    sol(duration_short);           // G
    la_prime(duration_short);      // A
    do_prime(duration_standard);    // C
    re_prime(duration_short);      // D
    re_prime(duration_standard);    // D
    la_prime(duration_short);       // A
    sol(duration_short);           // G
    fa_prime(duration_long);       // F
    sol(duration_short);           // G
    do_prime(duration_standard);    // C
    la_prime(duration_short);      // A
    sol(duration_short);           // G
    la_prime(duration_standard);    // A
    do_prime(duration_long);       // C
    sol(duration_short);           // G
    la_prime(duration_short);      // A
    do_prime(duration_standard);    // C
    re_prime(duration_short);      // D
    re_prime(duration_standard);    // D
    la_prime(duration_short);       // A
    sol(duration_short);           // G
    fa_prime(duration_long);       // F
    sol(duration_short);           // G
    do_prime(duration_long);       // C
}

//Sonido derrota

void ToneA() {
  Sound_Play(880, 50);
}

void ToneC() {
  Sound_Play(1046, 50);
}

void ToneE() {
  Sound_Play(1318, 50);
}

void Melody_Lose() {
  unsigned short i;
  for (i = 6; i > 0; i--) {
    ToneA();
    ToneC();
    ToneE();
  }
}







void main() {
 ANSEL  = 0;                        // Configure AN pins as digital
  ANSELH = 0;
  C1ON_bit = 0;                      // Disable comparators
  C2ON_bit = 0;


  TRISB  = 0xFF;

  TRISC  = 0x00;

  
  
  Sound_Init(&PORTC, 3);
  Sound_Play(880, 100);             // Play sound at 880Hz for 1 second

  while (1) {
    if (Button(&PORTB,7,1,1)) {       // RB7 plays Tone1
     videojuego();
          }
     if(Button(&PORTB,4,1,1)){
           Melody_Win();

    }

// Condición de derrota
    if(Button(&PORTB,5,1,1)){
      Melody_Lose();
    }




}

 //Condicion de victoria

  }


