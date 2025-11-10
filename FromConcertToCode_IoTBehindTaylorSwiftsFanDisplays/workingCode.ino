#include <IRremote.hpp>

#define red_key 4
#define green_key 5
#define blue_key 6
#define yellow_key 20
#define on_key 3
#define off_key 2


int red_led_pin = 7;
int green_led_pin = 6;
int blue_led_pin = 5;
int yellow_led_pin = 4;
#define IR_RECEIVE_PIN   8

int past_command = 0;



void setup()
{
    Serial.begin(115200);
    IrReceiver.begin(IR_RECEIVE_PIN);
    pinMode(red_led_pin, OUTPUT);
    pinMode(green_led_pin, OUTPUT);
    pinMode(blue_led_pin, OUTPUT);
    pinMode(yellow_led_pin, OUTPUT);
}

void loop() {
    if (IrReceiver.decode()) {
        
        IrReceiver.printIRResultShort(&Serial);
        Serial.println("command");
        Serial.println(IrReceiver.decodedIRData.command);

        if (IrReceiver.decodedIRData.command != past_command) {
            past_command = IrReceiver.decodedIRData.command;
            switch(IrReceiver.decodedIRData.command) {
                case red_key:
                    digitalWrite(red_led_pin, HIGH);
                    digitalWrite(green_led_pin, LOW);
                    digitalWrite(blue_led_pin, LOW);
                    digitalWrite(yellow_led_pin, LOW);
                    break;
                case green_key:
                    digitalWrite(red_led_pin, LOW);
                    digitalWrite(green_led_pin, HIGH);
                    digitalWrite(blue_led_pin, LOW);
                    digitalWrite(yellow_led_pin, LOW);
                    break;
                case blue_key:
                    digitalWrite(red_led_pin, LOW);
                    digitalWrite(green_led_pin, LOW);
                    digitalWrite(blue_led_pin, HIGH);
                    digitalWrite(yellow_led_pin, LOW);
                    break;
                case yellow_key:
                    digitalWrite(red_led_pin, LOW);
                    digitalWrite(green_led_pin, LOW);
                    digitalWrite(blue_led_pin, LOW);
                    digitalWrite(yellow_led_pin, HIGH);
                    break;
                case on_key:
                    digitalWrite(red_led_pin, HIGH);
                    digitalWrite(green_led_pin, HIGH);
                    digitalWrite(blue_led_pin, HIGH);
                    digitalWrite(yellow_led_pin, HIGH);
                    break;
                case off_key:
                    digitalWrite(red_led_pin, LOW);
                    digitalWrite(green_led_pin, LOW);
                    digitalWrite(blue_led_pin, LOW);
                    digitalWrite(yellow_led_pin, LOW);
                    break;
            }
        }
        
        IrReceiver.resume();
    }
}