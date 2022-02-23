#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SevenSegmentTM1637.h>

#define DHT_PIN 8

#define CLK_PIN 10
#define DIO_PIN 11

#define CHECK_DELAY_MILLIS 5000

void set_sensor_data();
void print_data();
void show_data();

unsigned long last_check_dht_sensor_millis = 0;

float temperature = 0.0;
float humidity = 0.0;
float heat_index = 0.0;

bool show_mode = true;
String show_value;

DHT dht_sensor = DHT(DHT_PIN, DHT11);
SevenSegmentTM1637 led_display(CLK_PIN, DIO_PIN);

void setup() {
	Serial.begin(9600);
	dht_sensor.begin();
	led_display.begin();
	led_display.setBacklight(10);
	led_display.clear();
}

void loop() {
	set_sensor_data();
}

bool wait_check_dht_sensor() {
	// use wait method instead delay
	unsigned long current_millis = millis();
	if (last_check_dht_sensor_millis == 0 || current_millis - last_check_dht_sensor_millis >= CHECK_DELAY_MILLIS) {
		last_check_dht_sensor_millis = current_millis;
		return false;
	}
	return true;
}

void set_sensor_data() {
	if (wait_check_dht_sensor()) {
		return;
	}
	temperature = dht_sensor.readTemperature();
	humidity = dht_sensor.readHumidity();
	if (isnan(temperature) || isnan(humidity)) {
		Serial.println("error");
	} else {
        heat_index = dht_sensor.computeHeatIndex(temperature, humidity, false);
        print_data();
        show_data();
    }
}

void show_data() {
	if (show_mode) {
		show_value = String(temperature, 1);
	} else {
		show_value = String((int) humidity) + " %";
	}
	led_display.print(show_value);
    show_mode = !show_mode;
}

void print_data() {
	Serial.print("Temperature: ");
	Serial.print(temperature);
	Serial.println(" C");

	Serial.print("Humidity:    ");
	Serial.print(humidity);
	Serial.println(" %");

    Serial.print("Heat index:  ");
    Serial.print(heat_index);
    Serial.println(" C");

	Serial.println();
}
