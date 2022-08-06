#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>


#include <ClickEncoder.h>
#include <TimerOne.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Wire.h>

#define ELEMENTS(x) (sizeof(x) / sizeof(x[0]))


const float sensor1 = 11.40;
const float sensor2 = 8.04;


int menuParam = 1;

//#include <Adafruit_ImageReader.h>

float initial_read, distance_read, initial_write, distance_write;
int eadress = 0;
int tut, tut2, tut3, tut4, tut5, tut6, tut7, tut8;
int sira2 = 0;
int f, g = 0;
int d = 2;
int dark_ref = 1;
int dk;
volatile int lcd_refresh = 1;



///////SD PARAMETERS////////

int kare1 = 1;
int value5 = 0;
int last5 = 0;
File root;
const int chipSelect = 3;
int in_array_folders = 0;
int count_files = 0;
//char* initial_storage[500];
char* file_names[100];
char* print_names[100];
int sd_last = 1;
int sd_value = 90;
int encoder_count_sd = 0;
double encoder_kalan = 1;
double encoder_bolum = 0;
int encoder_file_select;
long changetime = 0;
int backup_tool[3];
int eks;
int mevcut_tool;
int print_count = 0;
char* file_names3[100];
int elektrik_flag;

int parse_val = 0;
int parse_ary[4];
int parse_minus = 2;
int parse_mod;

int kesinti_durdurma = 0;


///////SD PARAMETERS////////
// http://www.barth-dev.de/online/rgb565-color-picker/

#define BLACK     0x0000
#define BLUE      0x001F
#define RED       0xF800
#define GREEN     0x07E0
#define CYAN      0x07FF
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define WHITE     0xFFFF
#define TURUNCU   0xFBC0
#define MENEKSE   0xA00C
#define GRAY      0xBDF7
#define DARKGRAY  0x39E7

int cw = WHITE;
int cb = BLACK;
int csw = ST77XX_WHITE;
int csb = ST77XX_BLACK;
//int csg = ST7735_GREY;
int c7sw = ST7735_WHITE;
int c7sb = ST7735_BLACK;

///////SD PARAMETERS////////

//////MILLIS/////

unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long currentMillis;
const long interval_kalibre = 25;
const long interval1 = 200;
const long interval2 = 500;
const long interval3 = 1500;
int onetimer = 0;
int loadtimer = 0;
char coprint[10] = "Co Print";
int interval_param = 0;
int load_param = 0;
int loadexit = 1;
int loadpaintwhite = 1;

//////MILLIS/////

/*
  #define TFT_CS         10
  #define  TFT_RST        8
  #define TFT_DC         9

  #include <Fonts/FreeSerif18pt7b.h>

  #define TFT_MOSI 11  // Data out
  #define TFT_SCLK 13  // Clock out

  Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

*/


/*
  #define SPI_SCK 8  //Labeled CLK on display
  #define SPI_DI  4 //Labeled MISO on display
  #define SPI_DO  7  //Labeled MOSI on display

  #define SD_CS    5  // Chip select line for SD card
*/

//Use these pins for the display
#define TFT_CS   4
#define TFT_DC   2 //Labeled RS on display
#define TFT_RST  1  // you can also connect this to the Arduino reset

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


uint8_t dizi2[1] ;

//uint8_t dizi2
ClickEncoder *encoder;
int flag = 0;

void timerIsr() {
  encoder->service();
}

int endStop = 21 ;

int last, lastMenu = 1, value, pos1 = 90, encPos, lastEncPos;

//uint8_t M1_STEP=7 ,M1_DIR = 8,M1_EN = 9;
int kosul1 = 0;
int M1_STEP = 16;
int M1_DIR = 17 ;
int M1_EN = 18; // DEGİSTİ 21 Dİ
uint8_t motorDir, motorStep, motorEn, sabitDeger, ilk = 0, sira = 0, motorIlk = 0;
unsigned  long int motorUzunluk;
uint8_t eskiZaman = 0, yeniZaman;

uint8_t sd_sorgu = 0;

int CLK = 5;
int DT = 6;

int sorgu1 = 1;
int ilkE = 0;
int baslangic = 0;

int sensor_param1;
int sensor2_param1;

int rotation, RotPosition;
int value2;
boolean LeftRight;

uint8_t ext_deger = 1, motor_param = 5;
float sensor_param, sensor2_param;



int last_ext = 1, now_ext = 1;
int anlik_tool;


void motorKontrol(uint8_t numara, uint8_t yon, float uzunluk);

void kalibre();



extern uint8_t deneme[];
extern uint8_t deneme2[];
extern uint8_t deneme3[];
extern uint8_t deneme6[];



void setup() {


  Serial.begin(9600);
  EEPROM.get(1, sensor_param);
  EEPROM.get(6, sensor2_param);
  last = -1;
  //sensor_param = 17.20;
  //
  //
  //sensor2_param = 8.13;


  /*while (!Serial);

    //Serial.print("Initializing SD card...");

    if (!SD.begin(chipSelect)) {

    //Serial.println("initialization failed.");

    while (true);

    }*/
  SD.begin(chipSelect);

  root = SD.open("/");
  //printDirectory(root, 0);

  pinMode (endStop, INPUT);
  pinMode (A3, INPUT);
  //pinMode (DT,INPUT);
  rotation = digitalRead(CLK);




  encoder = new ClickEncoder(24, 25, 26);
  Timer1.initialize(500);
  Timer1.attachInterrupt(timerIsr);

  pinMode(M1_STEP, OUTPUT);
  pinMode(M1_DIR, OUTPUT);
  pinMode(M1_EN, OUTPUT);


  tft.initR(INITR_BLACKTAB);
  //tft.fillRect(0,0, 200, 200, ST7735_YELLOW);
  tft.fillRect(0, 0, 200, 200, cw);
  //tft.setFont(&FreeSerif18pt7b);
  tft.setRotation(3);



  //kalibre();



}

void loop() {
  count_files = 0;
  in_array_folders = 0;


  //  memset(file_names, 0, sizeof(file_names));
  Serial.print("menu param:");
  Serial.println(menuParam);
  //  Serial.print("dk: ");
  //  Serial.println(dk);


  if (sorgu1 == 1) {
    kalibre();
    sorgu1 = 2;
  }
  reading();

}


