# jsonld-cpp
JSON-LD Implementation for C++

This is a C++ implementation of JSON-LD 1.1 Processing Algorithms and API (http://json-ld.org)

Development is still in progress--this library currently only supports the
[```expand()```](https://www.w3.org/TR/json-ld11-api/#expansion-algorithm) and
[```toRdf()```](https://www.w3.org/TR/json-ld11-api/#deserialize-json-ld-to-rdf-algorithm)
processing algorithms.

## Implementation Notes

Some parts of the JSON-LD API spec make note of using Promises to return
API results. This C++ library does not support any Promise-like capability.

The RemoteDocument implementation currently only supports local files, not
documents from arbitrary URLs.


## Building jsonld-cpp

To build jsonld-cpp, you will need:

* cmake
* g++, clang or Visual Studio (community edition)

jsonld-cpp uses [http-link-header-cpp](https://github.com/dcdpr/http-link-header-cpp.git)
and includes it as a submodule.  When cloning, use ```git clone --recursive```
to include the submodule.

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

### Installing prerequisites

If the above doesn't work, you probably need to install some
prerequisites. For example, on a fresh Debian 12 ("bookworm") system:

```
$ sudo apt update
$ sudo apt install make cmake g++ libssl-dev
```

Now you can again try to build jsonld-cpp.

## Examples

The ```examples``` directory contains a simple application called
```jsonld2rdf``` that shows how to interface with the library. It can
convert a JSON-LD document into RDF data in NQuads format. As part of
the conversion to RDF, the JSON-LD document is first ```expanded```.

```
$ cd jsonld-cpp/build
$ ./jsonld-cpp-examples-prefix/src/jsonld-cpp-examples-build/jsonld2rdf examples/ex01a.jsonld
<http://example.com/people/dave> <http://xmlns.com/foaf/0.1/knows> <http://example.com/people/markus> .
<http://example.com/people/dave> <http://xmlns.com/foaf/0.1/name> "Dave Longley" .
<http://example.com/people/gregg> <http://xmlns.com/foaf/0.1/knows> <http://example.com/people/markus> .
<http://example.com/people/gregg> <http://xmlns.com/foaf/0.1/name> "Gregg Kellogg" .
<http://example.com/people/markus> <http://xmlns.com/foaf/0.1/name> "Markus Lanthaler" .
```

