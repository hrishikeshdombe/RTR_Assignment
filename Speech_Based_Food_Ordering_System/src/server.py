from __future__ import unicode_literals, print_function

import plac 
import random
from pathlib import Path
import spacy
from spacy.util import minibatch, compounding
from pymongo import MongoClient

TRAIN_DATA = list()

@plac.annotations(
    model=("Model name. Defaults to blank 'en' model.", "option", "m", str),
    output_dir=("Optional output directory", "option", "o", Path),
    n_iter=("Number of training iterations", "option", "n", int),
)
def train_model(model=None, output_dir="./model", n_iter=100, train_data = TRAIN_DATA):
    """Load the model, set up the pipeline and train the entity recognizer."""
    if model is not None:
        nlp = spacy.load(model)  # load existing spaCy model
        print("Loaded model '%s'" % model)
    else:
        nlp = spacy.blank("en")  # create blank Language class
        print("Created blank 'en' model")

    # create the built-in pipeline components and add them to the pipeline
    # nlp.create_pipe works for built-ins that are registered with spaCy
    if "ner" not in nlp.pipe_names:
        ner = nlp.create_pipe("ner")
        nlp.add_pipe(ner, last=True)
    # otherwise, get it so we can add labels
    else:
        ner = nlp.get_pipe("ner")

    # add labels
    for _, annotations in train_data:
        for ent in annotations.get("entities"):
            ner.add_label(ent[2])

    # get names of other pipes to disable them during training
    other_pipes = [pipe for pipe in nlp.pipe_names if pipe != "ner"]
    with nlp.disable_pipes(*other_pipes):  # only train NER
        # reset and initialize the weights randomly – but only if we're
        # training a new model
        if model is None:
            nlp.begin_training()
        for itn in range(n_iter):
            random.shuffle(train_data)
            losses = {}
            # batch up the examples using spaCy's minibatch
            batches = minibatch(train_data, size=compounding(4.0, 32.0, 1.001))
            for batch in batches:
                #print(batch)
                texts, annotations = zip(*batch)
                #print("Texts :",texts)
                #print("annotations :",annotations)
                nlp.update(
                    texts,  # batch of texts
                    annotations,  # batch of annotations
                    drop=0.5,  # dropout - make it harder to memorise data
                    losses=losses,
                )
            #print("Losses", losses)

    print("*" * 50)
    if output_dir is not None:
        output_dir = Path(output_dir)
        if not output_dir.exists():
            output_dir.mkdir()
        nlp.to_disk(output_dir)
        print("Saved model to", output_dir)

        print("Loading from", output_dir)
        nlp2 = spacy.load(output_dir)
        for text, _ in train_data:
            doc = nlp2(text)
            print("Entities", [(ent.text, ent.label_) for ent in doc.ents])
            print("Tokens", [(t.text, t.ent_type_, t.ent_iob) for t in doc])

def init_database():
    con = MongoClient()
    db = con.traindata_database
    data = db.train_data
    return(data)

def extract_train_data(data):
    item = data.find()

    for i in item:
        if i is not None:
            print("*********************")
            try:
                length = len(i['Start_Char'])
            except TypeError:
                length = 1
                print(length)
            l = list()
            if length is 1:
                t = (i['Start_Char'],i['End_Char'],i['Entity'])
                l.append(t) 
            else:
                for j in range(length):
                    t = (i['Start_Char'][j],i['End_Char'][j],i['Entity'][j])
                    l.append(t)
            #print(l)
            #print(length)
            d = dict()
            d["entities"] = l
            l = [i['Train_Data'],d]
            t = tuple(l)
            TRAIN_DATA.append(t)
            print(TRAIN_DATA)

if __name__ == "__main__":
    data = init_database()
    extract_train_data(data)
    plac.call(train_model)