void reading() {

  ClickEncoder::Button encoderButton = encoder->getButton();

  switch (encoderButton) {

    case ClickEncoder::Clicked:

      if (menuParam == 1) {
        menuParam = menuParam * 10 + 1;
      }
      else if (menuParam == 2) {
        menuParam = menuParam * 100 + 10;
      }
      else if (menuParam == 3) {
        menuParam = (menuParam * 100) + 10 ;
      }
      else if (menuParam == 210) {
        menuParam = menuParam / 10;
      }
      else if (menuParam == 220) {
        menuParam = menuParam / 10;
      }
      else if (menuParam == 230) {
        menuParam = menuParam / 10;
        if (dk == 0) {
          tft.fillCircle(118, 87, 1, WHITE);
        }
        else {
          tft.fillCircle(118, 87, 1, BLACK);
        }

      }
      else if (menuParam == 240) {
        menuParam = menuParam / 10;
        printMenu2_4_1();
      }

      else if (menuParam == 310) {
        menuParam = menuParam / 10;
      }

      else if (menuParam == 11) {
        now_ext = ext_deger - last_ext;
        Serial.println(now_ext);

        if (now_ext > 0) {

          motorKontrol(1, 0, sensor2_param * now_ext);
        }
        else if (now_ext < 0) {

          motorKontrol(1, 1, -1 * sensor2_param * now_ext);
        }
        last_ext = ext_deger;
      }
      else if (menuParam == 21) {
        ////Serial.println(analogRead(A3));
        kosul1 == 0;
        ilkE = 0;
        kalibre();

      }
      else if (menuParam == 22) {
        kosul1 == 0;
        ilkE = 0;
        kalibre();
        motorKontrol(1, 0, sensor2_param * 6);

      }
      else if (menuParam == 23) {

        dk = d % 2;
        Serial.print("d: ");
        Serial.println(d);
        Serial.print("dk: ");
        Serial.println(dk);
        //dk==1 beyaz mod ;;;; dk==0 karanlık mod

        if (dk == 0) {
          //KARANLIK

          cw = BLACK;
          cb = WHITE;
          csw = ST77XX_BLACK;
          csb = ST77XX_WHITE;
          c7sw = ST7735_BLACK;
          c7sb = ST7735_WHITE;
          d++;
          printMenu2_1New(); // burası değişiecek on off gelince onu güncellemesi lazım o yüzden bu screeni değil diğer screeni yenileyeceksin ve aynı zamanda ekranda siyah olacak
          Serial.print("d: ");
          Serial.println(d);
          Serial.println();

          Serial.print("dk: ");
          Serial.println(dk);
          Serial.println();

        }
        else {
          //BEYAZ

          cw = WHITE;
          cb = BLACK;
          csw = ST77XX_WHITE;
          csb = ST77XX_BLACK;
          c7sw = ST7735_WHITE;
          c7sb = ST7735_BLACK;
          d++;
          printMenu2_1New();
          Serial.print("d: ");
          Serial.println(d);
          Serial.println();

          Serial.print("dk: ");
          Serial.println(dk);
          Serial.println();
        }

      }



      break;

    case ClickEncoder::DoubleClicked:
      if (menuParam == 11 ) {
        menuParam = (menuParam - 1) / 10;
      }

      else if (menuParam == 41) {
        menuParam = (menuParam * 10) - 100;
      }

      else if (menuParam == 21 || menuParam == 22 ) {
        menuParam = menuParam * 10;
      }

      else if (menuParam == 23 ) {
        menuParam = menuParam * 10;
        if (dk == 0) {
          tft.fillCircle(118, 87, 1, WHITE);
        }
        else {
          tft.fillCircle(118, 87, 1, BLACK);
        }
      }

      else if (menuParam == 24 ) {
        menuParam = menuParam * 10;
        if (dk == 1) {
          printMenu2_1Beyaz();
          Serial.println("beyazdayım");
        }
        else if (dk == 0) {
          printMenu2_1Siyah();
          Serial.println("siyahtayım");
        }
      }

      else if (menuParam == 210) {
        menuParam = (menuParam - 10) / 100;
      }
      else if (menuParam == 220) {
        menuParam = (menuParam - 20) / 100;
      }
      else if (menuParam == 230) {
        menuParam = (menuParam - 30) / 100;
      }
      else if (menuParam == 240) {
        menuParam = (menuParam - 40) / 100;
      }
      else if (menuParam == 310) {
        menuParam = (menuParam - 10) / 100;
        baslangic = 0;
        Serial.println("menuParam: ");
        Serial.print(menuParam);
        Serial.println("Baslangıc: ");
        Serial.print(baslangic);

      }



      //Serial.println("qqq");

      break ;

    case ClickEncoder::Held:



      if (menuParam == 21) {
        menuParam += 1;

      }
      else if (menuParam == 24) {

        for (int one_timer = 0; one_timer <= 30; one_timer++) {
          EEPROM.put(1, sensor1);
          EEPROM.put(6, sensor2);
          EEPROM.get(1, sensor_param);
          EEPROM.get(6, sensor2_param);

          //tft.fillCircle(57, 70, 5, cb);
          tft.drawRoundRect(65, 65, 32, 12, 4, cb);
          tft.fillRoundRect(66, 66, one_timer, 10, 3, cb);
          delay(10);



          if (one_timer == 30) {
            kosul1 == 0;
            ilkE = 0;
            kalibre();

            if (dk == 1) {
              printMenu2_1Beyaz();
              Serial.println("beyazdayım");
            }
            else if (dk == 0) {
              printMenu2_1Siyah();
              Serial.println("siyahtayım");
            }

            one_timer++;
            menuParam = 240;
          }
        }

      }

      break ;

  }

  value += encoder->getValue();
  if (value != last ) {

    if ((value < last && menuParam == 3) || (value > last && menuParam == 2)) {
      menuParam = 1;
    }

    else if ((value < last && menuParam == 1)) {
      if (g == 1) {
        menuParam = 2;
      }
      else {
        g++;
      }
    }
    else if ((value > last && menuParam == 1)) {
      menuParam = 3;
    }
    else if ((value > last && menuParam == 210 )) {
      menuParam += 10;
    }
    else if ((value > last && menuParam == 220 )) {
      menuParam += 10;
    }
    else if ((value < last && menuParam == 220)) {
      menuParam -= 10;
    }
    else if ((value > last && menuParam == 230 )) {
      menuParam += 10;
    }

    else if ((value < last && menuParam == 230)) {
      menuParam -= 10;
    }
    else if ((value < last && menuParam == 240)) {
      menuParam -= 10;
    }
    else if (value > last && menuParam == 11 && ext_deger < 7) {

      ext_deger = ext_deger + 1;
      printMenu1_1Rakam();

    }
    else if (value < last && menuParam == 11 && ext_deger > 1) {

      ext_deger = ext_deger - 1;
      printMenu1_1Rakam();

    }
    else if (value > last && menuParam == 21 && sensor_param < 100) {
      sensor_param = sensor_param + 0.1;
      EEPROM.put(1, sensor_param);
      printMenu2_1();
    }
    else if (value < last && menuParam == 21 && sensor_param > 0) {
      sensor_param = sensor_param - 0.1
                     ;
      EEPROM.put(1, sensor_param);
      printMenu2_1();

    }

    else if (value > last && menuParam == 22 && sensor2_param < 100) {
      sensor2_param = sensor2_param + 0.01;
      EEPROM.put(6, sensor2_param);
      printMenu2_2();

    }
    else if (value < last && menuParam == 22 && sensor2_param > 0) {
      sensor2_param = sensor2_param - 0.01;
      EEPROM.put(6, sensor2_param);
      printMenu2_2();

    }

  }

  refreshDisplay();
  ////Serial.println(menuParam);
  last = value;
}

void elektrik_kesintisi() {
  tft.fillRoundRect(20, 35, 120, 55, 3, BLACK);
  EEPROM.get(10, changetime);
  EEPROM.get(5, elektrik_flag);
  tft.setCursor(30, 40);
  tft.setTextColor(csw, csb);
  tft.setTextSize(1);
  tft.println("Lost Electricity");
  tft.setCursor(25, 60);
  tft.print("Last Change Count: ");
  tft.setCursor(35, 75 );
  tft.println(changetime);
  Serial.print("elektrik changetime: "), Serial.println(changetime);
  Serial.print("elektrik flag: "), Serial.println(elektrik_flag);

}

