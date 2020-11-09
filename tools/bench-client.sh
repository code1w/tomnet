#!/bin/bash
for i in {1..10}
do
    nohup ./tomnet_test client 127.0.0.1 8888 2 1 200 >> c.log 2>&1 &
    sleep 2
done