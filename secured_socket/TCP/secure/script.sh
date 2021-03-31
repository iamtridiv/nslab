openssl dgst -sha3-256 -out dig.bin plain.txt

openssl genrsa -out prkey.key

openssl rsa -in prkey.key -out pubkey.key

openssl rsautl -sign -in dig.bin -inkey prkey.key -out enc.enc

openssl enc -aes-256-cbc -in enc.enc -out enc2.enc -pass pass:vispi

openssl enc -aes-256-cbc -d -in enc2.enc -out dec.enc -pass pass:vispi

openssl rsautl -verify -in dec.enc -inkey pubkey.key -out dig2.bin

cmp dig.bin dig2.bin
