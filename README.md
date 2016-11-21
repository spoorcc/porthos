[![Build Status](https://travis-ci.org/spoorcc/porthos.svg?branch=master)](https://travis-ci.org/spoorcc/porthos)
[![Coverity Scan](https://img.shields.io/coverity/scan/3997.svg)](https://scan.coverity.com/projects/spoorcc-porthos)

Porthos
-----------------

This project tries to create an embedded linux robot system.

A high-level description of the system is given in the system_description.
Some requirements have been created, but these are still very much open to
 discussion.

The compiled documentation can be read at http://spoorcc.github.io/porthos/
And the PDF at http://spoorcc.github.io/porthos/Porthos.pdf

Compiling
---------

    mkdir bld
    cd bld
    cmake ..
    make

Generating documentation
------------------------

    cd bld
    make doc
