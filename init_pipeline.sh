#!/bin/bash
dir=akame-ga-kiru

x="ls $PWD/$dir -1q $dir*.jpg | wc -l"
eval $x

download(){
mangareader-dl --force $dir -do $dir 
}

rename(){
suffix=.cbz
for i in $dir/*cbz; do
        root=${i//$suffix/}
        mv "$PWD/$i" "$PWD/$root.zip"
done
}

extract(){
suffix=.zip
for i in $dir/*zip; do
        root=${i//$suffix/}
        unzip "$PWD/$i" -d "$PWD/$root"
        rm "$PWD/$i"
done;
}

run_cpp(){
"make"
"./detectLetters"
}

run_tesseract(){
for chapter in $dir/*; do
        root=${chapter//$dir-}
        root=${root//$dir"/"}
        for page in $chapter/*.jpg; do
                tesseract --psm 4 $page - >> "$dir/$root.txt" 
                done;
done;
}

#download
#rename
#extract
#run_cpp
run_tesseract
