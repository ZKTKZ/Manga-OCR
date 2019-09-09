import cv2
import numpy as np;

import os
import sys
from swt.SWTScrubber import SWTScrubber 

def blob_detect(file_path):
        im = cv2.imread(file_path, cv2.IMREAD_GRAYSCALE)

        params = cv2.SimpleBlobDetector_Params()
        
        params.filterByColor = 0
        #params.blobColor = 255
        
        #params.minThreshold = 0.1#240
        #params.maxThreshold = 255

        params.filterByArea = 1
        params.minArea = 1000#2500
        params.maxArea = 100000

        params.filterByConvexity = 0
        #params.minConvexity = 0.1
        #params.maxConvexity = 1
        
        params.filterByCircularity = 0
        #params.minCircularity = 0.1
        #params.maxCircularity = 1
        
        params.filterByInertia = 0
        
        #params.minDistBetweenBlobs = 15

        detector = cv2.SimpleBlobDetector_create(params)
        keypoints = detector.detect(im)
        
        #for kp in keypoints:
        #       print(kp.pt[0], ' ', kp.pt[1], end = ' ')

        im_with_keypoints = cv2.drawKeypoints(im, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
        cv2.imshow("Keypoints", im_with_keypoints)
        cv2.waitKey(0)

def mser_detect(file_path):
        mser = cv2.MSER_create()
        im = cv2.imread(file_path)#, cv2.IMREAD_GRAYSCALE)
        im = cv2.resize(im, (im.shape[1]*2, im.shape[0]*2))

        vis = im.copy()
        gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
        regions = mser.detectRegions(gray)
        hulls = [cv2.convexHull(p.reshape(-1, 1, 2)) for p in regions[0]]
        cv2.polylines(vis, hulls, 1, (0,255,0))
        #print(regions, end = ' ')
        mask = np.zeros((im.shape[0], im.shape[1], 1), dtype=np.uint8)
        for contour in hulls:
            cv2.drawContours(mask, [contour], -1, (255, 255, 255), -1)

        text_only = cv2.bitwise_and(im, im, mask=mask)
        print (text_only)
        cv2.imshow('img', vis)
        cv2.waitKey(0)

def sw_transform(file_path):
        SWT = SWTScrubber()
        final_mask = SWTScrubber.scrub(file_path)
        #cv2.imshow('final.jpg', final_mask * 255)        

def main():
        directory_in_str = os.getcwd() + "/manga/dragon-ball/dragon-ball-195/"
        directory = os.fsencode(directory_in_str)
        for file in os.listdir(directory):
                file_name = os.fsdecode(file)
                if file_name.endswith(".jpg"): 
                        file_path = os.path.join(directory_in_str, file_name)
                        #print(file_path)
                        blob_detect(file_path)
                        #sw_transform(file_path)
                        #mser_detect(file_path)

if __name__ == "__main__":
        main()

