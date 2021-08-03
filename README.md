# hls-crypto
FPGA Cryptography for High-Level Synthesis (Xilinx Vivado)

This project contains encryption algorithms in C for Vivado HLS (High-Level Synthesis), including testbenches. 

Currently, it contains:

- AES/Rijndael
- PRESENT
- Serpent

Each algorithm is available in two implementations: naïve and masked using LUTRAM-based polymorphism and precomputed S-boxes.
  
Cite as:

Socha, P., Miškovský, V., & Novotný, M. (2021). High-level synthesis, cryptography, and side-channel countermeasures: A comprehensive evaluation. Microprocessors and Microsystems, 104311.
  
See Embedded Security Lab at CTU FIT (https://crypto.fit.cvut.cz) for contact information.
