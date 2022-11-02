# Table of contents
1. [About](#about-sec)
2. [Disclaimer](#disclaimer-sec)

<a id="about-sec"><a/>

## About
Simple GUI program to select specific files and use them as arguments of a shell command. Useful if you need to run, say, various test-suites through the command line with things like [Mocha](https://mochajs.org/).

After encountering some issues and limitations using the GTK+ framework for my project [file-parameter-selector](https://github.com/arturfabriciohahaedgy/file-parameter-selector.git), I've decided to rewrite it using [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear.git) and [GLFW](https://www.glfw.org/), thus the "REDUX" in the name.

<a id="disclaimer-sec"><a/>

## Disclaimer
This project uses the single-header libraries [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear.git) and [stb_image](https://github.com/nothings/stb.git) along with the [TinyFileDialogs](https://sourceforge.net/projects/tinyfiledialogs/) library, therefore the credits and wrights to the respective part of the code which belong to their works (those being, the files included in the `lib/` directory and the file `tinyfiledialogs.c` in the `src/` directory) are given to their creators.
