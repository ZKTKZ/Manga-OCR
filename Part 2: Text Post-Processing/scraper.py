import requests
from lxml import html

def main():
        for chapter in range(2): 
                url = "https://dragonballuniverse.fandom.com/wiki/Chapter_" + str(chapter)
                page = requests.get(url, headers={"content-type":"text"})       
                tree = html.fromstring(page.content)
                paras = tree.xpath('//div[@class="WikiaArticle"]//p//text()')
                text = ' '
                if "What do you want to do?" in text[0]:
                        break
                else:        
                        for para in paras:
                                text += para
                        print(text) 

if __name__ == "__main__":
        main()