void refreshDisplay() {

  if (f == 0) { //ilk açılışta menüyü getirmek için.
    EEPROM.get(10, changetime);
    EEPROM.get(5, elektrik_flag);
    Serial.print("elektrik flag: "), Serial.println(elektrik_flag);

    if (elektrik_flag == 1) {
      if (kesinti_durdurma == 0) {
        printMenu1New();
        elektrik_kesintisi();
        kesinti_durdurma++;
      }
      while (1) {
        ClickEncoder::Button encoderButton = encoder->getButton();
        if (encoderButton == ClickEncoder::Clicked) {
          Serial.println("tıkladım");
          printMenu1New();
          printMenu1();
          f++;
          elektrik_flag = 0;
          changetime = 0;
          EEPROM.put(5, elektrik_flag);
          EEPROM.put(10, changetime);
          break;
        }
      }
    }
    else {
      printMenu1New();
      printMenu1();
      f++;
      changetime = 0;
      EEPROM.put(10, changetime);
    }

  }

  if (menuParam == 0 && lastMenu != 1)
    menuParam = menuParam;

  else if (menuParam == 1 && lastMenu != 2) {

    if (lcd_refresh == 1 || lcd_refresh == 2 || lcd_refresh == 3 ) { //Ana menüdeyken ekranın yenilenmemesi için.
      printMenu1();
    }
    else {
      printMenu1New();
      printMenu1();
    }
    lcd_refresh = 1;
  }
  else if (menuParam == 2 && lastMenu != 3) {

    if (lcd_refresh == 1 || lcd_refresh == 2 || lcd_refresh == 3 ) { //Ana menüdeyken ekranın yenilenmemesi için.
      printMenu2();
    }
    else {
      printMenu1New();
      printMenu2();
    }
    lcd_refresh = 2;
  }

  else if (menuParam == 3 && lastMenu != 4) {

    if (lcd_refresh == 1 || lcd_refresh == 2 || lcd_refresh == 3 ) { //Ana menüdeyken ekranın yenilenmemesi için.
      printMenu3();
    }
    else {
      printMenu1New();
      printMenu3();
    }
    lcd_refresh = 3;
  }
  else if (menuParam == 4 && lastMenu != 5)
    printMenu4();
  else if (menuParam == 11 && lastMenu != 6) {
    printMenu1_1();
    lcd_refresh = 11;
  }
  else if (menuParam == 21 && lastMenu != 7) {
    //printMenu2_1New();
    if (lcd_refresh == 210 || lcd_refresh == 220 || lcd_refresh == 230 || lcd_refresh == 240 || lcd_refresh == 21 || lcd_refresh == 22 || lcd_refresh == 23 || lcd_refresh == 24) { //2. menüdeyken ekranın yenilenmemesi için.

      printMenu2_1();
    }
    else {
      printMenu2_1New();
      printMenu2_1();
    }
    lcd_refresh = 21;
    //Serial.println("21");
  }
  else if (menuParam == 22 && lastMenu != 8) {
    //printMenu2_1New();
    if (lcd_refresh == 210 || lcd_refresh == 220 || lcd_refresh == 230 || lcd_refresh == 240 || lcd_refresh == 21 || lcd_refresh == 22 || lcd_refresh == 23 || lcd_refresh == 24) { //2. menüdeyken ekranın yenilenmemesi için.

      printMenu2_2();
    }
    else {
      printMenu2_1New();
      printMenu2_2();
    }
    lcd_refresh = 22;
    //Serial.println("22");
  }

  else if (menuParam == 23 && lastMenu != 62) {
    //printMenu2_1New();
    if (lcd_refresh == 210 || lcd_refresh == 220 || lcd_refresh == 230 || lcd_refresh == 240 || lcd_refresh == 21 || lcd_refresh == 22 || lcd_refresh == 23 || lcd_refresh == 24) { //2. menüdeyken ekranın yenilenmemesi için.
      Serial.println("if-refresh23teyim");
      printMenu2_3();
    }
    else {
      printMenu2_1New();
      printMenu2_3();
      Serial.println("else-refresh23teyim");
    }
    lcd_refresh = 23;
    //Serial.println("22");
  }

  else if (menuParam == 24 && lastMenu != 54) {
    //printMenu2_1New();
    if (lcd_refresh == 210 || lcd_refresh == 220 || lcd_refresh == 230 || lcd_refresh == 240 || lcd_refresh == 21 || lcd_refresh == 22 || lcd_refresh == 23 || lcd_refresh == 24) { //2. menüdeyken ekranın yenilenmemesi için.

      //printMenu2_4_1();
      //printMenu2_1();
      printMenu2_2();
    }
    else {
      printMenu2_1New();
    }
    lcd_refresh = 24;
    //Serial.println("22");
  }

  else if (menuParam == 210 && lastMenu != 71) {
    //printMenu2_1New();

    if (lcd_refresh == 210 || lcd_refresh == 220 || lcd_refresh == 230 || lcd_refresh == 240 || lcd_refresh == 21 || lcd_refresh == 22 || lcd_refresh == 23 || lcd_refresh == 24) { //2. menüdeyken ekranın yenilenmemesi için.

      printMenu2_1_0();
    }
    else {
      printMenu2_1New();
      printMenu2_1_0();
    }
    lcd_refresh = 210;

    //Serial.println("210");
  }
  else if (menuParam == 220 && lastMenu != 81) {
    //printMenu2_1New();

    if (lcd_refresh == 210 || lcd_refresh == 220 || lcd_refresh == 230 || lcd_refresh == 240 || lcd_refresh == 21 || lcd_refresh == 22 || lcd_refresh == 23 || lcd_refresh == 24) { //2. menüdeyken ekranın yenilenmemesi için.

      printMenu2_2_0();
    }
    else {
      printMenu2_1New();
      printMenu2_2_0();
    }
    lcd_refresh = 220;
    //Serial.println("220");
  }

  else if (menuParam == 230 && lastMenu != 97 ) {

    if (lcd_refresh == 210 || lcd_refresh == 220 || lcd_refresh == 230 || lcd_refresh == 240 || lcd_refresh == 21 || lcd_refresh == 22 || lcd_refresh == 23 || lcd_refresh == 24) { //2. menüdeyken ekranın yenilenmemesi için.
      printMenu2_3_0();
    }

    else {
      printMenu2_1New();
      printMenu2_3_0();

    }
    lcd_refresh = 230;

  }

  else if (menuParam == 240 && lastMenu != 79 ) {

    if (lcd_refresh == 210 || lcd_refresh == 220 || lcd_refresh == 230 || lcd_refresh == 240 || lcd_refresh == 21 || lcd_refresh == 22 || lcd_refresh == 23 || lcd_refresh == 24) { //2. menüdeyken ekranın yenilenmemesi için.

      printMenu2_4();
    }

    else {

      printMenu2_1New();

    }
    lcd_refresh = 240;

  }


  else if (menuParam == 310 && lastMenu != 34) {
    //sd_settings();
    printMenu3_1();
    lcd_refresh = 31;
  }
  else if (menuParam == 41 && lastMenu != 11)
    printMenu4_1();
  //printMenu4_1_1(file_names2);
}


void printMenu1New() {
  //tft.fillRect(0,0, 200, 200, ST7735_WHITE);
  tft.fillScreen(cw);
  drawBitmap(0, 0, deneme, 80, 60, cb);

  tft.setCursor(10, 52);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println("Change Tool");
  /*tft.setCursor(105,100);
    tft.setTextColor(ST77XX_BLACK,ST77XX_WHITE);
    tft.setTextSize(1);
    tft.println("Print");*/
  drawBitmap(10, 70, deneme2, 60, 45, cb);

  //tft.drawRect(2,2,80,60,BLACK);

  tft.setCursor(17, 115);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println("Settings");

  tft.fillTriangle(100, 70, 100, 39, 111, 64, YELLOW);
  tft.fillTriangle(100, 39, 111, 33, 111, 64, YELLOW);


  tft.fillTriangle(100, 39, 100, 26, 119, 28, GREEN);
  tft.fillTriangle(119, 28, 100, 26, 108, 22, GREEN);


  tft.fillTriangle(108, 22, 119, 15, 119, 28, BLUE);
  tft.fillTriangle(119, 28, 119, 15, 138, 26, BLUE);
  tft.fillTriangle(138, 26, 119, 28, 127, 33, BLUE);
  tft.fillTriangle(127, 33, 138, 26, 138, 35, BLUE);
  tft.fillTriangle(138, 35, 127, 33, 127, 42, BLUE);


  tft.fillTriangle(127, 42, 138, 35, 138, 48, RED);
  tft.fillTriangle(127, 42, 138, 48, 119, 46, RED);
  tft.fillTriangle(119, 46, 119, 59, 138, 48, RED);
  tft.fillTriangle(119, 59, 119, 46, 111, 54, RED);
  //tft.fillTriangle(111, 44, 119, 49, 111, 54, RED);
  tft.fillTriangle(111, 54, 111, 42, 119, 46, RED);


  tft.setCursor(90, 90);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println("Multicolor");
  tft.setCursor(105, 100);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println("Print");


  lastMenu = 2;

}

void printMenu1() {

  tft.drawRoundRect(2, 2, 80, 65, 3, cb);
  tft.drawRoundRect(2, 65, 80, 60, 3, cw);
  tft.drawRoundRect(82, 2, 76, 123, 3, cw);


  lastMenu = 2;

}

void printMenu2() {


  tft.drawRoundRect(2, 2, 80, 65, 3, cw);
  tft.drawRoundRect(2, 65, 80, 60, 3, cb);
  tft.drawRoundRect(82, 2, 76, 123, 3, cw);
  lastMenu = 3;
}
void printMenu3() {
  tft.drawRoundRect(2, 2, 80, 65, 3, cw);
  tft.drawRoundRect(2, 65, 80, 60, 3, cw);
  tft.drawRoundRect(82, 2, 76, 123, 3, cb);
  lastMenu = 4;
}
void printMenu4() {

  lastMenu = 5;
}

void printMenu1_1() {

  tft.fillRect(0, 0, 200, 200, csw);

  drawBitmap(8, 30, deneme, 80, 60, cb);


  tft.drawRoundRect(105, 45, 28, 28, 3, cb);

  tft.setCursor(114, 52);
  tft.setTextColor(csb, csw);
  tft.setTextSize(2);
  tft.println(ext_deger);


  tft.setCursor(15, 85);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println("Change Tool");

  /*
    tft.setCursor(84,78);
    tft.setTextColor(ST77XX_BLACK,ST77XX_WHITE);
    tft.setTextSize(1);
    tft.println( "Tool");

  */
  lastMenu = 6;
}

void printMenu1_1Rakam() {
  tft.setCursor(114, 52);
  tft.setTextColor(csb, csw);
  tft.setTextSize(2);
  tft.println( ext_deger);
}

//---------------SİYAH------------------//
void printMenu2_1Siyah() {
  Serial.println("2-1-siyah");

  //anlik_tool=last_ext+now_ext;
  tft.fillRect(0, 0, 200, 200, csw);

  //drawBitmap(8, 8, deneme6, 60, 45, cb);

  tft.setCursor(50, 10);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Settings");
  //tft.fillRect(95, 20, 60, 2, csb);

  tft.setCursor(5, 35);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Initial Calibration");

  tft.setCursor(5, 55);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Distance Between");

  tft.setCursor(5, 65);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Tool");

  tft.setCursor(5, 85);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Dark Mode");

  printMenu2_3_2();

  if (dk == 0) {
    printMenu2_3_2();
    Serial.println("siyahın içinde ifteyim");
  }
  else {
    Serial.println("siyahın içinde elseteyim");
    printMenu2_3_1();
  }


  tft.setCursor(5, 105);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Factory Data Reset");

  //printText(sensor_param, ST77XX_WHITE,80,40,2);
  tft.setCursor(125, 35);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println(sensor_param);

  tft.setCursor(125, 60);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println(sensor2_param);

  //28/07/22/13/55
}
//---------------SİYAH------------------//

