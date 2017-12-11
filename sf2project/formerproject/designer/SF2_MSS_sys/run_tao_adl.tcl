set_family {SmartFusion2}
read_adl {C:\Users\Mike Klopfer\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\sf2project\designer\SF2_MSS_sys\SF2_MSS_sys.adl}
map_netlist
read_sdc {C:\Users\Mike Klopfer\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\sf2project\constraint\SF2_MSS_sys_derived_constraints.sdc}
check_constraints {C:\Users\Mike Klopfer\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\sf2project\constraint\placer_sdc_errors.log}
write_sdc -strict {C:\Users\Mike Klopfer\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\sf2project\designer\SF2_MSS_sys\place_route.sdc}
