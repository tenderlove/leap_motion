# -*- coding: us-ascii -*-

require 'mkmf'

# :stopdoc:


dir_config 'libLeap'

$libs = append_library $libs, 'Leap'

create_makefile 'leap_motion'
