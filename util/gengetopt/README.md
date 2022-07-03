# README 

## What is this repository for?

This repository features the PRS project.

## What is this directory for?

This directory features the `gengetopt` script used for
generating the `maputil` program command line parser. 

## Prerequisites

Install the following prerequisites:

* [GNU Gengetopt](https://www.gnu.org/software/gengetopt/)

```
sudo apt-get install gengetopt
```

## How do I get setup?

### Generate the command line parser

Run the following command to generate the command line parser:

```
gengetopt --input=arguments.ggo --include-getopt
gengetopt --input=objectproperties.ggo --include-getopt --string-parser --func-name=cmdline_parser_object_properties --arg-struct=gengetopt_args_info_object_properties --file-name=cmdlineobjectproperties
```

The above commands will produce the `cmdline` and `cmdlineobjectproperties` 
headers and source files.

