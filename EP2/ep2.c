#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define mod(a) ((tam_pista + (a%tam_pista))%tam_pista)


#include "global.h"

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
	int pos = ((int)ciclistas[i].dist)%tam_pista;
	(pista[pos].raia)[raia] = -1;
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
	int ciclistas_ativos = 0;
	int j;
	for(j = 0;ciclistas[j].id != NULL;j++){
		ciclistas_ativos += 1 - ciclistas[j].destruido;
	}
	if(ciclistas_ativos <= 5)
		return 0;
	if(sorteio(1)){
		ciclistas[i].destruido = 1;
		return 1;
	}
	return 0;
}

void sorteia_velocidade(int i)
{
	if(!ciclistas[i].completou_volta) return;
	if((num_voltas - ciclistas[i].voltas) == 2 && i == ciclista_sortudo) {
		ciclistas[i].velocidade = 90;
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

void ciclista_avanca(int i)
{
	int atual_pos = mod((int)ciclistas[i].dist);
	int prox_pos = mod((int)(ciclistas[i].dist + distancia_a_percorrer(ciclistas[i].velocidade, dt)));
	int raia = ciclistas[i].raia;

	if(atual_pos == prox_pos){
		ciclistas[i].dist += distancia_a_percorrer(ciclistas[i].velocidade, dt);
		pista_aux[atual_pos].raia[raia] = i;
		printf("cara %d andou %f\n", i, ciclistas[i].dist); 
		return ;
	}

	if(pista_aux[prox_pos].raia[raia] != -1) { //tem alguém?
		int ciclista_frente = pista_aux[prox_pos].raia[raia];
		if((raia+1 <= 9) && (pista_aux[prox_pos].raia[raia+1] == -1)) { //é possível ultrapassar?
			ciclistas[i].dist += distancia_a_percorrer(ciclistas[i].velocidade, dt);
			//desloca_ciclista_pista(i,-1);
			pista_aux[prox_pos].raia[raia+1] = i;
		}
		else { //nesse caso, o ciclista não se desloca
			ciclistas[i].velocidade = ciclistas[ciclista_frente].velocidade;
			ciclistas[i].dist += distancia_a_percorrer(ciclistas[i].velocidade, dt);
			pista_aux[atual_pos].raia[raia] = i;
		}
	}
	else { //então pode ir sem medo de ser feliz
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


void *run_process(void * ii){
	int i = *((int *)ii);
	printf("run process cara %d\n", i);

	while(vai_rodar(i)){
		printf("entra while %d\n", i);
		/* Thread vai simular dt ms da corrida para o ciclista i */
		int pos = (tam_pista + ((int)ciclistas[i].dist)%tam_pista)%tam_pista;

		/* Vetor de mutexes pra lockar a linha e a proxima pra quando eu for colocar meu cara na pista */

		pthread_mutex_lock(pista[pos].linha);
		pthread_mutex_lock(pista[(pos+1)%tam_pista].linha);
		/* Atualizar dist, matriz de pista com a nova posição */
		ciclista_avanca(i);
		pthread_mutex_unlock(pista[pos].linha);
		pthread_mutex_unlock(pista[(pos+1)%tam_pista].linha);

		/* Verificacao de 20 pontos se o ind[0] ta uma rodada a mais que o segundo maior (ind[1]) */
		printf("PA!\n");
		printf("%d parou no arrive\n", i);

		/* Atualizar o lap e tempo */
		if(((int)ciclistas[i].dist)/tam_pista > ciclistas[i].voltas){
			printf("cara %d dist %lf tam pista %d voltas %d\n", i, ciclistas[i].dist, tam_pista, ciclistas[i].voltas); 
			ciclistas[i].voltas++;
			ciclistas[i].completou_volta = 1;

			int novalap = ciclistas[i].voltas;
			/* Colocar o cara na lista ligada da lap ciclistas[i].volta */
			pthread_mutex_lock(mutex_resultados[novalap]);
			queue_push(resultados[novalap], i);
			printf("Ciclista %d terminou como o %d-esimo da %d-esima volta\n", i, queue_size(resultados[novalap]), novalap-1);
			pthread_mutex_unlock(mutex_resultados[novalap]);
		}

		if(testa_quebrou(i)){
			pthread_mutex_lock(pista[pos].linha);
			remove_ciclista_pista(i);
			pthread_mutex_unlock(pista[pos].linha);
		}
		sorteia_velocidade(i);

		/* Thread sorteia velocidade da proxima rodada */
		ciclistas[i].completou_volta = 0;
		pthread_mutex_unlock(ciclistas[i].arrive);
		printf("%d passou no arrive\n", i);
		printf("%d parou no continue\n", i);
		pthread_mutex_lock(ciclistas[i].cont);
		printf("%d passou no continue\n", i);
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
		create_thread(i);

	}
}

int cmp(const void *aa, const void *bb){
	int a = *(int *)aa;
	int b = *(int *)bb;
	return (ciclistas[a].dist > ciclistas[b].dist || (ciclistas[a].dist == ciclistas[b].dist && ciclistas[a].raia < ciclistas[b].raia));
}

void barreira_threads(){
	int i;
	for(i = 0;i < num_ciclistas;i++){
		if(!vai_rodar(i)) continue;
		pthread_mutex_lock(ciclistas[i].arrive);
	}
}

void libera_threads(){
	int i;
	for(i = 0;i < num_ciclistas;i++){
		if(!vai_rodar(i)) continue;
		pthread_mutex_unlock(ciclistas[i].cont);
	}
}

int main(int argc, char* argv[]){
	tam_pista = atoi(argv[1]);
	num_ciclistas = atoi(argv[2]);
	num_voltas = atoi(argv[3]);
	volta_atual = 1;

	dt = 60;

	if(argc > 4)
		debug = (argv[4][0] == 'd' ? 1 : 0);

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

	ciclistas = malloc(num_ciclistas*(sizeof(ciclista)));

	inicializa_ciclistas();

	sorteio_ciclista_sortudo();



	while(volta_atual != num_voltas){
	//	printf("Coordenador parou no arrive\n");
		printf("%d\n", volta_atual);
		printf("tam:%d\n", queue_size(resultados[volta_atual-1]));
		barreira_threads();
		
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
						printf("%3c", 'X');
					else
						printf("%3d", pista[i].raia[r]);
				}
				putchar('\n');
			}
		}


		if(queue_size(resultados[volta_atual-1]) == num_ciclistas) {
			int colocacao = 1;
			printf("Volta %d:\n", volta_atual);
			while(!queue_empty(resultados[volta_atual-1])) {
				int atual = head(resultados[volta_atual-1]);
				queue_pop(resultados[volta_atual-1]);
				printf("Colocacao %d: %d\n", colocacao++, atual);
			}
			volta_atual++;
		}
	//	printf("Coordenador parou no continue\n");
		libera_threads();
		/* Libera os de 30
			libera queue
			libera 60
			libera 90
			*/

		/* Compacta caras na esquerda */

	//	printf("Coordenador passoou no continue\n");
		//printf("Sincronizou!\n");
	}
	return 0;
}

/* TODO
	Criar queues de colocacoes
	Fazer a contagem de pontos por volta
	Verificacao de size de queue = n pra imprimir e dar free
*/
