/*
 Name:		FlowerpotOS.ino
 Created:	2023/4/11 23:34:18
 Author:	qw200
*/


int LocalVersionint = 500;//版本号缩写
// the setup function runs once when you press reset or power the board
#include <PubSubClient.h>//默认，加载MQTT库文件
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPUpdateServer.h>
#include <Adafruit_SH110X.h> //驱动128x64 and 128x32 的屏幕显示的库
#include <Adafruit_GFX.h>    //Arduino的图形库
#include <Wire.h>
#include <iomanip>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
     // 引入WiFiManager库
#include "FS.h"
#include <LittleFS.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <string>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include <iostream>
#include <sstream>
WiFiUDP ntpUDP;

String UPst = "";
String NewVersion = "";
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 60 * 60 * 8, 60 * 1000);
//#include <DHT.h>
#include <ESP8266WiFi.h>
#include <Adafruit_AHTX0.h>
long timeval = 3 * 1000;
long lastMsg = 0;//时间戳
String LocalVersion = "V5.0.0";//版本号
Adafruit_AHTX0 aht;
#include "picture.cpp"
//#define DHTPIN 13//土壤湿度传感器
//#define DHTTYPE DHT11//传感器类型
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#include <WiFiManager.h>
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
int HOT = 14;//加热片
int M0 = 1024;  //在空气中AO读取的值最大为1024，代表干燥时的读数
int M1 = 0;   //浸泡在水里的最小值 464（最小值会改变），代表100%湿度
int PIN_AO = A0;  //土壤湿度传感器
int Pump = 0;//水泵
int LED = 13;//LED灯
//int LIGHT = 13;//光敏传感器
int buttonPin = 12;  //按键引脚
int buttonState = 0;//按键状态
int counter = 0;//按键次数计数器
//DHT dht(DHTPIN, DHTTYPE);//进行初始化设置
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);//显示设置
String upUrl = "http://bin.bemfa.com/b/1BcNDZkMzZjNTM2ODQ0NDIzNTkwMzk4OWFiOGQ1ODE5OTM=Flowerpot.bin";
String mqtt_server = "";//默认，MQTT服务器
int mqtt_server_port = 0;//默认，MQTT服务器
String mqtt_username = "";
String mqtt_password = "";
String ID_MQTT = "" ;    //修改，你的Client ID
String topic = "";  //主题名字，可在巴法云控制台自行创建，名称随意
WiFiClient espClient;
PubSubClient client(espClient);
//灯光函数及引脚定义
void Updata();
int LEDst = 0;//st结尾为状态记录
int HOTst = 0;
int PUMPst = 0;
boolean WIFIst = false;
void OffLed();//接收方法
void ONLed();
void OFFHOT();
void ONHOT();
void ONPUMP();
void OFFPUMP();
void ONAUTO();
void OFFAUTO();
//-----cpp文件全局定义---------
void open();
void Netshare();
void frist();
void second();
void third();
void fourth();
void fifth();
void six();
void updatainfo();
void updateBin();

int AUTOst = 0;
const int B_led = D4;
int updst = 0;
uint64_t chipId;


//WiFiManager wifiManager;
const char* AP_NAME = "配网WIFI"; // WiFi名称
char sta_ssid[32] = { 0 };
char sta_password[64] = { 0 };

void connectNewWifi();
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
int conv2int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return 0;  // default return if input is not a valid hexadecimal character
}

String urldecode(const String &str) {
    String ret;
    char ch;
    char code0;
    char code1;
    for (unsigned int i = 0; i < str.length(); i++) {
        if (str[i] == '%') {
            ch = conv2int(str[++i]);
            code0 = ch << 4;
            ch = conv2int(str[++i]);
            code1 = code0 + ch;
            ret.concat(code1);
        } else {
            ret.concat(str[i]);
        }
    }
    return ret;
}
void INDEXhtml() {
    File htmlFile = LittleFS.open("/index.html", "r");
    server.streamFile(htmlFile, "text/html");
    htmlFile.close();
}
void INFOhtml() {
    File htmlFile = LittleFS.open("/info.html", "r");
    server.streamFile(htmlFile, "text/html");
    htmlFile.close();
}
void WIFIhtml() {
    File htmlFile = LittleFS.open("/wifi.html", "r");
    server.streamFile(htmlFile, "text/html");
    htmlFile.close();
}
void UPDATEhtml() {
    File htmlFile = LittleFS.open("/update.html", "r");
    server.streamFile(htmlFile, "text/html");
    htmlFile.close();
}

