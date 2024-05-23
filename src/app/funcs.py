from transformers import pipeline
import numpy as np
import serial
import serial.tools.list_ports as ports_list
import time

# Transcriber function
transcriber = pipeline("automatic-speech-recognition", model="openai/whisper-small.en")


def send_input(input_text: str, form):
    
    """
    
    Send input from the web app to the Arduino

    Paramters: 
      - input_text: user's textual input or transcribed auditive input (of type str)
      - form: string with selected type of input

    Returns: transformed string 
    
    """
    
    # Clean the input
    transformed_transcription = clean_input(input_text, form)

    # Send the input
    set_up_port(transformed_transcription)

    return transformed_transcription



def set_up_port(final_input):
  
  # List available ports with indices
  serial_port = "/dev/cu.usbmodem1101"
  baud_rate = 9600
  serial_instance = serial.Serial(serial_port,baud_rate, timeout=1)

 

  # try:
  #     serial_instance.open()
  # except serial.SerialException as e:
  #     print(f"Could not open port {serial_port}: {e}")
  #     exit()

  print(f"Final input is {final_input}")
  
  time.sleep(5)
  
  serial_instance.write(final_input.encode("utf-8"))

  print("Message has already been sent")




def clean_input(input_text: str, form):

    """
    
    
    Clean input by removing punctuations

    Paramters: 
      - input_text: user's textual input or transcribed auditive input (of type str)
      - form: string with selected type of input

    Returns: transformed string 
    
    
    """
      
    # Interal procedure (for debugging purposes)  
    print("\n\nSTART INPUT TRANSFORMATION...")
    print("-"*60)
    print(f"Original input is '{input_text}' of type '{form}'")

    transformed_transcription = input_text.lower().replace(",", "").replace(".","").replace("!","").replace("?","").replace(";","") # Removing punctuations characters
    print(f"Arduino is receiving '{transformed_transcription}'\n")

    return transformed_transcription



def transcribe(audio):
    
    """
    
    Speech to text transcription

    Parameters: 
      - audio: audio file 

    Returns: str (with transcribed content)

    
    """  

    sr, y = audio
    y = y.astype(np.float32)  # Standard format for audio processing
    y /= np.max(np.abs(y))  # Normalizing data for range [-1, 1]

    # Calling the trancriber function
    transcribed_audio = transcriber({"sampling_rate": sr, "raw": y})["text"]

    return transcribed_audio



def process_input(audio, text):
    
    """
    
    Directs the input (either textual or auditive) to the corresponding functions for their textual representation

    Parameters: 
      audio: state of audio variable from gradio
      text: state of text variable from gradio

    Returns: str (with transcribed content)
    
    """

    if audio is not None: # If text and audio, will prioritize audio
        transcribed_audio = transcribe(audio) # Transcribing the function
        transformed_text = send_input(transcribed_audio, "spoken") # Adjusting input from transcribed audio
    elif text.strip() != "":
        transformed_text = send_input(text, "textual") # Adjusting input from user text 
    else: 
        transformed_text = "No valid input provided."

    return transformed_text
