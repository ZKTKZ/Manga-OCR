# Manga-OCR [WIP]

##Introduction
Text detection and reconstruction framework, utilizing OpenCV to detect bounding boxes, applying Tesseract to detected regions, conducting post-processingon obtained text via an assortment of Python libraries, and easily scalable by included Bash script. 

Note that the code and folder hierarchy is yet to be designed for public use, and may suffer from trivial issues.

##Setup on your machine [WIP]:

1. Install CMake, and copy the CMakeLists.txt file. If writing your own, make sure to link ${OpenCV_libs} and stdc++fs
2. Install manga-readerdl (https://github.com/jneidel/mangareader-dl)
3. Open the ```init_pipeline.sh``` script, and uncomment the commands

To-do:
- run unit tests to check for edge cases
- modify code to be generalizable to any directory hierarchy
- add python OCR-post processing to Bash pipeline

##Log 
- Currently working on configuring custom Named Entity recognition for text segmentation.

