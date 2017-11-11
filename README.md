# Ninjas - Sample Slicing Plugin
#### *Ninjas Is Not Just Another Slicer*

Creating a sample slicer plugin using DPF framework.
This is very early work.
There are **lots** of bugs and crashes, lots of things not implemented yet.


***

Build instructions:
dependencies : libg1-mesa-dev, libx11-dev, libsndfile1-dev.
If you want a standalone jack binary install either libjack-dev or libjack-jackd2-dev
+
```
git clone --recursive https://github.com/rghvdberg/ninjas.git
cd ninjas
make
```

There is no `make install`, binaries will appear in `~ninjas/bin/`, just copy the binaries to a appropiate location.

***

### Usage
+ load Ninjas in a suitable host.
+ click Diskette drive
+ load audio sample
+ set number of slices using the Slices knob.
+ Sample slices will be mapped to C4 and up.

Ninjas will look something like this.
![](https://github.com/rghvdberg/ninjas/blob/master/plugins/Ninjas/Art%20working%20directory/mockup.png) 
