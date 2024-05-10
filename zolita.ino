#include <Keypad.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns 

// using namespace std;

TFT_eSPI tft = TFT_eSPI();

const char* ssid = "Wifi";
const char* password = "";
bool inputBoo = false;
const char* chatgpt_token = "chatgpt-api-key";
// const char* chatgpt_Q = "\"What's the Scorpio horoscope today in <10 tokens\"";

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

String month = "";
String day = "";
bool fortune = false;

byte pin_rows[ROW_NUM] = {2, 15, 13, 12}; // GPIO18, GPIO5, GPIO17, GPIO16 connect to the row pins
byte pin_column[COLUMN_NUM] = {32, 33, 25, 26};  // GPIO4, GPIO0, GPIO2 connect to the column pins

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

void setup() {
  // myStepper.setSpeed(60);
  Serial.begin(9600);
  tft.init();
  tft.setRotation(1);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  while(!Serial);

  // wait for WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
}

// void wrapText(const char* text, int x, int y, int maxWidth, int lineHeight) {
//   size_t len = strlen(text);
//   char buffer[maxWidth + 1];
//   int currentWidth = 0;
//   int currentLine = 0;
//   int charWidth = tft.width('A'); // Assume 'A' as the average character for width

//   for (size_t i = 0; i < len; i++) {
//     char character = text[i];
    
//     if (character == '\n') {
//       currentLine++;
//       currentWidth = 0;
//     } else if (currentWidth + charWidth > maxWidth) {
//       currentLine++;
//       currentWidth = 0;
//     }
    
//     if (character != '\n') {
//       buffer[currentWidth] = character;
//       buffer[currentWidth + 1] = '\0'; // Null-terminate the buffer
      
//       tft.drawString(buffer, x + currentWidth * charWidth, y + currentLine * lineHeight);
//       currentWidth++;
//     }
//   }
// }

String getZodiacSign(int month, int day) {
    if ((month == 3 && day >= 21) || (month == 4 && day <= 19)) {
        return "Aries";
    } else if ((month == 4 && day >= 20) || (month == 5 && day <= 20)) {
        return "Taurus";
    } else if ((month == 5 && day >= 21) || (month == 6 && day <= 20)) {
        return "Gemini";
    } else if ((month == 6 && day >= 21) || (month == 7 && day <= 22)) {
        return "Cancer";
    } else if ((month == 7 && day >= 23) || (month == 8 && day <= 22)) {
        return "Leo";
    } else if ((month == 8 && day >= 23) || (month == 9 && day <= 22)) {
        return "Virgo";
    } else if ((month == 9 && day >= 23) || (month == 10 && day <= 22)) {
        return "Libra";
    } else if ((month == 10 && day >= 23) || (month == 11 && day <= 21)) {
        return "Scorpio";
    } else if ((month == 11 && day >= 22) || (month == 12 && day <= 21)) {
        return "Sagittarius";
    } else if ((month == 12 && day >= 22) || (month == 1 && day <= 19)) {
        return "Capricorn";
    } else if ((month == 1 && day >= 20) || (month == 2 && day <= 18)) {
        return "Aquarius";
    } else if ((month == 2 && day >= 19) || (month == 3 && day <= 20)) {
        return "Pisces";
    } else {
        return "Unknown";
    }
}

