


//////////////MAIN LOOP////////////////


void loop() {
  now = millis();
  // THE LVGL GUI MAKER
  lv_task_handler(); 
  delay(5);
  // FETCH NODE DETAILS EVERY 60SECS
  if (now - then > 60000){ 
    get_display_info();
    then = millis();
    Serial.println("1 min passed");
  }
}
