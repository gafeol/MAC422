#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define mod(a) ((tam_pista + (a%tam_pista))%tam_pista)

#include "global.h"
#include "heap.h"

int primeiro_ciclista, segundo_ciclista;

double floor(double x){
	int base = (int)x;
	if(x > 0)
		return base;
	else
		return base - (int)!(x == base);
}

//#include "aleatorio.h"
int sorteio(int p)
{
	if(semente == 0) {
		srand((unsigned)time(NULL));
		semente = 1;
	}
	int x = rand() % 100;
	return (x < p) ? 1 : 0;
}

void sorteio_ciclista_sortudo()
{
	if(sorteio(10))
		ciclista_sortudo = (rand() % num_ciclistas);
	else
		ciclista_sortudo = -1;
}

//#include "pista.h"
void inicializa_pista(){
	pista = malloc(tam_pista*sizeof(matriz_pista));
	pista_aux = malloc(tam_pista*sizeof(matriz_pista));
	int i,j;
	for(i = 0;i < tam_pista;i++) {
		pista[i].raia = malloc(10*sizeof(int));
		pista_aux[i].raia = malloc(10*sizeof(int));
		for(j = 0; j < 10; j++){
			pista[i].raia[j] = -1;
			pista_aux[i].raia[j] = -1;
		}

		pista_aux[i].linha = malloc(sizeof(pthread_mutex_t));
		pista[i].linha = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(pista_aux[i].linha, NULL);
		pthread_mutex_init(pista[i].linha, NULL);
	}
}

void remove_ciclista_pista(int i)
{
	int raia = ciclistas[i].raia;
	int pos = mod((int)ciclistas[i].dist);
	printf("REMOVE DA PISTA %d que ta na pista[%d].raia %d = %d\n", i, pos, raia, pista[pos].raia[raia]);
	pista_aux[pos].raia[raia] = -1;
}

/*dir = -1 -> direita, 0 -> reto, 1 -> esquerda
void desloca_ciclista_pista(int i, int dir)
{
	int raia = ciclistas[i].raia;
	int pos = ((int)ciclistas[i].dist) % tam_pista;
	int ant_pos = (pos - 1 + tam_pista) % tam_pista;
	(pista[ant_pos].raia)[raia+dir] = -1;
	(pista[pos].raia)[raia] = i;
}
*/

//#include "ciclista.h"
void destroi_ciclista(int i){
	ciclistas[i].destruido = 1;
}

int testa_quebrou(int i){
	if(!ciclistas[i].completou_volta || ciclistas[i].voltas%15 != 0) return 0;
	if(ciclistas_ativos <= 5)
		return 0;
	if(sorteio(1)){ 
		ciclistas[i].destruido = 1;
		pthread_mutex_lock(quebrado);
		quebrou = 1;
		ciclistas_ativos--;
		pthread_mutex_unlock(quebrado);
		return 1;
	}
	return 0;
}

void sorteia_velocidade(int i)
{
	if(!ciclistas[i].completou_volta) return;
	printf("SORTEIA PRO I %d\n", i);
	if((num_voltas - ciclistas[i].voltas) == 2 && i == ciclista_sortudo) {
		ciclistas[i].velocidade = 90;
		printf("VELOCIDADE = 90\n");
		return;
	}
	if(ciclistas[i].velocidade == 30) {
			if(sorteio(70))
				ciclistas[i].velocidade = 60;
			else
				ciclistas[i].velocidade = 30;
	}
	else {
			if(sorteio(50))
				ciclistas[i].velocidade = 60;
			else
				ciclistas[i].velocidade = 30;
	}
	printf("VELOCIDADE = %d\n", ciclistas[i].velocidade);
}

double distancia_a_percorrer(int vel, int dt)
{
	switch(vel) {
		case 30:
			if(dt == 60)
				return 0.5;
			else
				return 0.5/3;
			break;
		case 60:
			if(dt == 60)
				return 1;
			else
				return 1.0/3;
			break;
		case 90:
			return 0.5;
	}
}

int devolve_pontuacao(int pos)
{
	switch(pos) {
		case 0:
			return 5;
		case 1:
			return 3;
		case 2:
			return 2;
		case 3:
			return 1;
		default:
			return 0;
	}
}

