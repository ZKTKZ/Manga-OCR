import cv2
import re
import enchant
import hunspell


def loadDictionary():
        dictionaryFile = open('dictionary.txt')
        englishWords = {}
        
        read = dictionaryFile.read()
        for word in read.split('\n'):
                englishWords[word] = word 
                dictionaryFile.close()
        return englishWords

def remove_special_characters(text, remove_digits=False):
        pattern = r'[^a-zA-Z0-9!,.?\-;:\'()\n]' if not remove_digits else r'[^a-zA-Z!,./?\-;:\'()\n]'
        text = re.sub(pattern, '', text)
        return text

def remove_noise(text):
        pattern = r'^[\d\D]\s+[\d\D]{1,2}'
        matches = re.finditer(pattern, text, flags = re.MULTILINE) 

        pattern = re.compile(pattern, re.MULTILINE)
        text = re.sub(pattern, '', text)
        return text 
        # re.sub(pattern, '', text) 

def textSegment(text):
        ENGLISH_WORDS = loadDictionary()
        spaces = []
        for i, char in enumerate(text):
                if char == ' ':
                        spaces.append(i)
                        maybeWord = ''
                        length = len(spaces)
                        if length>1:
                                start = spaces[length-2]
                        else:
                                start = 0
                        for j in range(start, spaces[length-1]):
                                maybeWord += text[j]
                        if maybeWord in ENGLISH_WORDS:
                                print(maybeWord)
        return text
        

def main():
        k1 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5,15))
        k2 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (3, 15))
        '''
        print("Ellipse (5, 15): \n",k1)
        print("Ellipse (3, 15): \n",k2)
        '''

        file = open("dragon-ball-195_MORPH_GRADIENT.txt", "r+")
        text = file.read()
        text = remove_special_characters(text,remove_digits=False) 
        '''
        question_marks = ([pos for pos, char in enumerate(text) if char == '?'])
        print(len(question_marks))
        '''
        print(text)
        file.close()

        #text = textSegment(text)
        #text = re.sub('\n(?!.)', '', text)

if __name__ == "__main__":
        main()
