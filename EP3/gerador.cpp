/*
  Gerador de trace
  Seta a memoria virtual grande pra nao ter problema de falta
  de espaco.
*/

#include <bits/stdc++.h>
using namespace std;

const int VIRT_MEM = 1048576; // memoria virtual;
const int PHYS_MEM = 256;     // memoria fisica (pequeno para forcar page faults)
const int MAXB = 32;          // tamanho maximo de um processo
const int MAXT = 100;         // duracao maxima de um processo
const int MAXA = 500;          // numero maximo de acessos de um processo
const int S = 2, P = 8;

typedef pair<int, int> pii;

struct Process {
    int t0, tf, b;
    string name;
    vector<pii> mem_access;
    bool operator< (const Process &that) const {
        return t0 < that.t0;
    }
};

int main(int argc, char* argv[]) {
    int N, T;
    int seed = time(0);
    sscanf(argv[1], "%d", &N); // numero de processos
    sscanf(argv[2], "%d", &T); // tempo maximo de simulacao
    if (argc > 3) sscanf(argv[3], "%d", &seed);
    srand(seed);

    vector<Process> processes;
    for (int i = 0; i < N; i++) {
        Process proc;
        proc.name = "proc" + to_string(i + 1);
        proc.t0 = rand() % T;
        int length = min(T - proc.t0, rand() % MAXT + 1);
        proc.tf = proc.t0 + length;
        proc.b = rand() % MAXB + 1;
        int n_access = rand() % MAXA + 1;
        for (int i = 0; i < n_access; i++) {
            int t = rand() % length;
            int p = rand() % proc.b;
            proc.mem_access.push_back(pii(proc.t0 + t, p));
        }
        sort(proc.mem_access.begin(), proc.mem_access.end());
        processes.push_back(proc);
    }

    sort(processes.begin(), processes.end());
    printf("%d %d %d %d\n", PHYS_MEM, VIRT_MEM, S, P);
    for (const Process &proc: processes) {
        printf("%d %d %d %s", proc.t0, proc.tf, proc.b, proc.name.c_str());
        for (const pii &access: proc.mem_access)
            printf(" %d %d", access.second, access.first);
        printf("\n");
    }
    return 0;
}