//---------------BEYAZ------------------//
void printMenu2_1Beyaz() {
  //anlik_tool=last_ext+now_ext;
  tft.fillRect(0, 0, 200, 200, csw);
  Serial.println("2-1-beyaz");
  //drawBitmap(8, 8, deneme6, 60, 45, cb);

  tft.setCursor(55, 10);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Settings");
  //tft.fillRect(95, 20, 60, 2, csb);

  tft.setCursor(5, 35);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Initial Calibration");

  tft.setCursor(5, 55);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Distance Between");

  tft.setCursor(5, 65);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Tool");

  tft.setCursor(5, 85);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Dark Mode");

  printMenu2_3_1();

  if (dk == 0) {
    printMenu2_3_2();
  }
  else {
    printMenu2_3_1();
  }


  tft.setCursor(5, 105);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Factory Data Reset");

  //printText(sensor_param, ST77XX_WHITE,80,40,2);
  tft.setCursor(125, 35);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println(sensor_param);

  tft.setCursor(125, 60);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println(sensor2_param);

  //28/07/22/13/55
}

//---------------BEYAZ------------------//

//---------------ORIJINAL---------------//
void printMenu2_1New() {
  //anlik_tool=last_ext+now_ext;
  tft.fillRect(0, 0, 200, 200, csw);
  Serial.println("2-1-new");
  //drawBitmap(8, 8, deneme6, 60, 45, cb);

  tft.setCursor(55, 10);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Settings");
  //tft.fillRect(95, 20, 60, 2, csb);

  tft.setCursor(5, 35);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Initial Calibration");

  tft.setCursor(5, 55);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Distance Between");

  tft.setCursor(5, 65);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Tool");

  tft.setCursor(5, 85);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Dark Mode");

  if (dark_ref == 1) {
    printMenu2_3_1();
    //tft.fillCircle(118, 78, 1, WHITE);
    dark_ref++;
  }
  else {
    if (dk == 0) {
      printMenu2_3_2();
    }
    else {
      printMenu2_3_1();
    }
  }

  tft.setCursor(5, 105);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Factory Data Reset");

  //printText(sensor_param, ST77XX_WHITE,80,40,2);
  tft.setCursor(125, 35);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println(sensor_param);

  tft.setCursor(125, 60);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println(sensor2_param);

  //28/07/22/13/55
}
//---------------ORIJINAL---------------//

void printMenu2_1() {
  // printMenu2_1New();
  Serial.println("2-1");
  tft.drawRoundRect(122, 33, 37, 12, 3, cb);
  tft.drawRoundRect(2, 33, 118, 12, 3, cw);

  //tft.fillRect(125, 80, 31, 8, WHITE);
  tft.setCursor(125, 35);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println(sensor_param);

  /*tft.setCursor(125, 100);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(1);
    tft.println(sensor2_param);*/


  lastMenu = 7;
}


void printMenu2_1_0() {
  //printMenu2_1New();
  Serial.println("2-1-0");
  tft.drawRoundRect(2, 33, 118, 12, 3, cb);
  tft.drawRoundRect(2, 53, 118, 24, 3, cw);
  tft.drawRoundRect(122, 33, 37, 12, 3, cw);
  tft.drawRoundRect(122, 58, 37, 12, 3, cw);

  lastMenu = 71;
}

void printMenu2_2() {
  //printMenu2_1New();
  Serial.println("2-2");
  tft.drawRoundRect(122, 58, 37, 12, 3, cb);
  tft.drawRoundRect(2, 53, 118, 24, 3, cw);

  tft.setCursor(125, 60);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println(sensor2_param);

  lastMenu = 8;
}
void printMenu2_2_0() {
  //printMenu2_1New();
  Serial.println("2-2-0");
  tft.drawRoundRect(2, 53, 118, 24, 3, cb);
  tft.drawRoundRect(2, 33, 118, 12, 3, cw);
  tft.drawRoundRect(122, 33, 37, 12, 3, cw);
  tft.drawRoundRect(122, 58, 37, 12, 3, cw);
  tft.drawRoundRect(2, 83, 75, 12, 3, cw);


  lastMenu = 81;
}

void printMenu2_3() {
  Serial.println("2-3");
  tft.drawRoundRect(2, 33, 118, 12, 3, cw);
  tft.drawRoundRect(2, 53, 118, 24, 3, cw);
  tft.drawRoundRect(122, 33, 37, 12, 3, cw);
  tft.drawRoundRect(122, 58, 37, 12, 3, cw);
  tft.fillCircle(118, 87, 1, cb);
  tft.drawRoundRect(2, 103, 120, 12, 3, cw);
  lastMenu = 62;

}

void printMenu2_3_0() {
  //printMenu2_1New();
  Serial.println("2-3-0");
  tft.drawRoundRect(2, 53, 118, 24, 3, cw);
  tft.drawRoundRect(2, 33, 118, 12, 3, cw);
  tft.drawRoundRect(122, 33, 37, 12, 3, cw);
  tft.drawRoundRect(122, 58, 37, 12, 3, cw);
  tft.drawRoundRect(2, 83, 60, 12, 3, cb);
  tft.drawRoundRect(2, 103, 120, 12, 3, cw);


  lastMenu = 97;
}

void printMenu2_3_1() {
  //beyaz
  Serial.println("2-3-1-beyaz");
  tft.fillRect(120, 79, 35, 15, WHITE);
  tft.fillRoundRect(123, 83, 32, 11, 3, WHITE);
  tft.drawRoundRect(122, 82, 33, 12, 3, BLACK);
  tft.fillCircle(127, 87, 5, BLACK);
  //tft.fillCircle(118, 78, 1, BLACK);

}
void printMenu2_3_2() {
  //karanlık
  Serial.println("2-3-2-siyah");
  tft.fillRect(120, 79, 35, 15, BLACK);
  tft.fillRoundRect(123, 83, 32, 11, 3, BLUE);
  tft.drawRoundRect(122, 82, 33, 12, 3, WHITE);
  tft.fillCircle(149, 87, 5, cb);
  //tft.fillCircle(118, 78, 1, WHITE);

}

void printMenu2_4() {
  Serial.println("2-4");
  tft.drawRoundRect(2, 53, 118, 24, 3, cw);
  tft.drawRoundRect(2, 33, 118, 12, 3, cw);
  tft.drawRoundRect(122, 33, 37, 12, 3, cw);
  tft.drawRoundRect(122, 58, 37, 12, 3, cw);
  tft.drawRoundRect(2, 83, 60, 12, 3, cw);
  tft.drawRoundRect(2, 103, 120, 12, 3, cb);

  lastMenu = 79;
}

void printMenu2_4_1() {

  tft.fillRect(0, 0, 200, 200, csw);
  tft.setCursor(10, 5);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println("Calibration values");
  tft.setCursor(10, 15);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println("will be reset");
  tft.drawRoundRect(42, 43, 82, 12, 3, cb);
  tft.setCursor(45, 45);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println("Are you sure?");
  tft.fillCircle(7, 93, 1, cb);
  tft.setCursor(10, 90);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println("Hold for resetting");
  tft.fillCircle(7, 113, 1, cb);
  tft.setCursor(10, 110);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println("Double click for quiting");

  lastMenu = 54;
}

void printMenu2_4_2() {

  tft.setCursor(125, 45);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println(sensor1);

  tft.setCursor(125, 65);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println(sensor2);

  // lastMenu = 41;
}


