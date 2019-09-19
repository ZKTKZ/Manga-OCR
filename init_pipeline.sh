#!/bin/bash
dir=manga/dragon-ball

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

#download
#rename
#extract
#run_cpp
