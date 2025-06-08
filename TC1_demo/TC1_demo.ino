
//适配斐讯tc1 改esp8266 接入HomeKit url https://github.com/bilibilifmk/TC1_demo


#define FS_CONFIG
#include <wifi_link_tool.h>
#include <Arduino.h>
#include <arduino_homekit_server.h>

#define LOG_D(fmt, ...) printf_P(PSTR(fmt "\n"), ##__VA_ARGS__);

//======================= 引脚定义 ========================
#define KEY_PIN 14
#define SWITCH1_PIN 5
#define SWITCH2_PIN 16
#define SWITCH3_PIN 13
#define SWITCH4_PIN 4
#define SWITCH5_PIN 16
#define SWITCH6_PIN 15

//======================= 外部 HomeKit 特性声明 ========================
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch1_on;
extern "C" homekit_characteristic_t cha_switch2_on;
extern "C" homekit_characteristic_t cha_switch3_on;
extern "C" homekit_characteristic_t cha_switch4_on;
extern "C" homekit_characteristic_t cha_switch5_on;
extern "C" homekit_characteristic_t cha_switch6_on;

//======================= 设置和循环 ========================
void ICACHE_RAM_ATTR REST_SET();


void setup() {
  GPIO_INIT();
  pinMode(KEY_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(KEY_PIN), REST_SET, FALLING);
  Serial.begin(115200);

  ////////// Wi-Fi 基础配置 //////////
  rstb = 0;
  stateled = 2;
  Hostname = "六路插排";
  wxscan = true;
  Version = "1.0.0";
  load();  // 加载WiFi配置信息
  webServer.on("/get_url", get_url);
  if (WiFi_State == 1)
  {
    generate_homekit_config();
    my_homekit_setup();  // 初始化 HomeKit
    print_homekit_uri();
  }
  ota();


}

void loop() {
  pant();  // wifi_link_tool 内部循环
  if (WiFi_State == 1)
  {
    my_homekit_loop();
  }

}

String print_homekit_uri() {
  char uri[64];
  if (homekit_get_setup_uri(&config, uri, sizeof(uri)) == 0) {
    Serial.print("HomeKit 配对 URL: ");
    Serial.println(uri);
    return uri;
  } else {
    Serial.println("生成 HomeKit URI 失败");
    return "ERROR";
  }
}


void get_url() {
  webServer.send(200, "text/plain", print_homekit_uri());
}

uint32_t mac_hash(const uint8_t *mac, size_t len) {
  uint32_t h = 0x811C9DC5;
  for (size_t i = 0; i < len; i++) {
    h ^= mac[i];
    h *= 0x01000193;
  }
  return h;
}
void generate_homekit_config() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  uint32_t hash = mac_hash(mac, sizeof(mac));
  uint32_t password_raw = hash % 1000000000; // 限定为最多9位数
  char password[11];
  snprintf(password, sizeof(password), "%03d-%02d-%03d",
           (password_raw / 100000) % 1000,
           (password_raw / 1000) % 100,
           password_raw % 1000);
  config.password = strdup(password);  // 动态分配，保证生命周期

  // 生成 setupId：4位[A-Z0-9]
  const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  char setupId[5];
  for (int i = 0; i < 4; i++) {
    setupId[i] = charset[(hash >> (i * 5)) & 0x1F];  // 取5位
  }
  setupId[4] = '\0';
  config.setupId = strdup(setupId);

  LOG_D("生成 HomeKit 配置：password = %s, setupId = %s", config.password, config.setupId);

}

void REST_SET() {
  Serial.println("长按10秒后重置");
  bool res_state = true;
  unsigned int res_time = millis();
  while (millis() - res_time < 10000) {
    ESP.wdtFeed();
    //喂狗
    //    while (millis() - res_time < 2000);
    if (digitalRead(KEY_PIN) != LOW) {
      res_state = false;
      Serial.println("终止重置");
      break;
    }
  }
  if (res_state == true) {

    EEPROM.write(WiFi_State_Addr, 0);
    EEPROM.commit();
    if (WiFi.status() == WL_CONNECTED) {
      WiFi.disconnect(true);
    }
    delay(300);
    homekit_storage_reset();
    Serial.println("等待重启");
    for (int i = 0; i < 10; i++) {
      digitalWrite(stateled, LOW);
      res_time = millis();
      while (millis() - res_time < 200)
        ;
      digitalWrite(stateled, HIGH);
      res_time = millis();
      while (millis() - res_time < 200)
        ;
    }
    Serial.println("重置!");
    ESP.restart();
  }
}

void GPIO_INIT() {
  // 设置 GPIO 为输出
  pinMode(SWITCH1_PIN, OUTPUT);
  digitalWrite(SWITCH1_PIN, LOW);
  pinMode(SWITCH2_PIN, OUTPUT);
  digitalWrite(SWITCH2_PIN, LOW);
  pinMode(SWITCH3_PIN, OUTPUT);
  digitalWrite(SWITCH3_PIN, LOW);
  pinMode(SWITCH4_PIN, OUTPUT);
  digitalWrite(SWITCH4_PIN, LOW);
  pinMode(SWITCH5_PIN, OUTPUT);
  digitalWrite(SWITCH5_PIN, LOW);
  pinMode(SWITCH6_PIN, OUTPUT);
  digitalWrite(SWITCH6_PIN, LOW);
}

//======================= 设置器函数 ========================
void switch1_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  digitalWrite(SWITCH1_PIN, on ? HIGH : LOW);
  LOG_D("Switch1: %s", on ? "ON" : "OFF");
}

void switch2_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  digitalWrite(SWITCH2_PIN, on ? HIGH : LOW);
  LOG_D("Switch2: %s", on ? "ON" : "OFF");
}

void switch3_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  digitalWrite(SWITCH3_PIN, on ? HIGH : LOW);
  LOG_D("Switch3: %s", on ? "ON" : "OFF");
}

void switch4_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  digitalWrite(SWITCH4_PIN, on ? HIGH : LOW);
  LOG_D("Switch4: %s", on ? "ON" : "OFF");
}

void switch5_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  digitalWrite(SWITCH5_PIN, on ? HIGH : LOW);
  LOG_D("Switch5: %s", on ? "ON" : "OFF");
}

void switch6_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  digitalWrite(SWITCH6_PIN, on ? HIGH : LOW);  //
  LOG_D("Switch6: %s", on ? "ON" : "OFF");
}

//======================= HomeKit 初始化 ========================
void my_homekit_setup() {


  // 绑定 setter
  cha_switch1_on.setter = switch1_setter;
  cha_switch2_on.setter = switch2_setter;
  cha_switch3_on.setter = switch3_setter;
  cha_switch4_on.setter = switch4_setter;
  cha_switch5_on.setter = switch5_setter;
  cha_switch6_on.setter = switch6_setter;

  // 启动 HomeKit 服务
  arduino_homekit_setup(&config);
}

//======================= HomeKit 循环处理 ========================
static uint32_t next_heap_millis = 0;

void my_homekit_loop() {
  arduino_homekit_loop();

  const uint32_t t = millis();
  if (t > next_heap_millis) {
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
          ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
  }
}