void printMenu4_1() {


  //printMenuPrinting();
  //Colors();
  ColorChangeCount();

  tft.drawRoundRect(20, 15, 28, 28, 3, cb);
  tft.setCursor(30, 23);
  tft.setTextColor(csb, csw);
  tft.setTextSize(2);
  tft.println(ext_deger);
  EEPROM.put(10, changetime);

  Serial.print("--4-1 içinde parse-val: "), Serial.println(parse_val);
  Serial.print("--4-1 içinde parse-mod: "), Serial.println(parse_mod);


  if (changetime == 0 ) {
    tft.fillRect(9, 22, 5, 5, cw);
    tft.setCursor(50, 25);
    tft.setTextColor(csb, csw);
    tft.setTextSize(1);
    tft.println(parse_ary[1] + 1);
    Serial.print("--changetime>1 ary(0): "), Serial.println(parse_ary[0]);
    Serial.print("--changetime>1 ary(0)+1: "), Serial.println(parse_ary[0] + 1);
    Serial.print("--changetime>1 ary(1): "), Serial.println(parse_ary[1]);
    Serial.print("--changetime>1 ary(1)+1: "), Serial.println(parse_ary[1] + 1);
    Serial.print("--changetime>1 ary(2): "), Serial.println(parse_ary[2]);
    Serial.print("--changetime>1 ary(2)+1: "), Serial.println(parse_ary[2] + 1);

  }
  else if (changetime >= 1) {

    if (parse_mod == 0) {
      tft.setCursor(10, 23);
      tft.setTextColor(csb, csw);
      tft.setTextSize(1);
      tft.println(parse_ary[0] + 1);
      Serial.print("changetime>1 ary(0): "), Serial.println(parse_ary[0]);
      Serial.print("--changetime>1 ary(0)+1: "), Serial.println(parse_ary[0] + 1);
      Serial.print("changetime>1 ary(1): "), Serial.println(parse_ary[1]);
      Serial.print("changetime>1 ary(1)+1: "), Serial.println(parse_ary[1] + 1);
      Serial.print("changetime>1 ary(2): "), Serial.println(parse_ary[2]);
      Serial.print("changetime>1 ary(2)+1: "), Serial.println(parse_ary[2] + 1);

      tft.setCursor(50, 25);
      tft.setTextColor(csb, csw);
      tft.setTextSize(1);
      tft.println(parse_ary[0]);
      Serial.print("--changetime>1 ary(0): "), Serial.println(parse_ary[0]);
      Serial.print("changetime>1 ary(0)+1: "), Serial.println(parse_ary[0] + 1);
      Serial.print("changetime>1 ary(1): "), Serial.println(parse_ary[1]);
      Serial.print("changetime>1 ary(1)+1: "), Serial.println(parse_ary[1] + 1);
      Serial.print("changetime>1 ary(2): "), Serial.println(parse_ary[2]);
      Serial.print("changetime>1 ary(2)+1: "), Serial.println(parse_ary[2] + 1);
    }
    else if (parse_mod == 1) {
      tft.setCursor(10, 23);
      tft.setTextColor(csb, csw);
      tft.setTextSize(1);
      tft.println(parse_ary[2]);
      Serial.print("changetime>1 ary(0): "), Serial.println(parse_ary[0]);
      Serial.print("changetime>1 ary(0)+1: "), Serial.println(parse_ary[0] + 1);
      Serial.print("changetime>1 ary(1): "), Serial.println(parse_ary[1]);
      Serial.print("changetime>1 ary(1)+1: "), Serial.println(parse_ary[1] + 1);
      Serial.print("--changetime>1 ary(2): "), Serial.println(parse_ary[2]);
      Serial.print("changetime>1 ary(2)+1: "), Serial.println(parse_ary[2] + 1);

      tft.setCursor(50, 25);
      tft.setTextColor(csb, csw);
      tft.setTextSize(1);
      tft.println(parse_ary[1]);
      Serial.print("changetime>1 ary(0): "), Serial.println(parse_ary[0]);
      Serial.print("changetime>1 ary(0)+1: "), Serial.println(parse_ary[0] + 1);
      Serial.print("--changetime>1 ary(1): "), Serial.println(parse_ary[1]);
      Serial.print("changetime>1 ary(1)+1: "), Serial.println(parse_ary[1] + 1);
      Serial.print("changetime>1 ary(2): "), Serial.println(parse_ary[2]);
      Serial.print("changetime>1 ary(2)+1: "), Serial.println(parse_ary[2] + 1);
    }
    else if (parse_mod == 2) {
      tft.setCursor(10, 23);
      tft.setTextColor(csb, csw);
      tft.setTextSize(1);
      tft.println(parse_ary[0]);
      Serial.print("--changetime>1 ary(0): "), Serial.println(parse_ary[0]);
      Serial.print("changetime>1 ary(0)+1: "), Serial.println(parse_ary[0] + 1);
      Serial.print("changetime>1 ary(1): "), Serial.println(parse_ary[1]);
      Serial.print("changetime>1 ary(1)+1: "), Serial.println(parse_ary[1] + 1);
      Serial.print("changetime>1 ary(2): "), Serial.println(parse_ary[2]);
      Serial.print("changetime>1 ary(2)+1: "), Serial.println(parse_ary[2] + 1);

      tft.setCursor(50, 25);
      tft.setTextColor(csb, csw);
      tft.setTextSize(1);
      tft.println(parse_ary[2]);
      Serial.print("changetime>1 ary(0): "), Serial.println(parse_ary[0]);
      Serial.print("changetime>1 ary(0)+1: "), Serial.println(parse_ary[0] + 1);
      Serial.print("changetime>1 ary(1): "), Serial.println(parse_ary[1]);
      Serial.print("changetime>1 ary(1)+1: "), Serial.println(parse_ary[1] + 1);
      Serial.print("--changetime>1 ary(2): "), Serial.println(parse_ary[2]);
      Serial.print("changetime>1 ary(2)+1: "), Serial.println(parse_ary[2] + 1);
    }
  }

  drawBitmap(100, 50, deneme3, 20, 20, cb);

  tft.setCursor(23, 48);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Tool");
  //printMenu4_1_1(file_names3[1]);
  lastMenu = 11;
  //calisma();

}

void printMenu4_1_1(char* file_names3) {
  tft.setCursor(80, 10);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Printing...");
  tft.drawLine(75, 23, 150, 23, cb);
  tft.setCursor(75, 30);
  tft.println(file_names3);
  lastMenu = 23;
}

void motorKontrol( uint8_t numara, uint8_t yon, float uzunluk) {

  if (numara == 1 ) {
    motorDir = M1_DIR;
    motorStep = M1_STEP;
    motorEn = M1_EN;
    motorUzunluk = 30000UL * uzunluk / 302.42;
  }




  digitalWrite(motorEn, LOW);
  if (yon == 1) {
    digitalWrite(motorDir, HIGH);
  }
  else if (yon == 0) {
    digitalWrite(motorDir, LOW);
  }

  for (unsigned long int x = 0; x < motorUzunluk; x++) {
    digitalWrite(motorStep, HIGH);
    delayMicroseconds(380);
    digitalWrite(motorStep, LOW);
    delayMicroseconds(380);

  }
  //    Serial.print("nowex-mot: ");
  //    Serial.println(now_ext);
  //    Serial.print("last_ext-mot: ");
  //    Serial.println(last_ext);

  anlik_tool = now_ext + last_ext ;

  //  Serial.print("anlik_tool: ");
  //  Serial.println(anlik_tool);

  int kalan_extruder = changetime % 2;

  //    Serial.print("kalan extruder-mot: ");
  //    Serial.println(kalan_extruder);
  memset(backup_tool, 0, sizeof(backup_tool));

  if (changetime <= 1) {

    //    Serial.print("changetime: ");
    //    Serial.println(changetime);

    if (kalan_extruder == 0) {
      backup_tool[0] = ext_deger;

      //backup_tool[0] = ext_deger;

      //      Serial.print("kalan extruder: ");
      //      Serial.println(kalan_extruder);
      //      Serial.print("backuptool 0. eleman: ");
      //      Serial.println(backup_tool[0]);
      //      Serial.print("backuptool 1. eleman: ");
      //      Serial.println(backup_tool[1]);

    }
    else if (kalan_extruder == 1) {
      backup_tool[1] = anlik_tool;
      backup_tool[0] = ext_deger;
      //backup_tool[1] = eks;
      //      Serial.print("kalan extruder: ");
      //      Serial.println(kalan_extruder);
      //      Serial.print("backuptool 0. eleman: ");
      //      Serial.println(backup_tool[0]);
      //      Serial.print("backuptool 1. eleman: ");
      //      Serial.println(backup_tool[1]);

    }
  }

  else if (changetime > 1) {


    //    Serial.print("changetime 2 den büyük: ");
    //    Serial.println(changetime);

    if (kalan_extruder == 0) {

      memset(backup_tool, 0, sizeof(backup_tool));
      backup_tool[0] = last_ext;
      //  backup_tool[1] = eks;

      //            Serial.print("kalan extruder: ");
      //            Serial.println(kalan_extruder);
      //            Serial.print("backuptool 0. eleman: ");
      //            Serial.println(backup_tool[0]);
      //            Serial.print("backuptool 1. eleman: ");
      //            Serial.println(backup_tool[1]);

    }
    else if (kalan_extruder == 1) {

      memset(backup_tool, 0, sizeof(backup_tool));
      backup_tool[0] = last_ext;
      //  backup_tool[1] = eks;


      //            Serial.print("kalan extruder: ");
      //            Serial.println(kalan_extruder);
      //            Serial.print("backuptool 0. eleman: ");
      //            Serial.println(backup_tool[0]);
      //            Serial.print("backuptool 1. eleman: ");
      //            Serial.println(backup_tool[1]);

    }
  }



}


void previous_tool() {




}

void next_tool() {




}



void kesme(int cek, int it) {

  now_ext = it - last_ext;

  if (now_ext > 0)
    motorKontrol(1, 0, sensor2_param * now_ext);
  else if (now_ext < 0)
    motorKontrol(1, 1, -1 * sensor2_param * now_ext);
  last_ext = it;
  ext_deger = it;

}


void kalibre() {

  //Serial.println("kalibre devam ediyor");
  kosul1 = 0;
  ilkE = 0;
  digitalWrite(18, LOW);


  while (1) {

    currentMillis = millis();
    LoadScreen2();

    if (loadtimer == 1) {

      loadtimer = 2;

    }

    //Serial.println("dönüyom mu");
    int giris = 1;


    digitalWrite(17, HIGH);
    digitalWrite(16, HIGH);
    delayMicroseconds(25);
    digitalWrite(16, LOW);
    delayMicroseconds(25);


    if (analogRead(A3) < 100 and ilkE == 0) {
      //Serial.println("dönüyom mu 4");
      kosul1++;
      ilkE = 1;
    }
    if (analogRead(A3) > 800) {
      //Serial.println("dönüyom mu 5");
      ilkE = 0;
    }

    ////Serial.println(kosul1);

    if (kosul1 >= 2 && loadtimer == 2) {
      //Serial.println("box box");
      break;
    }

  }

  kosul1 = 0;
  ilkE = 0;
  motorKontrol(1, 0, sensor_param);
  //Serial.println("kalibre bitti");
}

