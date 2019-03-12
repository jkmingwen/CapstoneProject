# Implementation of a portable video synthesizer
## MCS Capstone 2018/2019

There are several video synthesizers made specifically for music visualisation that are designed to run off a computer, however, such an implementation makes them cumbersome for use in a live context --- space on a stage can be sparse, and having to bring a computer to performances is inconvenient, especially if that means having to set up another piece of equipment. The solution to this is to have a video synthesizer that is implemented in a more portable enclosure. These do exist, nonetheless, most portable video synthesizers offer only simple graphics.

This project addresses the shortcomings of the current range of video synthesizers by taking advantage of the increasingly powerful processing capabilities of single-board computers (SBCs). Its aim is to design and implement a portable video synthesizer in openFrameworks that takes in an audio signal as input and --- using information extracted from the audio signal --- produces and manipulates a visual output in real-time. The goal is to design and implement a video synthesizer that will (1) produce complex and aesthetically pleasing visuals that react in real-time to a musician's playing and (2) integrate smoothly into a musician's live setup.

This repository serves as a means to record the progress of implementing the various animations that will be used in the video synthesizer. It also contains the .tex files that make up the capstone report on this project.
