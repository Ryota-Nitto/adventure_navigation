/** stateに応じてブザーを鳴らす **/
void alert() {
  /** stateに応じたブザーのオンオフ **/
  if (state_new == 0){ //OUT OF AREA
    analogWrite(WIO_BUZZER, 0);
  }
  else if (state_new == 1) { //OUT
    if (soundMode_new == 1) {
      analogWrite(WIO_BUZZER, 128);
    }
  }
  else if (state_new == 2) { //IN
    analogWrite(WIO_BUZZER, 0);
  }
  else if (state_new == 3) { //STRANGE
    analogWrite(WIO_BUZZER, 0);
  }
}
