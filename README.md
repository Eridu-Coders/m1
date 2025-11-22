![](https://github.com/Eridu-Coders/m1/blob/main/img/ShankhaChakra.png?raw=true)
# Introduction

This project, which is still unnamed, aims to create a software suite to help the Vaishnava scholar write _Bhashyas_ (commentaries) on ancient texts. Specifically, the goal is to preserve the links inherent in the quotations within each _Bhashya_ while retaining the ease of use of WYSIWIG tools like Microsoft Word and other wordprocessors.

At its core is a graph database (NOSQL) which is a natural fit for the graph of quotations inherent in all _Bhashyas_ taken as a whole. The interface gives access to the texts both in a graph-structured way (left-hand panel on the screenshot below) and in a WYSIWIG fashion (right-hand panel).

![](https://github.com/Eridu-Coders/m1/blob/main/img/m1_Screenshot_01.png?raw=true)

# Naming

For the moment, the software goes under the temporary moniker "m1". In due course, a suitable name should be chosen.

# Installation

## Prerequisites

Linux is strongly recommended as a development platform for m1; Specifically [Ubuntu](https://en.wikipedia.org/wiki/Ubuntu) or its derivatives ([mint](https://en.wikipedia.org/wiki/Linux_Mint), etc.)

### Lmdb

[Lmdb](https://en.wikipedia.org/wiki/Lightning_Memory-Mapped_Database) is a widely used key/value dabase library with C++ bindings. On Ubuntu, and Ubuntu-derived, Linux distributions, Lmdb installation is done with:
```
sudo apt install liblmdb-dev
``` 

m1 uses Lmdb to store ancillary data and strings. Its main storage space uses the same memory-mapped OS feature as Lmdb, but not Lmdb itself.

### Boost

[Boost](https://en.wikipedia.org/wiki/Boost_(C%2B%2B_libraries)) is a widely used general-purpose set of C++ libraries. On Ubuntu, and Ubuntu-derived, Linux distributions, Boost installation is done with:
```
sudo apt install libboost-all-dev
```

### Qt

[Qt](https://en.wikipedia.org/wiki/Qt_(software)) is a widely used GUI library for C++. In fact it is more than just a GUI toolkit and includes an entire development environment complete with several IDE interfaces and even extensions to the C++ language itself (the so-called "[signals and slots](https://en.wikipedia.org/wiki/Signals_and_slots)").

Qt has both a paid and an open-source version. The latter can be downloaded from [the company's website](https://www.qt.io/download-open-source).

**CAUTION**: Qt's regional mirrors are not always reliable (especially in India), and one of the better ones, from the US or Europe, should sometimes be manually chosen (as a CMD line option passed to the installer). See [this page](https://download.qt.io/static/mirrorlist/mirmon/) or [this one](https://download.qt.io/static/mirrorlist/) for a list of Qt mirrors.

## Source Download

As usual with any GitHub repo: 
```
git clone https://github.com/Eridu-Coders/m1.git
```

## m1 Compilation

Load the project (the `CMakeLists.txt` file) into the _Qt Creator_ IDE and launch the compilation from there, in Debug (many runtime messages) or Release mode (faster). In Release mode, the option `-DQT_NO_DEBUG_OUTPUT` in `CMAKE_CXX_FLAGS` should be added to the options list in advanced mode ("Projects" tab of the IDE), to disable the runtime messages.

![](https://github.com/Eridu-Coders/m1/blob/main/img/m1_Screenshot_02.png?raw=true)

Also, make sure that the build directories are at the same level as `m1_src`, so that the executables can access the icon `svg` files within that directory.

Launch the application with the command (in the Debug or Release build directory):
```
./m1 -rpg
```
This will load the sample data from the _Baghad Gita_ and Plato's _Republic_. Afterwards, the application can be re-launched without the load process (and therefore without erasing your data) with:
```
./m1
```

# Documentation

To generate the documentation with [Doxygen](https://en.wikipedia.org/wiki/Doxygen), make sure it is installed (`sudo apt install doxygen`), and then run:
```
doxygen Doxyfile
```
from the project Root directory. The creates the directory `Doc`. Load the file `Doc/html/index.htm` into your browser to view the documentation.


![](https://github.com/Eridu-Coders/m1/blob/main/ShankhaChakra.png?raw=true)