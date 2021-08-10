# Write a C++ commandline application that encodes a set of WAV files to MP3

This was a takehome excercise from a job interview.

## Requirements:

- call application with path to folder as argument
- use all cpu cores
- statically link libmp3lame
- should compile and run on linux and windows
- ignore non-wav files
- portable multithread (pthread?)
- no frameworks such as boost or qt
- reasonable standard settings


## Build
- the libmp3lame.a in lib/ was compiled on linux with `./configure --disable-shared --enable-static --disable-frontend`.
- at least on linux this builds by running `cmake CMakeLists.txt` followed by `make`
- I'm not good with c++ build tools so I know this is pretty scuffed
- havent put much thought into the settings
- should have done some error checking. non wav files with a .wav ending would probably cause this to crash