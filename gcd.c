#include <stdlib.h>
#include <stdio.h>

unsigned long long int gcd(unsigned long long int a, unsigned long long int b){
    if (a == 0){
        return b;
    }
    else {
        gcd(b % a, a);
    }
}

int main(){
    int n;
    unsigned long long int a, b, res;
    scanf("%d", &n);
    for (int i = 0; i < n; i++){
        scanf("%llu %llu", &a, &b);
        res = gcd(a, b);
        if (i + 1 != n) printf("%llu\n", res);
        else printf("%llu", res);
    }
    return 0;
}