What is this?
=============

This is a simple implementation of [Langton's Ant](https://en.wikipedia.org/wiki/Langton%27s_ant).
It takes a bitstring as input and outputs an image in [Portable Greymap Format](https://en.wikipedia.org/wiki/Netpbm_format).

Compilation
===========

Should be as easy as typing ```make```.

Usage
=====

```
$ ./ant 
usage:
  ./ant 10 25
     Simulate Langton's Ant 10 on a quadratic board with size 25x25

  ./ant 10
     Same as above, but with default board size of 500
```

