/*
  Copyright 2016 Carter Turnbaugh

  This file is part of LEP Arduino.

  LEP Arduino is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LEP Arduino is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LEP Arduino.  If not, see <http://www.gnu.org/licenses/>.
*/
//Version 10.0

const short num_devices = 4;
const short num_toggle = 1;

bool devices[num_devices - num_toggle];
short cycle = 0;
short cyclec = 0;
short mode = 0;

void setup() {

	Serial.begin(9600);

	//Main control outputs
	for (short i = 2; i < num_devices + 2; i++) {
		pinMode(i, OUTPUT);
		devices[i - 2] = 0;
	}
	digitalWrite(13, HIGH);

	//LED Cube (Mega only)
	for (short i = 20; i < 42; i += 2) pinMode(i, OUTPUT);
}

void loop() {

	if (Serial.available() > 0) {
		String command = Serial.readStringUntil(';') + ",-1,";

    if(command == "status,-1,"){}
		else if (command == "a,-1,") {
			mode = 1;
		}
		else if (command == "b,-1,") {
			mode = 2;
		}
		else if (command == "c,-1,") {
			mode = 3;
		}
		else if (command == "d,-1,") {
			mode = 0;
		}
		else {
			short split_idx = command.indexOf(',');
			short device_id = command.substring(0, split_idx).toInt();
			short next_split_idx = command.indexOf(',', split_idx + 1);
			short state = command.substring(split_idx + 1, next_split_idx).toInt();

			if (device_id < num_toggle) {
				digitalWrite(device_id + 2, HIGH);
				delay(50);
				digitalWrite(device_id + 2, LOW);
			}
			else {
				if (state == -1) {
					devices[device_id - num_toggle] = !devices[device_id - num_toggle];
				}
				else {
					devices[device_id - num_toggle] = (state != 0);
				}

				digitalWrite(device_id + 2, devices[device_id - num_toggle]);
			}
		}

		printStatus();
	}
	updateCube();
	delay(50);
}

void printStatus() {

	for (int i = 0; i < num_devices - num_toggle; i++) {
		Serial.print(devices[i]);
		if (i < num_devices - num_toggle - 1) Serial.print(",");
	}
	Serial.println();

}

void updateCube() {
	// Set ground
	digitalWrite(20, !(cycle == 0));
	digitalWrite(22, !(cycle == 1));
	digitalWrite(24, !(cycle == 2));
	cyclec++;
	if(cyclec >= 100){
		cyclec = 0;
		cycle = (cycle + 1) % 3;
	}

	digitalWrite(26, !devices[0]);
	digitalWrite(38, devices[0]);
	digitalWrite(28, !devices[1]);
	digitalWrite(40, devices[1]);
	digitalWrite(30, !devices[2]);
	digitalWrite(42, devices[2]);

	// Mode LED settings
	digitalWrite(32, mode == 3);
	digitalWrite(34, mode == 2);
	digitalWrite(36, mode == 1);
}
