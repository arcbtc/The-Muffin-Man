


//////////////SETIP LOOP////////////////


void setup() {
  WiFi.begin(wifiSSID, wifiPASS);   
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  then = millis();
  ledcSetup(10, 5000, 10);
  ledcAttachPin(32, 10);
  analogReadResolution(10);
  ledcWrite(10,768);
  Serial.begin(115200); // FOR DEBUG
  lv_init();
  #if USE_LV_LOG != 0
    lv_log_register_print_cb(my_print); // REG LVGL PRINT FOR DEBUG
  #endif
  tft.begin(); // BEGIN TFT
  tft.setRotation(1);
  uint16_t calData[5] = { 293, 3612, 349, 3457, 3 };
  tft.setTouch(calData);
  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
  
  //LOAD LVGL
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  //LOAD DEVICE DRIVER
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);             /*Descriptor of a input device driver*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;    /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = my_touchpad_read;      /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);         /*Finally register the driver*/

  //SET LVGL THEME
  lv_theme_t * th = lv_theme_material_init(LV_THEME_DEFAULT_COLOR_SECONDARY, LV_THEME_DEFAULT_COLOR_SECONDARY, LV_THEME_MATERIAL_FLAG_DARK, LV_THEME_DEFAULT_FONT_SMALL , LV_THEME_DEFAULT_FONT_NORMAL, LV_THEME_DEFAULT_FONT_SUBTITLE, LV_THEME_DEFAULT_FONT_TITLE);     
  lv_theme_set_act(th);
  lv_obj_t * scr = lv_cont_create(NULL, NULL);
  lv_disp_load_scr(scr);

  //GO GET AND DISPLAY NODE STUFF
  get_display_info();

}
