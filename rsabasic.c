// Basic RSA Implementation
// by Fangzhou Yu
// Winter 2023 CS62

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// introduce some PRNG to get different e each time
unsigned long long getE(unsigned long long phi, unsigned long long primes[]){
    int MAX_SIZE = 3500;
    int e[MAX_SIZE];
    int size = 0;
    int index;
    for (int i = 1; i < 3432; i++){
        if (primes[i] > phi){
            break;
        }
        if (phi % primes[i] != 0){
            e[size] = primes[i];
            size++;
        }
    }
    // use this to generate a number between 1 and 10, only return e if indicator geq 8
    // this decision is rather arbitrary but I just want to make the point that using
    // same e over and over is unsafe
    srand(time(0));
    index = rand() % size;
    return e[index];
}

// use extended euclidean algorithm
unsigned long long extendedEuclidean(unsigned long long a, unsigned long long b, unsigned long long *x, unsigned long long *y){
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    unsigned long long x1, y1;
    unsigned long long gcd = extendedEuclidean(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcd;
}

// Find d by calculating from x and y
unsigned long long modInverse(unsigned long long e, unsigned long long phi){
    unsigned long long x, y;
    unsigned long long g = extendedEuclidean(e, phi, &x, &y);
    if (g != 1) {       // No Inverse
        return -1;
    }
    else { 
        return (x + phi) % phi;
    }
}

/*
This works but according to HW3 Q1 it shouldn't be implemented this way
unsigned long long getD(unsigned long long e, unsigned long long phi){
    unsigned long long d = 0;       
    while(true){            // theoretically shouldn't be a problem
        if ((d*e-1) % phi == 0){
            return d;
        }
        d++;
    }
    return -1;
}
*/

void convertDecimaltoBin(int num, unsigned char output[30]){
    for (int i = 31; i > 0; i--){
        output[i] = num%2;
        num/=2;
    }
}

unsigned long long raiseToPower(unsigned long long base, unsigned long long power, unsigned long long max){
    unsigned long long res = 1;
    while (power > 0){
        if (power & 1){
            res = (res*base) % max;
        }
        base = (base*base) % max;
        power >>= 1;
    }
    return res;
}

int main() {
    // initialize the array
    printf("Loading array...\n");
    unsigned long long primes[5000];
    FILE* fp;
    fp = fopen("primelist.txt", "r");
    int pos = 0;
    while (fscanf(fp, "%llu", &primes[pos]) == 1) {
        pos++;
    }
    fclose(fp);
    int total = pos;
    printf("read in %d primes\n", total);
    
    // variables that specifies indices of two primes
    int index1;
    int index2;
    unsigned long long p;
    unsigned long long q;

    // prompt for user input
    printf("Enter indices for two primes: \n");
    scanf("%d", &index1);
    scanf("%d", &index2);
    if (index1 > pos || index2 > pos || index1 < 0 || index2 < 0){
        printf("Invalid indexes, please enter a non-negative value less than %d\n", pos);
        return 1;
    }
    p = primes[index1];
    q = primes[index2];
    printf("p = %llu, q = %llu \n", p, q);
    
    // compute n and phi
    unsigned long long n = p*q;
    unsigned long long phi = (p-1)*(q-1);
    printf("n = %llu, phi = %llu \n", n, phi);

    while (!feof(stdin)){
        // compute e and d
        unsigned long long e;
        unsigned long long d;
        e = getE(phi, primes);
        if (e == -1){
            printf("encountered error in generating e \n");
            return 2;
        }
        d = modInverse(e, phi);
        printf("e = %llu, d = %llu \n", e, d);

        // convert keys to binary
        // e
        unsigned char eBin[30];
        convertDecimaltoBin(e, eBin);
        printf("e = ");
        for(int i = 0; i<32; i++){
            printf("%d ", eBin[i]);
        }
        printf("\n");
        // d
        unsigned char dBin[30];
        convertDecimaltoBin(d, dBin);
        printf("d = ");
        for(int i = 0; i<32; i++){
            printf("%d ", dBin[i]);
        }
        printf("\n");
        // n
        unsigned char nBin[30];
        convertDecimaltoBin(n, nBin);
        printf("n = ");
        for(int i = 0; i<32; i++){
            printf("%d ", nBin[i]);
        }
        printf("\n");

        // prompt for user input
        printf("PRESS CTRL+D TO QUIT\n");
        unsigned long long m;
        printf("Enter message: \n");
        scanf("%llu", &m);
        printf("message: %llu\n", m);

        // Encryption
        unsigned long long cm;
        cm = raiseToPower(m, e, n);

        // Decryption
        unsigned long long msg;
        msg = raiseToPower(cm, d, n);
        printf("c^d mod n = %llu \n", msg);
        printf("\n \n");
    }

    return 0;
}
