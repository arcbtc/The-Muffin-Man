


//////////////GET INVOICE///////////////////////////////


void req_invoice(){

  WiFiClientSecure client;
  if (!client.connect(server, lndport)){
     return;   
  }
  String topost = "{\"value\": \""+ String(invoicesats) 
                   +"\", \"memo\": \""+ memo + String(random(1,1000)) 
                   +"\", \"expiry\": \"1000\","+
                   +"\"private\": true}";
                 
  client.print(String("POST ")+ "https://" + server +":"+ String(lndport) +"/v1/invoices HTTP/1.1\r\n" +
                 "Host: "  + server +":"+ String(lndport) +"\r\n" +
                 "User-Agent: ESP322\r\n" +
                 "Grpc-Metadata-macaroon:" + invoicemacaroon + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Connection: close\r\n" +
                 "Content-Length: " + topost.length() + "\r\n" +
                 "\r\n" + 
                 topost + "\n");

  String line = client.readStringUntil('\n');
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {   
      break;
    }
  }
  String content = client.readStringUntil('\n');
  client.stop();  
  
  const size_t capacity = JSON_OBJECT_SIZE(3) + 620;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, content);
    payment_hash = doc["r_hash"];
    payment_request = doc["payment_request"]; 
}



//////////////GET INFO / DISPLAY DATA///////////////////////////////


void get_display_info(){
  //MAKE TABS
  lv_obj_t *tabview;
  tabview = lv_tabview_create(lv_scr_act(), NULL);
  tab1 = lv_tabview_add_tab(tabview, "Home");
  tab2 = lv_tabview_add_tab(tabview, "NodeID");
  tab3 = lv_tabview_add_tab(tabview, "Invoice");
  label1 = lv_label_create(tab1, NULL);
  lv_label_set_text(label1, "Node info...");
  
  //CONNECT TO NODE AND GET INFO
  WiFiClientSecure client;
  if (!client.connect(server, lndport)){
    lv_label_set_text(label1, "Node info...");
    static const char * btns[] ={"Close", ""};
    mbox1 = lv_msgbox_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_bg_color(mbox1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_msgbox_set_text(mbox1, "ERROR: NODE NOT CONNECTED");
    lv_msgbox_add_btns(mbox1, btns);
    lv_obj_set_width(mbox1, 400);
    lv_obj_set_height(mbox1, 500);
    lv_obj_set_event_cb(mbox1, btn_event_handler);
    lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0); 
    return;   
  }
  client.print(String("GET ")+ "https://" + server +":"+ String(lndport) +"/v1/getinfo HTTP/1.1\r\n" +
                 "Host: "  + server +":"+ String(lndport) +"\r\n" +
                 "User-Agent: ESP322\r\n" +
                 "Grpc-Metadata-macaroon:" + readmacaroon + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Connection: close\r\n" +
                 "\n");
    String line = client.readStringUntil('\n');
    while (client.connected()) {
     String line = client.readStringUntil('\n');
     if (line == "\r") {    
       break;
     }
    }
    String content = client.readStringUntil('\n');
    client.stop();
    const size_t capacity = JSON_OBJECT_SIZE(3) + 620;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, content);
    alias = doc["alias"];
    identity_pubkey = doc["identity_pubkey"]; 
    num_active_channels = doc["num_active_channels"]; 
    num_peers = doc["num_peers"]; 
    block_height = doc["block_height"]; 
    synced_to_chain = doc["synced_to_chain"]; 
    if(synced_to_chain == true){
      synched = " synched-";
    }
    else{
      synched = "(not synched)";
    }
    lndversion = doc["version"]; 
    chain = doc["chains"][0]["chain"]; 
    network = doc["chains"][0]["network"]; 

    //TAB1
    strinfopage = "\n\n Alias: " + String(alias) + "\n\n " 
    + String(chain) + " " + String(network) + String(synched) + String(block_height) + "\n " 
    + "Peers: " + String(num_peers) + "\n " 
    + "Active Channels: " + String(num_active_channels) + "\n "
    + "Server: " + server + " Port: "+ lndport + "\n "
    + "Version: " + String(lndversion)+ "\n\n "
    + "ID: " + String(identity_pubkey).substring(0, 30) + "...";
    lv_label_set_text(label1, strinfopage.c_str());

    //TAB2
    showQR(tab2, identity_pubkey, 9, 4);

    //TAB3
    label3 = lv_label_create(tab3, NULL);
    lv_label_set_text(label3, "");
    lv_obj_t * slider = lv_slider_create(tab3, NULL);
    lv_obj_set_width(slider, screenWidth-50);                        /*Set the width*/
    lv_obj_set_height(slider, 20);
    lv_obj_align(slider, NULL, LV_ALIGN_CENTER, 0, -30);    /*Align to the center of the parent (screen)*/
    lv_obj_set_event_cb(slider, slider_event_cb);         /*Assign an event function*/
    slider_label = lv_label_create(tab3, NULL);
    lv_obj_set_auto_realign(slider, true);
    lv_obj_align(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, -30, 0);
    lv_label_set_text(slider_label, "0 sats");
    lv_obj_t * btn1 = lv_btn_create(tab3, NULL);
    lv_obj_set_event_cb(btn1, event_handler);
    lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, -0, 50);
    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Get Invoice");
}



