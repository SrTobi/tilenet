# This page discribes how to build tilenet

## Preconditions ##

To build tilenet you need

  * A compiler supporting c++11
  * boost
  * SFML 2.0

# Get and build #

To get and build tilenet use

```
git clone https://Code.DataByte@code.google.com/p/tilenet/
cd tilenet
git submodule init
git submodule update
mkdir build
cd build
cmake ..
make
```