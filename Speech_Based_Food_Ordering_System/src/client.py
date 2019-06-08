from __future__ import unicode_literals, print_function

import speech_recognition as sr
import plac 
import random
from pathlib import Path
import spacy
from spacy.util import minibatch, compounding
from gtts import gTTS
import pyglet
import time, os
from pymongo import MongoClient
from server import train_model

@plac.annotations(
    model=("Model name. Defaults to blank 'en' model.", "option", "m", str),
    output_dir=("Optional output directory", "option", "o", Path),
    n_iter=("Number of training iterations", "option", "n", int),
)
def main(model=".\model", output_dir=None, n_iter=100):
    """Load the model, set up the pipeline and train the entity recognizer."""
    if model is not None:
        nlp = spacy.load(model)  # load existing spaCy model
        print("Loaded model '%s'" % model)
    else:
        nlp = spacy.blank("en")  # create blank Language class
        print("Created blank 'en' model")

    pizza  = ""
    pizza_type = ""
    # test the trained model
    doc = nlp(text)
    for ent in doc.ents:
        if (ent.label_ == "Pizza"):
            pizza = ent.text
        elif (ent.label_ == "Pizza Type"):
            pizza_type = ent.text
    Print_Lines()
    print(doc)
    print("Entities", [(ent.text,ent.start_char,ent.end_char,ent.label_) for ent in doc.ents])
    print("Tokens", [(t.text, t.ent_type_, t.ent_iob) for t in doc])
    return((doc),(pizza_type,pizza))

def Print_Lines():
    print("=" * 50)

def speech_to_text():
    r = sr.Recognizer()
    r.energy_threshold = 4000
    with sr.Microphone() as source:
        print ('Say Something!')
        audio = r.listen(source)
        print ('Done!')
        
    text = r.recognize_google(audio)
    print (text)
    return(text)

def text_to_speech(text, lang = 'en'):
    file = gTTS(text = text, lang = lang)
    filename = '/tmp/temp.mp3'
    file.save(filename)

    music = pyglet.media.load(filename, streaming = False)
    music.play()

    time.sleep(music.duration)
    os.remove(filename)

def update_database(doc):
    con = MongoClient()
    db = con.traindata_database
    data = db.train_data

    temp = str()
    for t in doc:
        temp = temp + " " + t.text
    temp = temp[1:len(temp)]
    search_result = data.find_one({'Train_Data' : temp})
    if(type(search_result) is type(None)):
        ent_start_list = list()
        ent_end_list = list()
        ent_ent_list = list()
        temp_list = list()
        temp_dict = dict()
        temp_tuple = list()
        for ents in doc.ents:
            ent_start_list.append(ents.start_char)
            ent_end_list.append(ents.end_char)
            ent_ent_list.append(ents.label_)
            temp_tuple.append(ents.start_char)
            temp_tuple.append(ents.end_char)
            temp_tuple.append(ents.label_)
            temp_tuple = tuple(temp_tuple)
            temp_list.append(temp_tuple)
            temp_tuple = list()
        temp_dict["entities"] = temp_list
        temp_tuple.append(temp)#for training data
        temp_tuple.append(temp_dict)#for training data
        ent_start_tuple = tuple(ent_start_list)
        ent_end_tuple = tuple(ent_end_list)
        ent_ent_tuple = tuple(ent_ent_list)
        Print_Lines()
        temp_tuple = tuple(temp_tuple)
        temp_list = list()
        temp_list.append(temp_tuple)
        print(temp_list)
        Print_Lines()
        print("before train_model()")
        train_model(model="./model", output_dir="./model", n_iter=100, train_data = temp_list)
        print("after train_model()")
        data.insert({'Train_Data' : temp, 'Start_Char' : ent_start_tuple, 'End_Char' : ent_end_tuple, 'Entity' : ent_ent_tuple})
        
        item = data.find()
        for i in item:
            print(i)
    
if __name__ == "__main__":
    flag = False
    while(flag != True):
        text = "What would you like to order"
        text_to_speech(text)
        text = speech_to_text()
        #text = ("I want to order a Deluxe Veggie Pizza")
        data = plac.call(main)
        #print(a)
        text = ("Your order for " + data[1][0] + " " + data[1][1] + " is placed")
        print(text)
        print("Doc Type", type(data[0]))
        text_to_speech(text)
        Print_Lines()
        text = ("Is your order placed correctly")
        print(text)
        text_to_speech(text)
        text = speech_to_text()
        if(text != "yes"):
            continue
        flag = True
        doc = data[0]
        update_database(doc)
        
