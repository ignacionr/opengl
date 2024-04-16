FROM ubuntu:23.04

# Avoid warnings by switching to noninteractive

ENV DEBIAN_FRONTEND=noninteractive

# Configure apt and install packages

RUN apt-get update \
    && apt-get -y install gcc-13 g++-13 gdb valgrind cmake make git

# install opengl libraries
RUN apt-get -y install libgl1-mesa-dev libglu1-mesa-dev

# install glfw, glad and glm
RUN apt-get -y install libglfw3-dev libglm-dev

# install the ffmpg libraries
RUN apt-get -y install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev

# link the gcc and g++ to the version 13
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 100 --slave /usr/bin/g++ g++ /usr/bin/g++-13

RUN apt-get install -y libglew-dev libglfw3-dev libglm-dev

RUN apt-get install -y ffmpeg
