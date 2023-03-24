#include <WiFi.h>

// wifi details 
const char* ssid = "CrazyNet @Shamim";
const char* password = "shamim01";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//initial value of asset one and two
int assetOne = 0;
int assetTwo = 0;

//these strings will show the asset count in html page
String AssetOne=" ";
String AssetTwo=" ";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

//using the built in led for indicating incoming request
int LED_BUILTIN = 2;

void setup() {
  Serial.begin(115200);

  pinMode (LED_BUILTIN, OUTPUT);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //connecting to my wifi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
            
            // 
            if (header.indexOf("GET /one/1") >= 0) {
              Serial.println("ASSET ONE->");
              assetOne++;
              Serial.println(assetTwo);

              //converting the value into string so that can be used with html              
              AssetOne="Asset one =  "+String((int)assetOne);

              //incoming request indicating with a light blink
              digitalWrite(LED_BUILTIN, HIGH);
              delay(50);
              digitalWrite(LED_BUILTIN, LOW);
            }
         
            if (header.indexOf("GET /two/2") >= 0) {
              Serial.println("ASSET TWO->");
              assetTwo++;
              Serial.println(assetTwo);

              //converting the value into string so that can be used with html
              AssetTwo="Asset two =  "+String((int)assetTwo);


              digitalWrite(LED_BUILTIN, HIGH);
              delay(50);
              digitalWrite(LED_BUILTIN, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS 
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Asset Tracking & Monitor</h1>");
            
            // body 
            client.println("<p> " + AssetOne + "</p>");
            
            client.println("<p><a href=\"/one/1\"><button class=\"button\">+</button></a></p>");
               
            // Display count
            client.println("<p>" + AssetTwo + "</p>");
            
            client.println("<p><a href=\"/two/2\"><button class=\"button\">+</button></a></p>");
            
            client.println("</body></html>");
            
            // The HTTP response ends with two blank line
            client.println();
            client.println();

            // Break out of the while loop
            break;
          } 
          else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    // client.stop();
    // Serial.println("Client disconnected.");
    // Serial.println("");
  }

}