void degistir(uint8_t extruder) {
  now_ext = extruder - last_ext;

  //    Serial.print("nowex: ");
  //    Serial.println(now_ext);
  //    Serial.print("extruder: ");
  //    Serial.println(extruder);
  //    Serial.print("last_ext: ");
  //    Serial.println(last_ext);

  if (now_ext > 0)
    motorKontrol(1, 0, sensor2_param * now_ext);
  else if (now_ext < 0)
    motorKontrol(1, 1, -1 * sensor2_param * now_ext);
  last_ext = extruder;
  ext_deger = extruder;

  // return now_ext
  // return extruder;
  // return last_ext
}


void donmeKontrol() {
  value2 = digitalRead(CLK);
  if (value != rotation) { // we use the DT pin to find out which way we turning.
    if (digitalRead(DT) != value) {  // Clockwise
      RotPosition ++;
      LeftRight = true;
    } else { //Counterclockwise
      LeftRight = false;
      RotPosition--;
    }
    if (LeftRight) { // turning right will turn on red led.

    } else {       // turning left will turn on green led.

    }
    // this will print in the serial monitor.

  }
  rotation = value2;
}

void drawBitmap(int16_t x, int16_t y,
                const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      if (i & 7) byte <<= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if (byte & 0x80) tft.drawPixel(x + i, y + j, color);
    }
  }
}







void printMenu3_1() {

  tft.fillRect(0, 0, 200, 200, csw);

  sd_sorgu = 0;

  sd_selecting_screen();

  //sd_1();

  if (sd_sorgu == 2) {
    tft.setCursor(0, 20);
    tft.setTextColor(csb);
    tft.setTextSize(1);
    tft.println("could not be read");
  }



  lastMenu = 10;
}



int sd_1(char* file_names2) {

  if (!SD.begin(3)) {

  }
  int backup_flag = 0;
  int atlama_flag = 0;
  int j = 0;
  root = SD.open(file_names2);
  memset(parse_ary, 0, sizeof(parse_ary));
  Serial.println("sd_1 de memsetten sonra parse_ary(0)");
  Serial.print("parse_ary[0]: "),  Serial.println(parse_ary[0]);
  parse_val = 0;
  Serial.println();

  if (root) {
    encoder = new ClickEncoder(24, 25, 26);

    while (root.available()) {

      eks = root.parseInt();



      if (parse_val % 3 == 0) {
        parse_ary[0] = eks;
        Serial.println("mod: 0");
        Serial.print("eks: "),   Serial.println(eks);
        Serial.print("parse_ary[0]: "),  Serial.println(parse_ary[0]);
        Serial.print("parse_val: "),   Serial.println(parse_val);
        Serial.println();
      }
      else if (parse_val % 3 == 1) {
        parse_ary[1] = eks;
        Serial.println("mod: 1");
        Serial.print("eks: "),   Serial.println(eks);
        Serial.print("parse_ary[1]: "),  Serial.println(parse_ary[1]);
        Serial.print("parse_val: "),     Serial.println(parse_val);
        Serial.println();
      }
      else if (parse_val % 3 == 2) {
        parse_ary[2] = eks;
        Serial.println("mod: 2");
        Serial.print("eks: "), Serial.println(eks);
        Serial.print("parse_ary[2]: "),  Serial.println(parse_ary[2]);
        Serial.print("parse_val: "),   Serial.println(parse_val);
        Serial.println();
      }


      parse_val++;
      Serial.print("Parse_val arttktan sonra: "),  Serial.println(parse_val);

      if (flag == 1) {

        break;
      }

      while (baslangic == 1 && parse_val >= 3) {


        ClickEncoder::Button encoderButton = encoder->getButton();
        switch (encoderButton) {

          case ClickEncoder::Clicked:

            //BURAYA SETTINGS GELECEK, BURAYA DEĞİL GALİBA

            break ;

          case ClickEncoder::DoubleClicked:
            if (flag == 0) {

              flag = 1;
              menuParam = 1;
              //refreshDisplay();
              changetime = 0;
              parse_val = 0;
              Serial.print("parse_val: "), Serial.println(parse_val);
              parse_minus = 2;
              Serial.print("parse_minus: "), Serial.println(parse_minus);
              elektrik_flag = 0;
              EEPROM.put(5, elektrik_flag);
            }
            break ;


          case ClickEncoder::Held:
            if (backup_flag == 0) {
              if (mevcut_tool < 8) {
                mevcut_tool++;
                Serial.print("mevcut artıyor: ");
                Serial.println(mevcut_tool);
                kesme(0, mevcut_tool);
                printMenu4_1();
                delay(500);
                if (mevcut_tool > 7) {
                  mevcut_tool = 1;
                  kesme(0, mevcut_tool);
                  printMenu4_1();
                  Serial.print("mevcut 0 landı: "), Serial.println(mevcut_tool);
                }
              }
            }
            break;
        }

        if (flag == 1) {
          break;
        }

        if (digitalRead(endStop) == 0 && ilk == 0 && atlama_flag == 0) {
          Serial.println("while içinde endstop && ilk && atamadayım");

          changetime++;
          EEPROM.put(10, changetime);

          if (sira2 > 5) {
            kalibre();
            last_ext = 1;
            sira2 = 0;
          }
          /*if (parse_val > 4) {
            Serial.print("parse_minus: ");
            Serial.println(parse_minus);
            parse_minus++;
            Serial.print("parse_minus: ");
            Serial.println(parse_minus);

            }*/
          if (parse_val % 3 == 2) {
            parse_mod = parse_val % 3;
            Serial.print("--WHİLE içinde parse_mod: "),  Serial.println(parse_mod);
            Serial.print("if=2 ve (0): "), Serial.println(parse_ary[0]);
            kesme(0, (parse_ary[0]) + 1 );

          }
          else {
            parse_mod = parse_val % 3;
            Serial.print("--WHİLE içinde parse_mod: "),  Serial.println(parse_mod);
            Serial.print("else ve %3+1: "), Serial.println(parse_val % 3 + 1);
            kesme(0, (parse_ary[(parse_val % 3) + 1]) + 1 );
          }


          elektrik_flag = 1;
          EEPROM.put(5, elektrik_flag);

          Serial.println();
          Serial.print("parse_val: "), Serial.println(parse_val);
          Serial.print("(val%3)+1): "), Serial.println((parse_val % 3) + 1);
          Serial.print("kesme (parse_ary (val%3)+1) + 1: ");
          Serial.println((parse_ary[(parse_val % 3) + 1]) + 1);
          printMenu4_1();
          printMenu4_1_1(file_names2);
          ilk = 1;
          sira += 1;
          sira2 += 1;
          break;
        }

        else if (digitalRead(endStop) == 1) {

          ilk = 0;
        }
      }

      if (baslangic == 0) {

        tft.fillScreen(cw);
        printMenu4_1();
        printMenu4_1_1(file_names2);
        degistir((parse_ary[0]) + 1);
        Serial.println();
        Serial.println("baslangıc0"), Serial.print("parse_ary0 + 1: "),
                       Serial.println(parse_ary[0] + 1),     Serial.println();
        printMenu4_1();
        printMenu4_1_1(file_names2);
        baslangic = 1 ;

        elektrik_flag = 1;
        EEPROM.put(5, elektrik_flag);
      }
    }
    //myfile.available
    root.close();
    sd_sorgu = 1;
  }
  else {
    // if the file didn't open, print an error:
    //489
    sd_sorgu = 2;
  }
}

void sd_settings() {
  tft.fillRect(0, 0, 200, 200, cw);
  tft.setCursor(30, 50);
  tft.setTextColor(csb, csw);
  tft.setTextSize(1);
  tft.println("SETTINGS");
}

double count_encoder_sd() {

  double kalan = 0;
  double bolum = 0;

  kalan = count_files % 4;
  bolum = (count_files - kalan) / 4;

  //  //Serial.println("KALAN: ");
  //  //Serial.println(kalan);
  //  //Serial.println("BÖLÜM: ");
  //  //Serial.println(bolum);
  //
  // value += encoder->getValue();
  // return value;
  return kalan;
  return bolum;
  ////Serial.println(value);
}


