# hls-crypto
FPGA Cryptography for High-Level Synthesis (Xilinx Vivado)

This project contains encryption algorithms in C for Vivado HLS (High-Level Synthesis), including testbenches. 

Currently, it contains:

- AES/Rijndael
  - Naive implementation using LUT-based S-boxes (8-bit S-boxes, therefore quite inefficient)
- PRESENT
  - Naive implementation using LUT-based S-boxes (PRESENT uses 4-bit S-boxes, therefore efficient in most FPGA)
  - Side-channel protected implementation utilizing reconfigurable LUTRAM S-boxes for first-order masking
- Serpent
  - Naive implementation using LUT-based S-boxes (Serpent also uses 4-bit S-boxes)
  
See Embedded Security Lab at CTU FIT (https://crypto.fit.cvut.cz) for contact information.