void Contolhtml() {
    String autoButton = (AUTOst == 0) ? "关" : "开";
    String ledButton = (LEDst == 0) ? "关" : "开";
    String hotButton = (HOTst == 0) ? "关" : "开";
    String pumpButton = (PUMPst == 0) ? "关" : "开";

    String page = "\
        <!DOCTYPE html>\
        <html lang='en'>\
        <head>\
            <meta charset='UTF-8'>\
            <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
            <title>控制台</title>\
            <script>\
                function toggleStatus(id) {\
                    var currentState = document.getElementById('status' + id).value;\
                    var newState = (currentState === '0') ? '1' : '0';\
                    document.getElementById('status' + id).value = newState;\
                    document.getElementById('toggleButton' + id).innerText = (newState === '0') ? '关' : '开';\
                    var xhr = new XMLHttpRequest();\
                    xhr.open('POST', '/contol?id=' + id, true);\
                    xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');\
                    xhr.send('status=' + newState);\
                }\
            </script>\
 <style>\
        body {\
            margin: 0;\
            padding: 0;\
            font-family: Arial, sans-serif;\
        }\
        .container {\
            max-width: 800px;\
            margin: 0 auto;\
            padding: 20px;\
            text-align: center;\
        }\
        h1 {\
            text-align: center;\
        }\
        .button {\
            display: inline-block;\
            height: 65px;\
            width: 300px;\
            margin-top: 3px;\
            padding: 10px 20px;\
            background-color: darkgray;\
            color: #fff;\
            border: none;\
            border-radius: 20px; /* 添加圆角 */\
            text-decoration: none;\
            line-height: 2; /* 通过调整line-height的值来调整文字的垂直位置 */\
            text-align: center; /* 文字居中 */\
            box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2); /* 添加立体感 */\
            transition: all 0.3s ease; /* 添加过渡效果 */\
        }\
        .button:hover {\
            background-color: #86b7fe; /* 鼠标悬停时的背景颜色 */\
            transform: translateY(2px); /* 点击效果 */\
            box-shadow: 2px 2px 8px rgba(0, 0, 0, 0.3); /* 添加更多立体感 */\
        }\
    </style>\
        </head>\
        <body class='container'>\
  <h1>离线控制台</h1>\
    <p>自动控制</p>\
            <button class='button' id='toggleButton1' onclick='toggleStatus(1)'>" + autoButton + "</button>\
            <input type='hidden' id='status1' value='" + AUTOst + "'>\
  <p>补光灯控制</p>\
            <button class='button' id='toggleButton2' onclick='toggleStatus(2)'>" + ledButton + "</button>\
            <input type='hidden' id='status2' value='" + LEDst + "'>\
  <p>土壤恒温控制</p>\
            <button class='button' id='toggleButton3' onclick='toggleStatus(3)'>" + hotButton + "</button>\
            <input type='hidden' id='status3' value='" + HOTst + "'>\
  <p>浇水控制</p>\
            <button class='button' id='toggleButton4' onclick='toggleStatus(4)'>" + pumpButton + "</button>\
            <input type='hidden' id='status4' value='" + PUMPst + "'>\
        </body>\
        </html>";

    server.send(200, "text/html", page);
}

void ContolhtmlPost() {
    if (server.hasArg("status") && server.hasArg("id") && server.method() == HTTP_POST) {
        int id = server.arg("id").toInt();
        int newStatus = server.arg("status").toInt();

        if (id == 1) {
            AUTOst = newStatus;
            server.send(200, "text/plain", "AUTOst 已更新为: " + String(AUTOst));
        } else if (id == 2) {
            LEDst = newStatus;
            server.send(200, "text/plain", "LEDst 已更新为: " + String(LEDst));
        } else if (id == 3) {
            HOTst = newStatus;
            server.send(200, "text/plain", "HOTst 已更新为: " + String(HOTst));
        } else if (id == 4) {
            PUMPst = newStatus;
            server.send(200, "text/plain", "PUMPst 已更新为: " + String(PUMPst));
        } else {
            server.send(400, "text/plain", "无效的按钮 ID");
        }
    } else {
        server.send(400, "text/plain", "缺少参数或请求方法不正确");
    }
}



void initBasic(void) {
    Serial.begin(115200);
    WiFi.hostname("Smart-ESP8266");
}

void initSoftAP(void) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    if (WiFi.softAP(AP_NAME)) {
        Serial.println("ESP8266 SoftAP is started");
    }
}

void WIFIhtmlPost() {
    Serial.println("WIFIhtmlPost");
    if (server.hasArg("ssid")) {
        String encodedSSID = server.arg("ssid");
        String decodedSSID = urldecode(encodedSSID);
        strcpy(sta_ssid, decodedSSID.c_str());
        Serial.println(sta_ssid);
    } else {
        Serial.println("error, SSID not found");
        server.send(200, "text/html", "<meta charset='UTF-8'>error, SSID not found");
        return;
    }

    if (server.hasArg("password")) {
        String password = server.arg("password");
        strcpy(sta_password, password.c_str());
    } else {
        Serial.println("error, password not found");
        server.send(200, "text/html", "<meta charset='UTF-8'>error, password not found");
        return;
    }
    //mqtt
    if (server.hasArg("host")) {
        mqtt_server = server.arg("host");
    } else {
        Serial.println("error, password not found");
      //  server.send(200, "text/html", "<meta charset='UTF-8'>error, password not found");
        return;
    }

    if (server.hasArg("port")) {
        mqtt_server_port = server.arg("port").toInt();

    } else {
        Serial.println("error, password not found");
        //server.send(200, "text/html", "<meta charset='UTF-8'>error, password not found");
        return;
    }

    if (server.hasArg("client")) {
        ID_MQTT = server.arg("client");

    } else {
        Serial.println("error, password not found");
        //  server.send(200, "text/html", "<meta charset='UTF-8'>error, password not found");
        return;
    }

    if (server.hasArg("topic")) {
        topic = server.arg("topic");
      //  strcpy(sta_password, password.c_str());
    } else {
        Serial.println("error, password not found");
    //    server.send(200, "text/html", "<meta charset='UTF-8'>error, password not found");
        return;
    }

    if (server.hasArg("user")) {
        mqtt_username = server.arg("user");
        //  strcpy(sta_password, password.c_str());
    } else {
        Serial.println("error, password not found");
        //    server.send(200, "text/html", "<meta charset='UTF-8'>error, password not found");
        return;
    }
    if (server.hasArg("mqttpassword")) {
        mqtt_password = server.arg("mqttpassword");
        //  strcpy(sta_password, password.c_str());
    } else {
        Serial.println("error, password not found");
        //    server.send(200, "text/html", "<meta charset='UTF-8'>error, password not found");
        return;
    }
    delay(2000);
    connectNewWifi();

    if (WiFi.status() == WL_CONNECTED) {
        // 如果已连接到WiFi，则重定向到新IP
        String newIPAddress = "http://" + WiFi.localIP().toString();
        server.sendHeader("Location", newIPAddress, true);
        server.send(302, "text/plain", "");
        server.client().stop();
        delay(3000);
        WiFi.mode(WIFI_STA);
        return; // 结束处理
       // return;

    }

    //  WiFi.mode(WIFI_STA);



}
void initWebServer(void) {
   server.on("/", HTTP_GET, INDEXhtml);
    server.on("/wifi", HTTP_GET, WIFIhtml);
    server.onNotFound(WIFIhtml);
    server.on("/wifi", HTTP_POST, WIFIhtmlPost);
    server.on("/contol", HTTP_GET, Contolhtml);
    server.on("/contol", HTTP_POST, ContolhtmlPost);
    server.on("/info", HTTP_GET, INFOhtml);
    server.on("/update", HTTP_GET, UPDATEhtml);
    server.begin();
    Serial.println("WebServer started!");
}

