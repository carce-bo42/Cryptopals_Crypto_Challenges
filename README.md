# Cryptopals_Crypto_Challenges
Collection of exercises that demonstrate attacks on real-world cryptography.

Exercises from https://cryptopals.com/ \
Developed in C using OPENSSL 3.0.7

To run the exercises it is not enough with calling `./compile.sh` on
each folder. First you need to build the cryptopals shared library:

At the `<somewhere>/cryptopals/lib/` directory, run \
`$> make`

Then, add the path of the shared library cryptopals.so to the dynamic linker path:
`$> LD_LIBRARY_PATH=<wherever cryptopals.so is>:$LD_LIBRARY_PATH`

Now all the executables generated should not have any runtime errors.
