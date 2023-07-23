#include "clk_face_1.h"
#include <TRGBSuppport.h>
#include <ui.h>
#include <time.h>
#include "WiFi.h"

const char* ntpServer = "pool.ntp.org";
const char* ssid = "walker";
String password = "";
IPAddress ip; 

#define TIMEZONE -8
#define getCurrentUnixTime() (time(NULL) + TIMEZONE * 3600)

// Function to process the command and its argument
void processCommand(const String& command, const String& argument) {
  if (command == "a") {
    // Handle command 'A'
    // Do something with the argument (argument)
    Serial.println("Command 'ssid' executed with argument: " + argument);
  } else if (command == "ssid") {
      Serial.println("Command 'ssid' executed with argument: " + argument);
      Serial.println("Setting wifi ssid: " + argument);
      Serial.println("TODO");
  } else if (command == "t") {
      time_t current_time;
      struct tm *local_time;
      Serial.println("Command 't' executed with argument: " + argument);
      current_time = getCurrentUnixTime();
      local_time = localtime(&current_time);
      Serial.printf("The time is: %s", asctime(local_time));
      
  } else if (command == "pw") {
    // set up wifi
      Serial.println("Setting wifi password: " + argument);
      password = argument;
      WiFi.mode(WIFI_MODE_STA);
      int retry = 0;
      while (WiFi.status() != WL_CONNECTED && retry < 20) {
        Serial.printf("Connecting to %s %s\n", ssid, password);
        WiFi.begin(ssid, password);
        ip = WiFi.localIP();
        Serial.println(ip);
        retry++;
        delay(1000);
        Serial.printf("%d >", retry);
      }
      if (WiFi.status() == WL_CONNECTED) {
        ip = WiFi.localIP();
        Serial.println(ip);
        Serial.println("Setup NTP.");
        configTime(3600, 3600, ntpServer);   // Configure Timezone and NTP
      }
  } else {
    Serial.println("Invalid command.");
  }
}

// Function to handle command input and processing
void handleCommandInput() {
  static String inputBuffer = ""; // Stores the input until Enter is received

  // Check if any data is available on Serial
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      // Process the command if Enter is pressed
      inputBuffer.trim(); // Remove leading and trailing whitespaces
      // set all of inputBuffer to lower case
      for (int i = 0; i < inputBuffer.length(); i++) {
        inputBuffer[i] = tolower(inputBuffer[i]);
      }


      // Parse the command and argument
      int spaceIndex = inputBuffer.indexOf(' ');
      String command = inputBuffer.substring(0, spaceIndex);
      String argument = inputBuffer.substring(spaceIndex + 1); // Read the argument as a string

      // Process the command
      processCommand(command, argument);

      // Print command definitions after each command execution
      Serial.println();
      Serial.println("Command Definitions (case-insensitive separated by a space):");
      Serial.println("pw <arg>: Set the password to <arg>.");
      Serial.println("t : report the current time.");
      // Add more command definitions here

      // Clear the input buffer for the next command
      inputBuffer = "";
    } else if (c == 8) {
      // Handle backspace
      if (inputBuffer.length() > 0) {
        inputBuffer.remove(inputBuffer.length() - 1);
        Serial.write("\b \b"); // Send backspace-space-backspace to clear the last character on the Serial Monitor
      }
    } else {
      // Add the received character to the input buffer
      inputBuffer += c;
      Serial.print(c); // Echo the character back to the Serial Monitor
    }
  }
}


TRGBSuppport trgb;
void setup() {
  Serial.begin(115200);
  Serial.setTxTimeoutMs(1);  // workaround for blocking output if no host is connected to native USB CDC
  delay(100);   // Rumors say it helps avoid sporadical crashes after wakeup from deep-sleep
  trgb.init();
  
  // load your UI etc. (see example https://github.com/fablabnbg/TRGBArduinoSupport/tree/main/examples/ui_example)
  ui_init();
 
  Serial.write("Done with setup - now you're alive! \n");
}

void loop() {

static int ctr = 0;
static time_t last_time = 0;
static String cmd = "";
static String cmd_last = "";

handleCommandInput();
lv_task_handler();

time_t now = getCurrentUnixTime();
if (now > last_time)
{
  last_time = now;
  char* datetime = ctime(&now);
  if (VERBOSE > 1) Serial.printf("Current date and time: %s", datetime);

  set_hands(now);
}
delay(10);
ctr++;
}

