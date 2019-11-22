#!/bin/sh

PREFIX="https://api.blockcypher.com/v1/btc/main/blocks/"
ITER=$(curl -s https://api.blockcypher.com/v1/btc/main | grep -w 'hash' | cut -d ':' -f 2 | tr -d '" ,')
for i in `seq 10`; do
	ITER=$(curl -s $PREFIX$ITER | grep -w 'hash\|height\|total\|time\|relayed_by\|prev_block'| tr -d '",' | tee -a blockdb | grep prev_block | cut -d ':' -f 2|cut -c 2-);
done
