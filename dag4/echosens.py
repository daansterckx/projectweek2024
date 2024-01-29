import OPi.GPIO as GPIO
import time

# Set the GPIO mode
GPIO.setmode(GPIO.BOARD)

# Define the GPIO pins for the ultrasonic sensor
TRIG_PIN = 5
ECHO_PIN = 3

# Set the GPIO pins as output and input
GPIO.setup(TRIG_PIN, GPIO.OUT)
GPIO.setup(ECHO_PIN, GPIO.IN)

def measure_distance():
    # Send a trigger signal to the ultrasonic sensor
    GPIO.output(TRIG_PIN, GPIO.HIGH)
    time.sleep(0.00001)
    GPIO.output(TRIG_PIN, GPIO.LOW)

    # Wait for the echo signal
    while GPIO.input(ECHO_PIN) == GPIO.LOW:
        pulse_start = time.time()

    while GPIO.input(ECHO_PIN) == GPIO.HIGH:
        pulse_end = time.time()

    # Calculate the distance based on the time difference
    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150
    distance = round(distance, 2)

    return distance

try:
    while True:
        # Measure the distance
        dist = measure_distance()
        print("Distance:", dist, "cm")
        time.sleep(1)

except KeyboardInterrupt:
    # Clean up the GPIO pins
    GPIO.cleanup()
