#include <TRGBSuppport.h>
#include <ui.h>
#include <time.h>

TRGBSuppport trgb;
void setup() {
  Serial.begin(115200);
  Serial.setTxTimeoutMs(1);  // workaround for blocking output if no host is connected to native USB CDC
  delay(100);   // Rumors say it helps avoid sporadical crashes after wakeup from deep-sleep
  trgb.init();
  
  // load your UI etc. (see example https://github.com/fablabnbg/TRGBArduinoSupport/tree/main/examples/ui_example)
  ui_init();
  // Get the display size
 
  Serial.write("Done with setup - now you're alive! \n");
}

void loop() {

static int ctr = 0;
static time_t last_time = 0;

if (Serial.available() > 0) {
        // Read the incoming character
        char incomingChar = Serial.read();

        // Do something with the received character
        // For example, you can print it back to the Serial Monitor
        Serial.print("Received: ");
        Serial.println(incomingChar);
}
lv_task_handler();

time_t now = time(0);
if (now > last_time)
{
  last_time = now;
  char* datetime = ctime(&now);
  Serial.printf("Current date and time: %s", datetime);

  set_hands(now);
}
delay(10);
ctr++;
}

