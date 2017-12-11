set_device \
    -fam SmartFusion2 \
    -die PA4M1000_N \
    -pkg tq144
set_input_cfg \
	-path {C:/Users/Mike Klopfer/Documents/GitHub/Microsemi_SmartFusion2_DigikeyBoardDemo/sf2project/component/work/SF2_MSS_sys_sb_MSS/ENVM.cfg}
set_output_efc \
    -path {C:\Users\Mike Klopfer\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\sf2project\designer\SF2_MSS_sys\SF2_MSS_sys.efc}
set_proj_dir \
    -path {C:\Users\Mike Klopfer\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\sf2project}
gen_prg -use_init false
