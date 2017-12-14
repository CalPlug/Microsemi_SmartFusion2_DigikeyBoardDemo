set_device \
    -fam SmartFusion2 \
    -die PA4M1000_N \
    -pkg tq144
set_input_cfg \
	-path {C:/Users/calplug/Documents/GitHub/Microsemi_SmartFusion2_DigikeyBoardDemo/toolkit/ESP_FWProg/component/work/SF2_MSS_sys_sb_MSS/ENVM.cfg}
set_output_efc \
    -path {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\designer\SF2_MSS_sys\SF2_MSS_sys.efc}
set_proj_dir \
    -path {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg}
gen_prg -use_init false