void ciclista_avanca(int i){
	int atual_pos = mod((int)floor(ciclistas[i].dist));
	int prox_pos = mod((int)floor(ciclistas[i].dist + distancia_a_percorrer(ciclistas[i].velocidade, dt)));
	printf("AVANCA %d atualpos %d prox pos %d\n", i, atual_pos, prox_pos);
	int raia = ciclistas[i].raia;

	if(atual_pos == prox_pos){
		ciclistas[i].dist += distancia_a_percorrer(ciclistas[i].velocidade, dt);
		pista_aux[atual_pos].raia[raia] = i;
		printf("cara %d andou %f\n", i, ciclistas[i].dist); 
		return;
	}

	if(pista_aux[prox_pos].raia[raia] != -1) { //tem alguém?
		int ciclista_frente = pista_aux[prox_pos].raia[raia];
		printf("ULTRAPASSAGEM: apressadinho %d lerdao %d\n", i, ciclista_frente);
		if((raia+1 <= 9) && (pista_aux[prox_pos].raia[raia+1] == -1)) { //é possível ultrapassar?
			printf("ULTRAPASSAGEM: do lado ta livre pos %d %d\n", prox_pos, raia+1);
			ciclistas[i].dist += distancia_a_percorrer(ciclistas[i].velocidade, dt);
			//desloca_ciclista_pista(i,-1);
			pista_aux[prox_pos].raia[raia+1] = i;
		}
		else { //nesse caso, o ciclista não se desloca
			printf("ULTRAPASSAGEM: do lado nao ta livre pos %d %d\n", prox_pos, raia+1);
			ciclistas[i].velocidade = ciclistas[ciclista_frente].velocidade;
			ciclistas[i].dist += distancia_a_percorrer(ciclistas[i].velocidade, dt);
			pista_aux[atual_pos].raia[raia] = i;
		}
	}
	else { //então pode ir sem medo de ser feliz
		printf("ULTRAPASSAGEM: nao tem ninguem na frente do cara %d\n", i);
		ciclistas[i].dist += distancia_a_percorrer(ciclistas[i].velocidade, dt);
		//desloca_ciclista_pista(i,0);
		pista_aux[prox_pos].raia[raia] = i;
	}

}

#include "queue.h"

Queue *resultados;
Queue *pontuacoes;
pthread_mutex_t **mutex_resultados;

int vai_rodar(int i){
	return !(ciclistas[i].destruido == 1 ||
				ciclistas[i].dist >= tam_pista*num_voltas);
}

void atualiza_posicoes(int i){
	if(num_ciclistas < 2) return ;

	int aux = primeiro_ciclista;
	if(i == primeiro_ciclista)
		return ;
	if(i == segundo_ciclista){
		if(ciclistas[i].dist > ciclistas[primeiro_ciclista].dist){
			primeiro_ciclista = i;
			segundo_ciclista = aux;
		}
		return ;
	}
	if(ciclistas[i].dist > ciclistas[primeiro_ciclista].dist){
		primeiro_ciclista = i;
		segundo_ciclista = aux;
	}
	else{
		if(ciclistas[i].dist > ciclistas[segundo_ciclista].dist)
			segundo_ciclista = i;
	}
}

void roda(int i){
	/* Thread vai simular dt ms da corrida para o ciclista i */
	int pos = (tam_pista + ((int)ciclistas[i].dist)%tam_pista)%tam_pista;

	/* Vetor de mutexes pra lockar a linha e a proxima pra quando eu for colocar meu cara na pista */
	if(pos == 0){
		pthread_mutex_lock(pista[mod(pos+1)].linha);
		pthread_mutex_lock(pista[mod(pos)].linha);
	}
	else{
		pthread_mutex_lock(pista[mod(pos)].linha);
		pthread_mutex_lock(pista[mod(pos+1)].linha);
	}
	/* OK! Atualizar dist, matriz de pista com a nova posição */
	ciclista_avanca(i);
	atualiza_posicoes(i);
	pthread_mutex_unlock(pista[mod(pos)].linha);
	pthread_mutex_unlock(pista[mod(pos+1)].linha);

	/* OK! Verificacao de 20 pontos se o ind[0] ta uma rodada a mais que o segundo maior (ind[1]) */

	/* Atualizar o lap e tempo */
	if(((int)ciclistas[i].dist)/tam_pista > ciclistas[i].voltas){
		printf("cara %d dist %lf tam pista %d voltas %d\n", i, ciclistas[i].dist, tam_pista, ciclistas[i].voltas); 
		ciclistas[i].voltas++;
		ciclistas[i].completou_volta = 1;

		int novalap = ciclistas[i].voltas;
		pthread_mutex_lock(mutex_resultados[novalap-1]);
		int pont = devolve_pontuacao(queue_size(resultados[novalap-1]));

		ciclistas[i].pontuacao += pont;

		if(novalap%10==0) 
			queue_push(pontuacoes[novalap-1], ciclistas[i].pontuacao);

		queue_push(resultados[novalap-1], i);
		pthread_mutex_unlock(mutex_resultados[novalap-1]);
		if(novalap == num_voltas){
			remove_ciclista_pista(i);
			ciclistas[i].tempo_chegada = tempo;
		}
	}

	if(testa_quebrou(i)){
		printf("QUEBROUUUUUUUUUUUUUUU %d\n", i);
		pthread_mutex_lock(pista[pos].linha);
		remove_ciclista_pista(i);
		pthread_mutex_unlock(pista[pos].linha);
	}
	sorteia_velocidade(i);
	ciclistas[i].completou_volta = 0;

	/* Thread sorteia velocidade da proxima rodada */
	//pthread_mutex_unlock(ciclistas[i].arrive);
	printf("%d parou no arrive\n", i);
	pthread_barrier_wait(arrive);
	printf("%d passou no arrive\n", i);
	printf("%d parou no continue\n", i);
	pthread_barrier_wait(cont);
	printf("%d passou no continue\n", i);
	if(ciclistas[i].destruido)
		return;
	if(queue_size(resultados[num_voltas-1]) == num_ciclistas)
		return ;


	//pthread_mutex_lock(ciclistas[i].cont);
}

