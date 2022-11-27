#Python code, save this on the Raspberry Pi as sendtoteensy.py
import serial
port = serial.Serial("/dev/serial0", 115200)
port.write(b"Hello\n")
def echoline1():
    line = input("Write your text:")
    line = line+"\n"
    port.write(line.encode('utf-8'))
    print(line)
    return line.rstrip()
if _name_ == "__main__":
    while True:
        line = echoline1()
        if line == "EXIT":
            break