const char* fetchHoroscope(String month, String day) {
  HTTPClient https;
  int monthInt = month.toInt();
  int dayInt = day.toInt();
  String starSign = getZodiacSign(monthInt, dayInt);
  String chatgpt_Q = "\"Give the " + starSign + " horoscope today in <10 tokens\"";
  Serial.println(chatgpt_Q);
  const char* chatgpt_Q_cstr = chatgpt_Q.c_str(); // Convert to const char*
  // String fullPrompt = String("What's the horoscope today in <100 tokens if birthday is ") + month + "/" + day + "\"";
  // String payload = "{\"model\": \"gpt-3.5-turbo-instruct\", \"prompt\": \"" + fullPrompt + "\", \"temperature\": 0, \"max_tokens\": 100}";
  String payload = String("{\"model\": \"gpt-3.5-turbo-instruct\", \"prompt\": ") + chatgpt_Q + String(", \"temperature\": 0, \"max_tokens\": 50}"); //Instead of TEXT as Payload, can be JSON as Paylaod
  https.begin("https://api.openai.com/v1/completions");
  https.addHeader("Content-Type", "application/json");
  https.addHeader("Authorization", String("Bearer ") + chatgpt_token);
  
  int httpCode = https.POST(payload);
  if (httpCode == HTTP_CODE_OK) {
    String response = https.getString();
    // Serial.println("Raw API Response: ");
    // Serial.println(response);
    DynamicJsonDocument doc(1024); // Adjust size as needed
    deserializeJson(doc, response);
    
    // Extract the fortune text
    const char* fortune = doc["choices"][0]["text"].as<const char*>();

    // const char* cleanedFortune;
    // for (size_t i = 0; i < strlen(fortune); i++) {
    //   if (fortune[i] != '\n') {
    //     cleanedFortune += fortune[i];
    //   }
    // }

    // fortune = cleanedFortune;

    // Print the extracted fortune
    Serial.println("Your Fortune: ");
    Serial.println(fortune);
    // month = "";
    // day = "";
    // delay(1000);
    // Serial.println("delay");
    
    return fortune;
  } else {
    Serial.print("Failed to get horoscope, error: ");
    Serial.println(https.errorToString(httpCode).c_str());
    https.end();
    return "None";
  }
  https.end();
  
  
  // Reset for next input
  
  // fortune = false;
}