void initDNS(void) {
    if (dnsServer.start(DNS_PORT, "*", apIP)) {
        Serial.println("DNS Server started successfully");
    } else {
        Serial.println("Failed to start DNS Server");
    }
}

void connectNewWifi(void) {
  //   WiFi.mode(WIFI_STA);
    Serial.println(WIFIst);
    WiFi.setAutoConnect(true);
    WiFi.begin(sta_ssid, sta_password);
    Serial.print("connectNewWifi: in");
    Serial.println(sta_ssid);
    Serial.println(sta_password);
    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        count++;
        Serial.print(count);
        if (count > 10) {
            Serial.print("connectNewWifi: "+count);
            initSoftAP();
            initWebServer();
            initDNS();
            break;
        }
    }

   Serial.println(WIFIst);
    if (WiFi.status() == WL_CONNECTED) {

      //  Serial.println(WIFIst);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        // server.stop();
        Serial.println("Connected to WiFi");
        Serial.println("Connected to WiFi！！！！！！！！！！");
        Serial.println(WiFi.localIP());
        initWebServer();
        //server.on("/sdd", HTTP_GET, WIFIhtml);
      //  server.on("/wifiok", []() {
       //     server.send(200, "text/plain", "WIFI OK");
       //     });

      //  server.begin();
        WIFIst = true;
        Serial.println(WIFIst);
    }
}

//----------------------------------------------
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    String Mqtt_Buff = "";
    for (int i = 0; i < length; i++) {
        Mqtt_Buff += (char)payload[i];
    }
    Serial.print(Mqtt_Buff);
    Serial.println();
    chipId = ESP.getChipId();
    // 将 uint64_t 转换为字符串格式
    char chipIdStr[20];  // 定义一个足够大的字符数组来存储转换结果
    itoa(chipId, chipIdStr, 10);

    // 将转换的结果构建为Arduino的String对象
    String chipIdString(chipIdStr);
    Serial.println(chipIdString);

    // Switch on the LED if an 1 was received as first character
    if (Mqtt_Buff == "#IDTO" + chipIdString + "&Updata") {//如果接收字符on，亮灯
        Updata();//开灯函数
        Serial.println("updata");
    }
    else if (Mqtt_Buff == "#IDTO" + chipIdString + "&OFFLED") {//如果接收字符off，亮灯
        OffLed();//关灯函数
        Serial.println("updata!!!!");
    }
    else if (Mqtt_Buff == "#IDTO" + chipIdString + "&ONLED") {//如果接收字符off，亮灯
        ONLed();//关灯函数
    }
    else if (Mqtt_Buff == "#IDTO" + chipIdString + "&OFFHOT") {//如果接收字符off，亮灯
        OFFHOT();//关灯函数
    }
    else if (Mqtt_Buff == "#IDTO" + chipIdString + "&ONHOT") {//如果接收字符off，亮灯
        ONHOT();//关灯函数

    }
    else if (Mqtt_Buff == "#IDTO" + chipIdString + "&OFFPUMP") {//如果接收字符off，亮灯
        OFFPUMP();//关灯函数
    }
    else if (Mqtt_Buff == "#IDTO" + chipIdString + "&ONPUMP") {//如果接收字符off，亮灯
        ONPUMP();//关灯函数

    }
    else if (Mqtt_Buff == "#IDTO" + chipIdString + "&OFFAUTO") {//如果接收字符off，亮灯
        OFFAUTO();//关灯函数
    }
    else if (Mqtt_Buff == "#IDTO" + chipIdString + "&ONAUTO") {//如果接收字符off，亮灯
        ONAUTO();//关灯函数

    }
    Mqtt_Buff = "";
}

