# Ninjas - Sample Slicing Plugin
#### *Ninjas Is Not Just Another Slicer*

Creating a sample slicer plugin using DPF framework.
Most (all?) crashes resolved at the moment. There are still a lot of things not implemented yet though.
Look at the [issues](https://github.com/rghvdberg/ninjas/issues) and feel free to report bugs, features and so on.


***

###Build instructions

**Dependencies**

+  libgl1-mesa-dev 
+  libx11-dev
+  libsndfile1-dev
+  libsamplerate0-dev

If you want a standalone jack binary install either `libjack-dev` or `libjack-jackd2-dev`

```
git clone --recursive https://github.com/rghvdberg/ninjas.git
cd ninjas
make
```
**notice the `clone --recursive`part to pull in the DPF framework.**

There is no `make install`, binaries will appear in `~ninjas/bin/`, just copy the binaries to a appropiate location.

***

#### Usage
+ load Ninjas in a suitable host.
+ click Diskette drive
+ load audio sample
+ set number of slices using the Slices knob.
+ Sample slices will be mapped to C4 (midi note 60) and up
+ Change the pitch of slice with pitchbend
+ Each slice has it's own **ADSR** and **playmode** settings. Use the righthand **Slice Selector** to select the slice you want to edit

Ninjas looks like this:
![](https://github.com/rghvdberg/ninjas/blob/master/plugins/Ninjas/Art%20working%20directory/ninjas_current_ui.png)
