# jsonld-cpp
JSON-LD Implementation for C++

This is a C++ implementation of JSON-LD (http://json-ld.org)

Development is still in progress--currently it only supports the
expand(), toRdf() and normalize() functions.

jsonld-cpp uses [http-link-header-cpp](https://github.com/danpape/http-link-header-cpp.git)
and includes it as a submodule.  When cloning, use ```git clone --recursive```
to include the submodule.  See repo documentation for dependencies.

## Building jsonld-cpp

To build jsonld-cpp, you will need:

* cmake
* g++, clang or Visual Studio (community edition)

jsonld-cpp uses a pretty standard cmake build system:

```
mkdir build
cd build
cmake ..
make
```

You can also run all the tests:

```
make test
```

### Installing prerequirements

If the above doesn't work, you probably need to install some
prerequirements. For example, on a fresh Debian Stretch system:

```
$ sudo apt-get install make gcc g++
```

It is worth getting the latest cmake, so you may want to install that the hard way:

```
wget https://cmake.org/files/v3.13/cmake-3.16.1.tar.gz
tar xzf cmake-3.16.1.tar.gz
cd cmake-3.16.1
./configure
make 
sudo make install
```


Now you can again try to build jsonld-cpp.

## Examples

You can find in the examples directory a simple application showing how
to interface with the library called jsonld2rdf. It can convert a
json-ld document into normalized RDF data in NQuads format. There are
also two example files showing how the normalized RDF could be hashed to
show that the two documents are equivalent:

```
$ ./build/debug/examples/jsonld2rdf examples/ex01a.jsonld | shasum -a 256
cb92d57d8b7abf09e4642338049bd9ee91a0ee0fe327e0a6061a8bbc2f4314bf  -
$ ./build/debug/examples/jsonld2rdf examples/ex01b.jsonld | shasum -a 256
cb92d57d8b7abf09e4642338049bd9ee91a0ee0fe327e0a6061a8bbc2f4314bf  -
```