void reconnect() {
    client.setServer(mqtt_server.c_str(), mqtt_server_port);
    client.setCallback(callback);
    // Loop until we're reconnected
   // while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(ID_MQTT.c_str(), mqtt_username.c_str(), mqtt_password.c_str())) {
            Serial.println("connected");

            client.subscribe(topic.c_str());//修改，修改为你的主题
        }
        else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
 //  }
}


void setup() {

    Serial.println("128x64 OLED FeatherWing test");

    chipId = ESP.getChipId();
    if (aht.begin()) {
        Serial.println("Found AHT20");
    }
    else {
        Serial.println("Didn't find AHT20");
    }
    // dht.begin();//DHT开始工作
    analogWrite(13, 31);  // 使用PWM设置数字引脚13（通常对应D7）的输出占空比为31

    timeClient.begin();
    // PWM的值范围是0到255，其中0表示0%的占空比（完全关闭），255表示100%的占空比（完全打开）
    // 因此，设置占空比为31表示大约12%的占空比

    // 通过调整占空比，可以控制引脚上输出的电压平均值的大小
    // 在这种情况下，引脚13将以约12%的占空比以一定的频率产生一个脉冲信号
    // 该脉冲信号的平均值对应于31/255*Vref（假设使用5V供电）的电压输出
    // 这样可以模拟出一个介于0V和Vref之间的中间值
    pinMode(PIN_AO, INPUT);
    pinMode(HOT, OUTPUT);
    pinMode(Pump, OUTPUT);


    pinMode(LED, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);  // 设置buttonPin引脚的模式为输入模式，并启用上拉电阻

    // 使用pinMode函数将buttonPin引脚设置为输入模式
    // INPUT_PULLUP表示在引脚输入模式下，启用内部上拉电阻
    // 上拉电阻可确保引脚在未连接到外部电路时保持高电平状态

    // 通常，当按钮未按下时，按钮引脚处于高电平状态（由内部上拉电阻提供）
    // 当按钮按下时，按钮引脚会连接到地，形成低电平状态

    // 配置输入引脚时，使用上拉电阻可以简化电路设计，无需额外的外部上拉电阻器
    // 这有助于防止未连接到外部电路时的"悬空"引脚状态，提高电路稳定性
    digitalWrite(Pump, LOW);

    digitalWrite(LED, LOW);
    open();//开机LOGO
    Serial.println("128x64 OLED FeatherWisssssng test");
    if (!LittleFS.begin()) {
        Serial.println("Failed to mount LittleFS");
        return;
    }
   Serial.println(WIFIst);
    initBasic();
    connectNewWifi();

    httpUpdater.setup(&server);

    server.begin();
  //  Serial.println(WIFIst);
}
int numwifi = 0;
int chos = 0;
void loop() {
   // Serial.println(WIFIst);
    server.handleClient();
    dnsServer.processNextRequest();
  //  Serial.println("   Serial.println(status);");
  //Serial.println(status);

 //   Serial.println("Connected was...  OK");


    //  if (chos == 0) {
    //    connectNewWifi();

    //        chos++;

    //  }
    Serial.println("---------------------");
    Serial.println(LEDst);
    Serial.println(PUMPst);
    if(WIFIst) {
        Netshare();
    }
    unsigned long currentMillis = 0;
    buttonState = digitalRead(buttonPin); // 读取按钮状态
    // 按钮按下时，计数器加1
    if (buttonState == LOW) {
        currentMillis = millis(); // 获取当前时间
        delay(500); // 延时去抖动
        counter++;
        if (counter > 5) {
            counter = 0; // 计数器超过2时，重置为0
        }
    }
    if (buttonState == HIGH) {
        if (millis() - currentMillis < 3000) {// 获取当前时间
            delay(500); // 延时去抖动
            counter++;
            if (counter > 5) {
                counter = 0; // 计数器超过2时，重置为0
            }
        }
    }
    //WifiConnect();
    Serial.println("mqtt-------------------------");
    Serial.println(mqtt_server);
    Serial.println(mqtt_server_port);
    Serial.println(mqtt_password);
    Serial.println(mqtt_username);
    Serial.println(ID_MQTT);
    Serial.println(topic);
    if (numwifi != 0) {

    }
    else {
        if (counter == 0) {

            frist();
        }
        else if (counter == 1) {
            second();
        }
        else if (counter == 2) {
            third();

        }
        else if (counter == 3) {
            fourth();
        }
        else if (counter == 4) {
            fifth();
        }
        else if (counter == 5) {
            six();
        }
        Serial.println(counter); //输出计数器数值
    }

    /*
    Serial.println("Connected was...  OK");
    server.handleClient();
    dnsServer.processNextRequest();

  //  if (chos == 0) {
    //    connectNewWifi();

    //        chos++;

  //  }

    Netshare();
    unsigned long currentMillis = 0;
    buttonState = digitalRead(buttonPin); // 读取按钮状态
    // 按钮按下时，计数器加1
    if (buttonState == LOW) {
       currentMillis = millis(); // 获取当前时间
        delay(500); // 延时去抖动
        counter++;
        if (counter > 5) {
            counter = 0; // 计数器超过2时，重置为0
        }
    }
    if (buttonState == HIGH) {
        if (millis() - currentMillis < 3000) {// 获取当前时间
            delay(500); // 延时去抖动
            counter++;
            if (counter > 5) {
                counter = 0; // 计数器超过2时，重置为0
            }
        }
    }
    //WifiConnect();
    Serial.println("connect");
    if (numwifi != 0) {

    }
    else {
        if (counter == 0) {

            frist();
        }
        else if (counter == 1) {
            second();
        }
        else if (counter == 2) {
            third();

        }
        else if (counter == 3) {
            fourth();
        }
        else if (counter == 4) {
            fifth();
        }
        else if (counter == 5) {
            six();
        }
        Serial.println(counter); //输出计数器数值
    }
    */
}

