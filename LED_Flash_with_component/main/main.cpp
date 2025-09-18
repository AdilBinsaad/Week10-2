#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "LED.h"

LED led1(GPIO_NUM_16); 
LED led2(GPIO_NUM_17); 
LED led3(GPIO_NUM_5); 
LED led4(GPIO_NUM_18); 
LED led5(GPIO_NUM_19); 
LED led6(GPIO_NUM_21); 
LED led7(GPIO_NUM_22); 
LED led8(GPIO_NUM_23); 

LED* leds[] = {&led1, &led2, &led3, &led4, &led5, &led6, &led7, &led8};
const int num_leds = sizeof(leds) / sizeof(leds[0]);

void allOff() {
    for (int i = 0; i < num_leds; i++) leds[i]->OFF();
}

void printPattern(int on1, int on2 = -1) {
    for (int i = 0; i < num_leds; i++) {
        if (i == on1 || i == on2) printf("*");
        else printf(".");
    }
    printf("\n");
}

// ---------------------------
// รูปแบบการแสดงผล LED
// ---------------------------
void patternOne() { // ไฟวิ่งดวงเดียว
    for (int i = 0; i < num_leds; i++) {
        allOff();
        leds[i]->ON();
        printPattern(i);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void patternTwo() { // ไฟวิ่งสองดวงสวนกันตรงกลาง
    for (int step = 0; step < num_leds; step++) {
        allOff();
        int left = step;
        int right = num_leds - 1 - step;
        leds[left]->ON();
        leds[right]->ON();
        printPattern(left, right);
        vTaskDelay(pdMS_TO_TICKS(200));
        if (left >= right) break;
    }
    for (int step = num_leds/2 - 1; step >= 0; step--) {
        allOff();
        int left = step;
        int right = num_leds - 1 - step;
        leds[left]->ON();
        leds[right]->ON();
        printPattern(left, right);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void patternThree() { // ไฟวิ่งไปกลับ
    // ไปขวา
    for (int i = 0; i < num_leds; i++) {
        allOff();
        leds[i]->ON();
        printPattern(i);
        vTaskDelay(pdMS_TO_TICKS(150));
    }
    // กลับซ้าย
    for (int i = num_leds - 2; i >= 0; i--) { // แก้ i>0 เป็น i>=0
        allOff();
        leds[i]->ON();
        printPattern(i);
        vTaskDelay(pdMS_TO_TICKS(150));
    }
}

extern "C" void app_main(void)
{
    // เริ่ม Serial
    printf("เลือกการแสดงผล LED:\n");
    printf("1: ไฟวิ่งดวงเดียว\n");
    printf("2: ไฟวิ่งสองดวงสวนกันตรงกลาง\n");
    printf("3: ไฟวิ่งไปกลับ \n");

    while (true) {
        char c;
        if (scanf(" %c", &c) == 1) {
            switch(c) {
                case '1':
                    patternOne();
                    break;
                case '2':
                    patternTwo();
                    break;
                case '3':
                    patternThree();
                    break;
                default:
                    printf("เลือก 1, 2 หรือ 3 เท่านั้น\n");
                    break;
            }
            printf("เลือกการแสดงผล LED:\n");
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // ป้องกัน loop เร็วเกินไป
    }
}
