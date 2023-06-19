# mandelbrot
I read a great article about the mandelbrot set, how it is calculated and for what it is used for. If you're interested in such things too, i highly recommmend reading the articles too:

* https://web.archive.org/web/20230328052353/https://plus.maths.org/content/what-mandelbrot-set
* https://web.archive.org/web/20230525074817/https://plus.maths.org/content/unveiling-mandelbrot-set
* https://web.archive.org/web/20220929135724/https://plus.maths.org/content/computing-mandelbrot-set

During the read, i thought it would be a fun exercise to just implement it from scratch.

## How to build
It's written in plain C89, so it should - at least in theory - compile on almost every potato. Tested it with GCC and Visual C++.

*GCC:*
```
cc -std=c89 mandelbrot.c -o mandelbrot
```

*VC:*
```
cl mandelbrot.c
```

## What does it look like?
```
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
..................................................:.................................................
...............................................:....................................................
.............................................    *..................................................
.............................................     ..................................................
....................................*  .:             ...:.:........................................
.....................................                     :.........................................
..................................:                        .........................................
.....................*:..:.......                           ........................................
.....................         ..                            :.......................................
...................:           :                            ........................................
:****@ *@@@                                              ...........................................
...................:           :                            ........................................
.....................         ..                            :.......................................
.....................*:..:.......                           ........................................
..................................:                        .........................................
.....................................                     :.........................................
....................................*  .:             ...:.:........................................
.............................................     ..................................................
.............................................    *..................................................
...............................................:....................................................
..................................................:.................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
....................................................................................................
```
