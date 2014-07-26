CedarX h264 encoding example for linux, modified from Allwinner examples.

Captures frames from /dev/video0 (tested with an uvc webcam, Logitech C901)
for 10 seconds saving the raw h264 stream to /tmp/h264.h264.

You will have to mux the raw stream to a container to add fps information
(for ex. mkvemerge -o h264.mkv h264.h264).

Known bugs: output is not perfect, probably a colorspace conversion problem
(see h264.mkv sample).

Notes: soft-float libraries (armel). Edit Makefile to match your toolchain.
Tested with kernel from [1], commit 6db4ad895c8f4b4f17dec1bf98778906945215ce.

Author: Enrico Butera <ebutera@users.sourceforge.net>

[1]: https://github.com/jwrdegoede/linux-sunxi/tree/sunxi-3.4