//---------------------------开机LOGO---------------------------------------------
void open() {
    display.begin(0x3C, true); //初始化OLED并设置其IIC地址为 0x3C
    display.clearDisplay();   //清屏
    delay(200);
    display.setTextSize(1.3);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);  //设置光标的位置
    display.drawBitmap(0, 0, hand_128x64, 128, 64, SH110X_WHITE);//开机LOGO
    display.display();
    delay(5000);
}
//---------------------------拟人表情(自动模式)---------------------------------------------
void frist() {
    int Hours = timeClient.getHours();
    if (Hours > 19 || Hours < 7) {
        if (AUTOst == 1) {
            digitalWrite(LED, HIGH);
            LEDst = 1;
        }
    }
    else {
        if (AUTOst == 1) {
            digitalWrite(LED, LOW);
            LEDst = 0;
        }

    }
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
    if (temp.temperature < 15 && humidity.relative_humidity > 40) {
        if (AUTOst == 1) {//自动模式开启时启动
            digitalWrite(HOT, HIGH);
            HOTst = 1;
        }

    }
    else {
        if (AUTOst == 1) {
            digitalWrite(HOT, LOW);
            HOTst = 0;
        }

    }
    display.setTextSize(1.3);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);  //设置光标的位置
    float HUMI = (M0 - analogRead(PIN_AO)) / ((M0 - M1) / 100); //??????????????????????????????????????????
    //float HUMI2 = (M0 - analogRead(led2)) / ((M0 - M1) / 100); //????????????????????????????????????????????
   // int x = analogRead(led2);
   // Serial.print("HOT= ");
  //  Serial.print(HOTst);
  //  Serial.print(AUTOst);
    //Serial.print(x);
    if (HUMI > 100.00) {
        HUMI = 100;//土壤湿度
    }
    if (HUMI > 45) {                //测试认为，浇水后土壤湿度在50-55上下
        display.clearDisplay();
        display.drawBitmap(0, 0, bmp4, 128, 64, SH110X_WHITE);//0-0
        display.display();
        if (AUTOst == 1) {
            PUMPst = 0;
            LEDst = 0;
            digitalWrite(Pump, LOW);
            digitalWrite(LED, LOW);
        }

    }
    else {
        display.clearDisplay();
        display.drawBitmap(0, 0, bmp1, 128, 64, SH110X_WHITE);//X-X
        display.display();
        if (AUTOst == 1) {
            digitalWrite(Pump, HIGH);//开启水泵电机
            //digitalWrite(LED,HIGH);
            PUMPst = 1;
        }


    }
    // if (HUMI2 > 100.00) {
     //    HUMI2 = 100;//光亮强度
    // }
     //if (HUMI2 > 35) {
     //   digitalWrite(LED, LOW);//控制灯光关闭
    // }
    // else {
     //    digitalWrite(LED, HIGH);
    // }
}
//---------------------------信息屏幕---------------------------------------------
void second() {
    //float h = dht.readHumidity();//读取湿度
   // float t = dht.readTemperature();//读取温度
      //  display.print("  SSD Temp: "); display.print(temp.temperature); display.println(" C");
       //display.print("  SSD Hum:  "); display.print(humidity.relative_humidity); display.println(" %");
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
    if (temp.temperature < 15 && humidity.relative_humidity > 40) {
        if (AUTOst == 1) {
            digitalWrite(HOT, HIGH);
            HOTst = 1;
        }

    }
    else {
        if (AUTOst == 1) {
            digitalWrite(HOT, LOW);
            HOTst = 0;
        }

    }
    Serial.print("Humidity????");//??????
    Serial.print(temp.temperature);
    Serial.println("%");
    Serial.print("Temperatrue????");//??????
    Serial.print(humidity.relative_humidity);
    //Serial.print(analogRead(led2));
    Serial.println("C");
    display.clearDisplay();
    //   delay(200);
    display.setTextSize(1.3);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);  //设置光标
    // float HUMI2 =analogRead(led2);
    float HUMI = (M0 - analogRead(PIN_AO)) / ((M0 - M1) / 100);// 使用公式计算湿度：(M0 - analogRead(PIN_AO)) / ((M0 - M1) / 100)
    //float HUMI2 = (M0 - analogRead(led2)) / ((M0 - M1) / 100); //??????????????????????????????????????????
    if (HUMI > 100.00) {
        HUMI = 100;
    }
    if (HUMI > 80) {
        if (AUTOst == 1) {
            digitalWrite(Pump, LOW);
            PUMPst = 0;
        }


    }
    else {
        if (AUTOst == 1) {
            digitalWrite(Pump, HIGH);
            PUMPst = 1;
        }


    }
    // if (HUMI2 > 100.00) {
    //     HUMI2 = 100;
   //  }
    // if (HUMI2 > 35) {
    //     digitalWrite(LED, LOW);
    // }
     //else {
     //    digitalWrite(LED, HIGH);
    // }
    Serial.print("HUMI= ");
    // Serial.print(HUMI2);
    Serial.println("% ");
    display.println("------Flowerpot------");
    display.drawBitmap(0, 7, wen, 16, 16, SH110X_WHITE);
    display.drawBitmap(20, 7, du, 16, 16, SH110X_WHITE);
    display.drawBitmap(40, 7, maohao, 16, 16, SH110X_WHITE);
    display.println("");//温度
    display.print("               ");
    display.print(temp.temperature);
    display.println("C");
    display.drawBitmap(0, 27, shi, 16, 16, SH110X_WHITE);
    display.drawBitmap(20, 27, du, 16, 16, SH110X_WHITE);
    display.drawBitmap(40, 27, maohao, 16, 16, SH110X_WHITE);
    display.println("");//湿度
    display.print("               ");//布局
    display.print(humidity.relative_humidity);
    display.println("%");
    display.println("");//??????
    display.println("");//??????
    display.drawBitmap(0, 47, tu, 16, 16, SH110X_WHITE);
    display.drawBitmap(20, 47, rang, 16, 16, SH110X_WHITE);
    display.drawBitmap(40, 47, shi, 16, 16, SH110X_WHITE);
    display.drawBitmap(60, 47, du, 16, 16, SH110X_WHITE);
    display.drawBitmap(80, 47, maohao, 16, 16, SH110X_WHITE);
    display.print("               ");//土壤湿度
    Serial.println("light");
    display.println(HUMI);
    display.display();

}
//---------------------------手动浇水模式（关闭所有自动程序）---------------------------------------------
void third() {
    display.clearDisplay();   //清屏
    display.setTextSize(1.3);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);   //设置光标的位置

    digitalWrite(Pump, LOW);
    digitalWrite(LED, LOW);
    display.println("-------Warning-------");
    display.drawBitmap(5, 10, shou, 16, 16, SH110X_WHITE);
    display.drawBitmap(25, 10, dong, 16, 16, SH110X_WHITE);
    display.drawBitmap(45, 10, jiao, 16, 16, SH110X_WHITE);
    display.drawBitmap(65, 10, shui, 16, 16, SH110X_WHITE);
    display.drawBitmap(85, 10, mo, 16, 16, SH110X_WHITE);
    display.drawBitmap(105, 10, shi2, 16, 16, SH110X_WHITE);
    display.println("|                   |");
    display.println("|                   |");
    display.println("|                   |");
    display.println("|                   |");
    display.println("|All programs have  |");
    display.println("|been closed for you|");
    display.println("---------P 2---------");
    display.display();
}
//---------------------------配网模式---------------------------------------------
void fourth() {
    display.clearDisplay();  //清屏
    delay(200);
    display.setTextSize(1.3);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);  //设置光标位置

    digitalWrite(Pump, LOW);
    digitalWrite(LED, LOW);
    display.println("-----WIFI Connect----");
    display.println("|                   |");
    display.drawBitmap(5, 10, wang, 16, 16, SH110X_WHITE);
    display.drawBitmap(25, 10, luo, 16, 16, SH110X_WHITE);
    display.drawBitmap(45, 10, pei, 16, 16, SH110X_WHITE);
    display.drawBitmap(65, 10, dui, 16, 16, SH110X_WHITE);
    display.drawBitmap(85, 10, mo, 16, 16, SH110X_WHITE);
    display.drawBitmap(105, 10, shi2, 16, 16, SH110X_WHITE);
    display.println("|                   |");
    display.println("|                   |");
    display.println("Password:    12345678");
    display.display();

}
//---------------------------系统信息---------------------------------------------
void fifth() {
    display.clearDisplay();  //清屏
    delay(200);
    display.setTextSize(1.3);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);  //设置光标的位置
    String chipIdString = String();
    chipIdString += (uint32_t)(chipId >> 32);
    chipIdString += (uint32_t)chipId;
    chipIdString = chipIdString.substring(1);
    digitalWrite(Pump, LOW);
    digitalWrite(LED, LOW);
    display.println("-----Flowerpot OS----");
    display.println("|FlowerpotOS| " + LocalVersion + "|");
    display.println("|CPU        |ESP8266|");
    display.println("|Storage    |    4MB|");
    display.println("|Firmware   |   V5.5|");
    display.println("|ID:     fp" + chipIdString + "|");
    display.print("|IP  ");
    display.print(WiFi.localIP());
    display.println("|");
    display.println("---------P 4---------");
    display.display();
}
//-----------------------系统更新界面---------------------------------------

