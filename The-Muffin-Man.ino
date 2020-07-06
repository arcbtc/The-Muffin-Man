

//////////////LOAD LIBRARIES////////////////

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "qrcode.h"


//////////////VARIABLES TO CHANGE////////////////

//WIFI DETAILS
char wifiSSID[] = "YOUR_WIFI";
char wifiPASS[] = "YOUR_WIFI_PASS";

//LND NODE ADDRESS, UNCOMMENT AND USE IPAddress IF CONNECTING TO IP
//IPAddress server = {5,27,178,50}; 8180 //
const char*  server = "YOUR_NODES_ADDRESS"; 

//LND MACAROONS
String readmacaroon = "YOUR_READ_MAC";
String invoicemacaroon = "YOUR_INVOICE_MAC";

//VERY OCCASIONALLY WIFICLIENT NEEDS THE LND SSL, IF SO UNCOMMENT, SET TLSCert.h, UNCOMMENT CERT CALLS IN CALLBACKS
//#include "TLSCert.h"

String memo = "Muffin Man "; //INVOICE MEMO SUFFIX


//////////////OTHER VARIABLES////////////////

unsigned long then;
unsigned long now;
bool checker = false;
const int httpsPort = 443;
const int lndport = 443;
String pubkey;
String totcapacity;
const char* payment_request;
const char* payment_hash;
const char* alias;
const char* identity_pubkey;
int num_active_channels;
int num_peers;
int block_height;
bool synced_to_chain;
const char* synched; 
const char* lndversion; 
const char* chain;
const char* network;
String strinfopage;
int invoicesats;
String theqr;
#define LVGL_TICK_PERIOD 60
TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
lv_obj_t * slider_label;
lv_obj_t * QRlabel;
lv_obj_t * mbox1;
lv_obj_t * label;
lv_obj_t * label1;
lv_obj_t * label2;
lv_obj_t * label3;
lv_obj_t * label4;
lv_obj_t *tab1;
lv_obj_t *tab2;
lv_obj_t *tab3; 
int screenWidth = 480;
int screenHeight = 320;
#if USE_LV_LOG != 0



/////////////////LVGL FUNCTIONS/////////////////////


void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{
  Serial.printf("%s@%d->%s\r\n", file, line, dsc);
  delay(100);
}
#endif

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint16_t c;
  tft.startWrite(); /* Start new TFT transaction */
  tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
  for (int y = area->y1; y <= area->y2; y++) {
    for (int x = area->x1; x <= area->x2; x++) {
      c = color_p->full;
      tft.writeColor(c, 1);
      color_p++;
    }
  }
  tft.endWrite(); /* terminate TFT transaction */
  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

bool my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
{
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY, 600);
    if(!touched)
    {
      return false;
    }
    if(touchX>screenWidth || touchY > screenHeight)
    {
      Serial.println("Y or y outside of expected parameters..");
      Serial.print("y:");
      Serial.print(touchX);
      Serial.print(" x:");
      Serial.print(touchY);
    }
    else
    {
      data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL; 
      data->point.x = touchX;
      data->point.y = touchY;
      Serial.print("Data x");
      Serial.println(touchX); 
      Serial.print("Data y");
      Serial.println(touchY);

    }
    return false; 
}
