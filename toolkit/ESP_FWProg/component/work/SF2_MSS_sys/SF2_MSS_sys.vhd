----------------------------------------------------------------------
-- Created by SmartDesign Wed Dec 13 14:58:54 2017
-- Version: v11.8 SP2 11.8.2.4
----------------------------------------------------------------------

----------------------------------------------------------------------
-- Libraries
----------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;

library smartfusion2;
use smartfusion2.all;
----------------------------------------------------------------------
-- SF2_MSS_sys entity declaration
----------------------------------------------------------------------
entity SF2_MSS_sys is
    -- Port list
    port(
        -- Inputs
        DEVRST_N         : in  std_logic;
        FTDI_C_RX        : in  std_logic;
        GPIO_IN          : in  std_logic_vector(7 downto 0);
        RX               : in  std_logic;
        RX_0             : in  std_logic;
        SPI_0_CLK_F2M    : in  std_logic;
        SPI_0_DI_F2M     : in  std_logic;
        SPI_0_SS0_F2M    : in  std_logic;
        -- Outputs
        FTDI_C_TX        : out std_logic;
        GPIO_OUT         : out std_logic_vector(7 downto 0);
        PWM              : out std_logic_vector(0 to 0);
        SPI_0_CLK_M2F    : out std_logic;
        SPI_0_DO_M2F     : out std_logic;
        SPI_0_SS0_M2F    : out std_logic;
        SPI_0_SS0_M2F_OE : out std_logic;
        SPI_0_SS1_M2F    : out std_logic;
        SPI_0_SS2_M2F    : out std_logic;
        SPI_0_SS3_M2F    : out std_logic;
        SPI_0_SS4_M2F    : out std_logic;
        TX               : out std_logic;
        TX_0             : out std_logic
        );
end SF2_MSS_sys;
----------------------------------------------------------------------
-- SF2_MSS_sys architecture body
----------------------------------------------------------------------
architecture RTL of SF2_MSS_sys is
----------------------------------------------------------------------
-- Component declarations
----------------------------------------------------------------------
-- SF2_MSS_sys_sb
component SF2_MSS_sys_sb
    -- Port list
    port(
        -- Inputs
        DEVRST_N         : in  std_logic;
        FAB_RESET_N      : in  std_logic;
        FTDI_C_RX        : in  std_logic;
        GPIO_IN          : in  std_logic_vector(7 downto 0);
        RX               : in  std_logic;
        RX_0             : in  std_logic;
        SPI_0_CLK_F2M    : in  std_logic;
        SPI_0_DI_F2M     : in  std_logic;
        SPI_0_SS0_F2M    : in  std_logic;
        -- Outputs
        FAB_CCC_GL0      : out std_logic;
        FAB_CCC_LOCK     : out std_logic;
        FTDI_C_TX        : out std_logic;
        GPIO_OUT         : out std_logic_vector(7 downto 0);
        INIT_DONE        : out std_logic;
        MSS_READY        : out std_logic;
        POWER_ON_RESET_N : out std_logic;
        PWM              : out std_logic_vector(0 to 0);
        SPI_0_CLK_M2F    : out std_logic;
        SPI_0_DO_M2F     : out std_logic;
        SPI_0_SS0_M2F    : out std_logic;
        SPI_0_SS0_M2F_OE : out std_logic;
        SPI_0_SS1_M2F    : out std_logic;
        SPI_0_SS2_M2F    : out std_logic;
        SPI_0_SS3_M2F    : out std_logic;
        SPI_0_SS4_M2F    : out std_logic;
        TX               : out std_logic;
        TX_0             : out std_logic
        );
end component;
----------------------------------------------------------------------
-- Signal declarations
----------------------------------------------------------------------
signal FTDI_C_TX_net_0        : std_logic;
signal GPIO_OUT_0             : std_logic_vector(7 downto 0);
signal PWM_3                  : std_logic_vector(0 to 0);
signal SPI_0_CLK_M2F_net_0    : std_logic;
signal SPI_0_DO_M2F_net_0     : std_logic;
signal SPI_0_SS0_M2F_net_0    : std_logic;
signal SPI_0_SS0_M2F_OE_net_0 : std_logic;
signal SPI_0_SS1_M2F_net_0    : std_logic;
signal SPI_0_SS2_M2F_net_0    : std_logic;
signal SPI_0_SS3_M2F_net_0    : std_logic;
signal SPI_0_SS4_M2F_net_0    : std_logic;
signal TX_net_0               : std_logic;
signal TX_0_net_0             : std_logic;
signal SPI_0_SS1_M2F_net_1    : std_logic;
signal SPI_0_SS2_M2F_net_1    : std_logic;
signal SPI_0_SS3_M2F_net_1    : std_logic;
signal SPI_0_SS4_M2F_net_1    : std_logic;
signal SPI_0_DO_M2F_net_1     : std_logic;
signal SPI_0_CLK_M2F_net_1    : std_logic;
signal SPI_0_SS0_M2F_net_1    : std_logic;
signal SPI_0_SS0_M2F_OE_net_1 : std_logic;
signal TX_net_1               : std_logic;
signal FTDI_C_TX_net_1        : std_logic;
signal TX_0_net_1             : std_logic;
signal PWM_3_net_0            : std_logic_vector(0 to 0);
signal GPIO_OUT_0_net_0       : std_logic_vector(7 downto 0);
----------------------------------------------------------------------
-- TiedOff Signals
----------------------------------------------------------------------
signal VCC_net                : std_logic;

