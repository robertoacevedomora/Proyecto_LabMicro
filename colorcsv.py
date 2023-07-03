import serial
import csv
import os


#cambiar aqui el nombre del puerto (debe coincidir con el que esta en Arduino IDE)
puerto = "COM6"
#serializamos
#En port ponemos el valor que se obtiene al ejeciutar el script virtualport.sh
ser = serial.Serial(port = puerto,baudrate=9600,parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS) 
#ser = serial.Serial(port = '/dev/pts/3',baudrate=9600,parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS) 
current_directory = os.getcwd()

csv_path = os.path.join(current_directory, "cafe.csv")

with open(csv_path, mode ='w') as csv_file:
  writer = csv.writer(csv_file)
  writer.writerow(['Red', 'Green', 'Blue'])
  #cambiar el valor muestras por el valor de muestras que se desean
  #1000 son como 9 segundos, 1500 son como 13 segundos 
  muestras = 100
  cnt = 0
  while True:
    line = ser.readline().decode().strip()
    data = line.split(',')
    r = float(data[0])
    g = float(data[1])
    b = float(data[2])
    writer.writerow([r, g, b])
    #Con fines de debug
    print(f"r: {r}, g: {g}, b: {b}")
    cnt += 1
    if cnt >= muestras:
      break
