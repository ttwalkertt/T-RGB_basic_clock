# Arduino IDE project skeleton

If you like Arduino, but also like to use your text editor of choice and you're not afraid of the command line, this project is for you! I'm using VSCode and PlatformIO.

# Dependencies

* VSCode and PlatformIO 
* bash or zsh
* git         (optional)

# Usage

To get started clone this repo or download zip and extract on your local machine. To clone using git:

```shell
$ git clone https://github.com/bremme/arduino-project-skeleton
```


# Structure

The structure of your new Arduino project is as follows:

```shell
.
├── doc                 Aditional documentation file
├── libraries           Arduino library files
├── src                 CPP & C
├── include             Header files
├── LICENCE.txt         Licence file
├── config.sh           Arduino config file
└── README.md           Project README file
```

You (main) sketch goes in `src`. This is where `void setup()` and `void loop()` live. Your libraries (dependencies) go into the libraries folder. You can ether copy your libraries here or symlink them. If you have additional documentation, like datasheets for example, they go into the `doc` folder.
