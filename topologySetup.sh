#!/bin/bash

while IFS=: read number_of_hosts number_of_routers
do
    echo "NumHosts : $number_of_hosts \n
NumRouters : $number_of_routers \n"    
done < topology