void six() {
    if (updst == 0) {
        updatainfo();
    }
    if (LocalVersion.equals(NewVersion)) {
        UPst = "  No Update!   ";

    }
    else {

        UPst = "  New Version! ";
    }
    display.clearDisplay();  //清屏
    delay(200);
    display.setTextSize(1.3);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);  //设置光标的位置

    digitalWrite(Pump, LOW);
    digitalWrite(LED, LOW);
    display.println("--------Update-------");
    display.println("|                   |");
    display.println("|  " + UPst + "  |");
    // 假设这里获取到了芯片的ID

    // 将chipId转换为String类型
  //  String chipIdString = String(chipId);
    display.println("|                   |");
    display.println("|-------------------|");
    display.println("|LocalVersion|" + LocalVersion + " | ");
    display.println("|New  Version|" + NewVersion + " | ");
    display.println("---------P 5---------");
    display.display();
}
void Updata() {
    //  ledstatus = "on";
    Serial.println("Updata!");
    //  digitalWrite(B_led, LOW);
    updateBin();
}
//关闭灯泡
void OffLed() {
    // ledstatus = "off";
    Serial.println("turn off light");
    // digitalWrite(B_led, HIGH);
    digitalWrite(LED, LOW);
    LEDst = 0;
}
void ONLed() {
    // ledstatus = "off";
    Serial.println("turn on light");
    // digitalWrite(B_led, HIGH);
    digitalWrite(LED, HIGH);
    LEDst = 1;
}
void OFFHOT() {
    // ledstatus = "off";
    Serial.println("turn off HOT");
    // digitalWrite(B_led, HIGH);
    digitalWrite(HOT, LOW);
    HOTst = 0;
}
void ONHOT() {
    // ledstatus = "off";
    Serial.println("turn on HOT");
    // digitalWrite(B_led, HIGH);
    digitalWrite(HOT, HIGH);
    HOTst = 1;
}
void OFFPUMP() {
    // ledstatus = "off";
    Serial.println("turn off PUMP");
    // digitalWrite(B_led, HIGH);
    digitalWrite(Pump, LOW);
    PUMPst = 0;
}
void ONPUMP() {
    // ledstatus = "off";
    Serial.println("turn on PUMP");
    // digitalWrite(B_led, HIGH);
    digitalWrite(Pump, HIGH);
    PUMPst = 1;
}
void ONAUTO() {
    // ledstatus = "off";
    Serial.println("turn on AUTO");
    // digitalWrite(B_led, HIGH);
    AUTOst = 1;
}
void OFFAUTO() {
    // ledstatus = "off";
    Serial.println("turn off AUTO");
    // digitalWrite(B_led, HIGH);
    digitalWrite(Pump, HIGH);
    AUTOst = 0;
}
unsigned long connectInterval = 60000; // 连接Wi-Fi的间隔时间，单位为毫秒
unsigned long previousConnectMillis = 0; // 上一次尝试连接的时间


