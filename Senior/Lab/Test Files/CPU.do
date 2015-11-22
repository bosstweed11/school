--CPU do add signals
--add wave \
--{sim:/cpu/clock } \
--{sim:/cpu/reset } 
--add wave \
--{sim:/cpu/ex_control_if_id } \
--{sim:/cpu/mem_control_if_id } \
--{sim:/cpu/wb_control_if_id } \
--{sim:/cpu/instruction_buf } 
--add wave \
--{sim:/cpu/readdataa } \
--{sim:/cpu/readdatab } \
--{sim:/cpu/sign_ext_imd_id } 
--add wave \
--{sim:/cpu/aluop_buff } \
--{sim:/cpu/destreg_mem } \
--{sim:/cpu/overflow_ex } \
--{sim:/cpu/r } \
--{sim:/cpu/zero_ex } 
--add wave \
--{sim:/cpu/data_memorydatain } \
--{sim:/cpu/data_memread } \
--{sim:/cpu/data_memwrite } \
--{sim:/cpu/data_memoryaddress } \
--{sim:/cpu/data_memorydataout } 
--add wave \
--{sim:/cpu/memtoreg } \
--{sim:/cpu/writeaddr } \
--{sim:/cpu/writedata } \
--{sim:/cpu/writetoreg } 

restart -f
-- Initialize
force -freeze sim:/cpu/clock 1 0, 0 {50 ns} -r 100
force -freeze sim:/cpu/reset 1 0

run 200ns

force -freeze sim:/cpu/reset 0 0
-- ( 0 + 3 -> r1) 3 in r1
force INST_MemoryDataIn X"20010003"

run 100ns

force INST_MemoryDataIn X"00000000"

run 400ns
--( r1 +r1 -> r1 )6 in r1
force INST_MemoryDataIn X"20210003" 

run 100ns

force INST_MemoryDataIn X"00000000"

run 400ns
-- ( 0 + 2 -> r2) 2 in r2
force INST_MemoryDataIn X"20020002" 

run 100ns

force INST_MemoryDataIn X"00000000"

run 400ns
-- ( r1 - r2 -> r2) 4 in r2
force INST_MemoryDataIn X"00221022"

run 100ns


