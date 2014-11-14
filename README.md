im_sort
=======

in memory sort replacement for sort command in linux systems

requirements
-----------
boost_program_options

build
-----
scons is needed, so if it is installed run
> scons

after build process was successfully you can use im_sort, e.g.

>./im_sort -h

    Allowed options:
     -h [ --help ]         produce help message
     -f [ --file ] arg     input file
     -o [ --output ] arg   output file

im_sort defaults behavior is to read from stdin and do the output to stdout.
