// Baset Sowab 
// Project 3 RSA
// CSCI 360
// Due 11/25/2025
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

// Fast modular exponent
long long powmod_sm(long long base, long long exp, long long mod) {
    long long result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (long long)((__int128)result * base % mod);
        base = (long long)((__int128)base * base % mod);
        exp >>= 1;
    }
    return result;
}

// gcd
long long EA(long long a, long long b) {
    while (b) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a >= 0 ? a : -a;
}

// modular inverse
long long EEA(long long a, long long b) {
    long long old_r = a, r = b;
    long long old_s = 1, s = 0;

    while (r != 0) {
        long long q = old_r / r;

        long long temp = old_r - q * r;
        old_r = r;
        r = temp;

        temp = old_s - q * s;
        old_s = s;
        s = temp;
    }

    if (old_r != 1) return -1;

    long long inv = old_s % b;
    if (inv < 0) inv += b;
    return inv;
}

// Miller–Rabin
int MRT(long long n, int k = 10) {
    if (n < 2) return 0;

    long long primes[] = {2,3,5,7,11,13,17,19,23,29};
    for (long long p : primes) {
        if (n == p) return 1;
        if (n % p == 0) return 0;
    }

    long long d = n - 1, r = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        r++;
    }

    for (int i = 0; i < k; i++) {
        long long a = 2 + rand() % (n - 3);
        long long x = powmod_sm(a, d, n);

        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (long long j = 1; j < r; j++) {
            x = powmod_sm(x, 2, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }

        if (composite) return 0;
    }

    return 1;
}

// small random prime
long long generate_prime() {
    long long n;
    do {
        n = rand() % 100 + 10;
        if ((n & 1) == 0) n++;
    } while (!MRT(n));
    return n;
}

int main() {
    srand((unsigned)time(NULL));

    long long p = generate_prime();
    long long q = generate_prime();
    while (p == q) q = generate_prime();

    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    long long e;
    do {
        e = (rand() % (phi - 3)) + 3;
        if (e % 2 == 0) e++;
    } while (EA(e, phi) != 1);

    long long d = EEA(e, phi);

    long long plaintext = (rand() % (n - 2)) + 1;

    long long ciphertext = powmod_sm(plaintext, e, n);
    long long decrypted  = powmod_sm(ciphertext, d, n);

    // ======================= OUTPUT =======================
    cout << "================ RSA Key Generation ================\n";
    cout << "Prime p: " << p << "\n";
    cout << "Prime q: " << q << "\n";
    cout << "---------------------------------------------\n";
    cout << "n  = " << n << "\n";
    cout << "φ(n) = " << phi << "\n";
    cout << "e  = " << e << "\n";
    cout << "d  = " << d << "\n";
    cout << "---------------------------------------------\n";
    cout << "Original Plaintext: " << plaintext << "\n";
    cout << "Encrypted Value   : " << ciphertext << "\n";
    cout << "Decrypted Value   : " << decrypted << "\n";
    cout << "===============================================\n";

    return 0;
}
