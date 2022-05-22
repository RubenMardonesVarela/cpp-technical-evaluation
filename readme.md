# AllRead C++ Evaluation

## Introduction

Allread app evaluation. This app gets the rtsp stream from some cameras (emulated with docker containers), and start/stop recording based on motion detection. The motion detection is all mocked. GStreamer is used to get the frames from rtsp streams, and also for recording, taking advantage of its multi threading implementation.

## Pre-requisites

* Install Docker CE (follow [Installation Guide](https://docs.docker.com/engine/install/ubuntu/)).
* Make sure about [post installation step for Docker](https://docs.docker.com/engine/install/linux-postinstall/).
* Install docker-compose (follow [installation guide](https://docs.docker.com/compose/install/#install-compose-on-linux-systems)).
* Install gstreamer (follow[installation guide](https://gstreamer.freedesktop.org/documentation/installing/on-linux.html?gi-language=c)).
* Download video from [s3://allread-public-technical-evaluation/train-example.mp4](s3://allread-public-technical-evaluation/train-example.mp4)


## Build

To build, it is necessary to install CMake and a C++ compiler that supports at least standard C++17, otherwise the software cannot be compiled. 

First of all, it is necessary to initialize the git submodules before starting the build process. There are two third party repositories used for different purposes: 

* Googletest : https://github.com/google/googletest.git (used for unit testing).
* spdlog: https://github.com/gabime/spdlog.git (used for logging).

To initialize git submodules:

```
git submodule update --init --recursive

```

Then, you can build with cmake:

```
mkdir build
cd build
cmake .. 
cmake --build . --config Release -j4
```

As the result of the build, two exec files are generated in folder build/out:

* allreadevaluation : console app to be used in emulated environment. 
* allreadevaluation_test: app to run the unit tests.

## Launch unit tests

```
cd build/out
./allreadevaluation_test
```
There are only 3 unit tests so far covering only one class. More unit tests must be written. 

## Launch application in emulated environment

1. Download from s3 the video and write in ./video.
2. Start docker compose in docker folder
	```bash
	$ docker-compose -f ./docker/docker-compose.yml up
	```
3. Launch console app:
	```
	cd build/out
	./allreadevaluation
	
	```
	You can see in console log traces to track what is happening. Due to the implementation of the mock motion detector, the app starts recording from each stream every 20 seconds. The files with records are stored in folders cam01 and cam02, and every log trace that is shown on the console is also write into a log file in folder log. 
	You can exit the console app whenever you want typing "exit" and pressing enter. 
4. Stop docker compose
	 ```bash
	 $ docker-compose -f ./docker/docker-compose.yml down
	 ```

## Next steps

There is still some work to do, which has not been carried out due to lack of time. 

* Implement timer to wait for configurable amount of time to stop recording after last no motion event received.
* Write more unit tests. There is only one class that is covered under unit testing so far.
* Write end to end tests. 
* Refactor. It is convenient to decouple some classes and use interfaces instead, injecting dependencies between classes, so that the software is more maintenable and open to future modifications. 