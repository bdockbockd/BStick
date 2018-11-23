from scipy.io import wavfile
from flask import Flask, request
import os
import pyaudio  
import wave

app = Flask(__name__)

@app.route('/')
def play():
    #get key
    key = int(request.args.get('key'))
    print('Playing key {}'.format(key))

    #define stream chunk   
    chunk = 1024

    #list keep directory
    directory = [];
    for i in range(0,18):
        directory.append("translate_tts (" + i + ")")

    #open a wav format music
    f = wave.open(r"" + os.getcwd() + "\\" + directory[key] + ".wav" ,"rb")
    #instantiate PyAudio  
    p = pyaudio.PyAudio()  
    #open stream  
    stream = p.open(format = p.get_format_from_width(f.getsampwidth()),  
                    channels = f.getnchannels(),  
                    rate = f.getframerate(),  
                    output = True)  
    #read data  
    data = f.readframes(chunk)  

    #play stream  
    while data:  
        stream.write(data)  
        data = f.readframes(chunk)  

    #stop stream  
    stream.stop_stream()  
    stream.close()  

    #close PyAudio  
    p.terminate()  
    return 'Hello, World!'

# start the server at the port 5000
app.run(host = '0.0.0.0')



