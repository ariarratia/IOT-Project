# IOT-Project
# 🐾 AUTOMATED PET FEEDER

A simple IoT pet feeder system that dispenses food at scheduled times using Wi-Fi and a web interface.
## FEATURES
- Remote feeding schedule configuration through web browser
- Automatic food dispensing using servo motor
- Pressure sensor detects if food is already in tray
- Wi-Fi communication between feeder and cloud server
- Minimal and affordable hardware setup

## HARDWARE COMPONENTS
- TTGO Board (1)
- Servo Motor (1)
- Pressure Sensor (1)
- Food Tray (1)
- Storage Bucket (1)

## SYSTEM ARCHITECTURE
- Controller: TTGO microcontroller  
- Sensor: Pressure-based weight sensor  
- Actuator: Servo motor  
- Communication: Wi-Fi  
- Cloud Server: Manages schedule and communicates with device  
- User Interface: Web browser

## HOW TO USE
1. Connect TTGO to Wi-Fi.
2. Open the web interface from any browser.
3. Set your pet’s feeding times.
4. The system checks if the tray is empty before dispensing.
5. Food is automatically dispensed according to schedule.

## CURRENT LIMITATIONS
- No phone notifications implemented yet
- Supports one pet and one type of dry food
- Requires manual tray cleaning and refill

## FUTURE IMPROVEMENTS
- Add push notifications to alert user about uneaten meals
- Improve portion control with a rotating measurement cup
- Enhance sensor reliability with dual-sensor setup

## By:
- Ari Arratia  
- Eric Ribeiro