//////////////SHOW QR CODE///////////////////////////////


void showQR(lv_obj_t *obj, const char* strtoqr, int qrsize, int scale){
    #define CANVAS_WIDTH  230
    #define CANVAS_HEIGHT  230
    
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(qrsize)];
    qrcode_initText(&qrcode, qrcodeData, qrsize, ECC_LOW, strtoqr);

    static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(CANVAS_WIDTH, CANVAS_HEIGHT)];
    lv_obj_t * canvas = lv_canvas_create(obj, NULL);
    lv_canvas_set_buffer(canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_INDEXED_1BIT);
    lv_obj_align(canvas, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_canvas_set_palette(canvas, 0, LV_COLOR_BLACK);
    lv_canvas_set_palette(canvas, 1, LV_COLOR_WHITE);
    lv_color_t c0;
    lv_color_t c1;
    c0.full = 0;
    c1.full = 1;
    lv_canvas_fill_bg(canvas, c1, LV_OPA_TRANSP);

    for (uint8_t y = 0; y < qrcode.size; y++) {
      for (uint8_t x = 0; x < qrcode.size; x++) {
        if(qrcode_getModule(&qrcode, x, y)){  
         for(int i = 0; i < scale; i++){
          for(int g = 0; g < scale; g++){
            lv_canvas_set_px(canvas, 7+x*scale+g, 7+y*scale+i, c0); //GHETTO AF, 7 IS THE QR PADDING
          }
         }
        }
        else{
         for(int i = 0; i < scale; i++){
          for(int g = 0; g < scale; g++){
           lv_canvas_set_px(canvas, 7+x*scale+g, 7+y*scale+i, c1);
          }
         }
        }
      }
     }
}



//////////////EVENTS FOR BUTTONS AND SLIDERS////////////////


void slider_event_cb(lv_obj_t * slider, lv_event_t event)
{
  if(event == LV_EVENT_VALUE_CHANGED) {
      invoicesats = lv_slider_get_value(slider) * 500;
      String sliderval = String(invoicesats);
      String inbuttonstr = sliderval + " sats";
      static char cbuff[32];
      inbuttonstr.toCharArray(cbuff, sizeof(cbuff));
      lv_label_set_text(slider_label, cbuff);   
  }
}

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
  if(event == LV_EVENT_CLICKED) {
      req_invoice();
      static const char * btns[] ={"Close", ""};
      mbox1 = lv_msgbox_create(lv_scr_act(), NULL);
      lv_msgbox_set_text(mbox1, "");
      showQR(mbox1, payment_request,14, 3);
      lv_msgbox_add_btns(mbox1, btns);
      lv_obj_set_event_cb(mbox1, btn_event_handler);
      lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0);   
  }
  else if(event == LV_EVENT_VALUE_CHANGED) {
  }
}

static void btn_event_handler(lv_obj_t * obj, lv_event_t event)
{
  if(event == LV_EVENT_VALUE_CHANGED) {
     printf("Button: %s\n", lv_msgbox_get_active_btn_text(obj));
     lv_msgbox_start_auto_close(mbox1, 0);
     get_display_info(); 
  }
}



//////////////HELPERS///////////////////////////////

      
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length();

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
} 