void *run_process(void * ii){
	int i = *((int *)ii);
	printf("run process cara %d\n", i);
	while(1){
		if(vai_rodar(i))
			roda(i);
		else if(!ciclistas[i].destruido){
			printf("cara %d jacabou\n", i);
			pthread_barrier_wait(arrive);	
			pthread_barrier_wait(cont);
		}
		else 
			break;
	}
}

void create_thread(int i){
	ciclistas[i].arrive = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(ciclistas[i].arrive, NULL);
	pthread_mutex_lock(ciclistas[i].arrive);
	ciclistas[i].cont = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(ciclistas[i].cont, NULL);
	pthread_mutex_lock(ciclistas[i].cont);
	ciclistas[i].thread = malloc(sizeof(pthread_t));
	pthread_create(ciclistas[i].thread, NULL, run_process, ciclistas[i].id);
}

void inicializa_ciclistas(){
	int n = num_ciclistas;
	ciclistas = malloc(n*(sizeof(ciclista)));

	int i;
	for(i=0;i<n;i++){
		ciclistas[i].id = malloc(sizeof(int));
		*ciclistas[i].id = i;
		ciclistas[i].dist = -(i/10);
		ciclistas[i].voltas = 0;
		ciclistas[i].tempo = 0;
		ciclistas[i].raia = (i%10);
		ciclistas[i].velocidade = 30;
		ciclistas[i].destruido = 0;
		ciclistas[i].terminou = 0;
		ciclistas[i].completou_volta = 0;
		ciclistas[i].pontuacao = 0;
		ciclistas[i].max_volta_extra = 0;
		ciclistas[i].tempo_chegada = 0;
		create_thread(i);
	}
}

int cmp(const void *aa, const void *bb){
	int a = *(int *)aa;
	int b = *(int *)bb;
	return (ciclistas[a].dist > ciclistas[b].dist || (ciclistas[a].dist == ciclistas[b].dist && ciclistas[a].raia < ciclistas[b].raia));
}

void barreira_threads(){
	pthread_barrier_wait(arrive);
	//atualizar destruidos
	if(quebrou) {
		pthread_barrier_destroy(arrive);
		pthread_barrier_init(arrive, NULL, ciclistas_ativos+1);
	}
	/*
	   int i;
	   for(i = 0;i < num_ciclistas;i++){
	   if(!vai_rodar(i)) continue;
	   pthread_mutex_lock(ciclistas[i].arrive);
	   }
	 */
}

void libera_threads(){
	pthread_barrier_wait(cont);
	if(quebrou) {
		pthread_barrier_destroy(cont);
		printf("INICIA CONT com %d\n", ciclistas_ativos+1);
		pthread_barrier_init(cont, NULL, ciclistas_ativos+1);
		quebrou = 0;
	}
	/*
	   int i;
	   for(i = 0;i < num_ciclistas;i++){
	   if(!vai_rodar(i)) cotinue;
	   pthread_mutex_unlock(ciclistas[i].cont);
	   }
	 */
}

