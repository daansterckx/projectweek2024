import wiringpi
import time

# Initialize WiringPi and set the GPIO mode
wiringpi.wiringPiSetupPhys()

# Define the GPIO pins for the ultrasonic sensor
TRIG_PIN = 5
ECHO_PIN = 3

# Set the GPIO pins as output and input
wiringpi.pinMode(TRIG_PIN, wiringpi.OUTPUT)
wiringpi.pinMode(ECHO_PIN, wiringpi.INPUT)

def measure_distance():
    # Send a trigger signal to the ultrasonic sensor
    wiringpi.digitalWrite(TRIG_PIN, wiringpi.HIGH)
    time.sleep(0.00001)
    wiringpi.digitalWrite(TRIG_PIN, wiringpi.LOW)

    # Wait for the echo signal
    while wiringpi.digitalRead(ECHO_PIN) == wiringpi.LOW:
        pulse_start = time.time()

    while wiringpi.digitalRead(ECHO_PIN) == wiringpi.HIGH:
        pulse_end = time.time()

    # Calculate the distance based on the time difference
    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150
    distance = round(distance, 2)

    return distance
while True:
    distance = measure_distance()
    print(f"Distance: {distance} cm")
