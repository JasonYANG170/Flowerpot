/*
 Name:		智能节水花盆.ino
 Created:	2023/3/15 0:22:26
 Author:	qw200
*/

#include <GFX4d.h>
#include <UnoWiFiDevEd.h>
#include <Adafruit_SSD1306.h>  //驱动128x64 and 128x32 的屏幕显示的库
#include <Adafruit_GFX.h>      //Arduino的图形库
#include <Wire.h>
#include <SPI.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

int M0 = 1024;  //在空气中AO读取的值最大为1024，代表干燥时的读数
int M1 = 0;   //浸泡在水里的最小值 464（最小值会改变），代表100%湿度
int PIN_AO = 14;  //与wifi不冲突的pin
int in1 = 0;
int in2 = 2;
int LED = 15;
#include <DHT.h>
#define DHTPIN 13//刚才插得2号引脚
#define DHTTYPE DHT11//定义类型
DHT dht(DHTPIN, DHTTYPE);//进行初始化设置

int buttonPin = 12;  // 将开关连接到
int buttonState = 0;
void setup() {
    Serial.begin(9600);
    dht.begin();//DHT开始工作
    pinMode(PIN_AO, INPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(LED, OUTPUT);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(LED, LOW);
    pinMode(buttonPin, INPUT_PULLUP);  // 设
    //Serial.println("OLED FeatherWing test");  //串口输出信息

    display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  //初始化OLED并设置其IIC地址为 0x3C
    display.clearDisplay();                     //清屏
    delay(200);

    display.setTextSize(1.3);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);  //设置光标的位置

    //绘制像素点
    //  display.drawPixel(64, 32, WHITE);//(x坐标，y坐标，颜色)

    //绘制水平线
    //  display.drawFastHLine(64,32,64,SSD1306_WHITE);//(x起点，y起点，长度，颜色)
    //绘制竖直线
    // display.drawFastVLine(64,10,50,SSD1306_WHITE);

    //绘制线
    // display.drawLine(0,0,128,64,SSD1306_WHITE);//（x起，y起，x终，y终）

    //绘制空心矩形
    // display.drawRect(0,0,128,64,SSD1306_WHITE);//(左上角x坐标，左上角y坐标，矩形长，矩形宽)

    //绘制实心矩形
    //display.fillRect(20,10,88,44,SSD1306_WHITE);//同上

    //点亮全屏
    //  display.fillScreen(SSD1306_WHITE);

    //绘制空心圆
    //display.drawCircle(64,32,32,SSD1306_WHITE);//(圆心x坐标，圆心y坐标，半径)

    //绘制实心圆
    //display.fillCircle(64,32,31,SSD1306_WHITE);

    //绘制空心三角形
    //display.drawTriangle(63,0, 0,63, 127,63, SSD1306_WHITE);//(第一顶点坐标[x,y]，第二，第三)

    //绘制实心三角形
    // display.fillTriangle(63,0, 0,63, 127,63, SSD1306_WHITE);

    //绘制空心圆角矩形
    //display.drawRoundRect(10,5,107, 43,8, SSD1306_WHITE);//(左上角x，左上角y，矩形长，矩形宽，矩形圆角半径)

    //实心圆角
    //display.fillRoundRect(10,5,107,53,3, SSD1306_WHITE);

    //绘制Bitmap图像
    //Bimap图像数据




    display.display();
    display.println("");
    display.println("");

    display.println("     Wellcome To\n         YANG\nIntelligent Flowerpot");
    display.display();
    delay(8000);
    //  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  //初始化OLED并设置其IIC地址为 0x3C
}

void loop() {
    // delay(2000);

    buttonState = digitalRead(buttonPin);

    Serial.println(analogRead(14));//湿度传感器接ESP32上的GPIO号
    delay(100);
    if (buttonState == LOW) {  // 判断是否按下开关
        display.clearDisplay();  //清屏
        delay(200);


        display.setTextSize(1.3);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);  //设置光标的位置

        // display.clearDisplay();
        //delay(8000);

        digitalWrite(in2, LOW);
        digitalWrite(in1, LOW);
        digitalWrite(LED, LOW);

        display.println("Hello, user!");
        display.println("");
        display.println("Enter Emergency\nBraking");
        display.println("");
        display.println("All programs have\nbeen closed for you");
        display.display();

        // actually display all of the above

        delay(2000);
    }
    else {

        float h = dht.readHumidity();//读湿度
        float t = dht.readTemperature();//读湿度
        Serial.print("Humidity：");//湿度
        Serial.print(h);
        Serial.println("%");
        Serial.print("Temperatrue：");//温度
        Serial.print(t);
        Serial.println("C");
        display.clearDisplay();
        delay(200);
        display.setTextSize(1.3);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);  //设置光标的位置
        float HUMI = ((M0 - analogRead(PIN_AO)) / ((M0 - M1) / 100)); //将湿度模拟量转换成数字量
        if (HUMI > 100.00) {
            HUMI = 100;
        }
        if (HUMI > 80) {
            digitalWrite(in2, LOW);
            digitalWrite(in1, LOW);
            digitalWrite(LED, LOW);
        }
        else {
            digitalWrite(in2, LOW);
            digitalWrite(in1, HIGH);
            digitalWrite(LED, HIGH);
        }
        Serial.print("HUMI= ");
        Serial.print(HUMI);
        Serial.println("% ");
        display.println("---------YANG--------");
        display.println("");
        display.print("Temperature:   ");
        display.print(t);
        display.println("C");
        display.print("Humidity:      ");//湿度
        display.print(h);
        display.println("%");
        display.print("Soil Moisture: ");
        display.println(HUMI);
        if (HUMI > 80) {
            display.println("Watter:           OFF");
            display.println("Photosynthesis:   OFF");
        }
        else {
            display.println("Watter:            ON");
            display.println("Photosynthesis:    ON");
        }

        display.println("---Have a nice DAY!--");
        //delay(1000);
        display.display();


    }
}
