make clean
make "DFLAGS=-DSHARED_EVERYTHING -DENABLE_DL_DETECT_CC -DSE_LATCH -DSE_INDEX_LATCH -DPTHREAD_SPINLOCK "
./trireme -s 1 -b 2 > d_d_1_1.txt
./trireme -s 1 -b 2 > d_d_1_2.txt
./trireme -s 1 -b 2 > d_d_1_3.txt
./trireme -s 4 -b 2 > d_d_4_1.txt
./trireme -s 4 -b 2 > d_d_4_2.txt
./trireme -s 4 -b 2 > d_d_4_3.txt
./trireme -s 8 -b 2 > d_d_8_1.txt
./trireme -s 8 -b 2 > d_d_8_2.txt
./trireme -s 8 -b 2 > d_d_8_3.txt
./trireme -s 12 -b 2 > d_d_12_1.txt
./trireme -s 12 -b 2 > d_d_12_2.txt
./trireme -s 12 -b 2 > d_d_12_3.txt
./trireme -s 16 -b 2 > d_d_16_1.txt
./trireme -s 16 -b 2 > d_d_16_2.txt
./trireme -s 16 -b 2 > d_d_16_3.txt
./trireme -s 20 -b 2 > d_d_20_1.txt
./trireme -s 20 -b 2 > d_d_20_2.txt
./trireme -s 20 -b 2 > d_d_20_3.txt
./trireme -s 24 -b 2 > d_d_24_1.txt
./trireme -s 24 -b 2 > d_d_24_2.txt
./trireme -s 24 -b 2 > d_d_24_3.txt

