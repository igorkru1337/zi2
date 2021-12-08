#include <bits/stdc++.h>
#include <random>

using namespace std;

string translateToBinary(unsigned long long int a) {
    string res;
    while (a != 0) {
        res += (char) (a % 2 + 48);
        if (a % 2)
            a--;
        a /= 2;
    }
    return res;
}

unsigned long long int translateToTen(string code) {
    unsigned long long int res = 0;
    for (int i = 0; i < code.size(); i++) {
        if (code[i] == '1') res += pow(2, i);
    }
    return res;
}

int isPrime(unsigned long long n) {

    if ((n & 1) == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    if (n < 25) return n > 1;
    unsigned long long p = 5;
    while (p * p <= n) {
        if (n % p == 0) return 0;
        p += 2;
        if (n % p == 0) return 0;
        p += 4;
    }
    return 1;
}

unsigned long long randBigPrime(long long int lower, long long int upper) {
    random_device rd;
    mt19937 mt(rd());
    mt.seed(time(nullptr));
    uniform_int_distribution<unsigned long long int> dist(lower, upper);

    while (true) {
        unsigned long long p = (unsigned long long int) dist(mt);
        if (isPrime(p)) return p;
    }
}

unsigned long long randBig(long long int lower, long long int upper) {
    random_device rd;
    mt19937 mt(rd());
    mt.seed(time(nullptr));
    uniform_int_distribution<unsigned long long int> dist(lower, upper);

    unsigned long long p = (unsigned long long int) dist(mt);
    return p;

}

unsigned long long int fastPowOnModule(unsigned long long int a, unsigned long long int x, unsigned long long int p) {
    string xs = translateToBinary(x);
    unsigned long long int y = 1, s = a;
    for (char i: xs) {
        if ((int) (i - 48)) {
            y = y * s % p;
        }
        s = s * s % p;
    }
    return y;
}

string decode(long long a) {
    long long a1, a2, a3;
    a3 = (a % 1000 / 100) * 100 + (a % 100 / 10) * 10 + a % 10;
    a /= 1000;

    a2 = (a % 1000 / 100) * 100 + (a % 100 / 10) * 10 + a % 10;
    a /= 1000;

    a1 = (a % 1000 / 100) * 100 + (a % 100 / 10) * 10 + a % 10;
    string str;
    str += (char) (a1 - 128);
    str += (char) (a2 - 128);
    str += (char) (a3 - 128);
    return str;

}

void euclideanAlgorithmGeneral(long long int a, long long int b, long long int s[3]) {
    long long int u[3] = {a, 1, 0}, v[3] = {b, 0, 1}, T[3], q;
    while (v[0] != 0) {
        q = u[0] / v[0];
        T[0] = u[0] % v[0];
        T[1] = u[1] - q * v[1];
        T[2] = u[2] - q * v[2];

        for (int i = 0; i < 3; i++) {
            u[i] = v[i];
            v[i] = T[i];
            s[i] = u[i];
        }
    }
}

long long int generator(long long int p) {
    vector<long long int> fact;
    long long int phi = p - 1, n = phi;
    for (long long int i = 2; i * i <= n; i++)
        if (n % i == 0) {
            fact.push_back(i);
            while (n % i == 0)
                n /= i;
        }
    if (n > 1)
        fact.push_back(n);
    for (int i = 0; i < fact.size(); i++) {
        cout << fact[i] << "---" << endl;
    }
    for (long long int res = 2; res <= p; res++) {
        bool ok = true;
        for (size_t i = 0; i < fact.size() && ok; ++i)
            ok &= fastPowOnModule(res, phi / fact[i], p) != 1;
        if (ok) return res;
    }
    return -1;
}

string binaryEqual(string x1, string x2) {
    string res = x1;
    for (int i = 0; i < x1.size(); i++) {
        if (x1[i] != x2[i]) res[i] == '1';
        else res[i] == '0';
    }
    return res;
}

long long int shamir(long long int m) {
    long long x[4];
    unsigned long long Ca, Cb, Da, Db, p;
    long long int bottomRange = 300000000, topRange = 700000000;
    p = randBigPrime(bottomRange, topRange);

    Ca = randBigPrime(bottomRange, topRange);
    Da = randBigPrime(bottomRange, topRange);
    while (fastPowOnModule(Ca * Da, 1, p - 1) != 1) {
        Ca = randBigPrime(bottomRange, topRange);
        Da = randBigPrime(bottomRange, topRange);
    }
    Cb = randBigPrime(bottomRange, topRange);
    Db = randBigPrime(bottomRange, topRange);
    while (fastPowOnModule(Cb * Db, 1, p - 1) != 1) {
        Cb = randBigPrime(bottomRange, topRange);
        Db = randBigPrime(bottomRange, topRange);
    }

    x[0] = fastPowOnModule(m, Ca, p);
    x[1] = fastPowOnModule(x[0], Cb, p);
    x[2] = fastPowOnModule(x[1], Da, p);
    x[3] = fastPowOnModule(x[2], Db, p);

    return x[3];
}

long long int elgamal(long long int m) {
    long long int bottomRange = 300000000, topRange = 400000000;
    unsigned long long k, a, b, m2;
    long long p = randBigPrime(bottomRange, topRange);
    unsigned long long g = generator(p);
    unsigned long long cb = randBig(1, p - 1);
    unsigned long long db = fastPowOnModule(g, cb, p);

    k = randBig(1, p - 2);
    a = fastPowOnModule(g, k, p);
    b = fastPowOnModule(m * fastPowOnModule(db, k, p), 1, p);
    m2 = fastPowOnModule(b * fastPowOnModule(a, p - 1 - cb, p), 1, p);

    return m2;
}


unsigned long long int vernam(long long int m) {
    long long int bottomRange = 100000000, topRange = 300000000;
    unsigned long long k;
    k = randBigPrime(bottomRange, topRange);
    string ms, ks, es, res;
    ms = translateToBinary(m);
    ks = translateToBinary(k);
    es = binaryEqual(ms, ks);
    res = binaryEqual(es, ks);

    return translateToTen(res);
}

long long int RSA(long long int m) {
    long long int bottomRange = 30000, topRange = 40000, fi = 10000;
    long long int e, res, c, f, d, euclideanAlgorithmAnswer[3];
    unsigned long long P = randBigPrime(bottomRange, topRange);
    unsigned long long Q = randBigPrime(bottomRange + fi, topRange + fi);
    unsigned long long n = P * Q;
    f = (P - 1) * (Q - 1);
    d = randBigPrime(100, 200);

    while (d > f) d = generator(f);
    euclideanAlgorithmGeneral(d, f, euclideanAlgorithmAnswer);
    c = f + euclideanAlgorithmAnswer[1];

    e = fastPowOnModule(m, d, n);
    res = fastPowOnModule(e, c, n);

    return res;
}

int main() {

    string name = "1.png";
    long long int s[3];
    euclideanAlgorithmGeneral(1013944333, 1801710790, s);
    cout << s[1] << "====" << endl << endl;

    ifstream fin1(name, std::ios::binary);
    string data1((std::istreambuf_iterator<char>(fin1)),
                 std::istreambuf_iterator<char>());

    for (int turn = 1; turn <= 4; turn++) {

        string data2 = "", writeName;
        long long int j = 0, a, code;

        for (int i = 0; i < data1.size(); i += 3) {
            j = 0;
            j += 1000000 * ((int) data1[i] + 128);
            j += 1000 * ((int) data1[i + 1] + 128);
            j += ((int) data1[i + 2] + 128);

            if (turn == 1) code = shamir(j);
            else if (turn == 2) code = elgamal(j);
            else if (turn == 3) code = vernam(j);
            else if (turn == 4) code = RSA(j);

            data2 += decode(code);
        }
        writeName = (char) (turn + 48) + name;
        ofstream fout(writeName, std::ios::binary);
        fout << data2;

        cout << turn << endl;
    }
}
