from itertools import groupby
from operator import itemgetter
from statistics import median,stdev,mean, StatisticsError 
from math import sqrt

from vaderSentiment.vaderSentiment import SentimentIntensityAnalyzer

import os
import re
from collections import OrderedDict

from symspellpy.symspellpy import SymSpell, Verbosity
from Levenshtein import distance as levenshtein_distance

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

def isNonAlpha(string):
        isNonAlpha = 0
        string = ''.join(string.split())
        
        for char in string:
                if char.isalpha():
                      continue 
                else:
                       isNonAlpha += 1
        return isNonAlpha

def powerOfList(elements):
        return [x**2 for x in elements]

def pre_process(file, path, corpus_txt, frequency_txt):
        ENGLISH_WORDS = loadDictionary()

        text = file.read()
        lines = []
        with open(path) as fp:
                for line in fp:
                      lines.append(line)
       
        dialogue = {}
        for i, line in enumerate(lines):
                #if line has text 
                if line.strip():
                        dialogue[i]=line
    

        max_edit_distance_dictionary = 2
        prefix_length = 7
        sym_spell = SymSpell(max_edit_distance_dictionary, prefix_length)
        dictionary_path = os.path.join(os.path.dirname(__file__),"frequency_dictionary_en_82_765.txt")


        if not sym_spell.create_dictionary("scraper.txt"):
                print("Corpus file not found")
                return

        sorted_dict = sorted(sym_spell.words.items(), key=itemgetter(1))
        sorted_dict = OrderedDict(sorted_dict)
        with open('frequency.txt', 'w+') as file:
                for key, count in reversed(sorted_dict.items()):
                        file.write(str(key) + ' ' + str(count) + '\n')

        term_index = 0
        count_index = 1
        if not sym_spell.load_dictionary(dictionary_path, term_index, count_index):
                print("Dictionary file not found")
                return
        
        #analyzer = SentimentIntensityAnalyzer()       
        for key, iter in groupby(enumerate(dialogue.keys()), lambda ix : ix[0]-ix[1]):
                line_indices = list(map(itemgetter(1), iter))
                if len(line_indices) >= 3:
                        lengths = (len(dialogue[index]) for index in line_indices)
                        if median(lengths) > 3:
                                sentence = ''
                                re_sentence = ''
                                rms = 0
                                phrase_lengths = []
                                phrase_distances = []
                                for index in line_indices:
                                        punctuation=[ch for ch in ['!', '?', '.', ',', ';', ':', '-'] if ch in dialogue[index]]
                                        #if all(word in ENGLISH_WORDS for word in __words):
                                        phrase = dialogue[index].lower()
                                        phrase = re.sub(r'^\w\s-.?!', '', phrase)
                                        if phrase.strip():
                                                segments = sym_spell.lookup_compound(phrase, 1)
                                                segments2 = sym_spell.word_segmentation(phrase)
                                                re_phrase = ''
                                                '''for segment in segments:
                                                        re_phrase += (segment.term) + ' '
                                                '''
                                                re_phrase += segments2.corrected_string
                                                sentence += phrase + ' '
                                                re_sentence += re_phrase + ' '
                                                
                                                stripped_phrase = re.sub(r'[^a-zA-Z0-9]', '', phrase) 
                                                stripped_re_phrase = re.sub(r'[^a-zA-Z0-9]', '', re_phrase) 
                                                if len(stripped_phrase) > 0 and len(stripped_re_phrase) > 0:
                                                        phrase_distance = levenshtein_distance(stripped_re_phrase, stripped_phrase)
                                                        phrase_lengths.append(len(stripped_phrase)) 
                                                        phrase_distances.append(phrase_distance) 
                                                        print(phrase, re_phrase, end = ' ')
                                                        print(phrase_distance)

                                #vs = analyzer.polarity_scores(sentence)
                                '''
                                nonAlphaCount = isNonAlpha(sentence)  
                                noise_factor = nonAlphaCount * stdev(phrase_lengths)
                                if noise_factor < 25:
                                        sentence = ''.join(sentence.split())
                                        re_sentence = ''.join(re_sentence.split())
                                        num_chars = len(sentence)
                                        edit_distance = levenshtein_distance(sentence, re_sentence) 
                                        print(sentence)
                                        print(re_sentence) 
                                        print(edit_distance)
                                        print(num_chars/10)
                                        print(edit_distance * 10 / (num_chars))
                                        '''
                                        #print(noise_factor)
                                try:
                                        rms = sqrt(mean(powerOfList(phrase_distances)))
                                        print(rms / mean(phrase_lengths))
                                except StatisticsError:
                                        print("Whoop")
        return text

def main():
        '''
        directory_in_str = os.getcwd() + "/manga/dragon-ball/"
        directory = os.fsencode(directory_in_str)
        for file in os.listdir(directory):
                file_name = os.fsdecode(file)
                if file_name.endswith(".txt"): 
                        file_path = os.path.join(directory_in_str, file_name)
                        print(file_path)
                        file = open(file_path, "r+")
                        text = pre_process(file, file_path, "scraper.txt", "corpus.txt")
        '''
        file_path = "/manga/dragon-ball/dragon-ball-196/dragon-ball-196.txt" 
        file = open(os.getcwd() + file_path, "r+")
        text = pre_process(file, os.getcwd() + file_path, "scraper.txt", "corpus.txt")

if __name__ == "__main__":
        main()
