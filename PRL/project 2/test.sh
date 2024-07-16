#!/bin/bash

# zvoleny pocet procesorov 4 - vzdy sa mriezka rozdeli na rovnomerne stvrtiny
processors=4

#skombiluje
mpic++ --prefix /usr/local/share/OpenMPI -o life life.cpp -pedantic

#spusti
mpirun --prefix /usr/local/share/OpenMPI -np $processors life $@

#uklid
rm -f life