begin
----------------------------------------------------------------------
-- Constant assignments
----------------------------------------------------------------------
 VCC_net <= '1';
----------------------------------------------------------------------
-- Top level output port assignments
----------------------------------------------------------------------
 SPI_0_SS1_M2F_net_1    <= SPI_0_SS1_M2F_net_0;
 SPI_0_SS1_M2F          <= SPI_0_SS1_M2F_net_1;
 SPI_0_SS2_M2F_net_1    <= SPI_0_SS2_M2F_net_0;
 SPI_0_SS2_M2F          <= SPI_0_SS2_M2F_net_1;
 SPI_0_SS3_M2F_net_1    <= SPI_0_SS3_M2F_net_0;
 SPI_0_SS3_M2F          <= SPI_0_SS3_M2F_net_1;
 SPI_0_SS4_M2F_net_1    <= SPI_0_SS4_M2F_net_0;
 SPI_0_SS4_M2F          <= SPI_0_SS4_M2F_net_1;
 SPI_0_DO_M2F_net_1     <= SPI_0_DO_M2F_net_0;
 SPI_0_DO_M2F           <= SPI_0_DO_M2F_net_1;
 SPI_0_CLK_M2F_net_1    <= SPI_0_CLK_M2F_net_0;
 SPI_0_CLK_M2F          <= SPI_0_CLK_M2F_net_1;
 SPI_0_SS0_M2F_net_1    <= SPI_0_SS0_M2F_net_0;
 SPI_0_SS0_M2F          <= SPI_0_SS0_M2F_net_1;
 SPI_0_SS0_M2F_OE_net_1 <= SPI_0_SS0_M2F_OE_net_0;
 SPI_0_SS0_M2F_OE       <= SPI_0_SS0_M2F_OE_net_1;
 TX_net_1               <= TX_net_0;
 TX                     <= TX_net_1;
 FTDI_C_TX_net_1        <= FTDI_C_TX_net_0;
 FTDI_C_TX              <= FTDI_C_TX_net_1;
 TX_0_net_1             <= TX_0_net_0;
 TX_0                   <= TX_0_net_1;
 PWM_3_net_0(0)         <= PWM_3(0);
 PWM(0)                 <= PWM_3_net_0(0);
 GPIO_OUT_0_net_0       <= GPIO_OUT_0;
 GPIO_OUT(7 downto 0)   <= GPIO_OUT_0_net_0;
----------------------------------------------------------------------
-- Component instances
----------------------------------------------------------------------
-- SF2_MSS_sys_sb_0
SF2_MSS_sys_sb_0 : SF2_MSS_sys_sb
    port map( 
        -- Inputs
        FAB_RESET_N      => VCC_net,
        DEVRST_N         => DEVRST_N,
        SPI_0_DI_F2M     => SPI_0_DI_F2M,
        SPI_0_CLK_F2M    => SPI_0_CLK_F2M,
        SPI_0_SS0_F2M    => SPI_0_SS0_F2M,
        RX               => RX,
        FTDI_C_RX        => FTDI_C_RX,
        RX_0             => RX_0,
        GPIO_IN          => GPIO_IN,
        -- Outputs
        POWER_ON_RESET_N => OPEN,
        INIT_DONE        => OPEN,
        FAB_CCC_GL0      => OPEN,
        FAB_CCC_LOCK     => OPEN,
        MSS_READY        => OPEN,
        SPI_0_DO_M2F     => SPI_0_DO_M2F_net_0,
        SPI_0_CLK_M2F    => SPI_0_CLK_M2F_net_0,
        SPI_0_SS0_M2F    => SPI_0_SS0_M2F_net_0,
        SPI_0_SS0_M2F_OE => SPI_0_SS0_M2F_OE_net_0,
        SPI_0_SS1_M2F    => SPI_0_SS1_M2F_net_0,
        SPI_0_SS2_M2F    => SPI_0_SS2_M2F_net_0,
        SPI_0_SS3_M2F    => SPI_0_SS3_M2F_net_0,
        SPI_0_SS4_M2F    => SPI_0_SS4_M2F_net_0,
        TX               => TX_net_0,
        FTDI_C_TX        => FTDI_C_TX_net_0,
        TX_0             => TX_0_net_0,
        PWM              => PWM_3,
        GPIO_OUT         => GPIO_OUT_0 
        );

end RTL;
