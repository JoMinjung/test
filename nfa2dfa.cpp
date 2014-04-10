/*
	NFA --> DFA conversion program

	국민대학교 컴퓨터공학부 강승식
	Email: sskang@kookmin.ac.kr
	컴파일러 과목 카페 -- http://cafe.daum.net/sskang-compiler
	홈페이지 -- http://nlp.kookmin.ac.kr/
*/
#include <stdio.h>
#include <string.h>

#define STATES	256
#define SYMBOLS	20

int N_symbols;	/* number of input symbols */
int NFA_states;	/* number of NFA states */
int DFA_states;	/* number of DFA states */
int Final_states[STATES];
int NFAtab[STATES][SYMBOLS][STATES];
int DFAtab[STATES][SYMBOLS];
int DFinal_states[STATES];
int p = 0;

/*
	Print state-transition table.
	State names: 'A', 'B', 'C', ...
*/
void put_dfa_table(
	int tab[][SYMBOLS],	/* DFA table */
	int nstates,	/* number of states */
	int nsymbols)	/* number of input symbols */
{
	int i, j;

	puts("DFA: STATE TRANSITION TABLE");

	/* input symbols: '0', '1', ... */
	printf("    | ");
	for (i = 0; i < nsymbols; i++) printf("\t%c", '0'+i);

	printf("\n----+--");
	for (i = 0; i < nsymbols; i++) printf("-------");
	printf("\n");

	for (i = 0; i < nstates; i++) {
		printf("  %c | ", 'A'+i);	/* state */
		for (j = 0; j < nsymbols; j++)
			printf("\t%c", 'A'+tab[i][j]);
		printf("\n");
	}

	printf("final states { ");
	for(i=0; i<STATES; i++){
		if(DFinal_states[i] != -1)
			printf("%c ", i+'A');
	}
	printf("}\n\n");
}

void put_nfa_table( 
	int tab[STATES][SYMBOLS][STATES],	/* NFA table */
	int nstates,	/* number of states */
	int nsymbols)	/* number of input symbols */
{
	int i, j;
	int k;

	puts("NFA: STATE TRANSITION TABLE");

	/* input symbols: '0', '1', ... */
	printf("    | ");
	for (i = 0; i < nsymbols; i++) printf("\t%d", i);

	printf("\n----+--");
	for (i = 0; i < nsymbols; i++) printf("-------");
	printf("\n");

	for (i = 0; i < nstates; i++) {
		printf("  %d |", i);	/* state */
		for (j = 0; j < nsymbols; j++){
			printf("\t");
			for(k = 0; k < nstates; k++){
				if(tab[i][j][k] < 0 )
					break;
				printf("%d", tab[i][j][k]);
			}
		}
		printf("\n");
	}

	printf("final states {");
	for(i=0; i<STATES; i++){
		if(Final_states[i] == -1)
			break;

		printf(" %d", Final_states[i]);
	}
	printf(" }\n");
	printf("\n");
}

/*
	Initialize NFA table.
*/
void init_NFA_table()
{
/*
	Sample NFA table -- 1번째

	NFAtab[0][0] = "01";
	NFAtab[0][1] = "0";
	NFAtab[1][0] = "";
	NFAtab[1][1] = "01";

	NFA_states = 2;
	DFA_states = 0;
	N_symbols = 2;
	// Final states = { 1 } 로 해서 기능 추가
*/
/*
	Sample NFA table -- 2번째
*/
	int *NFAtabPoint = NFAtab[0][0];
	int *FSp = Final_states;
	int *DFSp = DFinal_states;
	int i;

	for(i=0; i<STATES*SYMBOLS*STATES; i++)
		*NFAtabPoint++ = -1;
	for(i=0; i<STATES; i++)
		*FSp++ = -1;
	for(i=0; i<STATES; i++)
		*DFSp++ = -1;

	NFAtab[0][0][0] = 1;
	NFAtab[0][0][1] = 2;

	NFAtab[0][1][0] = 1;
	NFAtab[0][1][1] = 3;

	NFAtab[1][0][0] = 1;
	NFAtab[1][0][1] = 2;

	NFAtab[1][1][0] = 1;
	NFAtab[1][1][1] = 3;

	NFAtab[2][0][0] = 4;

	NFAtab[2][1][0] = -1;

	NFAtab[3][0][0] = -1;

	NFAtab[3][1][0] = 4;

	NFAtab[4][0][0] = 4;

	NFAtab[4][1][0] = 4;

	NFA_states = 5;
	DFA_states = 0;
	N_symbols = 2;
	//Final_states = { 3, 4 } 로 해서 기능 추가
	
	Final_states[0] = 3;
	Final_states[1] = 4;
}