// 转换为字符串
std::string uint64ToString(uint64_t num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}
std::string appendIntToString(int intValue) {
    // 将整型数字转换为字符串
    std::stringstream ss;

    // 将整型数字写入 stringstream
    ss << intValue;

    // 将 stringstream 转换为字符串
    std::string intString = ss.str();

    // 拼接字符串
    std::string message = intString;


    return message;
}
std::string appendFloatToString(float floatValue) {
    // 将浮点数转换为字符串
    if (floatValue < 0 || floatValue > 100) {
        return "0";
    }

    // 将浮点数转换为字符串
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << floatValue;
    std::string floatString = ss.str();

    // 拼接字符串
    std::string message = floatString;

    return message;
}
//int wifimqttfirst=0;
void Netshare() {//矛盾点，建议加入WIFI连通性验证已启用MQTT
    unsigned long currentMillis3 = millis(); // 获取当前时间

    // 检查是否到了连接Wi-Fi的时间间隔
    if (client.connected()) {
        // 连接成功
     //   wifimqttfirst++;
        // 在这里执行连接成功的任务
       // Serial.println("Failed to connect!!!!!!!!");
        client.loop();

        long now = millis();//获取当前时间戳
        if (now - lastMsg > timeval) {//如果达到3s，进行数据上传
            lastMsg = now;
            sensors_event_t humidity, temp;
            aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
            // Serial.print("Humidity????");//??????
            Serial.print(temp.temperature);//发送有问题，需要聚合函数值！！！
            Serial.println("%");
            // Serial.print("Temperatrue????");//??????
            Serial.print(humidity.relative_humidity);
            float tempss = temp.temperature;
            float humdss = humidity.relative_humidity;
            std::string TEMP = appendFloatToString(tempss);
            std::string HUMD = appendFloatToString(humdss);

            // Serial.println("   as  ");
             // 获取芯片 ID


             // 打印芯片 ID


             // 转换为字符串
            std::string strNumber = uint64ToString(chipId);
            std::string  LocalVersionintsst = appendIntToString(LocalVersionint);
            std::string HOTsst = appendIntToString(HOTst);
            std::string LEDsst = appendIntToString(LEDst);
            std::string PUMPsst = appendIntToString(PUMPst);
            std::string autosst = appendIntToString(AUTOst);
            float HUMI = (M0 - analogRead(PIN_AO)) / ((M0 - M1) / 100);
            std::string HUMIs = appendFloatToString(HUMI);
            // 拼接字符串
            std::string msg = "#ID" + strNumber + "#" + TEMP + "#" + HUMD + "#" + HUMIs + "#" + PUMPsst + "#" + HOTsst + "#" + LEDsst + "#" + LocalVersionintsst + "#" + autosst;
            // + "#" + LIGHT待实现（光强紫外线识别）（ChatGPT AI自动化）（AI生长趋势检测）

            // 打印消息
            std::cout << msg << std::endl;
            client.publish(topic.c_str(), msg.c_str());//数据上传
            Serial.println(msg.c_str());
        }


    }
    else {
        Serial.println("netshare else");
        if (currentMillis3 - previousConnectMillis >= connectInterval) {
            previousConnectMillis = currentMillis3; // 更新上一次尝试连接的时间
            Serial.println("Fatasdt");
            // 尝试连接Wi-Fi网络
            if (client.connected()) {
                // 连接成功
                // 在这里执行连接成功的任务
                Serial.println("Failed to connec22222222222t");
                client.loop();
            }
            else {
                // 连接失败
                // 在这里执行连接失败的任务
                Serial.println("Failed to connect");
                reconnect();
            }
        }

    }
}
/*
unsigned long pressStartTime = 0; // 记录长按开始时间
unsigned long pressDuration = 5000; // 长按持续时间，单位为毫秒

void WifiConnect() {
    if (digitalRead(buttonPin) == LOW) {
        // 如果按钮刚刚被按下，记录开始时间
        if (pressStartTime == 0) {
            pressStartTime = millis();
            numwifi = numwifi + 1;
            display.clearDisplay();
            display.setRotation(0);
            display.setTextSize(1);
            display.setTextColor(SH110X_WHITE);
            display.setCursor(0, 0);

            display.println("------WIFI Reset-----");
            display.println("|                   |");
            display.println("|   Please press    |");
            // 假设这里获取到了芯片的ID

            // 将chipId转换为String类型
          //  String chipIdString = String(chipId);
            display.println("|                   |");
            display.println("|-------------------|");
            display.println("|LocalVersion|" + LocalVersion + " | ");
            display.println("|New  Version|" + NewVersion + " | ");
            display.println("---------P 5---------");
            display.display();
        }


        // 检查按钮是否被长按
        if (millis() - pressStartTime >= pressDuration) {
            // 长按时间已达到指定时间，执行相应的代码
            executeAfterLongPress();

            // 重置开始时间，以允许下一次长按触发
            pressStartTime = 0;
            numwifi = 0;
        }
    }
    else {
        // 按钮没有被按下，重置开始时间
        pressStartTime = 0;
        numwifi = 0;
    }
}

void executeAfterLongPress() {
    // 在这里执行长按后的任务
    // 例如执行millis()函数和其他代码

    Serial.println("test");

    // 在这里实现你想要执行的任务代码
    // 这部分代码每隔60秒就会被触发执行
    WiFi.mode(WIFI_STA); // 设置模式，esp默认为STA+AP
    WiFiManager wm;  //定义manager对象
    bool res;

    res = wm.autoConnect("YANG-SSD", "12345678"); //定义ap名称和密码

    if (!res) {
        Serial.println("Connected was...  OK");
        Serial.println("Failed to connec22222222222t");
    }
    else {
        //if you get here you have connected to the WiFi
        Serial.println("Connected was...  OK");
        Serial.println("Fc22222222222t");


    }


    // 在这里实现长按后的逻辑
}
*/
//升级状态记录

