make clean
make "DFLAGS=-DSHARED_NOTHING -DPTHREAD_SPINLOCK"
./trireme -s 1 -b 2 > ro_1_1.txt
./trireme -s 4 -b 2 > ro_4_1.txt
./trireme -s 8 -b 2 > ro_8_1.txt
./trireme -s 12 -b 2 > ro_12_1.txt
./trireme -s 16 -b 2 > ro_16_1.txt
./trireme -s 20 -b 2 > ro_20_1.txt
./trireme -s 24 -b 2 > ro_24_1.txt
./trireme -s 1 -b 2 > ro_1_2.txt
./trireme -s 4 -b 2 > ro_4_2.txt
./trireme -s 8 -b 2 > ro_8_2.txt
./trireme -s 12 -b 2 > ro_12_2.txt
./trireme -s 16 -b 2 > ro_16_2.txt
./trireme -s 20 -b 2 > ro_20_2.txt
./trireme -s 24 -b 2 > ro_24_2.txt
./trireme -s 1 -b 2 > ro_1_3.txt
./trireme -s 4 -b 2 > ro_4_3.txt
./trireme -s 8 -b 2 > ro_8_3.txt
./trireme -s 12 -b 2 > ro_12_3.txt
./trireme -s 16 -b 2 > ro_16_3.txt
./trireme -s 20 -b 2 > ro_20_3.txt
./trireme -s 24 -b 2 > ro_24_3.txt
./trireme -s 1 -b 2 > ro_1_4.txt
./trireme -s 4 -b 2 > ro_4_4.txt
./trireme -s 8 -b 2 > ro_8_4.txt
./trireme -s 12 -b 2 > ro_12_4.txt
./trireme -s 16 -b 2 > ro_16_4.txt
./trireme -s 20 -b 2 > ro_20_4.txt
./trireme -s 24 -b 2 > ro_24_4.txt
./trireme -s 1 -b 2 > ro_1_5.txt
./trireme -s 4 -b 2 > ro_4_5.txt
./trireme -s 8 -b 2 > ro_8_5.txt
./trireme -s 12 -b 2 > ro_12_5.txt
./trireme -s 16 -b 2 > ro_16_5.txt
./trireme -s 20 -b 2 > ro_20_5.txt
./trireme -s 24 -b 2 > ro_24_5.txt

