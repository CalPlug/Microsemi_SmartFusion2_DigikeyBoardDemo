open_project -project {C:\Users\Hiperwall\Desktop\Servo-arm3 - Copy\designer\SF2_MSS_sys\SF2_MSS_sys_fp\SF2_MSS_sys.pro}\
         -connect_programmers {FALSE}
if { [catch {load_programming_data \
    -name {M2S010} \
    -fpga {C:\Users\Hiperwall\Desktop\Servo-arm3 - Copy\designer\SF2_MSS_sys\SF2_MSS_sys.map} \
    -header {C:\Users\Hiperwall\Desktop\Servo-arm3 - Copy\designer\SF2_MSS_sys\SF2_MSS_sys.hdr} \
    -envm {C:\Users\Hiperwall\Desktop\Servo-arm3 - Copy\designer\SF2_MSS_sys\SF2_MSS_sys.efc}  \
    -spm {C:\Users\Hiperwall\Desktop\Servo-arm3 - Copy\designer\SF2_MSS_sys\SF2_MSS_sys.spm} \
    -dca {C:\Users\Hiperwall\Desktop\Servo-arm3 - Copy\designer\SF2_MSS_sys\SF2_MSS_sys.dca} } return_val] } {
save_project
close_project
exit }
export_single_stapl \
    -name {M2S010} \
    -file {C:\Users\Hiperwall\Desktop\Servo-arm3 - Copy\designer\SF2_MSS_sys\export\SF2_MSS_sysact00001.stp} \
    -secured
set_programming_file -name {M2S010} -no_file
save_project
close_project