//当升级开始时，打印日志
void update_started() {
    Serial.println("CALLBACK:  HTTP update process started");
    UPst = " Updata start! ";
}

//当升级结束时，打印日志
void update_finished() {
    Serial.println("CALLBACK:  HTTP update process finished");
    UPst = "Update succeed!";
}

//当升级中，打印日志
void update_progress(int cur, int total) {
    Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
    UPst = "   Updating!   ";
}

//当升级失败时，打印日志
void update_error(int err) {
    Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
    UPst = "Update failed! ";
}


/**
 * 固件升级函数
 * 在需要升级的地方，加上这个函数即可，例如setup中加的updateBin();
 * 原理：通过http请求获取远程固件，实现升级
 */
void updateBin() {
    WiFiClient UpdateClient;
    ESPhttpUpdate.onStart(update_started);//当升级开始时
    ESPhttpUpdate.onEnd(update_finished); //当升级结束时
    ESPhttpUpdate.onProgress(update_progress); //当升级中
    ESPhttpUpdate.onError(update_error); //当升级失败时

    t_httpUpdate_return ret = ESPhttpUpdate.update(UpdateClient, upUrl);
    switch (ret) {
    case HTTP_UPDATE_FAILED:      //当升级失败
        Serial.println("Update failed");
        UPst = "Update failed! ";
        break;
    case HTTP_UPDATE_NO_UPDATES:  //当无升级
        Serial.println("No Update");
        UPst = "  No Update!   ";
        break;
    case HTTP_UPDATE_OK:         //当升级成功
        Serial.println("Update Ok");
        UPst = "  Update Ok!   ";
        break;
    }
}

void updatainfo() {
    String url = "https://apis.jxcxin.cn/api/qqcollection?url=https://sharechain.qq.com/fc921df52dff5ae9417ed753b417168e";
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure client;
        client.setInsecure(); // Do not verify certificate
        HTTPClient https;
        https.begin(client, url);
        int httpCode = https.GET();
        if (httpCode > 0) {
            String payload = https.getString();
            Serial.println(payload);
            Serial.println("");
            // 重点1：即将解析的json文件
            String json = payload;
            Serial.println(String("") + "JSON大小：" + json.length());
            // 重点2：解析的JSON数据大小
            DynamicJsonDocument doc(json.length() * 2); //解析的JSON数据大小


            // 重点3：反序列化数据
            deserializeJson(doc, json);

            // 重点4：获取解析后的数据信息
            String nameStr = doc["data"]["content"].as<String>();

            // int numberInt = doc["time"].as<int>();
        //    en = nameStr;
            NewVersion = nameStr.substring(0, 6);

            //Serial.print("errnoInt = ");Serial.println(numberInt);

        }
        https.end();
    }
    updst = 1;
}