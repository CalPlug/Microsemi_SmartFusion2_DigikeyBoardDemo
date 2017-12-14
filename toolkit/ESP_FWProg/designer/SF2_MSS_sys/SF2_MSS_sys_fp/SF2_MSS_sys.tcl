open_project -project {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\designer\SF2_MSS_sys\SF2_MSS_sys_fp\SF2_MSS_sys.pro}\
         -connect_programmers {FALSE}
if { [catch {load_programming_data \
    -name {M2S010} \
    -fpga {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\designer\SF2_MSS_sys\SF2_MSS_sys.map} \
    -header {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\designer\SF2_MSS_sys\SF2_MSS_sys.hdr} \
    -envm {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\designer\SF2_MSS_sys\SF2_MSS_sys.efc}  \
    -spm {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\designer\SF2_MSS_sys\SF2_MSS_sys.spm} \
    -dca {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\designer\SF2_MSS_sys\SF2_MSS_sys.dca} } return_val] } {
save_project
close_project
exit }
export_single_stapl \
    -name {M2S010} \
    -file {C:\Users\calplug\Documents\GitHub\Microsemi_SmartFusion2_DigikeyBoardDemo\toolkit\ESP_FWProg\designer\SF2_MSS_sys\export\SF2_MSS_sysFLASH_M2S010.stp} \
    -secured
set_programming_file -name {M2S010} -no_file
save_project
close_project
