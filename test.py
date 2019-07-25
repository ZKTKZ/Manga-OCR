from collections import Counter
from itertools import groupby
from operator import itemgetter
from statistics import median
import wordsegment
#from wordsegment import load, segment
from vaderSentiment.vaderSentiment import SentimentIntensityAnalyzer
import os

#based on inventwithpython
#but dict[index] = index
#wasn't working before iirc
def loadDictionary():
        dictionaryFile = open('google-10000-english.txt')
        englishWords = {}
        
        read = dictionaryFile.read()
        for word in read.split('\n'):
                englishWords[word] = word 
                dictionaryFile.close()
        return englishWords

def binarySearch(arr, x):
    l = 0
    r = len(arr) 
    while (l <= r): 
        m = l + ((r - l) // 2) 
  
        res = (x == arr[m]) 
  
        # Check if x is present at mid 
        if (res == 0): 
            return m - 1
  
        # If x greater, ignore left half 
        if (res > 0): 
            l = m + 1
  
        # If x is smaller, ignore right half 
        else: 
            r = m - 1
  
    return -1

def pre_process(file):
        ENGLISH_WORDS = loadDictionary()
        #wordsegment lib
        wordsegment.load()
        #wordsegment.UNIGRAMS.clear()
        wordsegment.UNIGRAMS.update(Counter("tenkaichi"))

        text = file.read()
        lines = []
        with open(os.getcwd()+"dragon-ball/dragon-ball-195_MORPH_GRADIENT-edited.txt") as fp:
                for line in fp:
                      lines.append(line)
       
        dialogue = {}
        for i, line in enumerate(lines):
                #if line has text 
                if line.strip():
                        dialogue[i]=line
       
        analyzer = SentimentIntensityAnalyzer()       
        for key, iter in groupby(enumerate(dialogue.keys()), lambda ix : ix[0]-ix[1]):
                line_indices = list(map(itemgetter(1), iter))
                if len(line_indices) > 3:
                        lengths = (len(dialogue[index]) for index in line_indices)
                        if median(lengths) > 3:
                                detected_sentence = '' 
                                for index in line_indices:
                                        detected_words = wordsegment.segment(dialogue[index])
                                        punctuation=[ch for ch in ['!', '?', '.', ',', ';', ':', '-'] if ch in dialogue[index]]
                                        for match in punctuation:
                                                detected_words.append(match)
                                        #if all(word in ENGLISH_WORDS for word in detected_words):
                                        detected_phrase=''
                                        for word in detected_words:
                                                detected_phrase += word+' '
                                        #print(detected_phrase end=' ')
                                        detected_sentence += detected_phrase
                                
                                vs = analyzer.polarity_scores(detected_sentence)
                                print(detected_sentence)#, vs)
                                #print()
        return text

def main():
        file = open("dragon-ball-195_MORPH_GRADIENT-edited.txt", "r+")
        text = pre_process(file)

if __name__ == "__main__":
        main()

