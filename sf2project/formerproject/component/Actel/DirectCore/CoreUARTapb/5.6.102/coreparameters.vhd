----------------------------------------------------------------------
-- Created by Microsemi SmartDesign Sun Dec 10 17:39:01 2017
-- Parameters for CoreUARTapb
----------------------------------------------------------------------


LIBRARY ieee;
   USE ieee.std_logic_1164.all;
   USE ieee.std_logic_unsigned.all;
   USE ieee.numeric_std.all;

package coreparameters is
    constant BAUD_VAL_FRCTN : integer := 0;
    constant BAUD_VAL_FRCTN_EN : integer := 0;
    constant BAUD_VALUE : integer := 1;
    constant FAMILY : integer := 19;
    constant FIXEDMODE : integer := 0;
    constant HDL_license : string( 1 to 1 ) := "U";
    constant PRG_BIT8 : integer := 0;
    constant PRG_PARITY : integer := 0;
    constant RX_FIFO : integer := 1;
    constant RX_LEGACY_MODE : integer := 1;
    constant testbench : string( 1 to 4 ) := "None";
    constant TX_FIFO : integer := 1;
    constant USE_SOFT_FIFO : integer := 1;
end coreparameters;