int sd_selecting_screen() {
  int i = 0;
  flag = 0;
  baslangic = 0;
  int cursor_scroll = 15;

  while (1) {
    if (!SD.begin(chipSelect)) {


      count_files = 0;
      in_array_folders = 0;

      //Serial.println("Insert SD!");

      tft.fillRect(0, 0, 200, 200, csw);
      tft.setTextColor(csb, csw);
      tft.setCursor(9, 58);
      tft.setTextSize(1);
      tft.println("Please Insert an SD Card" );
      //      tft.setCursor(58, 68);
      //      tft.setTextSize(1);
      //      tft.println("SD Card" );

      ClickEncoder::Button encoderButton = encoder->getButton();
      if (encoderButton == ClickEncoder::DoubleClicked) {
        Serial.print("çift tık");
        menuParam = 3;
        lastMenu = 99;
        break;
      }

    }
    else {
      //array_cleaning ();
      //count_files=0;
      tft.fillRect(0, 0, 200, 200, csw);
      tft.setTextColor(csb, csw);
      tft.setTextSize(1);
      //printDirectory(root, 0);
      root = SD.open("/");
      int count_files = printDirectory(root, 0);
      drawBitmap(100, 30, deneme6, 48, 48, cb);

      tft.setCursor(123, 80);
      tft.println("/");
      tft.setCursor(130, 80);
      tft.println(count_files);
      break;
    }


  }

  while (SD.begin(chipSelect)) {


    value5 += encoder->getValue();

    if (value5 != last5) {


      if ((value5 > last5) && kare1 > 3) {
        if ((i + 4) != count_files) {
          tft.fillRect(8, 9, 75, 17, cw);
          tft.fillRect(8, 39, 75, 17, cw);
          tft.fillRect(8, 69, 75, 17, cw);
          tft.fillRect(8, 99, 75, 17, cw);
          i++;

        }

      }

      else if ((value5 > last5 && kare1 < 4)) {

        if (count_files <= 4) {

          if (kare1 != count_files) {
            kare1 += 1;

          }
        }
        else {
          kare1 += 1;

        }

      }

      else if ((value5 < last5 && kare1 > 1)) {
        kare1 -= 1;

      }

      else if ((value5 < last5 && kare1 == 1  && i > 0)) {
        tft.fillRect(8, 9, 75, 17, cw);
        tft.fillRect(8, 39, 75, 17, cw);
        tft.fillRect(8, 69, 75, 17, cw);
        tft.fillRect(8, 99, 75, 17, cw);

        i -= 1;

      }

    }

    if (kare1 == 1) {
      sd_cursor_refresh_first();
    }

    else if (kare1 == 2) {
      sd_cursor_refresh_second();
    }

    else if (kare1 == 3) {
      sd_cursor_refresh_third();
    }
    else if (kare1 == 4) {
      sd_cursor_refresh_fourth();

    }

    //Serial.println("dizi içerisi");
    //Serial.println(file_names[i]);
    //Serial.println(file_names[i + 1]);
    //Serial.println(file_names[i + 2]);
    //Serial.println(file_names[i + 3]);

    if (count_files <= 1) {
      tft.setCursor(10, 15);
      tft.println(file_names[i]);

      tft.fillRect(10, 45, 30, 150, WHITE);
    }
    else if (count_files <= 2) {
      tft.setCursor(10, 15);
      tft.println(file_names[i]);

      tft.setCursor(10, 45);
      tft.println(file_names[i + 1]);

      tft.fillRect(10, 65, 30, 150, WHITE);
    }
    else if (count_files <= 3) {
      tft.setCursor(10, 15);
      tft.println(file_names[i]);

      tft.setCursor(10, 45);
      tft.println(file_names[i + 1]);

      tft.setCursor(10, 75);
      tft.println(file_names[i + 2]);

      tft.fillRect(10, 900 , 30, 150, WHITE);
    }
    else if (count_files >= 4) {
      tft.setCursor(10, 15);
      tft.println(file_names[i]);

      tft.setCursor(10, 45);
      tft.println(file_names[i + 1]);

      tft.setCursor(10, 75);
      tft.println(file_names[i + 2]);

      tft.setCursor(10, 105);
      tft.println(file_names[i + 3]);

    }

    tft.fillRect(109, 78, 12, 10, cw);
    tft.setCursor(110, 80);
    tft.println(kare1 + i);

    ClickEncoder::Button encoderButton = encoder->getButton();

    if (encoderButton == ClickEncoder::Clicked) {
      //Serial.println("kırdım");
      //Serial.println(file_names[kare1+i-1]);

      sd_1(file_names[kare1 + i - 1]);


      break;
    }
    else if (encoderButton == ClickEncoder::DoubleClicked) {
      //      memset(file_names, 0, sizeof(file_names));
      //      //Serial.print(file_names[0]);

      menuParam = 1;
      lastMenu = 99;
      break;
    }
    /*
      switch (encoderButton) {

        case ClickEncoder::Clicked:

        break;
        break;
        case ClickEncoder::DoubleClicked:

        break;

      }
    */
    last5 = value5;
    //Serial.print("i: ");
    //Serial.print(i);
    //Serial.print(", kare1: ");
    //Serial.print(kare1);
    //Serial.print(", count_files: ");
    //Serial.println(count_files);
  }

  count_encoder_sd();

  //   uint8_t encoder_count = 0;
  //   encoder_count +=encoder->getValue();



  // if encoder clicked
  //printMenu3_1();

  lastMenu = 34;
}


int printDirectory(File dir, int numTabs) {


  if (!SD.begin(chipSelect)) {
    while (true);
  }

  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      //Serial.println("no other directory");
      break;
    }

    for (uint16_t i = 0; i < numTabs; i++) {

    }

    if ( isFnTxt(entry.name())) {                                // Here is the magic
      int i = 0;



      char* initial_storage = entry.name();      //put entry.name to the char array
      String (toString) = String(initial_storage);        //convert file_names to the String
      //toString = toString.substring(0, String.length()-4);      //we do not want to take ".txt" part so we delate last 4 bits
      toString.toCharArray(initial_storage, toString.length() + 1); //we convert it back to char to store it in array(idiot)

      file_names[in_array_folders] = strdup(initial_storage);
      ////Serial.println(file_names[in_array_folders]);
      //Serial.println(file_names[in_array_folders]);
      in_array_folders++;
      count_files++;

      //Serial.println(count_files);



    }
    entry.close();
  }
  return count_files;


}


bool isFnTxt(char* filename) {
  int len = strlen(filename);
  bool result;
  if (strstr(strlwr(filename + (len - 4)), ".txt")) {
    result = true;
  }
  else {
    result = false;
  }
  return result;
}

void sd_cursor_refresh_first() {

  tft.drawRoundRect(8, 9, 80, 20, 3, cb);
  tft.drawRoundRect(8, 39, 80, 20, 3, cw);
  tft.drawRoundRect(8, 69, 80, 20, 3, cw);
  tft.drawRoundRect(8, 99, 80, 20, 3, cw);
}
void sd_cursor_refresh_second() {
  tft.drawRoundRect(8, 9, 80, 20, 3, cw);
  tft.drawRoundRect(8, 39, 80, 20, 3, cb);
  tft.drawRoundRect(8, 69, 80, 20, 3, cw);
  tft.drawRoundRect(8, 99, 80, 20, 3, cw);
}
void sd_cursor_refresh_third() {
  tft.drawRoundRect(8, 9, 80, 20, 3, cw);
  tft.drawRoundRect(8, 39, 80, 20, 3, cw);
  tft.drawRoundRect(8, 69, 80, 20, 3, cb);
  tft.drawRoundRect(8, 99, 80, 20, 3, cw);
}
void sd_cursor_refresh_fourth() {
  tft.drawRoundRect(8, 9, 80, 20, 3, cw);
  tft.drawRoundRect(8, 39, 80, 20, 3, cw);
  tft.drawRoundRect(8, 69, 80, 20, 3, cw);
  tft.drawRoundRect(8, 99, 80, 20, 3, cb);
}


//void array_cleaning () {
//
//  for(int i=0;i<5;i++)
//        file_names[i] = 0;
//  }



void printMenuPrinting() {

  tft.fillTriangle(100, 70, 100, 39, 111, 64, YELLOW);
  tft.fillTriangle(100, 39, 111, 33, 111, 64, YELLOW);


  tft.fillTriangle(100, 39, 100, 26, 119, 28, GREEN);
  tft.fillTriangle(119, 28, 100, 26, 108, 22, GREEN);


  tft.fillTriangle(108, 22, 119, 15, 119, 28, BLUE);
  tft.fillTriangle(119, 28, 119, 15, 138, 26, BLUE);
  tft.fillTriangle(138, 26, 119, 28, 127, 33, BLUE);
  tft.fillTriangle(127, 33, 138, 26, 138, 35, BLUE);
  tft.fillTriangle(138, 35, 127, 33, 127, 42, BLUE);


  tft.fillTriangle(127, 42, 138, 35, 138, 48, RED);
  tft.fillTriangle(127, 42, 138, 48, 119, 46, RED);
  tft.fillTriangle(119, 46, 119, 59, 138, 48, RED);
  tft.fillTriangle(119, 59, 119, 46, 111, 54, RED);
  tft.fillTriangle(111, 54, 111, 42, 119, 46, RED);


}

void printMenuPrinting2() {

  tft.fillTriangle(10, 70, 10, 39, 21, 64, YELLOW);
  tft.fillTriangle(10, 39, 21, 33, 21, 64, YELLOW);


  tft.fillTriangle(10, 39, 10, 26, 29, 28, GREEN);
  tft.fillTriangle(29, 28, 10, 26, 18, 22, GREEN);


  tft.fillTriangle(18, 22, 29, 15, 29, 28, BLUE);
  tft.fillTriangle(29, 28, 29, 15, 48, 26, BLUE);
  tft.fillTriangle(48, 26, 29, 28, 37, 33, BLUE);
  tft.fillTriangle(37, 33, 48, 26, 48, 35, BLUE);
  tft.fillTriangle(48, 35, 37, 33, 37, 42, BLUE);


  tft.fillTriangle(37, 42, 48, 35, 48, 48, RED);
  tft.fillTriangle(37, 42, 48, 48, 29, 46, RED);
  tft.fillTriangle(29, 46, 29, 59, 48, 48, RED);
  tft.fillTriangle(29, 59, 29, 46, 21, 54, RED);
  tft.fillTriangle(21, 54, 21, 42, 29, 46, RED);


}

