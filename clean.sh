#!/bin/bash
# vim: set ts=4 sw=4 expandtab syntax=shell:
#
# Clean build process artifacts
#

# Files to remove
read -r -d '' ACFILES <<-EOF
	*.loT include
EOF

phpize --clean
[ -f Makefile ] && make clean 2>/dev/null
rm -Rfv $ACFILES 2>/dev/null

echo "** Clean complete"
