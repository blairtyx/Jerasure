## Jerasure 2.0

This is revision 2.0 of Jerasure.  

This is pretty much Jerasure 1.2 without the original Galois Field backend.  

Version 2.0 links directly to GF-Complete, which is more flexible than the original, and **much** faster, because it leverages SIMD instructions.

#### Authors: 

​		James S. Plank (University of Tennessee)
​         Kevin M. Greenan (Box)


The online home for jerasure is:

  - http://jerasure.org/jerasure/jerasure


## External Documentation:

##### Manual

See the file Manual.pdf for the programmer's manual and tutorial.

##### Jerasure.org

~~See http://jerasure.org/jerasure/gf-complete for GF-Complete.~~

##### Dependency

NOTE: You must have **GF-Complete** installed (or compiled) in order to use Jerasure 2.0.

(find a mirror here https://github.com/ceph/gf-complete)


## Directory

There are two directories of source code:

The **src** directory contains the jerasure code.
The **Examples** directory contains the example programs.





------------------------------------------------------------

## Installation

##### If you do not have **Autoconf 2.65** or later installed, 

you can simply build from the tarball distribution:

​	http://www.kaymgee.com/Kevin_Greenan/Software_files/jerasure.tar.gz

##### Installing if you are allowed to install GF-Complete on your machine:

(You can skip the autoreconf step if you're using a tarball distribution.)

1.) Install GF-Complete
2.) autoreconf --force --install (*skip* if you are building from tarball)
3.) ./configure
4.) make
5.) sudo make install

This will install 

- the library into your machine's lib directory,
- the headers into include, 
- and the example programs into bin.

The configuration process assumes shared objects are searched for in
/usr/local/lib. If this is not the case on your system, you can specify a
search path at configuration time. For example:
  ./configure LD_LIBRARY_PATH=/usr/local/lib



##### Installing if you can compile GF-Complete, but you cannot install it:

1.) Install GF-Complete.  Let's suppose the full path to GF-Complete is
    in the environment variable  GFP
2A.) On Linux, set the environment variable LD_LIBRARY_PATH so that it
     includes \$GFP/src/.libs
2B.) On a mac, set the environment variable DYLD_LIBRARY_PATH so that it
     includes ​\$GFP/src/.libs
2.) ./configure LDFLAGS=-L​\$GFP/src/.libs/ CPPFLAGS=-I​\$GFP/include
3.) make

- The examples will be in the directory Examples.  
- The include files will be in the directory include, 
- and the library will be called libJerasure.a in the directory src/.libs.



## Notes

#### About Galois Fields

As long as GF-Complete is installed, Jerasure 2.0 can be used just as previous
versions.  There is no need to define custom Galois Fields.  Jerasure will
determine the default field to use, if one is not specified.

If you would like to explore a using a different Galois Field implementation,
please see the manual.

#### Testing GF-Complete

If the GF-Complete tools are installed in /usr/local/bin

  make check

If the GF-Complete tools are installed elsewhere

  make GF_COMPLETE_DIR=$(pwd)/../gf-complete/tools check

To run some tests with valgrind

  make VALGRIND='valgrind --tool=memcheck --quiet' \
       GF_COMPLETE_DIR=$(pwd)/../gf-complete/tools \
       check