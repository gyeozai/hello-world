#include <stdio.h>
#include <ctype.h>
#include <set>

char readchar() {
    static char buf[1048576], *p = buf, *q = buf;
    if(p == q && (q = (p=buf)+fread(buf,1,1048576,stdin)) == buf) return EOF;
    return *p++;
}

void nextstr(char *str) {
    char c = readchar();
    while(isspace(c) && c != EOF) c = readchar();
    int i = 0;
    while(!isspace(c) && c != EOF) str[i++] = c, c = readchar();
    str[i] = '\0'; return;
}

void nextlong(long long *num) {
    long long c = readchar(), neg = 0; *num = 0;
    while(('0' > c || c > '9') && c!='-' && c!=EOF) c = readchar();
    if(c == '-') neg = 1, c = readchar();
    while('0' <= c && c <= '9') *num = *num*10 + (c^'0'), c = readchar();
    if(neg) *num = *num * -1; return;
}

long long box[500000];
void init_box(long long m){
    for (long long i=0; i<m; i++){
        box[i] = 0;
    }
}

std::multiset<long long> kiwi;
auto mid = kiwi.begin();
long long sum;
void insert_kiwi(long long m){
    if (kiwi.size() == 0){
        kiwi.insert(m);
        mid = kiwi.begin();
        return;
    }
    if (m > *mid){
        kiwi.insert(m);
        if (kiwi.size() % 2 != 0){
            mid++;
            sum = sum + m - *mid;
        } else {
            sum = sum + m - *mid;
        }
    } else if (m < *mid){
        kiwi.insert(m);
        if (kiwi.size() % 2 != 0){
            sum = sum -m + *mid;
        } else {
            sum = sum -m + *mid;
            mid--;
        }
    } else {
        kiwi.insert(std::next(mid), m);
        if (kiwi.size() % 2 != 0){
            mid++;
        }
    }
    return;
}

void remove_kiwi(long long m){
    if (kiwi.size() == 1){
        kiwi.erase(m);
        return;
    }
    if (m > *mid){
        kiwi.erase(kiwi.find(m));
        if (kiwi.size() % 2 != 0){
            sum = sum - m + *mid;
        } else {
            sum = sum - m + *mid;
            mid--;
        }
    } else if (m < *mid){
        kiwi.erase(kiwi.find(m));
        if (kiwi.size() % 2 != 0){
            mid++;
            sum = sum + m - *mid;
        } else {
            sum = sum + m - *mid;
        }
    } else {
        auto kill = mid;
        if (kiwi.size() % 2 == 0){
            mid++;
            sum = sum + m - *mid;
        } else {
            mid--;
        }
        kiwi.erase(kill);
    }
    return;
}

int main (){
    char cmd[5];
    long long Q, m;
    nextlong(&Q);
    while (Q--){
        nextstr(cmd);
        if (cmd[0] == 'k'){
            if (cmd[2] == 'k'){ // kiki: merge
                nextlong(&m);
                if (m == kiwi.size()){
                    // do nothing
                } else if (m > kiwi.size() / 2){
                    long long n_kiwi = kiwi.size();
                    auto curr1 = kiwi.begin();
                    for (long long i=0; i<n_kiwi-m; i++){
                        box[i] += *curr1;
                        sum += *curr1;
                        curr1 = kiwi.erase(curr1);
                    }
                    for (long long i=0; i<2*m-n_kiwi; i++){
                        curr1++;
                    }
                    for (long long i=0; i<n_kiwi-m-1; i++){
                        box[i] += *curr1;
                        sum -= *curr1;
                        curr1 = kiwi.erase(curr1);
                    }
                    box[n_kiwi-m-1] += *curr1;
                    sum -= *curr1;
                    kiwi.erase(curr1);
                    for (long long i=0; i<n_kiwi-m; i++){
                        insert_kiwi(box[i]);
                    }
                    init_box(n_kiwi-m);
                } else {
                    long long idx = 0;
                    for (const auto &element : kiwi){
                        box[idx++] += element;
                        if (idx == m){
                            idx = 0;
                        }
                    }
                    kiwi.clear();
                    sum = 0;
                    for (long long i=0; i<m; i++){
                        insert_kiwi(box[i]);
                    }
                    init_box(m);
                }
            } else { // kiwi: insert
                nextlong(&m); 
                insert_kiwi(m);
            }
        } else {
            if (cmd[2] == 'w'){ // wiwi: find_min
                printf("%lld\n", sum);
            } else { // wiki: remove
                nextlong(&m);
                remove_kiwi(m);
            }
        }
        // printf("[%lld, %lld, %d] ", sum, *mid, kiwi.size());
    }
    return 0;
}