void loop() {
  
  while(inputBoo == false){
    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);

    tft.drawString("I am", 0, 0, 1);
    delay(2000);
    tft.setTextSize(5);
    tft.drawString("ZOLITA!!!", 0, 50, 1);
    delay(4000);

    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("Who am I,", 0, 0);
    tft.drawString("you might ask?", 40, 40);
    delay(2000);
    tft.drawString("Well well.", 0, 80);
    delay(2000);

    tft.fillScreen(TFT_BLACK);
    tft.drawString("I am the", 0, 0);
    tft.drawString("most famous...",40,40);
    delay(1000);
    tft.drawString("powerful,", 0, 80);
    delay(1000);
    tft.drawString("and FIERCE,", 0, 120);
    delay(2000);
    tft.fillScreen(TFT_BLACK);
    delay(500);
    tft.setTextSize(3);
    tft.drawString("BARNARD",0,0);
    tft.drawString("RACOON!!!",50,100);
    delay(3000);
    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);

    tft.drawString("I will tell you", 0, 40);
    tft.drawString("your future.", 0, 80);
    delay(3000);

    tft.fillScreen(TFT_BLACK);
    tft.drawString("*sniff, sniff*", 20, 40);
    delay(1000);
    tft.drawString("Is that trash??!?!!! Hmmmm...", 0, 80);
    delay(1000);

    tft.fillScreen(TFT_BLACK);
    tft.drawString("Hmmmm...", 20, 80);
    delay(1000);

    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
    tft.drawString("DeliCiOuS!!!", 0, 1);
    delay(3000);
    tft.setTextSize(2);

    tft.fillScreen(TFT_BLACK);
    tft.drawString("Let's seeeee…", 0, 0);
    tft.drawString("You want", 0, 40);
    tft.drawString("to know YOUR", 0, 80);
    tft.drawString("future?", 40, 120);
    delay(5000);
    
    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    
    char key = keypad.getKey();
    // Serial.print("KEY: ");
    // Serial.println(key);
    if (month.length() == 0){
      tft.fillScreen(TFT_BLACK);
      tft.drawString("Enter your birth", 0, 0, 1);
      tft.drawString("month as two digits:", 0, 40, 1);
      delay(250);
    } else if (month.length() ==2 && day.length() == 0){
      tft.fillScreen(TFT_BLACK);
      tft.drawString("Enter your birth", 0, 0, 1);
      tft.drawString("day as two digits:", 0, 40, 1);
      delay(250);
    }
    if (std::isdigit(key)) {
      Serial.println(key);
      if (month.length() < 2) {
        month += key;
        tft.fillScreen(TFT_BLACK);
        tft.drawString("Enter your birth", 0, 0, 1);
        tft.drawString("month as two digits:", 0, 40, 1);
        tft.drawString(month, 20, 80, 1);
        Serial.print("Month: ");
        Serial.println(month);
        if (month.length() == 1){
          delay(250);
        }else if (month.length() == 2){
          delay(250);
        }
      } else if (day.length() < 2) {
        day += key;
        tft.fillScreen(TFT_BLACK);
        tft.drawString("Enter your birth", 0, 0, 1);
        tft.drawString("day as two digits:", 0, 40, 1);

        tft.drawString(month, 10, 80, 1);
        tft.drawChar('/', 40, 80, 1);
        tft.drawString(day, 60, 80, 1);

        Serial.print(month);
        Serial.print("/");
        Serial.println(day);
        if (day.length() == 1){
          delay(250);
        }else if (day.length() == 2){
          delay(500);
        }
      }
    } else if (key == 'D') {
      if (month.length() > 0 && day.length() == 0) {
        month.remove(month.length() - 1);
        tft.fillScreen(TFT_BLACK);
        tft.drawString("Enter your birth", 0, 0, 1);
        tft.drawString("month as two digits:", 0, 40, 1);
        tft.drawString(month, 20, 80, 1);
        Serial.print("Delete month: ");
        Serial.println(month);
      } else if (day.length() > 0) {
        day.remove(day.length() - 1);
        tft.fillScreen(TFT_BLACK);
        tft.drawString("Enter your birth", 0, 0, 1);
        tft.drawString("day as two digits:", 0, 40, 1);

        tft.drawString(month, 10, 80, 1);
        tft.drawChar('/', 40, 80, 1);
        tft.drawString(day, 60, 80, 1);
        Serial.print("Delete day: ");
        Serial.println(day);
      }
    }

    if(month.length() == 2 && day.length() == 2 && fortune != true){
      // fortune = false;
      int monthInt = month.toInt();
      int dayInt = day.toInt();
      String zodiacSign = getZodiacSign(monthInt, dayInt);
      tft.fillScreen(TFT_BLACK);
      
      if (zodiacSign != "Unknown"){
        tft.drawString("Your Zodiac Sign is: ", 0, 20, 1);
       
        tft.drawString(zodiacSign, 20, 50, 2);
        
        tft.drawString("Correeect????", 20, 80, 2);
        delay(1000);
        Serial.print("Your Zodiac Sign is: ");
        Serial.println(zodiacSign);
        Serial.println(zodiacSign);
        tft.fillScreen(TFT_BLACK);
        const char* fort = fetchHoroscope(month, day);
        // const char* fort = "Embrace grudges akd asdfjaskdf safsdfas fjkasdjf kadfkajdfiadsf askjdfkjasfjkdfjkahdfadf sdkfjakljsdfkjasdhfkjaf.";
        tft.setCursor(0, 0);

        tft.print(fort);

        // int textX = 0; // Initial X position
        // int textY = 0; // Initial Y position
        // int lineHeight = tft.fontHeight() * 2; // Line height based on text size
        // int maxWidth = tft.width(); // Maximum width of the screen
        
        // tft.fillScreen(TFT_BLACK); // Clear screen
        
        // wrapText(fort, textX, textY, maxWidth, lineHeight);
        delay(5000);

        // tft.drawString(fort, 20, 20, 1);
        fortune = true;
        delay(1000);
      } else{
        tft.drawString("YOU DARE TRY", 20, 20, 1);
        tft.drawString("TRICK ZoLItA?!", 20, 50, 2);
        tft.drawString("RESTART NOW", 20, 80, 2);
        Serial.println("Not a valid birth date. Restart");
        month="";
        day = "";
        delay(1000);
      }

    } 
    if (month.length() == 2 && day.length() == 2 && fortune == true){
      inputBoo=true;
    }
    
  }
  if (month.length() == 2 && day.length() == 2 && fortune == true){
      Serial.println("press A to restart");
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0,20);
      tft.print("Press A for more ZOLITA wisdom");
      delay(250);
      char key = keypad.getKey();
      if(key=='A'){
        fortune = false;
        inputBoo = false;
        month = "";
        day = "";
        Serial.println("pressed A");
        delay(500);
      }
    }
  

}
