
#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <stdio.h>

void my_accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
}

// === 每个开关的特性定义 ===
homekit_characteristic_t cha_switch1_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch1_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch 1");

homekit_characteristic_t cha_switch2_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch2_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch 2");

homekit_characteristic_t cha_switch3_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch3_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch 3");

homekit_characteristic_t cha_switch4_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch4_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch 4");

homekit_characteristic_t cha_switch5_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch5_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch 5");

homekit_characteristic_t cha_switch6_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch6_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch 6");

homekit_accessory_t *accessories[] = {
  HOMEKIT_ACCESSORY(
    .id = 1,
    .category = homekit_accessory_category_switch,
  .services = (homekit_service_t*[]) {
    // 信息服务
    HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics = (homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "六路插排"),
      HOMEKIT_CHARACTERISTIC(MANUFACTURER, "github.com/bilibilifmk"),
      HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "(づ｡◕‿‿◕｡)づ"),
      HOMEKIT_CHARACTERISTIC(MODEL, "斐讯遗产"),
      HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
      HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
      NULL
    }),

    // 六个开关服务
    HOMEKIT_SERVICE(SWITCH, .primary = true, .characteristics = (homekit_characteristic_t*[]) {
      &cha_switch1_on,
      &cha_switch1_name,
      NULL
    }),

    HOMEKIT_SERVICE(SWITCH, .characteristics = (homekit_characteristic_t*[]) {
      &cha_switch2_on,
      &cha_switch2_name,
      NULL
    }),

    HOMEKIT_SERVICE(SWITCH, .characteristics = (homekit_characteristic_t*[]) {
      &cha_switch3_on,
      &cha_switch3_name,
      NULL
    }),

    HOMEKIT_SERVICE(SWITCH, .characteristics = (homekit_characteristic_t*[]) {
      &cha_switch4_on,
      &cha_switch4_name,
      NULL
    }),

    HOMEKIT_SERVICE(SWITCH, .characteristics = (homekit_characteristic_t*[]) {
      &cha_switch5_on,
      &cha_switch5_name,
      NULL
    }),

    HOMEKIT_SERVICE(SWITCH, .characteristics = (homekit_characteristic_t*[]) {
      &cha_switch6_on,
      &cha_switch6_name,
      NULL
    }),

    NULL
  }
  ),
  NULL
};

homekit_server_config_t config = {
  .accessories = accessories,
  .password = "111-11-111",
  .setupId = "ABCD"
};
