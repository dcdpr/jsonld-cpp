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

You may run into permission issues during the build when the dependencies are
being installed during the build step.  To install them in the project directory
instead run cmake with the following command:

```
cmake .. "-DCMAKE_INSTALL_PREFIX=$(pwd)/install"
```

You can also run all the tests:

```
make test
```

### Installing prerequisites

If the above doesn't work, you probably need to install some
prerequisites. For example, on a fresh Debian Bullseye system:

```
$ sudo apt-get install make cmake gcc g++ libssl-dev
```

Now you can again try to build jsonld-cpp.

## Examples

You can find in the examples directory a simple application showing how
to interface with the library called jsonld2rdf. It can convert a
json-ld document into normalized RDF data in NQuads format. There are
also two example files showing how the normalized RDF could be hashed to
show that the two documents are equivalent:

```
$ ./build/debug/jsonld-cpp-examples-prefix/src/jsonld-cpp-examples-build/jsonld2rdf examples/ex01a.jsonld | shasum -a 256
cb92d57d8b7abf09e4642338049bd9ee91a0ee0fe327e0a6061a8bbc2f4314bf  -
$ ./build/debug/jsonld-cpp-examples-prefix/src/jsonld-cpp-examples-build/jsonld2rdf examples/ex01b.jsonld | shasum -a 256
cb92d57d8b7abf09e4642338049bd9ee91a0ee0fe327e0a6061a8bbc2f4314bf  -
```


### Debugging
To include debugging symbols for gdb and compile_commands.json file, run cmake
with the following arguments:

```
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1
```

