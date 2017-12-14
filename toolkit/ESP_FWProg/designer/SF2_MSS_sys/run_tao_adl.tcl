set_family {SmartFusion2}
read_adl {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\designer\SF2_MSS_sys\SF2_MSS_sys.adl}
map_netlist
read_sdc {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\constraint\SF2_MSS_sys_derived_constraints.sdc}
check_constraints {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\constraint\placer_sdc_errors.log}
write_sdc -strict {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\designer\SF2_MSS_sys\place_route.sdc}
