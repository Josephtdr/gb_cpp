# gb_cpp

`gb_cpp` is a Gameboy emulator written in C++.

## Building

Building the emulator requires `cmake`

```sh
$ ./configure.sh
$ ./build.sh
```

## Playing

gbemu will be located in build/src/
```
run via: gbemu <path/to/rom.gb> [--traceLog] [--bootRom] [--disableAudio]
```


Keys: <kbd>&uarr;</kbd>, <kbd>&darr;</kbd>, <kbd>&larr;</kbd>, <kbd>&rarr;</kbd>, <kbd>Q</kbd>, <kbd>E</kbd>, <kbd>D</kbd>, <kbd>A</kbd>.

## Tests

The emulator passes [Blargg's](http://gbdev.gg8.se/wiki/articles/Test_ROMs) CPU instruction tests,
but fails others.

## Pressing Issues

The emulator runs at a reduced framerate, making playing difficult. 
Audio is not fully implemented.
Various GB games still have issues. 