int main(int argc, char* argv[]){
	tam_pista = atoi(argv[1]);
	ciclistas_ativos = num_ciclistas = atoi(argv[2]);
	num_voltas = atoi(argv[3]);
	volta_atual = 1;
	quebrou = 0;

	primeiro_ciclista = 0;
	segundo_ciclista = 1;

	dt = 60;

	if(argc > 4)
		debug = (argv[4][0] == 'd' ? 1 : 0);

	imprvolta = 0;
	volta = 0;

	arrive = malloc(sizeof(pthread_barrier_t));
	pthread_barrier_init(arrive, NULL, num_ciclistas+1);
	cont = malloc(sizeof(pthread_barrier_t));
	pthread_barrier_init(cont, NULL, num_ciclistas+1);

	inicializa_pista();

	/* Queue e mutex de resultados */
	resultados = malloc(num_voltas*sizeof(Queue));
	pontuacoes = malloc(num_voltas*sizeof(Queue));
	mutex_resultados = malloc(num_voltas*sizeof(pthread_mutex_t*));
	for(int i = 0;i < num_voltas;i++){
		resultados[i] = queue_create();
		pontuacoes[i] = queue_create();
		mutex_resultados[i] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(mutex_resultados[i], NULL);
	}

	/* Inicializa mutex para ciclista_ativos */
	quebrado = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(quebrado, NULL);

	ciclistas = malloc(num_ciclistas*(sizeof(ciclista)));

	inicializa_ciclistas();

	sorteio_ciclista_sortudo();

	while(volta < num_voltas+1){
		printf("volta_atual %d num_voltas %d\n", volta_atual, num_voltas);
		tempo += dt;
		printf("Queue volta atual %d tam:%d\n", volta_atual-1,  queue_size(resultados[volta_atual-1]));
		printf("Coordenador parou no arrive ativos %d\n", ciclistas_ativos);
		barreira_threads();
		printf("Coordenador passou no arrive\n");
		volta = 0x3f3f3f3f;	
		for(int i=0;i<num_ciclistas;i++){
			if(vai_rodar(i)){
				if(ciclistas[i].voltas < volta)
					volta = ciclistas[i].voltas;
			}
		}

		for(int i=0;i<tam_pista;i++){
			for(int j=0;j<10;j++){
				pista[i].raia[j] = pista_aux[i].raia[j];
				pista_aux[i].raia[j] = -1;
			}
		}

		/* Printa o debug */
		if(debug){
			for(int i=tam_pista-1;i>=0;i--){
				for(int r=0;r<10;r++){
					if(pista[i].raia[r] == -1)
						fprintf(stderr, "%3c", 'X');
					else
						fprintf(stderr, "%3d", pista[i].raia[r]);
				}
				fprintf(stderr, "\n");
			}
			fprintf(stderr, "==========================\n");
		}


		/* Verificacao de pontos por volta extra */
		int voltas_extra = (ciclistas[primeiro_ciclista].dist - ciclistas[segundo_ciclista].dist - 1)/tam_pista;
		printf("voltas extra %d = (%f - %f - 1)/%d\n", voltas_extra, ciclistas[primeiro_ciclista].dist, ciclistas[segundo_ciclista].dist, tam_pista);
		if(ciclistas[primeiro_ciclista].max_volta_extra < voltas_extra){
			printf("RELAMPAGO MARQUINHOS de indice %d\n", primeiro_ciclista);
			ciclistas[primeiro_ciclista].max_volta_extra++;
			ciclistas[primeiro_ciclista].pontuacao += 20;
		}

		while(imprvolta < num_voltas && volta >= imprvolta) {
			int colocacao = 1;
			int flag_acc = (imprvolta%10 == 9 ? 1 : 0); //é uma volta múltipla de 10?
			printf("Volta %d:\n", imprvolta);
			if(flag_acc == 0) { // se não for, é só partir para o abraço
				while(!queue_empty(resultados[imprvolta])) {
					int atual = head(resultados[imprvolta]);
					queue_pop(resultados[imprvolta]);
					//if(imprvolta%10 != 0)
					printf("Colocacao %d: %d\n", colocacao++, atual);
					//else {
					//int pont = head(pontuacoes[imprvolta-1]);
					//queue_pop(pontuacoes[imprvolta-1]);
					//printf("Colocacao %d: %d %d\n", colocacao++, atual, pont++);
					//}
				}
			}
			else { //aí o negócio complica
				Heap hp = heap_create();
				while(!queue_empty(resultados[imprvolta])) {
					int atual = head(resultados[imprvolta]);
					queue_pop(resultados[imprvolta]);
					printf("passou\n");
					int pont = head(pontuacoes[imprvolta]);
					queue_pop(pontuacoes[imprvolta]);
					heap_push(hp, -pont, atual);				
				}
				while(!heap_empty(hp)){
					int atual = heap_min_element(hp);
					int pont = -heap_min_priority(hp);
					heap_pop(hp);
					printf("Colocacao %d: %d %d\n", colocacao++, atual, pont);
				}
			}
			imprvolta++;
		}
		printf("Coordenador parou no continue\n");
		libera_threads();
		printf("Coordenador passoou no continue\n");
		printf("Sincronizou!\n");
	}
	for(int a=0;a<num_ciclistas;a++){
		printf("Ciclista %d: ", a);
		if(ciclistas[a].destruido)
			printf("destruido na volta %d\n", ciclistas[a].voltas);
		else
			printf("terminou a corrida em %d ms\n", ciclistas[a].tempo_chegada);
	}
	return 0;
}

/*
   TODO
   Barreiras na run process
   Empacotamento na esquerda

   Testar cara de 90
   Testar ultrapassagem
 */
