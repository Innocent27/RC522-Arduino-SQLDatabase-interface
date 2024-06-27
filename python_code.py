import serial       #pyserial for serial data communication between arduino and python
import MySQLdb      #MySQLClient package for sql server interface
import time

#using MySQLdb connector to establish connection to MySQL database.
dbConn = MySQLdb.connect("localhost","Lindokuhle","123456789","serial_data") or die ("could not connect to database")
#open a cursor to the database
cursor = dbConn.cursor()

device = 'COM7' #this is arduino serial port 
try:
  print("Connecting to arduino on port",device) 
  arduino = serial.Serial(device, 9600)             # pyserial class for arduino serial communication
except: 
  print("Failed to connect on",device)
while True:
    time.sleep(1)
    try:
        data=arduino.readline()       #reading rfid sensor data from arduino
        print(data)
        pieces=data.split(" ")        #data structuring using # delimiter
        # Executing SQL commands
        try:
            cursor=dbConn.cursor()
            cursor.execute("""INSERT INTO arduino_rfid_data (ID,Member_ID,allowed_members) VALUES (NULL,%s,%s)""", (pieces[0],pieces[1]))
            dbConn.commit()
            cursor.close()
        except MySQLdb.IntegrityError:
            print("failed to insert data")
        finally:
            cursor.close()
    except:
        print("Processing")
    
            
