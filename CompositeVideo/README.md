# Compositive Video for the Atari 2600

This is a quick non-FPGA project to get a slightly better picture from the Atari 2600.
It tries to bypass the analog sound and video processing by directly tapping
into the digital output signals of the TIA.
The digital lines are feed trough an LVC245 buffer to get rid of all the noise and then
they are directly combined with resistors to form the composite video signal.