int LoadScreen1() {
  //Serial.println("loaddayım");

  if (loadpaintwhite == 1) {
    tft.fillScreen(cw);
    loadpaintwhite++;
    //Serial.println("1 kere beyaza boyadım");
  }

  if (onetimer <= 119) {
    //Serial.println("onetimerın içine girdim: ");
    //Serial.print(onetimer);
    if (currentMillis - previousMillis >= interval1 && interval_param == 0) {
      //Serial.println("load1");
      tft.fillTriangle(100, 70, 100, 39, 111, 64, YELLOW);
      tft.fillTriangle(100, 39, 111, 33, 111, 64, YELLOW);
      previousMillis = currentMillis;
      interval_param = 1;
    }

    else if (currentMillis - previousMillis >= interval2 && interval_param == 1) {
      //Serial.println("load2");
      tft.fillTriangle(100, 39, 100, 26, 119, 28, GREEN);
      tft.fillTriangle(119, 28, 100, 26, 108, 22, GREEN);
      previousMillis = currentMillis;
      interval_param = 2;
    }
    else if (currentMillis - previousMillis >= interval2 && interval_param == 2) {
      //Serial.println("load3");
      tft.fillTriangle(108, 22, 119, 15, 119, 28, BLUE);
      tft.fillTriangle(119, 28, 119, 15, 138, 26, BLUE);
      tft.fillTriangle(138, 26, 119, 28, 127, 33, BLUE);
      tft.fillTriangle(127, 33, 138, 26, 138, 35, BLUE);
      tft.fillTriangle(138, 35, 127, 33, 127, 42, BLUE);
      previousMillis = currentMillis;
      interval_param = 3;
    }
    else if (currentMillis - previousMillis >= interval2 && interval_param == 3) {
      //Serial.println("load4");
      tft.fillTriangle(127, 42, 138, 35, 138, 48, RED);
      tft.fillTriangle(127, 42, 138, 48, 119, 46, RED);
      tft.fillTriangle(119, 46, 119, 59, 138, 48, RED);
      tft.fillTriangle(119, 59, 119, 46, 111, 54, RED);
      tft.fillTriangle(111, 54, 111, 42, 119, 46, RED);
      previousMillis = currentMillis;
      interval_param = 4;
    }

    int first_cursor = 50;

    if (currentMillis - previousMillis >= interval2 && interval_param == 4) {
      //Serial.println("forun ifine girdim");
      for (int bn = 0; bn <= 8; bn++) {
        //Serial.println("fora giirdim");
        tft.setTextColor(csb, csw);
        tft.setTextSize(2);
        tft.setCursor(first_cursor, 80);
        tft.println(coprint[bn]);
        first_cursor = first_cursor + 12;
        previousMillis = currentMillis;
        //Serial.println(coprint[bn]);
      }
      interval_param = 5;
    }
    onetimer++;
  }
  return onetimer;
}






int LoadScreen2() {
  if (loadpaintwhite == 1) {
    tft.fillScreen(cw);
    loadpaintwhite++;
  }

  int first_cursor = 55;


  if (loadtimer < 1) {

    if (onetimer < 1) {


      tft.setTextColor(csb, csw);
      tft.setTextSize(2);

      for (int cn = 0; cn <= 8; cn++) {

        tft.setCursor(first_cursor, 20);
        tft.println(coprint[cn]);
        first_cursor = first_cursor + 12;
        previousMillis = currentMillis;

      }
      printMenuPrinting2();
      tft.setTextSize(1);
      tft.fillRect(55, 40, 95, 2, BLACK);
      tft.setCursor(72, 50);
      tft.println("Color Your 3D");
      tft.setCursor(72, 60);
      tft.println("Print Quickly");
      onetimer++;
      tft.drawRoundRect(9, 99, 142, 12, 4, BLACK);

    }
    for (int i = 10; i <= 140; i++) {
      tft.fillRoundRect(10, 100, i, 10, 3, DARKGRAY);
      delay(10);
    }



    loadtimer++;
    Serial.println(loadtimer);
  }

  return loadtimer;


}



void Colors() {
  // tft.drawRect(9, 0, 160, 12, BLACK);
  tft.fillRect(138, 0, 23, 10, TURUNCU);
  tft.fillRect(115, 0, 23, 10, MENEKSE);
  tft.fillRect(92, 0, 23, 10, CYAN);
  tft.fillRect(69, 0, 23, 10, RED);
  tft.fillRect(46, 0, 23, 10, BLUE);
  tft.fillRect(23, 0, 23, 10, GREEN);
  tft.fillRect(0, 0, 23, 10, YELLOW);

}

void ColorChangeCount() {

  tft.setCursor(45, 110);
  tft.setTextColor(csb);
  tft.setTextSize(1);
  tft.println("Change Count");

  if (changetime < 10) {



    tft.setCursor(74, 85);
    tft.setTextColor(csb, csw);
    tft.setTextSize(2);
    tft.println(changetime);
    tft.drawRoundRect(70, 78, 18, 28, 3, cb);

  }


  else if (changetime >= 10 && changetime < 100) {
    tft.fillRect(35, 78, 100, 28, cw);



    tft.setCursor(68, 85);
    tft.setTextColor(csb, csw);
    tft.setTextSize(2);
    tft.println(changetime);
    tft.drawRoundRect(65, 78, 29, 28, 3, cb);

  }

  else if (changetime >= 100 && changetime < 1000) {
    tft.fillRect(35, 78, 100, 28, cw);
    tft.fillRect(10, 90, 25, 10, cw);
    tft.fillRect(75, 90, 25, 10, cw);

    /* tft.setCursor(101, 90);
      tft.setTextColor(GRAY, csw);
      tft.setTextSize(1);
      tft.println(changetime + 1);

      tft.setCursor(37, 90);
      tft.setTextColor(GRAY, csw);
      tft.setTextSize(1);
      tft.println(changetime - 1);*/

    tft.setCursor(61, 85);
    tft.setTextColor(csb, csw);
    tft.setTextSize(2);
    tft.println(changetime);
    tft.drawRoundRect(59, 78, 39, 28, 3, cb);
  }

  else if (changetime >= 1000 && changetime < 10000) {
    tft.fillRect(25, 78, 100, 28, cw);
    tft.fillRect(10, 90, 25, 10, cw);
    tft.fillRect(75, 90, 25, 10, cw);

    /* tft.setCursor(110, 90);
      tft.setTextColor(GRAY, csw);
      tft.setTextSize(1);
      tft.println(changetime + 1);

      tft.setCursor(25, 90);
      tft.setTextColor(GRAY, csw);
      tft.setTextSize(1);
      tft.println(changetime - 1);*/

    tft.setCursor(56, 85);
    tft.setTextColor(csb, csw);
    tft.setTextSize(2);
    tft.println(changetime);
    tft.drawRoundRect(54, 78, 51, 28, 3, cb);
  }

  else if (changetime >= 10000 && changetime < 100000) {
    tft.fillRect(35, 78, 100, 28, cw);
    tft.fillRect(25, 78, 100, 28, cw);
    tft.fillRect(10, 90, 30, 10, cw);
    tft.fillRect(75, 90, 30, 10, cw);

    /* tft.setCursor(114, 90);
      tft.setTextColor(GRAY, csw);
      tft.setTextSize(1);
      tft.println(changetime + 1);

      tft.setCursor(16, 90);
      tft.setTextColor(GRAY, csw);
      tft.setTextSize(1);
      tft.println(changetime - 1);*/

    tft.setCursor(50, 85);
    tft.setTextColor(csb, csw);
    tft.setTextSize(2);
    tft.println(changetime);
    tft.drawRoundRect(48, 78, 63, 28, 3, cb);
  }

  else if (changetime >= 100000 && changetime < 1000000) {
    tft.fillRect(35, 78, 100, 28, cw);
    tft.fillRect(25, 78, 100, 28, cw);
    tft.fillRect(10, 90, 25, 10, cw);
    tft.fillRect(75, 90, 25, 10, cw);

    /* tft.setCursor(120, 90);
      tft.setTextColor(GRAY, csw);
      tft.setTextSize(1);
      tft.println(changetime + 1);

      tft.setCursor(3, 90);
      tft.setTextColor(GRAY, csw);
      tft.setTextSize(1);
      tft.println(changetime - 1);*/

    tft.setCursor(44, 85);
    tft.setTextColor(csb, csw);
    tft.setTextSize(2);
    tft.println(changetime);
    tft.drawRoundRect(42, 78, 76, 28, 3, cb);
  }

}
