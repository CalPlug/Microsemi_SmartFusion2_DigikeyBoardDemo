set_component SF2_MSS_sys_sb_FABOSC_0_OSC
# Microsemi Corp.
# Date: 2017-Dec-13 15:06:22
#

create_clock -ignore_errors -period 20 [ get_pins { I_RCOSC_25_50MHZ/CLKOUT } ]
