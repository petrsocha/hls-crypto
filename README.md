# hls-crypto
FPGA Cryptography for High-Level Synthesis (Xilinx Vivado)

This project contains encryption algorithms in C for Vivado HLS (High-Level Synthesis), including testbenches. 

Currently, it contains:

- AES/Rijndael
- PRESENT
- Serpent

Each algorithm is available in two implementations: naïve and masked using LUTRAM-based polymorphism and precomputed S-boxes.
  
Cite as:
Socha, P., & Novotný, M. (2020, August). Towards High-Level Synthesis of Polymorphic Side-Channel Countermeasures. In 2020 23rd Euromicro Conference on Digital System Design (DSD) (pp. 193-199). IEEE Computer Soc. 
  
See Embedded Security Lab at CTU FIT (https://crypto.fit.cvut.cz) for contact information.
