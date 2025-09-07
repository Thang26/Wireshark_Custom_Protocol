import serial
import sys
import time

# --- Configuration ---
SERIAL_PORT = 'COM4'  # <--- Change this to your ST-Link's Virtual COM Port
BAUD_RATE = 115200    # <--- This must match the baud rate set on the STM32

def read_from_com_port():
    """
    Connects to the specified serial port, reads incoming data line by line,
    and prints it to the console.
    """
    print(f"Attempting to connect to {SERIAL_PORT} at {BAUD_RATE} baud...")

    try:
        # Open the serial port. The timeout is set to 2 seconds, so readline()
        # will wait a maximum of 2 seconds for a line of data.
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2)
        print(f"Successfully connected to {SERIAL_PORT}. Press Ctrl+C to stop.")

    except serial.SerialException as e:
        print(f"Error: Could not open port {SERIAL_PORT}.")
        print(f"  -> {e}")
        print("Please check the following:")
        print("  1. Is the NUCLEO board plugged in?")
        print("  2. Is the correct COM port specified in the script?")
        print("  3. Is another program (like PuTTY or CubeIDE's terminal) using the port?")
        sys.exit(1)

    try:
        while True:
            # Read one line from the serial port. This will block until a newline
            # character is received or the timeout is reached.
            incoming_line = ser.readline()

            # ser.readline() returns a bytes object. We need to decode it to a string.
            if incoming_line:
                try:
                    # Decode using 'utf-8'. Use 'replace' for error handling
                    # in case of garbled data.
                    decoded_line = incoming_line.decode('utf-8', errors='replace').strip()
                    print(f"Received: {decoded_line}")
                except UnicodeDecodeError:
                    print(f"Received (raw bytes): {incoming_line}")

            # A small sleep can make the script more responsive to Ctrl+C
            time.sleep(0.01)

    except KeyboardInterrupt:
        # Handle Ctrl+C gracefully
        print("\n[INFO] Program interrupted by user. Closing serial port.")

    except serial.SerialException as e:
        print(f"\n[ERROR] Serial communication error: {e}")
        print("[INFO] Device may have been disconnected. Closing port.")

    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print(f"[INFO] Serial port {SERIAL_PORT} closed.")

if __name__ == '__main__':
    read_from_com_port()