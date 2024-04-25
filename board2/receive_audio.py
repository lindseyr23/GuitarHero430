import serial
import vlc
from time import sleep

ser = serial.Serial(port='COM7', baudrate=19200, bytesize=8, timeout=None, stopbits=serial.STOPBITS_ONE)
serialString = ""
commands = ["Play Song 1", "Stop Song", "Play Song 2", "Win", "Loss"]
curr_playing = 0

def read():
    reading = 1
    while reading:
        buffer = ser.read(1).decode("Ascii").strip()
        serialString = ser.readline().decode("Ascii").strip()
        try:
            serialString = buffer + serialString
            if any(serialString == x for x in commands):
                reading = 0
                print(serialString)
        except:
            pass
    return serialString

while(1):
    readval = read()
    if(readval == "Play Song 1"):
        p = vlc.MediaPlayer("Rock_Band_4_Fortunate_Son _Creedence_Clearwater_Revival_Full_Band_HD.mp3.mp3")
        p.play()
        curr_playing = 1
        sleep(5)
    if(readval == "Play Song 2"):
        p = vlc.MediaPlayer("Tears_For_Fears_Everybody_Wants_To_Rule_The_World_w_Lyrics.mp3")
        p.play()
        curr_playing = 1
        sleep(5)
    if(readval == "Win"):
        p = vlc.MediaPlayer("win_sound_effect.mp3")
        p.play()
        curr_playing = 1
        sleep(5)  
    if(readval == "Loss"):
        p = vlc.MediaPlayer("loss_sound_effect.mp3")
        p.play()
        curr_playing = 1
        sleep(5)      
    if(readval == "Stop Song"):
        if(curr_playing):
            p.stop()
            curr_playing = 0


            