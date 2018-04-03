make clean
make "DFLAGS=-DSHARED_NOTHING -DPTHREAD_SPINLOCK -DYCSB_BENCHMARK"
./trireme -s 1 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_1_1.txt
./trireme -s 4 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_4_1.txt
./trireme -s 8 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_8_1.txt
./trireme -s 12 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_12_1.txt
./trireme -s 16 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_16_1.txt
./trireme -s 20 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_20_1.txt
./trireme -s 24 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_24_1.txt
./trireme -s 1 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_1_2.txt
./trireme -s 4 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_4_2.txt
./trireme -s 8 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_8_2.txt
./trireme -s 12 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_12_2.txt
./trireme -s 16 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_16_2.txt
./trireme -s 20 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_20_2.txt
./trireme -s 24 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_24_2.txt
./trireme -s 1 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_1_3.txt
./trireme -s 4 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_4_3.txt
./trireme -s 8 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_8_3.txt
./trireme -s 12 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_12_3.txt
./trireme -s 16 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_16_3.txt
./trireme -s 20 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_20_3.txt
./trireme -s 24 -b 1 -w 0 -a 0.1 -d 0 -o 16 > RO_01_ycsb_24_3.txt


