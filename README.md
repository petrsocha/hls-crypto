# hls-crypto
FPGA Cryptography for High-Level Synthesis (Xilinx Vivado)

This project contains encryption algorithms in C for Vivado HLS (High-Level Synthesis), including testbenches. 

Currently, it contains:

- AES/Rijndael
  - Naive implementation using LUT-based S-boxes (8-bit S-boxes, therefore quite inefficient)
- PRESENT
  - Naive implementation using LUT-based S-boxes
  - Side-channel protected implementation utilizing reconfigurable LUTRAM S-boxes for first-order masking
- Serpent
  - Naive implementation using LUT-based S-boxes
  
Cite as:
Socha, P., & Novotný, M. (2020, August). Towards High-Level Synthesis of Polymorphic Side-Channel Countermeasures. In 2020 23rd Euromicro Conference on Digital System Design (DSD) (pp. 193-199). IEEE Computer Soc. 
  
See Embedded Security Lab at CTU FIT (https://crypto.fit.cvut.cz) for contact information.
