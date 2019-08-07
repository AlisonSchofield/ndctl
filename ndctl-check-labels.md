---
title: ndctl
layout: pmdk
---

NAME
====

ndctl-check-labels - determine if the given dimms have a valid namespace
index block

SYNOPSIS
========

>     ndctl check-labels <nmem0> [<nmem1>..<nmemN>] [<options>]

DESCRIPTION
===========

The namespace label area is a small persistent partition of capacity
available on some NVDIMM devices. The label area is used to resolve
aliasing between *pmem* and *blk* capacity by delineating namespace
boundaries. In addition to checking if a label area has a valid index
block, running this command in verbose mode reports the reason the index
block is deemed invalid.

OPTIONS
=======

`<memory device(s)>`  
One or more *nmemX* device names. The keyword *all* can be specified to
operate on every dimm in the system, optionally filtered by bus id (see
--bus= option).

`-s; --size=`  
Limit the operation to the given number of bytes. A size of 0 indicates
to operate over the entire label capacity.

`-O; --offset=`  
Begin the operation at the given offset into the label area.

`-b; --bus=`  
Limit operation to memory devices (dimms) that are on the given bus.
Where *bus* can be a provider name or a bus id number.

`-v`  
Turn on verbose debug messages in the library (if ndctl was built with
logging and debug enabled).

COPYRIGHT
=========

Copyright (c) 2016 - 2019, Intel Corporation. License GPLv2: GNU GPL
version 2 <http://gnu.org/licenses/gpl.html>. This is free software: you
are free to change and redistribute it. There is NO WARRANTY, to the
extent permitted by law.

SEE ALSO
========

[UEFI NVDIMM Label
Protocol](http://www.uefi.org/sites/default/files/resources/UEFI_Spec_2_7.pdf)