/*
	String 't' is merged into 's' in an alphabetical order.
*/
void merge(int *s, int *t)
{
	int temp[STATES], *r=temp, *p=s, *m=t;
	int i;

	for(i=0; i<STATES; i++)
		temp[i] = -1;

	while(*p != -1 || *m != -1 ){	
		if(*p != -1 && *m != -1){
			if(*p > *m)
				*r++ = *m++;
			else if(*p < *m)
				*r++ = *p++;
			else{
				*r++ = *p++;
				*m++;
			}
		}
		else if(*p == -1)
			*r++ = *m++;
		else if(*m == -1)
			*r++ = *p++;
	}

	for(i=0; i<STATES; i++)
		s[i] = temp[i];
}

/*
	Get next-state string for current-state string.
	(state 스트링이므로 각 state에 대해 nextstate를 merge)
*/
void get_next_state(int *nextstates, int *cur_states,
	int nfa[STATES][SYMBOLS][STATES], int n_nfa, int symbol)
{
	int i;
	int temp[STATES];

	for(i=0; i<STATES; i++)
		temp[i] = -1;

	for(i = 0; i < STATES; i++){
		if(cur_states[i]==-1)
			break;

		merge(temp, nfa[cur_states[i]][symbol]);
	}

	for(i=0; i<STATES; i++)
		nextstates[i] = temp[i];
}

int statecmp(int *state, int *statename)
{
	int i;
	for(i=0; i<STATES; i++){
		if(state[i] != statename[i])
			return 1;

		if (state[i] == -1)
			break;
	}
	return 0;
}
/*
	statename 테이블에서 'state'를 찾아 index를 return.
	'state'가 테이블에 없으면 끝에 추가하고 index를 return.
*/
int state_index(int *state, int statename[][STATES], int *pn)
{
	int i, j;

	if(state[0] == -1) return -1;	/* no next state */

	for (i = 0; i < *pn; i++)
		if(!statecmp(state, statename[i])) return i;

	for(j=0; j<STATES; j++)
		statename[i][j] = state[j];	/* new state-name */

	return (*pn)++;
}

/*
	Convert NFA table to DFA table.
	Method:
		0. state-name이 스트링이므로 statename 테이블 이용
		   'n' -- statename[]에 등록된 state 개수
		1. DFA table의 entry 개수를 1로 초기화 및 statename에 추가
		2. statename[i]의 각 symbol들에 대해 nextstate 계산
		3. nextstate가 스트링이므로 statename의 index를 DFA에 넣음
	Return value: number of DFA states.
*/
int nfa_to_dfa(int nfa[STATES][SYMBOLS][STATES], int n_nfa,
	int n_sym, int dfa[][SYMBOLS])
{
	int statename[STATES][STATES];
	int i = 0;	/* current index of DFA */
	int n = 1;	/* number of DFA states */

	int nextstate[STATES];
	int j;
	int k, l;

	for(k=0; k<STATES; k++){
		for(l=0; l<STATES; l++)
			statename[k][l] = -1;
		nextstate[k] = -1;
	}

	statename[0][0]=0;	/* start state */

	for (i = 0; i < n; i++) {	/* for each DFA state */
		for (j = 0; j < n_sym; j++) {	/* for each input symbol */
			get_next_state(nextstate, statename[i], nfa, n_nfa, j);
			dfa[i][j] = state_index(nextstate, statename, &n);
		}
	}

	for(i=0; i<n; i++){
		for(j=0; j<STATES; j++){
			for(k=0; k<STATES; k++){
				if(DFinal_states[i] == 1)
					break;

				if((statename[i][j]!=-1 && Final_states[k]!=-1) && statename[i][j] == Final_states[k])
					DFinal_states[i] = 1;
			}
			if(DFinal_states[i] == 1)
				break;
		}
	}

	return n;	/* number of DFA states */
}

void Final_State()
{
	char ch;

	while(ch = getchar()){
		if(ch == 10)
			break;

		p = DFAtab[p][ch-'0'];

		if(p == -1)
			break;
	}

	if(DFinal_states[p] == 1){
		puts("YES");
		return;
	}
	puts("NO");
}
void main()
{
	init_NFA_table();
	put_nfa_table(NFAtab, NFA_states, N_symbols);

	DFA_states = nfa_to_dfa(NFAtab, NFA_states, N_symbols, DFAtab);
	put_dfa_table(DFAtab, DFA_states, N_symbols);
	Final_State();
}
