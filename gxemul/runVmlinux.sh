#!/bin/bash
#gxemul -E testmips -C R3000 -M 64 ./vmlinux
gxemul -E testmips -C R3000 -M 64 -d ./fs.img ./vmlinux