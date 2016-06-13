make clean
make all
rmiregistry 3000 &
java NbodyServer &
java NbodyClient ../data/solar_system.txt
