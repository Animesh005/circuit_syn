#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include <bits/stdc++.h>

#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <tfhe/lwe-functions.h>
#include <tfhe/numeric_functions.h>
#include <tfhe/tlwe_functions.h>

using namespace std;

// Initialize block-cipher parameters
int in_size;
int out_size;
int out_arr_size;


// Initialize TFHE parameters
auto tfheParams = new_default_gate_bootstrapping_parameters(110);
auto params = tfheParams->in_out_params;

// TFHE key generation 
auto sk = new_random_gate_bootstrapping_secret_keyset(tfheParams);\
auto bk = &sk->cloud;

// Read netlist
std::vector<std::vector<int>> readNetlist(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::vector<int>> result;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> temp;
        int val;
        while (iss >> val) {
            temp.push_back(val);
        }

        result.push_back(temp);
    }
    std::cout << "\nCircuit size: " << result.size() << "\n";

    return result;
}

// Evalute the block-cipher on encrypted input
void cipherEval(std::vector<LweSample*> &enc_cipher_output, const std::vector<std::vector<int>>& netlist, 
                                        const std::vector<LweSample*>& enc_cipher_msg, 
                                        const std::vector<LweSample*>& enc_cipher_key, std::vector<LweSample*>& cipher_output_arr) {


    // Fill first 2n bits of output array with n bit cipher encrypted msg and n bit cipher encrypted key
    for (int i = 0; i < in_size; ++i) {
        lweCopy(cipher_output_arr[i], enc_cipher_msg[i], params);
        lweCopy(cipher_output_arr[i + in_size], enc_cipher_key[i], params);
    }

    for (int i = 0; i < netlist.size(); ++i) {
        if (netlist[i].back() == 0 && netlist[i].front() == 2) { // XOR operation
            int in1 = netlist[i][2];
            int in2 = netlist[i][3];
            int out = netlist[i][4];

            bootsXOR(cipher_output_arr[out], cipher_output_arr[in1], cipher_output_arr[in2], bk);

        }
        else if (netlist[i].back() == 0 && netlist[i].front() == 3) { // 3XOR operation
            int in1 = netlist[i][2];
            int in2 = netlist[i][3];
            int in3 = netlist[i][4];
            int out = netlist[i][5];

            bootsXOR3(cipher_output_arr[out], cipher_output_arr[in1], cipher_output_arr[in2], cipher_output_arr[in3], bk);

        }
        else if (netlist[i].back() == 1 && netlist[i].front() == 2) { // AND operation
            int in1 = netlist[i][2];
            int in2 = netlist[i][3];
            int out = netlist[i][4];

            bootsAND(cipher_output_arr[out], cipher_output_arr[in1], cipher_output_arr[in2], bk);

        }
        else if (netlist[i].back() == 1 && netlist[i].front() == 3) { // 3AND operation
            int in1 = netlist[i][2];
            int in2 = netlist[i][3];
            int in3 = netlist[i][4];
            int out = netlist[i][5];

            bootsAND3(cipher_output_arr[out], cipher_output_arr[in1], cipher_output_arr[in2], cipher_output_arr[in3], bk);

        }
        else if (netlist[i].back() == 2 && netlist[i].front() == 2) { // OR operation
            int in1 = netlist[i][2];
            int in2 = netlist[i][3];
            int out = netlist[i][4];

            bootsOR(cipher_output_arr[out], cipher_output_arr[in1], cipher_output_arr[in2], bk);

        }
        else if (netlist[i].back() == 2 && netlist[i].front() == 3) { // 3OR operation
            int in1 = netlist[i][2];
            int in2 = netlist[i][3];
            int in3 = netlist[i][4];
            int out = netlist[i][5];

            bootsOR3(cipher_output_arr[out], cipher_output_arr[in1], cipher_output_arr[in2], cipher_output_arr[in3], bk);

        }
        else if (netlist[i].back() == 10 && netlist[i].front() == 3) { // AND-XOR operation
            int in1 = netlist[i][2];
            int in2 = netlist[i][3];
            int in3 = netlist[i][4];
            int out = netlist[i][5];

            bootsAX3(cipher_output_arr[out], cipher_output_arr[in1], cipher_output_arr[in2], cipher_output_arr[in3], bk);

        }
        else if (netlist[i].back() == 20 && netlist[i].front() == 3) { // OR-XOR operation
            int in1 = netlist[i][2];
            int in2 = netlist[i][3];
            int in3 = netlist[i][4];
            int out = netlist[i][5];

            bootsOX3(cipher_output_arr[out], cipher_output_arr[in1], cipher_output_arr[in2], cipher_output_arr[in3], bk);

        }
        else{ // NOT operation
            int in1 = netlist[i][2];
            int out = netlist[i][3];

            bootsNOT(cipher_output_arr[out], cipher_output_arr[in1], bk);
        }
    }

    for(int i = 0; i < out_size; i++){
        lweCopy(enc_cipher_output[out_size-1-i], cipher_output_arr[cipher_output_arr.size()-1-i], params);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " Enter input-size output-size output-wire-size path-to-netlist" << std::endl;
        return 1;
    }

    in_size = std::stoi(argv[1]);
    out_size = std::stoi(argv[2]);
    out_arr_size = std::stof(argv[3]);
    std::string path_to_netlist = argv[4];

    std::vector<std::vector<int>> inp(2, std::vector<int>(in_size));
    std::vector<int> outp(out_size);

    for (int i= 0; i < in_size; i++)
    {
        inp[0][i]= rand() % 2; // Message
        inp[1][i]= rand() % 2; // Key
    }
    cout << "\n\nMessage: " << endl;

    for (int i=0; i < in_size; i++)
        cout << inp[0][i] << " ";

    cout << "\n\nKey: " << endl;

    for (int i=0; i < in_size; i++)
        cout << inp[1][i] << " ";

    // Read block-cipher netlist
    cout << "\n\nLoading Circuit . . ." << endl;
    std::vector<std::vector<int>> cipher_netlist = readNetlist(path_to_netlist);

    // Initialize block-cipher msg and key
    std::vector<LweSample*> cipher_msg(in_size);
    std::vector<LweSample*> cipher_key(in_size);

    // Encrypt block-cipher msg and key
    std::vector<LweSample*> enc_cipher_msg(in_size);
    std::vector<LweSample*> enc_cipher_key(in_size);

    for (int i = 0; i < in_size; ++i) {
        enc_cipher_msg[i] = new_LweSample(params);
        bootsSymEncrypt(enc_cipher_msg[i], inp[0][i], sk);

        enc_cipher_key[i] = new_LweSample(params);
        bootsSymEncrypt(enc_cipher_key[i], inp[1][i], sk);

    }

    std::vector<LweSample*> cipher_output_arr(out_arr_size);

    for (int i = 0; i < cipher_output_arr.size(); i++)
        cipher_output_arr[i] = new_LweSample(params);

    std::vector<LweSample*> enc_cipher_output;
    enc_cipher_output.resize(out_size);
    for (int i = 0; i < enc_cipher_output.size(); i++)
        enc_cipher_output[i] = new_LweSample(params);
    
    cout << "\nEvaluating Circuit . . ." << endl;
    clock_t begin_eval = clock();

    cipherEval(enc_cipher_output, cipher_netlist, enc_cipher_msg, enc_cipher_key, cipher_output_arr);
    
    clock_t end_eval = clock();
    double time_eval = ((double) end_eval - begin_eval)/CLOCKS_PER_SEC;
    cout << "Finished Evaluation: " << time_eval << " seconds"<< endl;


    // std::cout << "\n\nfinal decryption\n";
    // std::vector<int> cipher_clear_output(out_size);

    // for (int i = 0; i < in_size; ++i) {
    //     cipher_clear_output[i] = bootsSymDecrypt(enc_cipher_output[i], sk);
    // }

    // // Print clear output
    // for (int bit : cipher_clear_output) {
    //     std::cout << bit << " ";
    // }
    std::cout << "\n";

    return 0;
}