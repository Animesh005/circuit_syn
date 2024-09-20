TFHE_PREFIX = /usr/local
TFHE_OPTIONS = C_INCLUDE_PATH=$$C_INCLUDE_PATH:${TFHE_PREFIX}/include CPLUS_INCLUDE_PATH=$$CPLUS_INCLUDE_PATH:${TFHE_PREFIX}/include LIBRARY_PATH=$$LIBRARY_PATH:${TFHE_PREFIX}/lib LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:${TFHE_PREFIX}/lib
TFHE_LIB = -ltfhe-spqlios-fma
BLAS_LIB = -lopenblas

all: bin/keygen bin/encrypt bin/compute bin/decrypt bin/bootstrap_test bin/bootstrap_modules bin/cipher_eval

bin/bootstrap_modules: src/bootstrap_modules.cpp
	${TFHE_OPTIONS} g++ -g src/bootstrap_modules.cpp -o bin/bootstrap_modules ${TFHE_LIB}

bin/share.o: src/share.cpp
	g++ -g -c src/share.cpp -o bin/share.o

bin/threshold_decryption_functions.o: src/threshold_decryption_functions.cpp
	g++ -g -O3 -c -fopenmp src/threshold_decryption_functions.cpp -o bin/threshold_decryption_functions.o ${BLAS_LIB}

bin/bootstrap_test: src/bootstrap_test.cpp
	${TFHE_OPTIONS} g++ -g src/bootstrap_test.cpp -o bin/bootstrap_test ${TFHE_LIB}

bin/keygen: src/KeyGen.cpp
	${TFHE_OPTIONS} g++ src/KeyGen.cpp -o bin/keygen ${TFHE_LIB}

bin/encrypt: src/Encrypt.cpp
	${TFHE_OPTIONS} g++ src/Encrypt.cpp -o bin/encrypt ${TFHE_LIB}

bin/compute: src/Compute.cpp
	${TFHE_OPTIONS} g++ src/Compute.cpp -o bin/compute ${TFHE_LIB}

bin/decrypt: src/Decrypt.cpp
	${TFHE_OPTIONS} g++ src/Decrypt.cpp -o bin/decrypt ${TFHE_LIB}

bin/cipher_eval: src/cipher_eval.cpp
	${TFHE_OPTIONS} g++ -g src/cipher_eval.cpp -o bin/cipher_eval ${TFHE_LIB}

.PHONY: clean
clean:
	rm -rf bin/* test/*

.PHONY: install
install:
	cp src/thfhe.hpp /usr/local/include
	cp bin/libthfhe.a /usr/local/lib
