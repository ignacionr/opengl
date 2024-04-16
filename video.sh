#!/bin/bash

g++ -o myprogram src/main.cpp -lglfw -lGLEW -lGL -ldl -std=c++20
rm output_video.mp4
./myprogram | ffmpeg -f image2pipe -framerate 50 -i - -c:v libx264 -preset fast -r 50 -pix_fmt yuv420p output_video.mp4
