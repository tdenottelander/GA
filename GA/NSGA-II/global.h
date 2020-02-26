/* This file contains the variable and function declarations */

# ifndef _GLOBAL_H_
# define _GLOBAL_H_

# define INF 1.0e14
# define EPS 1.0e-14
# define E  2.71828182845905
# define PI 3.14159265358979
# define GNUPLOT_COMMAND "gnuplot -persist"

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-= Section Constants -=-=-=-=-=-=-=-=-=-=-=-=-=-*/
#define FALSE 0
#define TRUE 1

#define KILOMETER 1
#define METER 2

#define CABLE 1
#define TRANSFORMER 2
#define MV_TRANS_CABLE 3

#define BR_FROM 0
#define BR_TO 1
#define BR_LENGTH 2
#define BR_CATEGORY 3
#define BR_LOWER_BOUND 4
#define BR_UPPER_BOUND 5
#define BRANCH_DATA_NCOL 6

#define SMALL 1.0e-8
//#define PI 3.1415926

#define BUS_NCOL 13
#define GEN_NCOL 21
#define BR_NCOL 15

#define PQ_BUS 1
#define PV_BUS 2
#define REF_BUS 3

#define BUS_ID 0
#define BUS_TYPE 1
#define PD 2
#define QD 3
#define GS 4
#define BS 5
#define BUS_AREA 6
#define VM 7
#define VA 8
#define BASE_KV 9
#define ZONE 10
#define VMAX 11
#define VMIN 12

#define GEN_BUS 0
#define PG 1
#define QG 2
#define QMAX 3
#define QMIN 4
#define VG 5

#define F_BUS 0
#define T_BUS 1
#define BR_R 2
#define BR_X 3
#define BR_B 4
#define RATE_A 5
#define RATE_B 6
#define RATE_C 7
#define TAP 8
#define SHIFT 9
#define BR_STATUS 10
#define ANGMIN 11
#define ANGMAX 12
#define BR_CAT 13
#define BR_TYPE 14

#define PF 0
#define QF 1
#define PT 2
#define QT 3
#define PL 4	// Active Power Loss

#define CABLE_NCOL 6
#define CABLE_ID 0
#define CABLE_R 1
#define CABLE_X 2
#define CABLE_C 3
#define CABLE_INOM 4
#define CABLE_COST 5

#define TRANSFORMER_NCOL 8
#define TRANSFORMER_ID 0
#define TRANSFORMER_UNOM_1 1
#define	TRANSFORMER_UNOM_2 2
#define TRANSFORMER_SNOM 3
#define TRANSFORMER_ZEQ 4
#define TRANSFORMER_R 5
#define TRANSFORMER_X 6
#define TRANSFORMER_COST 7

#define UNIVARIATE 1
#define MPM 2
#define LINKAGE_TREE 3
#define LINKAGE_NEIGHBOR 4
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

typedef struct
{
    int rank;
    double constr_violation;
    double *xreal;
    int **gene;
    double *xbin;
    double *obj;
    double *constr;
    double crowd_dist;
    void print(){
        for (int j = 0; j < 2; j++){
            printf("%e\t", obj[j]);
        }
        for (int j = 0; j < 5; j++){
            printf("%d\t", gene[j][0]);
        }
        printf("%e\t", constr_violation);
        printf("%d\t", rank);
        printf("%e\n", crowd_dist);
    }
}
individual;

typedef struct
{
    individual *ind;
}
population;

typedef struct lists
{
    int index;
    struct lists *parent;
    struct lists *child;
}
list;

//extern int nreal;
//extern int nbin;
//extern int nobj;
//extern int ncon;
//extern int popsize;
//extern double pcross_real;
//extern double pcross_bin;
//extern double pmut_real;
//extern double pmut_bin;
//extern double eta_c;
//extern double eta_m;
//extern int ngen;
//extern int nbinmut;
//extern int nrealmut;
//extern int nbincross;
//extern int nrealcross;
//extern int *nbits;
//extern double *min_realvar;
//extern double *max_realvar;
//extern double *min_binvar;
//extern double *max_binvar;
//extern int bitlength;
//extern int choice;
//extern int obj1;
//extern int obj2;
//extern int obj3;
//extern int angle1;
//extern int angle2;

// int nreal;
// int nbin;
// int nobj;
// int ncon;
// int popsize;
// double pcross_real;
// double pcross_bin;
// double pmut_real;
// double pmut_bin;
// double eta_c;
// double eta_m;
// int ngen;
// int nbinmut;
// int nrealmut;
// int nbincross;
// int nrealcross;
// int *nbits;
// double *min_realvar;
// double *max_realvar;
// double *min_binvar;
// double *max_binvar;
// int bitlength;
// int choice;
// int obj1;
// int obj2;
// int obj3;
// int angle1;
// int angle2;

//int mainNSGA();
int mainNSGA (int nbinary, int populationSize, float seed);

void allocate_memory_pop (population *pop, int size);
void allocate_memory_ind (individual *ind);
void deallocate_memory_pop (population *pop, int size);
void deallocate_memory_ind (individual *ind);

double maximum (double a, double b);
double minimum (double a, double b);

void crossover (individual *parent1, individual *parent2, individual *child1, individual *child2);
void realcross (individual *parent1, individual *parent2, individual *child1, individual *child2);
void bincross (individual *parent1, individual *parent2, individual *child1, individual *child2);

void assign_crowding_distance_list (population *pop, list *lst, int front_size);
void assign_crowding_distance_indices (population *pop, int c1, int c2);
void assign_crowding_distance (population *pop, int *dist, int **obj_array, int front_size);

void decode_pop (population *pop);
void decode_ind (individual *ind);

void onthefly_display (population *pop, FILE *gp, int ii);

int check_dominance (individual *a, individual *b);

void evaluate_pop (population *pop);
void evaluate_ind (individual *ind);

void fill_nondominated_sort (population *mixed_pop, population *new_pop);
void crowding_fill (population *mixed_pop, population *new_pop, int count, int front_size, list *cur);

void initialize_pop (population *pop);
void initialize_ind (individual *ind);

void insert (list *node, int x);
list* del (list *node);

void merge(population *pop1, population *pop2, population *pop3);
void copy_ind (individual *ind1, individual *ind2);

void mutation_pop (population *pop);
void mutation_ind (individual *ind);
void bin_mutate_ind (individual *ind);
void real_mutate_ind (individual *ind);

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr);

void assign_rank_and_crowding_distance (population *new_pop);

void report_pop (population *pop, FILE *fpt);
void print_pop(population *pop);
void report_feasible (population *pop, FILE *fpt);
void report_ind (individual *ind, FILE *fpt);

void quicksort_front_obj(population *pop, int objcount, int obj_array[], int obj_array_size);
void q_sort_front_obj(population *pop, int objcount, int obj_array[], int left, int right);
void quicksort_dist(population *pop, int *dist, int front_size);
void q_sort_dist(population *pop, int *dist, int left, int right);

void selection (population *old_pop, population *new_pop);
individual* tournament (individual *ind1, individual *ind2);

# endif
