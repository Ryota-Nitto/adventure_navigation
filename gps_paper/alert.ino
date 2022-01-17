/** stateに応じてブザーを鳴らす **/
void alert() {
  /** stateに応じたブザーのオンオフ **/
  if (state_new == 0){ //OUT OF AREA
    analogWrite(BUZZER, 0);
  }
  else if (state_new == 1) { //OUT
    if (soundMode == 1) {
      analogWrite(BUZZER, 128);
    }
  }
  else if (state_new == 2) { //IN
    analogWrite(BUZZER, 0);
  }
  else if (state_new == 3) { //STRANGE
    analogWrite(BUZZER, 0);
  }
}
