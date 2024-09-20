# circuit_synthesis

# Dev Installation

```bash
git clone <repo url>
cd <repo>
mkdir output
cd output
mkdir Bristol_Optimized Bristol_Processed
cd ..
cd ckt_eval
cd tfhe
make
sudo make install
cd ..
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
mkdir bin
make
```

# Run Circuit Synthesis

```bash
python3 cipher_synthesis.py
```
Input file location- ./input/Bristol/circuit.txt
Output file location- ./output/Bristol_Optimized/circuit_opt.txt

# Parse Optimized Circuits
Parse optimized circuits to run it on encrypted data using TFHE

```bash
python3 circuit_parse.py
```
Input file location- ./output/Bristol_Optimized/circuit_opt.txt
Output file location- ./output/Bristol_Processed/circuit_proc.txt

Transfer the parsed circuits to test folder in ckt_eval directory 

```bash
mv ./output/Bristol_Processed/* ./ckt_eval/test/
```
# Run on TFHE

./bin/cipher_eval takes 4 paramters input-size output-size output-wire-size path-to-input-file

```bash
./bin/cipher_eval 64 64 504 ./test/adder64.txt
```
