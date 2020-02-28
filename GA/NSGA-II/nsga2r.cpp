/* NSGA-II routine (implementation of the 'main' function) */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>

# include "global.h"
# include "rand.h"

using namespace std;

int nreal;
int nbin;
int nobj;
int ncon;
int popsize;
double pcross_real;
double pcross_bin;
double pmut_real;
double pmut_bin;
double eta_c;
double eta_m;
int ngen;
int nbinmut;
int nrealmut;
int nbincross;
int nrealcross;
int *nbits;
double *min_realvar;
double *max_realvar;
double *min_binvar;
double *max_binvar;
int bitlength;
int choice;
int obj1;
int obj2;
int obj3;
int angle1;
int angle2;

int number_of_evaluations;
int maximum_number_of_evaluations;
int problem_index;
double vtr;
int elitist_archive_size_target;

int64_t random_seed_changing;
int number_of_objectives;
int number_of_parameters;
int number_of_generations;
int elitist_archive_size;
int elitist_archive_capacity;
int **elitist_archive;
double **elitist_archive_objective_values;
double *elitist_archive_constraint_values;

void *Malloc( long size );
void parseCommandLine( int argc, char **argv );
void parseParameters( int argc, char **argv, int *index );
double randomRealUniform01( void );
void initializeRandomNumberGenerator();
void updateElitistArchive( int *solution, double *solution_objective_values, double solution_constraint_value );
void addToElitistArchive( int *solution, double *solution_objective_values, double solution_constraint_value );

short constraintParetoDominates( double *objective_values_x, double constraint_value_x, double *objective_values_y, double constraint_value_y );
short paretoDominates( double *objective_values_x, double *objective_values_y );
short sameObjectiveBox( double *objective_values_a, double *objective_values_b );
void initializeElitistArchive();
void freeElitistArchive();
short isInListOfIndices( int index, int *indices, int number_of_indices );
void removeFromElitistArchive( int *indices, int number_of_indices );
int allPointsFound();
double computeDPFSMetric( double **default_front, int default_front_size, double **approximation_front, double *approximation_front_constraint_values, int approximation_front_size );
double distanceEuclidean( double *x, double *y, int number_of_dimensions );
double **getDefaultFront( int *default_front_size );
double **getDefaultFrontTrap5InverseTrap5( int *default_front_size );
double **getDefaultFrontOnemaxZeromax( int *default_front_size );
short haveDPFSMetric( void );

void writeGenerationalStatisticsWithoutDPFSMetric( void );
void writeGenerationalStatistics( population *parent_pop );
void writeGenerationalStatisticsWithDPFSMetric( population *parent_pop );

bool debuglog = false;

void *Malloc( long size )
{
  void *result;

  result = (void *) malloc( size );
  if( !result )
  {
    printf("\n");
    printf("Error while allocating memory in Malloc( %ld ), aborting program.", size);
    printf("\n");

    exit( 0 );
  }

  return( result );
}

void parseCommandLine( int argc, char **argv )
{
  int index;

  index = 1;
  
  parseParameters( argc, argv, &index );

}

/**
 * Parses only the EA parameters from the command line.
 */
void parseParameters( int argc, char **argv, int *index )
{
  int noError;

  if( (argc - *index) != 6 )
  {
    printf("Number of parameters is incorrect, require 8 parameters (you provided %d).\n\n", (argc - *index));
  }

  noError = 1;
  noError = noError && sscanf( argv[*index+0], "%d", &problem_index );
  noError = noError && sscanf( argv[*index+1], "%d", &nbin );
  /*noError = noError && sscanf( argv[*index+2], "%lf", &tau );*/
  noError = noError && sscanf( argv[*index+2], "%d", &popsize );
  /*noError = noError && sscanf( argv[*index+4], "%d", &number_of_mixing_components );*/
  noError = noError && sscanf( argv[*index+3], "%d", &elitist_archive_size_target );
  noError = noError && sscanf( argv[*index+4], "%d", &maximum_number_of_evaluations );
  noError = noError && sscanf( argv[*index+5], "%lf", &vtr );

  if( !noError )
  {
    printf("Error parsing parameters.\n\n");

  }
}

double randomRealUniform01( void )
{
  int64_t n26, n27;
  double  result;

  random_seed_changing = (random_seed_changing * 0x5DEECE66DLLU + 0xBLLU) & ((1LLU << 48) - 1);
  n26                  = (int64_t)(random_seed_changing >> (48 - 26));
  random_seed_changing = (random_seed_changing * 0x5DEECE66DLLU + 0xBLLU) & ((1LLU << 48) - 1);
  n27                  = (int64_t)(random_seed_changing >> (48 - 27));
  result               = (((int64_t)n26 << 27) + n27) / ((double) (1LLU << 53));

  return( result );
}

void initializeRandomNumberGenerator()
{
  struct timeval tv;
  struct tm *timep;

  while( random_seed_changing == 0 )
  {
	  gettimeofday( &tv, NULL );
  	timep = localtime (&tv.tv_sec);
	  random_seed_changing = timep->tm_hour * 3600 * 1000 + timep->tm_min * 60 * 1000 + timep->tm_sec * 1000 + tv.tv_usec / 1000;
  }
}

void initializeElitistArchive()
{
	int i;
	
	elitist_archive                               = (int **) Malloc( elitist_archive_capacity*sizeof( int * ) );
  elitist_archive_objective_values              = (double **) Malloc( elitist_archive_capacity*sizeof( double * ) );
  elitist_archive_constraint_values             = (double *) Malloc( elitist_archive_capacity*sizeof( double ) );

  for( i = 0; i < elitist_archive_capacity; i++ )
  {
    elitist_archive[i]                  = (int *) Malloc( number_of_parameters*sizeof( int ) );
    elitist_archive_objective_values[i] = (double *) Malloc( number_of_objectives*sizeof( double ) );
  }

	elitist_archive_size=0;
}

void freeElitistArchive()
{
	int i, default_front_size;
	double **default_front;
  for( i = 0; i < elitist_archive_capacity; i++ )
  {
    free( elitist_archive[i] );
    free( elitist_archive_objective_values[i] );
  }

  free( elitist_archive );
  free( elitist_archive_objective_values );
  free( elitist_archive_constraint_values );

  default_front = getDefaultFront( &default_front_size );
  if( default_front )
  {
    for( i = 0; i < default_front_size; i++ )
      free( default_front[i] );
    free( default_front );
  }
}

void updateElitistArchive( int *solution, double *solution_objective_values, double solution_constraint_value )
{
  short is_dominated_itself;
  int   i, *indices_dominated, number_of_solutions_dominated;

  if( elitist_archive_size == 0 )
    addToElitistArchive( solution, solution_objective_values, solution_constraint_value );
  else
  {
    indices_dominated             = (int *) Malloc( elitist_archive_size*sizeof( int ) );
    number_of_solutions_dominated = 0;
    is_dominated_itself           = 0;
    for( i = 0; i < elitist_archive_size; i++ )
    {
      if( constraintParetoDominates( elitist_archive_objective_values[i], elitist_archive_constraint_values[i], solution_objective_values, solution_constraint_value ) )
        is_dominated_itself = 1;
      else
      {
        if( !constraintParetoDominates( solution_objective_values, solution_constraint_value, elitist_archive_objective_values[i], elitist_archive_constraint_values[i] ) )
        {
          if( sameObjectiveBox( elitist_archive_objective_values[i], solution_objective_values ) )
            is_dominated_itself = 1;
        }
      }

      if( is_dominated_itself )
        break;
    }

    if( !is_dominated_itself )
    {
      for( i = 0; i < elitist_archive_size; i++ )
      {
        if( constraintParetoDominates( solution_objective_values, solution_constraint_value, elitist_archive_objective_values[i], elitist_archive_constraint_values[i] ) )
        {
          indices_dominated[number_of_solutions_dominated] = i;
          number_of_solutions_dominated++;
        }
      }

      if( number_of_solutions_dominated > 0 )
        removeFromElitistArchive( indices_dominated, number_of_solutions_dominated );

      addToElitistArchive( solution, solution_objective_values, solution_constraint_value );
    }

    free( indices_dominated );
  }
}

void addToElitistArchive( int *solution, double *solution_objective_values, double solution_constraint_value )
{
  int      i, j, elitist_archive_capacity_new;
  int **elitist_archive_new;
  double **elitist_archive_objective_values_new, *elitist_archive_constraint_values_new;

  if( elitist_archive_capacity == elitist_archive_size )
  {
    elitist_archive_capacity_new          = elitist_archive_capacity*2+1;
    elitist_archive_new                   = (int **) Malloc( elitist_archive_capacity_new*sizeof( int * ) );
    elitist_archive_objective_values_new  = (double **) Malloc( elitist_archive_capacity_new*sizeof( double * ) );
    elitist_archive_constraint_values_new = (double *) Malloc( elitist_archive_capacity_new*sizeof( double ) );
    for( i = 0; i < elitist_archive_capacity_new; i++ )
    {
      elitist_archive_new[i]         = (int *) Malloc( number_of_parameters*sizeof( int ) );
      elitist_archive_objective_values_new[i] = (double *) Malloc( number_of_objectives*sizeof( double ) );
    }

    for( i = 0; i < elitist_archive_size; i++ )
    {
      for( j = 0; j < number_of_parameters; j++ )
        elitist_archive_new[i][j] = elitist_archive[i][j];
      for( j = 0; j < number_of_objectives; j++ )
        elitist_archive_objective_values_new[i][j] = elitist_archive_objective_values[i][j];
      elitist_archive_constraint_values_new[i] = elitist_archive_constraint_values[i];
    }

    for( i = 0; i < elitist_archive_capacity; i++ )
    {
      free( elitist_archive[i] );
      free( elitist_archive_objective_values[i] );
    }
    free( elitist_archive );
    free( elitist_archive_objective_values );
    free( elitist_archive_constraint_values );

    elitist_archive_capacity          = elitist_archive_capacity_new;
    elitist_archive                   = elitist_archive_new;
    elitist_archive_objective_values  = elitist_archive_objective_values_new;
    elitist_archive_constraint_values = elitist_archive_constraint_values_new;
  }

  for( j = 0; j < number_of_parameters; j++ )
    elitist_archive[elitist_archive_size][j] = solution[j];
  for( j = 0; j < number_of_objectives; j++ )
    elitist_archive_objective_values[elitist_archive_size][j] = solution_objective_values[j];
  elitist_archive_constraint_values[elitist_archive_size] = solution_constraint_value;
  elitist_archive_size++;
}

short constraintParetoDominates( double *objective_values_x, double constraint_value_x, double *objective_values_y, double constraint_value_y )
{
  short result;

  result = 0;

  if( constraint_value_x > 0 ) /* x is infeasible */
  {
    if( constraint_value_y > 0 ) /* Both are infeasible */
    {
      if( constraint_value_x < constraint_value_y )
       result = 1;
    }
  }
  else /* x is feasible */
  {
    if( constraint_value_y > 0 ) /* x is feasible and y is not */
      result = 1;
    else /* Both are feasible */
      result = paretoDominates( objective_values_x, objective_values_y );
  }

  return( result );
}

/**
 * Returns 1 if x Pareto-dominates y, 0 otherwise.
 */
short paretoDominates( double *objective_values_x, double *objective_values_y )
{
  short strict;
  int   i, result;

  result = 1;
  strict = 0;
  for( i = 0; i < number_of_objectives; i++ )
  {
    if( fabs( objective_values_x[i] - objective_values_y[i] ) >= 0.00001 )
    {
      /*if( objective_values_x[i] > objective_values_y[i] )*/
      if( objective_values_x[i] < objective_values_y[i] )
      {
        result = 0;
        break;
      }
      /*if( objective_values_x[i] < objective_values_y[i] )*/
      if( objective_values_x[i] > objective_values_y[i] )
         strict = 1;
    }
  }
  if( strict == 0 && result == 1 )
    result = 0;

  return( result );
}

short sameObjectiveBox( double *objective_values_a, double *objective_values_b )
{
  int i;

  /* If the solutions are identical, they are still in the (infinitely small) same objective box. */
  for( i = 0; i < number_of_objectives; i++ )
  {
  	if( objective_values_a[i] != objective_values_b[i] )
  	  return( 0 );
  }

    return( 1 );
}

void removeFromElitistArchive( int *indices, int number_of_indices )
{
  int      i, j, elitist_archive_size_new;
  int **elitist_archive_new;
  double **elitist_archive_objective_values_new, *elitist_archive_constraint_values_new;

  elitist_archive_new                   = (int**) Malloc( elitist_archive_capacity*sizeof( int * ) );
  elitist_archive_objective_values_new  = (double **) Malloc( elitist_archive_capacity*sizeof( double * ) );
  elitist_archive_constraint_values_new = (double *) Malloc( elitist_archive_capacity*sizeof( double ) );
  for( i = 0; i < elitist_archive_capacity; i++ )
  {
    elitist_archive_new[i]                  = (int *) Malloc( number_of_parameters*sizeof( int ) );
    elitist_archive_objective_values_new[i] = (double *) Malloc( number_of_objectives*sizeof( double ) );
  }

  elitist_archive_size_new = 0;
  for( i = 0; i < elitist_archive_size; i++ )
  {
    if( !isInListOfIndices( i, indices, number_of_indices ) )
    {
      for( j = 0; j < number_of_parameters; j++ )
        elitist_archive_new[elitist_archive_size_new][j] = elitist_archive[i][j];
      for( j = 0; j < number_of_objectives; j++ )
        elitist_archive_objective_values_new[elitist_archive_size_new][j] = elitist_archive_objective_values[i][j];
      elitist_archive_constraint_values_new[elitist_archive_size_new] = elitist_archive_constraint_values[i];
      elitist_archive_size_new++;
    }
  }

  for( i = 0; i < elitist_archive_capacity; i++ )
  {
    free( elitist_archive[i] );
    free( elitist_archive_objective_values[i] );
  }
  free( elitist_archive );
  free( elitist_archive_objective_values );
  free( elitist_archive_constraint_values );

  elitist_archive_size              = elitist_archive_size_new;
  elitist_archive                   = elitist_archive_new;
  elitist_archive_objective_values  = elitist_archive_objective_values_new;
  elitist_archive_constraint_values = elitist_archive_constraint_values_new;
}

/**
 * Returns 1 if index is in the indices array, 0 otherwise.
 */
short isInListOfIndices( int index, int *indices, int number_of_indices )
{
  int i;

  for( i = 0; i < number_of_indices; i++ )
    if( indices[i] == index )
      return( 1 );

  return( 0 );
}

int allPointsFound()
{
		int      default_front_size;
	  double **default_front, metric_elitist_archive;

	  if( haveDPFSMetric() )
	  {
	    default_front          = getDefaultFront( &default_front_size );
	    metric_elitist_archive = computeDPFSMetric( default_front, default_front_size, elitist_archive_objective_values, elitist_archive_constraint_values, elitist_archive_size );
			if(metric_elitist_archive == 0.0)
				return 1;
	  }
		return 0;
}

short haveDPFSMetric( void )
{
  int default_front_size;

  getDefaultFront( &default_front_size );
  if( default_front_size > 0 )
    return( 1 );

  return( 0 );
}

double computeDPFSMetric( double **default_front, int default_front_size, double **approximation_front, double *approximation_front_constraint_values, int approximation_front_size )
{
  int    i, j;
  double result, distance, smallest_distance;

  if( approximation_front_size == 0 )
    return( 1e+308 );

  result = 0.0;
  for( i = 0; i < default_front_size; i++ )
  {
    smallest_distance = 1e+308;
    for( j = 0; j < approximation_front_size; j++ )
    {
      if( approximation_front_constraint_values[j] == 0 )
      {
        distance = distanceEuclidean( default_front[i], approximation_front[j], number_of_objectives );
        if( distance < smallest_distance )
          smallest_distance = distance;
      }
    }
    result += smallest_distance;
  }
  result /= (double) default_front_size;

  return( result );
}
double distanceEuclidean( double *x, double *y, int number_of_dimensions )
{
  int    i;
  double value, result;

  result = 0.0;
  for( i = 0; i < number_of_dimensions; i++ )
  {
    value   = y[i] - x[i];
    result += value*value;
  }
  result = sqrt( result );

  return( result );
}
double **getDefaultFront( int *default_front_size )
{
  switch( problem_index )
  {
    case 0: return( getDefaultFrontOnemaxZeromax( default_front_size ) );
    case 1: return( getDefaultFrontTrap5InverseTrap5( default_front_size ) );
  }

  *default_front_size = 0;

  return( NULL );
}
double **getDefaultFrontOnemaxZeromax( int *default_front_size )
{
  int  i;
  static double **result = NULL;

  *default_front_size = ( number_of_parameters + 1 );

  if( result == NULL )
  {
    result = (double **) Malloc( (*default_front_size)*sizeof( double * ) );
    for( i = 0; i < (*default_front_size); i++ )
      result[i] = (double *) Malloc( 2*sizeof( double ) );

    for( i = 0; i < (*default_front_size); i++ )
    {
      result[i][0] = i;											/* Zeromax*/
      result[i][1] = number_of_parameters - result[i][0];		/* Onemax*/
    }
  }

  return( result );
}

double **getDefaultFrontTrap5InverseTrap5( int *default_front_size )
{
  int  i, number_of_blocks;
  static double **result = NULL;

  number_of_blocks = number_of_parameters / 5;
  *default_front_size = ( number_of_blocks + 1 );

  if( result == NULL )
  {
    result = (double **) Malloc( (*default_front_size)*sizeof( double * ) );
    for( i = 0; i < (*default_front_size); i++ )
      result[i] = (double *) Malloc( 2*sizeof( double ) );

    for( i = 0; i < (*default_front_size); i++ )					/* i = number of all-1 blocks*/
    {
      result[i][0] = ( 5 * i ) + ( 4 * (number_of_blocks - i) ) ;	/* Trap-5*/
      result[i][1] = ( 5 * (number_of_blocks - i)) + ( 4 * i );		/* Inverse Trap-5*/
    }
  }

  return( result );
}

void writeGenerationalStatistics( population *parent_pop )
{
  if( haveDPFSMetric() )
    writeGenerationalStatisticsWithDPFSMetric(parent_pop);
  else
    writeGenerationalStatisticsWithoutDPFSMetric();
}
void writeGenerationalStatisticsWithDPFSMetric( population *parent_pop )
{
  int      i, j, k, default_front_size, population_rank0_size, *indices_of_rank0;
  char     string[1000];
  double **default_front, **population_rank0_objective_values, *population_rank0_constraint_values,
           metric_elitist_archive, metric_population_rank0;
  FILE    *file;

  default_front = getDefaultFront( &default_front_size );

  /* Elitist Archive */
  metric_elitist_archive = computeDPFSMetric( default_front, default_front_size, elitist_archive_objective_values, elitist_archive_constraint_values, elitist_archive_size );

  /* Population */
  indices_of_rank0      = (int *) Malloc( popsize*sizeof( int ) );
  population_rank0_size = 0;
  for( i = 0; i < popsize; i++ )
  {
    if( (parent_pop->ind[i]).rank == 1 ) 
    {
      indices_of_rank0[population_rank0_size] = i;
      population_rank0_size++;
    }
  }

  population_rank0_objective_values  = (double **) Malloc( population_rank0_size*sizeof( double * ) );
  population_rank0_constraint_values = (double *) Malloc( population_rank0_size*sizeof( double ) );
  for( i = 0; i < population_rank0_size; i++ )
    population_rank0_objective_values[i] = (double *) Malloc( number_of_objectives*sizeof( double ) );

  k = 0;
  for( i = 0; i < population_rank0_size; i++ )
  {
    for( j = 0; j < number_of_objectives; j++ )
      population_rank0_objective_values[k][j] = (parent_pop->ind[indices_of_rank0[i]]).obj[j];
		if(ncon != 0)
	    population_rank0_constraint_values[k] = (parent_pop->ind[indices_of_rank0[i]]).constr[0];
		else
			population_rank0_constraint_values[k] = 0;
    k++;
  }

  metric_population_rank0 = computeDPFSMetric( default_front, default_front_size, population_rank0_objective_values, population_rank0_constraint_values, population_rank0_size );

  /* Finally, output everything */
  file = NULL;
  if( number_of_generations == 0 )
  {
    file = fopen( "statistics.dat", "w" );
    sprintf( string, "# Generation Evaluation     D_{Pf->S}       D_{Pf->S}\n");
    fputs( string, file );
    sprintf( string, "#                            Elitist      Population\n");
    fputs( string, file );
  }
  else
    file = fopen( "statistics.dat", "a" );

  sprintf( string, "  %10d %11d %15e %15e\n  ", number_of_generations, number_of_evaluations, metric_elitist_archive, metric_population_rank0 );
  fputs( string, file );

  fclose( file );

  for( i = 0; i < population_rank0_size; i++ )
    free( population_rank0_objective_values[i] );
  free( population_rank0_objective_values );
  free( population_rank0_constraint_values );
  free( indices_of_rank0 );
}
void writeGenerationalStatisticsWithoutDPFSMetric( void )
{
  char    string[1000];
  FILE   *file;

  file = NULL;
  if( number_of_generations == 0 )
  {
    file = fopen( "statistics.dat", "w" );

    sprintf( string, "# Generation Evaluations \n");
    fputs( string, file );
  }
  else
    file = fopen( "statistics.dat", "a" );

  sprintf( string, "  %10d %11d  ", number_of_generations, number_of_evaluations );
  fputs( string, file );

  fclose( file );
}

//int mainNSGA ()
int mainNSGA (int nbinary, int populationSize, float seedd)
{
    problem_index = 0;
    nbin = nbinary;
    popsize = populationSize;
    elitist_archive_size_target = 1000;
    maximum_number_of_evaluations = 1000000;
    vtr = 0;
    
    int i,j,k;
    FILE *fpt1;
    FILE *fpt2;
    FILE *fpt3;
    FILE *fpt4;
    FILE *fpt5;
    /*FILE *gp;*/
    population *parent_pop;
    population *child_pop;
    population *mixed_pop;
		initializeRandomNumberGenerator();

//        parseCommandLine( argc, argv );
		number_of_parameters=nbin;
		number_of_objectives=2;
		number_of_generations=0;
		nobj=number_of_objectives;
		elitist_archive_capacity=10;
		initializeElitistArchive();
		ngen = maximum_number_of_evaluations / popsize;
		pcross_bin=0.9;
		pmut_bin=1.0/((double)number_of_parameters);
		/*pmut_bin=0;*/
    printf("\n Problem index = %d",problem_index);		
    printf("\n Number of binary variables = %d",nbin);
    printf("\n Population size = %d",popsize);
    printf("\n Number of generations = %d",ngen);
    printf("\n Number of objective functions = %d",nobj);
    printf("\n Number of constraints = %d",ncon);
    printf("\n Maximum number of evaluations = %d",maximum_number_of_evaluations);
		printf("\n Crossover prob. = %f",pcross_bin);
		printf("\n Mutation prob. = %f",pmut_bin);
		/*
    if (argc<2)
    {
        printf("\n Usage ./nsga2r random_seed \n");
        exit(1);
    }
    seed = (double)atof(argv[1]);
    if (seed<=0.0 || seed>=1.0)
    {
        printf("\n Entered seed value is wrong, seed value must be in (0,1) \n");
        exit(1);
    }*/
//        seed = randomRealUniform01();
    seed = seedd;

    fpt1 = fopen("initial_pop.out","w");
    fpt2 = fopen("final_pop.out","w");
    fpt3 = fopen("best_pop.out","w");
    fpt4 = fopen("all_pop.out","w");
    fpt5 = fopen("params.out","w");
    fprintf(fpt1,"# This file contains the data of initial population\n");
    fprintf(fpt2,"# This file contains the data of final population\n");
    fprintf(fpt3,"# This file contains the data of final feasible population (if found)\n");
    fprintf(fpt4,"# This file contains the data of all generations\n");
    fprintf(fpt5,"# This file contains information about inputs as read by the program\n");

    /*printf("\n Enter the problem relevant and algorithm relevant parameters ... ");    
		printf("\n Enter the population size (a multiple of 4) : ");
    scanf("%d",&popsize);*/
		
    /*if (popsize<4 || (popsize%4)!= 0)
    {
        printf("\n population size read is : %d",popsize);
        printf("\n Wrong population size entered, hence exiting \n");
        exit (1);
    }*/
    
		/*printf("\n Enter the number of generations : ");
    scanf("%d",&ngen);*/

    /*if (ngen<1)
    {
        printf("\n number of generations read is : %d",ngen);
        printf("\n Wrong nuber of generations entered, hence exiting \n");
        exit (1);
    }*/
	
    /*printf("\n Enter the number of objectives : ");
    scanf("%d",&nobj);
    if (nobj<1)
    {
        printf("\n number of objectives entered is : %d",nobj);
        printf("\n Wrong number of objectives entered, hence exiting \n");
        exit (1);
    }*/
		nobj=2;

    /*printf("\n Enter the number of constraints : ");
    scanf("%d",&ncon);
    if (ncon<0)
    {
        printf("\n number of constraints entered is : %d",ncon);
        printf("\n Wrong number of constraints enetered, hence exiting \n");
        exit (1);
    }*/
		ncon=0;

		nreal=0;

    /*printf("\n Enter the number of binary variables : ");
    scanf("%d",&nbin);
    if (nbin<0)
    {
        printf ("\n number of binary variables entered is : %d",nbin);
        printf ("\n Wrong number of binary variables entered, hence exiting \n");
        exit(1);
    }*/
    if (nbin != 0)
    {
        nbits = (int *)malloc(nbin*sizeof(int));
        min_binvar = (double *)malloc(nbin*sizeof(double));
        max_binvar = (double *)malloc(nbin*sizeof(double));
				for(i=0;i<nbin;i++)
				{
					min_binvar[i]=0;
					max_binvar[i]=1;
					nbits[i] = 1; /* 1 bit for every gene*/
				}
        /*for (i=0; i<nbin; i++)
        {
            printf ("\n Enter the number of bits for binary variable %d : ",i+1);
            scanf ("%d",&nbits[i]);
            if (nbits[i] < 1)
            {
                printf("\n Wrong number of bits for binary variable entered, hence exiting");
                exit(1);
            }
            printf ("\n Enter the lower limit of binary variable %d : ",i+1);
            scanf ("%lf",&min_binvar[i]);
            printf ("\n Enter the upper limit of binary variable %d : ",i+1);
            scanf ("%lf",&max_binvar[i]);
            if (max_binvar[i] <= min_binvar[i])
            {
                printf("\n Wrong limits entered for the min and max bounds of binary variable entered, hence exiting \n");
                exit(1);
            }
        }

        printf ("\n Enter the probability of crossover of binary variable (0.6-1.0): ");
        scanf ("%lf",&pcross_bin);
        if (pcross_bin<0.0 || pcross_bin>1.0)
        {
            printf("\n Probability of crossover entered is : %e",pcross_bin);
            printf("\n Entered value of probability of crossover of binary variables is out of bounds, hence exiting \n");
            exit (1);
        }
        printf ("\n Enter the probability of mutation of binary variables (1/nbits): ");
        scanf ("%lf",&pmut_bin);
        if (pmut_bin<0.0 || pmut_bin>1.0)
        {
            printf("\n Probability of mutation entered is : %e",pmut_bin);
            printf("\n Entered value of probability  of mutation of binary variables is out of bounds, hence exiting \n");
            exit (1);
        }*/
    }
    if (nreal==0 && nbin==0)
    {
        printf("\n Number of real as well as binary variables, both are zero, hence exiting \n");
        exit(1);
    }
    choice=0;
    /*printf("\n Do you want to use gnuplot to display the results realtime (0 for NO) (1 for yes) : ");*/
    printf("\n Input data successfully entered, now performing initialization \n");
    fprintf(fpt5,"\n Population size = %d",popsize);
    fprintf(fpt5,"\n Number of generations = %d",ngen);
    fprintf(fpt5,"\n Number of objective functions = %d",nobj);
    fprintf(fpt5,"\n Number of constraints = %d",ncon);
    fprintf(fpt5,"\n Number of real variables = %d",nreal);
    if (nreal!=0)
    {
        for (i=0; i<nreal; i++)
        {
            fprintf(fpt5,"\n Lower limit of real variable %d = %e",i+1,min_realvar[i]);
            fprintf(fpt5,"\n Upper limit of real variable %d = %e",i+1,max_realvar[i]);
        }
        fprintf(fpt5,"\n Probability of crossover of real variable = %e",pcross_real);
        fprintf(fpt5,"\n Probability of mutation of real variable = %e",pmut_real);
        fprintf(fpt5,"\n Distribution index for crossover = %e",eta_c);
        fprintf(fpt5,"\n Distribution index for mutation = %e",eta_m);
    }
    fprintf(fpt5,"\n Number of binary variables = %d",nbin);
    if (nbin!=0)
    {
        for (i=0; i<nbin; i++)
        {
            fprintf(fpt5,"\n Number of bits for binary variable %d = %d",i+1,nbits[i]);
            fprintf(fpt5,"\n Lower limit of binary variable %d = %e",i+1,min_binvar[i]);
            fprintf(fpt5,"\n Upper limit of binary variable %d = %e",i+1,max_binvar[i]);
        }
        fprintf(fpt5,"\n Probability of crossover of binary variable = %e",pcross_bin);
        fprintf(fpt5,"\n Probability of mutation of binary variable = %e",pmut_bin);
    }
    fprintf(fpt5,"\n Seed for random number generator = %e",seed);
    bitlength = 0;
    if (nbin!=0)
    {
        for (i=0; i<nbin; i++)
        {
            bitlength += nbits[i];
        }
    }
    fprintf(fpt1,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt2,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt3,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt4,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    nbinmut = 0;
    nrealmut = 0;
    nbincross = 0;
    nrealcross = 0;
    parent_pop = (population *)malloc(sizeof(population));
    child_pop = (population *)malloc(sizeof(population));
    mixed_pop = (population *)malloc(sizeof(population));
    allocate_memory_pop (parent_pop, popsize);
    allocate_memory_pop (child_pop, popsize);
    allocate_memory_pop (mixed_pop, 2*popsize);
    randomize();
    initialize_pop (parent_pop);
    if(debuglog) printf("\n Initialization done, now performing first generation\n");
    decode_pop(parent_pop);
    evaluate_pop (parent_pop);
    assign_rank_and_crowding_distance (parent_pop);
    report_pop (parent_pop, fpt1);
    fprintf(fpt4,"# gen = 1\n");
    report_pop(parent_pop,fpt4);
    print_pop(parent_pop);
    /*printf("\n gen = 1");*/
    fflush(stdout);
    /*if (choice!=0)    onthefly_display (parent_pop,gp,1);*/
    fflush(fpt1);
    fflush(fpt2);
    fflush(fpt3);
    fflush(fpt4);
    fflush(fpt5);
    sleep(1);
		
		/* */
		number_of_evaluations = popsize;
		for (j=0; j<popsize; j++)
 		{
			int *solution = (int*)Malloc(number_of_parameters*sizeof(int));
			for (k=0;k<number_of_parameters;k++)
				solution[k]=(parent_pop->ind[j]).gene[k][0];
	   	updateElitistArchive (solution,(parent_pop->ind[j]).obj,0);
			free(solution);
	  }
		/*printf("Generation: 0 - Number of evaluations: %d - Archive size: %d\n",number_of_evaluations,elitist_archive_size);*/
		writeGenerationalStatistics(parent_pop);
		/* */

    for (i=1; i<=ngen; i++)
    {
        selection (parent_pop, child_pop);
        mutation_pop (child_pop);
        decode_pop(child_pop);
        evaluate_pop(child_pop);
				/* */
				number_of_evaluations += popsize;
				number_of_generations++;
				for (j=0; j<popsize; j++)
		 		{
					int *solution = (int*)Malloc(number_of_parameters*sizeof(int));
					for (k=0;k<number_of_parameters;k++)
						solution[k]=(child_pop->ind[j]).gene[k][0];
	  			updateElitistArchive (solution,(child_pop->ind[j]).obj,0);
					free(solution);
	 		 }
        if(debuglog) printf("\n Elitist archive size = %d", elitist_archive_size);

				/* */
        merge (parent_pop, child_pop, mixed_pop);
        fill_nondominated_sort (mixed_pop, parent_pop);
        /* Comment following four lines if information for all
        generations is not desired, it will speed up the execution */
        fprintf(fpt4,"# gen = %d\n",i);
        report_pop(parent_pop,fpt4);
        if(debuglog) printf("# gen = %d\n",i);
        if(debuglog) print_pop(parent_pop);
        printf("\nGeneration: %d - Number of evaluations: %d - Archive size: %d\n",i,number_of_evaluations,elitist_archive_size);
        fflush(fpt4);
        /*if (choice!=0)    onthefly_display (parent_pop,gp,i);
        printf("\n gen = %d",i);*/

                /* */
                /*printf("\nGeneration: %d - Number of evaluations: %d - Archive size: %d\n",i,number_of_evaluations,elitist_archive_size);*/
                writeGenerationalStatistics(parent_pop);
                if(number_of_evaluations > maximum_number_of_evaluations)
                    break;
                if(allPointsFound())
                    break;
                /* */
    }
    printf("\n Generations finished, now reporting solutions");
    report_pop(parent_pop,fpt2);
    print_pop(parent_pop);
    report_feasible(parent_pop,fpt3);
    if (nreal!=0)
    {
        fprintf(fpt5,"\n Number of crossover of real variable = %d",nrealcross);
        fprintf(fpt5,"\n Number of mutation of real variable = %d",nrealmut);
    }
    if (nbin!=0)
    {
        fprintf(fpt5,"\n Number of crossover of binary variable = %d",nbincross);
        fprintf(fpt5,"\n Number of mutation of binary variable = %d",nbinmut);
    }
    fflush(stdout);
    fflush(fpt1);
    fflush(fpt2);
    fflush(fpt3);
    fflush(fpt4);
    fflush(fpt5);
    fclose(fpt1);
    fclose(fpt2);
    fclose(fpt3);
    fclose(fpt4);
    fclose(fpt5);
    /*if (choice!=0)
    {
        pclose(gp);
    }*/
    if (nreal!=0)
    {
        free (min_realvar);
        free (max_realvar);
    }
    if (nbin!=0)
    {
        free (min_binvar);
        free (max_binvar);
        free (nbits);
    }
    deallocate_memory_pop (parent_pop, popsize);
    deallocate_memory_pop (child_pop, popsize);
    deallocate_memory_pop (mixed_pop, 2*popsize);
    free (parent_pop);
    free (child_pop);
    free (mixed_pop);
		freeElitistArchive();
    printf("\n Routine successfully exited \n");
    return maximum_number_of_evaluations;
}

/* Function to allocate memory to a population */
void allocate_memory_pop (population *pop, int size)
{
    int i;
    pop->ind = (individual *)malloc(size*sizeof(individual));
    for (i=0; i<size; i++)
    {
        allocate_memory_ind (&(pop->ind[i]));
    }
    return;
}

/* Function to allocate memory to an individual */
void allocate_memory_ind (individual *ind)
{
    int j;
    if (nreal != 0)
    {
        ind->xreal = (double *)malloc(nreal*sizeof(double));
    }
    if (nbin != 0)
    {
        ind->xbin = (double *)malloc(nbin*sizeof(double));
        ind->gene = (int **)malloc(nbin*sizeof(int *));
        for (j=0; j<nbin; j++)
        {
            ind->gene[j] = (int *)malloc(nbits[j]*sizeof(int));
        }
    }
    ind->obj = (double *)malloc(nobj*sizeof(double));
    if (ncon != 0)
    {
        ind->constr = (double *)malloc(ncon*sizeof(double));
    }
    return;
}

/* Function to deallocate memory to a population */
void deallocate_memory_pop (population *pop, int size)
{
    int i;
    for (i=0; i<size; i++)
    {
        deallocate_memory_ind (&(pop->ind[i]));
    }
    free (pop->ind);
    return;
}

/* Function to deallocate memory to an individual */
void deallocate_memory_ind (individual *ind)
{
    int j;
    if (nreal != 0)
    {
        free(ind->xreal);
    }
    if (nbin != 0)
    {
        for (j=0; j<nbin; j++)
        {
            free(ind->gene[j]);
        }
        free(ind->xbin);
        free(ind->gene);
    }
    free(ind->obj);
    if (ncon != 0)
    {
        free(ind->constr);
    }
    return;
}

/* Function to return the maximum of two variables */
double maximum (double a, double b)
{
    if (a>b)
    {
        return(a);
    }
    return (b);
}

/* Function to return the minimum of two variables */
double minimum (double a, double b)
{
    if (a<b)
    {
        return (a);
    }
    return (b);
}

/* Function to cross two individuals */
void crossover (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    if (nreal!=0)
    {
        realcross (parent1, parent2, child1, child2);
    }
    if (nbin!=0)
    {
        bincross (parent1, parent2, child1, child2);
    }
    return;
}

/* Routine for real variable SBX crossover */
void realcross (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    int i;
    double rand;
    double y1, y2, yl, yu;
    double c1, c2;
    double alpha, beta, betaq;
    if (randomperc() <= pcross_real)
    {
        nrealcross++;
        for (i=0; i<nreal; i++)
        {
            if (randomperc()<=0.5 )
            {
                if (fabs(parent1->xreal[i]-parent2->xreal[i]) > EPS)
                {
                    if (parent1->xreal[i] < parent2->xreal[i])
                    {
                        y1 = parent1->xreal[i];
                        y2 = parent2->xreal[i];
                    }
                    else
                    {
                        y1 = parent2->xreal[i];
                        y2 = parent1->xreal[i];
                    }
                    yl = min_realvar[i];
                    yu = max_realvar[i];
                    rand = randomperc();
                    beta = 1.0 + (2.0*(y1-yl)/(y2-y1));
                    alpha = 2.0 - pow(beta,-(eta_c+1.0));
                    if (rand <= (1.0/alpha))
                    {
                        betaq = pow ((rand*alpha),(1.0/(eta_c+1.0)));
                    }
                    else
                    {
                        betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(eta_c+1.0)));
                    }
                    c1 = 0.5*((y1+y2)-betaq*(y2-y1));
                    beta = 1.0 + (2.0*(yu-y2)/(y2-y1));
                    alpha = 2.0 - pow(beta,-(eta_c+1.0));
                    if (rand <= (1.0/alpha))
                    {
                        betaq = pow ((rand*alpha),(1.0/(eta_c+1.0)));
                    }
                    else
                    {
                        betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(eta_c+1.0)));
                    }
                    c2 = 0.5*((y1+y2)+betaq*(y2-y1));
                    if (c1<yl)
                        c1=yl;
                    if (c2<yl)
                        c2=yl;
                    if (c1>yu)
                        c1=yu;
                    if (c2>yu)
                        c2=yu;
                    if (randomperc()<=0.5)
                    {
                        child1->xreal[i] = c2;
                        child2->xreal[i] = c1;
                    }
                    else
                    {
                        child1->xreal[i] = c1;
                        child2->xreal[i] = c2;
                    }
                }
                else
                {
                    child1->xreal[i] = parent1->xreal[i];
                    child2->xreal[i] = parent2->xreal[i];
                }
            }
            else
            {
                child1->xreal[i] = parent1->xreal[i];
                child2->xreal[i] = parent2->xreal[i];
            }
        }
    }
    else
    {
        for (i=0; i<nreal; i++)
        {
            child1->xreal[i] = parent1->xreal[i];
            child2->xreal[i] = parent2->xreal[i];
        }
    }
    return;
}

/* Routine for two point binary crossover */

void bincross (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    int i,site1,site2,temp;
    double rand;
    
    rand = randomperc();
    if (rand <= pcross_bin)
    {
        nbincross++;
        site1 = rnd(0,nbin - 1);
        site2 = rnd(0,nbin - 1);
        if (site1 > site2)
        {
            temp = site1;
            site1 = site2;
            site2 = temp;
        }
        if(debuglog) printf("Crossing over on %d and %d\n", site1, site2);
        for (i=0; i<site1; i++)
        {
            child1->gene[i][0] = parent1->gene[i][0];
            child2->gene[i][0] = parent2->gene[i][0];
        }
        for (i=site1; i <site2;i++)
        {
            child1->gene[i][0] = parent2->gene[i][0];
            child2->gene[i][0] = parent1->gene[i][0];
        }
        for (i=site2; i<nbin; i++)
        {
            child1->gene[i][0] = parent1->gene[i][0];
            child2->gene[i][0] = parent2->gene[i][0];
        }
    }
    else
    {
        for (i=0; i<nbin; i++)
        {
            child1->gene[i][0] = parent1->gene[i][0];
            child2->gene[i][0] = parent2->gene[i][0];
        }
    }
}

/* Routine to compute crowding distance based on ojbective function values when the population in in the form of a list */
void assign_crowding_distance_list (population *pop, list *lst, int front_size)
{
    int **obj_array;
    int *dist;
    int i, j;
    list *temp;
    temp = lst;
    if (front_size==1)
    {
        pop->ind[lst->index].crowd_dist = INF;
        return;
    }
    if (front_size==2)
    {
        pop->ind[lst->index].crowd_dist = INF;
        pop->ind[lst->child->index].crowd_dist = INF;
        return;
    }
    obj_array = (int **)malloc(nobj*sizeof(int*));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nobj; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = temp->index;
        temp = temp->child;
    }
    assign_crowding_distance (pop, dist, obj_array, front_size);
    free (dist);
    for (i=0; i<nobj; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}

/* Routine to compute crowding distance based on objective function values when the population in in the form of an array */
void assign_crowding_distance_indices (population *pop, int c1, int c2)
{
    int **obj_array;
    int *dist;
    int i, j;
    int front_size;
    front_size = c2-c1+1;
    if (front_size==1)
    {
        pop->ind[c1].crowd_dist = INF;
        return;
    }
    if (front_size==2)
    {
        pop->ind[c1].crowd_dist = INF;
        pop->ind[c2].crowd_dist = INF;
        return;
    }
    obj_array = (int **)malloc(nobj*sizeof(int*));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nobj; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = c1++;
    }
    assign_crowding_distance (pop, dist, obj_array, front_size);
    free (dist);
    for (i=0; i<nobj; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}

/* Routine to compute crowding distances */
void assign_crowding_distance (population *pop, int *dist, int **obj_array, int front_size)
{
    int i, j;
    //Loop over the objectives
    for (i=0; i<nobj; i++)
    {
        for (j=0; j<front_size; j++)
        {
            obj_array[i][j] = dist[j];
        }
        quicksort_front_obj (pop, i, obj_array[i], front_size);
    }
    for (j=0; j<front_size; j++)
    {
        pop->ind[dist[j]].crowd_dist = 0.0;
    }
    for (i=0; i<nobj; i++)
    {
        pop->ind[obj_array[i][0]].crowd_dist = INF;
    }
    for (i=0; i<nobj; i++)
    {
        for (j=1; j<front_size-1; j++)
        {
            if (pop->ind[obj_array[i][j]].crowd_dist != INF)
            {
                if (pop->ind[obj_array[i][front_size-1]].obj[i] == pop->ind[obj_array[i][0]].obj[i])
                {
                    pop->ind[obj_array[i][j]].crowd_dist += 0.0;
                }
                else
                {
                    pop->ind[obj_array[i][j]].crowd_dist += (pop->ind[obj_array[i][j+1]].obj[i] - pop->ind[obj_array[i][j-1]].obj[i])/(pop->ind[obj_array[i][front_size-1]].obj[i] - pop->ind[obj_array[i][0]].obj[i]);
                }
            }
        }
    }
    for (j=0; j<front_size; j++)
    {
        if (pop->ind[dist[j]].crowd_dist != INF)
        {
            pop->ind[dist[j]].crowd_dist = (pop->ind[dist[j]].crowd_dist)/nobj;
        }
    }
    return;
}

/* Function to decode a population to find out the binary variable values based on its bit pattern */
void decode_pop (population *pop)
{
    int i;
    if (nbin!=0)
    {
        for (i=0; i<popsize; i++)
        {
            decode_ind (&(pop->ind[i]));
        }
    }
    return;
}

/* Function to decode an individual to find out the binary variable values based on its bit pattern */
void decode_ind (individual *ind)
{
    int j, k;
    int sum;
    if (nbin!=0)
    {
        for (j=0; j<nbin; j++)
        {
            sum=0;
            for (k=0; k<nbits[j]; k++)
            {
                if (ind->gene[j][k]==1)
                {
                    sum += pow(2,nbits[j]-1-k);
                }
            }
            ind->xbin[j] = min_binvar[j] + (double)sum*(max_binvar[j] - min_binvar[j])/(double)(pow(2,nbits[j])-1);
        }
    }
    return;
}


/* Function to display the current population for the subsequent generation */
void onthefly_display (population *pop, FILE *gp, int ii)
{
    int i;
    int flag;
    FILE *fpt;
    fpt = fopen("plot.out","w");
    flag = 0;
    for (i=0; i<popsize; i++)
    {
        if (pop->ind[i].constr_violation==0)
        {
            if (choice!=3)
                fprintf(fpt,"%e\t%e\n",pop->ind[i].obj[obj1-1],pop->ind[i].obj[obj2-1]);
            else
                fprintf(fpt,"%e\t%e\t%e\n",pop->ind[i].obj[obj1-1],pop->ind[i].obj[obj2-1],pop->ind[i].obj[obj3-1]);
            fflush(fpt);
            flag = 1;
        }
    }
    if (flag==0)
    {
        printf("\n No feasible soln in this pop, hence no display");
    }
    else
    {
        if (choice!=3)
            fprintf(gp,"set title 'Generation #%d'\n unset key\n plot 'plot.out' w points pointtype 6 pointsize 1\n",ii);
        else
            fprintf(gp,"set title 'Generation #%d'\n set view %d,%d\n unset key\n splot 'plot.out' w points pointtype 6 pointsize 1\n",ii,angle1,angle2);
        fflush(gp);
    }
    fclose(fpt);
    return;
}


/* Routine for usual non-domination checking
 It will return the following values
 1 if a dominates b
 -1 if b dominates a
 0 if both a and b are non-dominated */

int check_dominance (individual *a, individual *b)
{
    int i;
    int flag1;
    int flag2;
    flag1 = 0;
    flag2 = 0;
    if (a->constr_violation<0 && b->constr_violation<0)
    {
        if (a->constr_violation > b->constr_violation)
        {
            return (1);
        }
        else
        {
            if (a->constr_violation < b->constr_violation)
            {
                return (-1);
            }
            else
            {
                return (0);
            }
        }
    }
    else
    {
        if (a->constr_violation < 0 && b->constr_violation == 0)
        {
            return (-1);
        }
        else
        {
            if (a->constr_violation == 0 && b->constr_violation <0)
            {
                return (1);
            }
            else
            {
                for (i=0; i<nobj; i++)
                {
                    /*if (a->obj[i] < b->obj[i])*/
                    if (a->obj[i] > b->obj[i])
                    {
                        flag1 = 1;
                        
                    }
                    else
                    {
                        /*if (a->obj[i] > b->obj[i])*/
                        if (a->obj[i] < b->obj[i])
                        {
                            flag2 = 1;
                        }
                    }
                }
                if (flag1==1 && flag2==0)
                {
                    return (1);
                }
                else
                {
                    if (flag1==0 && flag2==1)
                    {
                        return (-1);
                    }
                    else
                    {
                        return (0);
                    }
                }
            }
        }
    }
}


/* Routine to perform non-dominated sorting */
void fill_nondominated_sort (population *mixed_pop, population *new_pop)
{
    int flag;
    int i, j;
    int end;
    int front_size;
    int archieve_size;
    int rank=1;
    list *pool;
    list *elite;
    list *temp1, *temp2;
    pool = (list *)malloc(sizeof(list));
    elite = (list *)malloc(sizeof(list));
    front_size = 0;
    archieve_size=0;
    pool->index = -1;
    pool->parent = NULL;
    pool->child = NULL;
    elite->index = -1;
    elite->parent = NULL;
    elite->child = NULL;
    temp1 = pool;
    for (i=0; i<2*popsize; i++)
    {
        insert (temp1,i);
        temp1 = temp1->child;
    }
    i=0;
    do
    {
        temp1 = pool->child;
        insert (elite, temp1->index);
        front_size = 1;
        temp2 = elite->child;
        temp1 = del (temp1);
        temp1 = temp1->child;
        do
        {
            temp2 = elite->child;
            if (temp1==NULL)
            {
                break;
            }
            do
            {
                end = 0;
                flag = check_dominance (&(mixed_pop->ind[temp1->index]), &(mixed_pop->ind[temp2->index]));
                if (flag == 1)
                {
                    insert (pool, temp2->index);
                    temp2 = del (temp2);
                    front_size--;
                    temp2 = temp2->child;
                }
                if (flag == 0)
                {
                    temp2 = temp2->child;
                }
                if (flag == -1)
                {
                    end = 1;
                }
            }
            while (end!=1 && temp2!=NULL);
            if (flag == 0 || flag == 1)
            {
                insert (elite, temp1->index);
                front_size++;
                temp1 = del (temp1);
            }
            temp1 = temp1->child;
        }
        while (temp1 != NULL);
        temp2 = elite->child;
        j=i;
        if ( (archieve_size+front_size) <= popsize)
        {
            do
            {
                copy_ind (&mixed_pop->ind[temp2->index], &new_pop->ind[i]);
                new_pop->ind[i].rank = rank;
                archieve_size+=1;
                temp2 = temp2->child;
                i+=1;
            }
            while (temp2 != NULL);
            assign_crowding_distance_indices (new_pop, j, i-1);
            rank+=1;
        }
        else
        {
            crowding_fill (mixed_pop, new_pop, i, front_size, elite);
            archieve_size = popsize;
            for (j=i; j<popsize; j++)
            {
                new_pop->ind[j].rank = rank;
            }
        }
        temp2 = elite->child;
        do
        {
            temp2 = del (temp2);
            temp2 = temp2->child;
        }
        while (elite->child !=NULL);
    }
    while (archieve_size < popsize);
    while (pool!=NULL)
    {
        temp1 = pool;
        pool = pool->child;
        free (temp1);
    }
    while (elite!=NULL)
    {
        temp1 = elite;
        elite = elite->child;
        free (temp1);
    }
    return;
}

/* Routine to fill a population with individuals in the decreasing order of crowding distance */
void crowding_fill (population *mixed_pop, population *new_pop, int count, int front_size, list *elite)
{
    int *dist;
    list *temp;
    int i, j;
    assign_crowding_distance_list (mixed_pop, elite->child, front_size);
    dist = (int *)malloc(front_size*sizeof(int));
    temp = elite->child;
    for (j=0; j<front_size; j++)
    {
        dist[j] = temp->index;
        temp = temp->child;
    }
    quicksort_dist (mixed_pop, dist, front_size);
    for (i=count, j=front_size-1; i<popsize; i++, j--)
    {
        copy_ind(&mixed_pop->ind[dist[j]], &new_pop->ind[i]);
    }
    free (dist);
    return;
}

/* Routine to evaluate objective function values and constraints for a population */
void evaluate_pop (population *pop)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        evaluate_ind (&(pop->ind[i]));
    }
    return;
}

/* Routine to evaluate objective function values and constraints for an individual */
void evaluate_ind (individual *ind)
{
    int j;
    test_problem (ind->xreal, ind->xbin, ind->gene, ind->obj, ind->constr);
    if (ncon==0)
    {
        ind->constr_violation = 0.0;
    }
    else
    {
        ind->constr_violation = 0.0;
        for (j=0; j<ncon; j++)
        {
            if (ind->constr[j]<0.0)
            {
                ind->constr_violation += ind->constr[j];
            }
        }
    }
    return;
}

/* Function to initialize a population randomly */
void initialize_pop (population *pop)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        initialize_ind (&(pop->ind[i]));
    }
    return;
}

/* Function to initialize an individual randomly */
void initialize_ind (individual *ind)
{
    int j, k;
    if (nreal!=0)
    {
        for (j=0; j<nreal; j++)
        {
            ind->xreal[j] = rndreal (min_realvar[j], max_realvar[j]);
        }
    }
    if (nbin!=0)
    {
        for (j=0; j<nbin; j++)
        {
            for (k=0; k<nbits[j]; k++)
            {
                if (randomperc() <= 0.5)
                {
                    ind->gene[j][k] = 0;
                }
                else
                {
                    ind->gene[j][k] = 1;
                }
            }
        }
    }
    return;
}

/* Insert an element X into the list at location specified by NODE */
void insert (list *node, int x)
{
    list *temp;
    if (node==NULL)
    {
        printf("\n Error!! asked to enter after a NULL pointer, hence exiting \n");
        exit(1);
    }
    temp = (list *)malloc(sizeof(list));
    temp->index = x;
    temp->child = node->child;
    temp->parent = node;
    if (node->child != NULL)
    {
        node->child->parent = temp;
    }
    node->child = temp;
    return;
}

/* Delete the node NODE from the list */
list* del (list *node)
{
    list *temp;
    if (node==NULL)
    {
        printf("\n Error!! asked to delete a NULL pointer, hence exiting \n");
        exit(1);
    }
    temp = node->parent;
    temp->child = node->child;
    if (temp->child!=NULL)
    {
        temp->child->parent = temp;
    }
    free (node);
    return (temp);
}


/* Routine to merge two populations into one */
void merge(population *pop1, population *pop2, population *pop3)
{
    int i, k;
    for (i=0; i<popsize; i++)
    {
        copy_ind (&(pop1->ind[i]), &(pop3->ind[i]));
    }
    for (i=0, k=popsize; i<popsize; i++, k++)
    {
        copy_ind (&(pop2->ind[i]), &(pop3->ind[k]));
    }
    return;
}

/* Routine to copy an individual 'ind1' into another individual 'ind2' */
void copy_ind (individual *ind1, individual *ind2)
{
    int i, j;
    ind2->rank = ind1->rank;
    ind2->constr_violation = ind1->constr_violation;
    ind2->crowd_dist = ind1->crowd_dist;
    if (nreal!=0)
    {
        for (i=0; i<nreal; i++)
        {
            ind2->xreal[i] = ind1->xreal[i];
        }
    }
    if (nbin!=0)
    {
        for (i=0; i<nbin; i++)
        {
            ind2->xbin[i] = ind1->xbin[i];
            for (j=0; j<nbits[i]; j++)
            {
                ind2->gene[i][j] = ind1->gene[i][j];
            }
        }
    }
    for (i=0; i<nobj; i++)
    {
        ind2->obj[i] = ind1->obj[i];
    }
    if (ncon!=0)
    {
        for (i=0; i<ncon; i++)
        {
            ind2->constr[i] = ind1->constr[i];
        }
    }
    return;
}

/* Function to perform mutation in a population */
void mutation_pop (population *pop)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        mutation_ind(&(pop->ind[i]));
    }
    return;
}

/* Function to perform mutation of an individual */
void mutation_ind (individual *ind)
{
    if (nreal!=0)
    {
        real_mutate_ind(ind);
    }
    if (nbin!=0)
    {
        bin_mutate_ind(ind);
    }
    return;
}

/* Routine for binary mutation of an individual */
void bin_mutate_ind (individual *ind)
{
    int j, k;
    double prob;
    for (j=0; j<nbin; j++)
    {
        for (k=0; k<nbits[j]; k++)
        {
            prob = randomperc();
            if (prob <=pmut_bin)
            {
                if(debuglog) printf("\nMutate bit %d", j);
                if (ind->gene[j][k] == 0)
                {
                    ind->gene[j][k] = 1;
                }
                else
                {
                    ind->gene[j][k] = 0;
                }
                nbinmut+=1;
            }
        }
    }
    return;
}

/* Routine for real polynomial mutation of an individual */
void real_mutate_ind (individual *ind)
{
    int j;
    double rnd, delta1, delta2, mut_pow, deltaq;
    double y, yl, yu, val, xy;
    for (j=0; j<nreal; j++)
    {
        if (randomperc() <= pmut_real)
        {
            y = ind->xreal[j];
            yl = min_realvar[j];
            yu = max_realvar[j];
            delta1 = (y-yl)/(yu-yl);
            delta2 = (yu-y)/(yu-yl);
            rnd = randomperc();
            mut_pow = 1.0/(eta_m+1.0);
            if (rnd <= 0.5)
            {
                xy = 1.0-delta1;
                val = 2.0*rnd+(1.0-2.0*rnd)*(pow(xy,(eta_m+1.0)));
                deltaq =  pow(val,mut_pow) - 1.0;
            }
            else
            {
                xy = 1.0-delta2;
                val = 2.0*(1.0-rnd)+2.0*(rnd-0.5)*(pow(xy,(eta_m+1.0)));
                deltaq = 1.0 - (pow(val,mut_pow));
            }
            y = y + deltaq*(yu-yl);
            if (y<yl)
                y = yl;
            if (y>yu)
                y = yu;
            ind->xreal[j] = y;
            nrealmut+=1;
        }
    }
    return;
}


double seed;
double oldrand[55];
int jrand;

/* Get seed number for random and start it up */
void randomize()
{
    int j1;
    for(j1=0; j1<=54; j1++)
    {
        oldrand[j1] = 0.0;
    }
    jrand=0;
    warmup_random (seed);
    return;
}

/* Get randomize off and running */
void warmup_random (double seed)
{
    int j1, ii;
    double new_random, prev_random;
    oldrand[54] = seed;
    new_random = 0.000000001;
    prev_random = seed;
    for(j1=1; j1<=54; j1++)
    {
        ii = (21*j1)%54;
        oldrand[ii] = new_random;
        new_random = prev_random-new_random;
        if(new_random<0.0)
        {
            new_random += 1.0;
        }
        prev_random = oldrand[ii];
    }
    advance_random ();
    advance_random ();
    advance_random ();
    jrand = 0;
    return;
}

/* Create next batch of 55 random numbers */
void advance_random ()
{
    int j1;
    double new_random;
    for(j1=0; j1<24; j1++)
    {
        new_random = oldrand[j1]-oldrand[j1+31];
        if(new_random<0.0)
        {
            new_random = new_random+1.0;
        }
        oldrand[j1] = new_random;
    }
    for(j1=24; j1<55; j1++)
    {
        new_random = oldrand[j1]-oldrand[j1-24];
        if(new_random<0.0)
        {
            new_random = new_random+1.0;
        }
        oldrand[j1] = new_random;
    }
}

/* Fetch a single random number between 0.0 and 1.0 */
double randomperc()
{
    jrand++;
    if(jrand>=55)
    {
        jrand = 1;
        advance_random();
    }
    return((double)oldrand[jrand]);
}

/* Fetch a single random integer between low and high including the bounds */
int rnd (int low, int high)
{
    int res;
    if (low >= high)
    {
        res = low;
    }
    else
    {
        res = low + (randomperc()*(high-low+1));
        if (res > high)
        {
            res = high;
        }
    }
    return (res);
}

/* Fetch a single random real number between low and high including the bounds */
double rndreal (double low, double high)
{
    return (low + (high-low)*randomperc());
}


/* Function to assign rank and crowding distance to a population of size pop_size*/
void assign_rank_and_crowding_distance (population *new_pop)
{
    int flag;
    int i;
    int end;
    int front_size;
    int rank=1;
    list *orig;
    list *cur;
    list *temp1, *temp2;
    orig = (list *)malloc(sizeof(list));
    cur = (list *)malloc(sizeof(list));
    front_size = 0;
    orig->index = -1;
    orig->parent = NULL;
    orig->child = NULL;
    cur->index = -1;
    cur->parent = NULL;
    cur->child = NULL;
    temp1 = orig;
    for (i=0; i<popsize; i++)
    {
        insert (temp1,i);
        temp1 = temp1->child;
    }
    do
    {
        if (orig->child->child == NULL)
        {
            new_pop->ind[orig->child->index].rank = rank;
            new_pop->ind[orig->child->index].crowd_dist = INF;
            break;
        }
        temp1 = orig->child;
        insert (cur, temp1->index);
        front_size = 1;
        temp2 = cur->child;
        temp1 = del (temp1);
        temp1 = temp1->child;
        do
        {
            temp2 = cur->child;
            do
            {
                end = 0;
                flag = check_dominance (&(new_pop->ind[temp1->index]), &(new_pop->ind[temp2->index]));
                if (flag == 1)
                {
                    insert (orig, temp2->index);
                    temp2 = del (temp2);
                    front_size--;
                    temp2 = temp2->child;
                }
                if (flag == 0)
                {
                    temp2 = temp2->child;
                }
                if (flag == -1)
                {
                    end = 1;
                }
            }
            while (end!=1 && temp2!=NULL);
            if (flag == 0 || flag == 1)
            {
                insert (cur, temp1->index);
                front_size++;
                temp1 = del (temp1);
            }
            temp1 = temp1->child;
        }
        while (temp1 != NULL);
        temp2 = cur->child;
        do
        {
            new_pop->ind[temp2->index].rank = rank;
            temp2 = temp2->child;
        }
        while (temp2 != NULL);
        assign_crowding_distance_list (new_pop, cur->child, front_size);
        temp2 = cur->child;
        do
        {
            temp2 = del (temp2);
            temp2 = temp2->child;
        }
        while (cur->child !=NULL);
        rank+=1;
    }
    while (orig->child!=NULL);
    free (orig);
    free (cur);
    return;
}

/* Function to print the information of a population in a file */
void report_pop (population *pop, FILE *fpt)
{
    int i, j, k;
    for (i=0; i<popsize; i++)
    {
        for (j=0; j<nobj; j++)
        {
            fprintf(fpt,"%e\t",pop->ind[i].obj[j]);
        }
        if (ncon!=0)
        {
            for (j=0; j<ncon; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i].constr[j]);
            }
        }
        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i].xreal[j]);
            }
        }
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
                for (k=0; k<nbits[j]; k++)
                {
                    fprintf(fpt,"%d\t",pop->ind[i].gene[j][k]);
                }
            }
        }
        fprintf(fpt,"%e\t",pop->ind[i].constr_violation);
        fprintf(fpt,"%d\t",pop->ind[i].rank);
        fprintf(fpt,"%e\n",pop->ind[i].crowd_dist);
    }
    return;
}

void print_pop (population *pop) {
    int i, j, k;
    for (i=0; i<popsize; i++)
    {
        for (j=0; j<nobj; j++)
        {
            printf("%e\t",pop->ind[i].obj[j]);
        }
        if (ncon!=0)
        {
            for (j=0; j<ncon; j++)
            {
                printf("%e\t",pop->ind[i].constr[j]);
            }
        }
        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {
                printf("%e\t",pop->ind[i].xreal[j]);
            }
        }
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
                for (k=0; k<nbits[j]; k++)
                {
                    printf("%d\t",pop->ind[i].gene[j][k]);
                }
            }
        }
        printf("%e\t",pop->ind[i].constr_violation);
        printf("%d\t",pop->ind[i].rank);
        printf("%e\n",pop->ind[i].crowd_dist);
    }
    return;
}

/* Function to print the information of feasible and non-dominated population in a file */
void report_feasible (population *pop, FILE *fpt)
{
    int i, j, k;
    for (i=0; i<popsize; i++)
    {
        if (pop->ind[i].constr_violation == 0.0 && pop->ind[i].rank==1)
        {
            for (j=0; j<nobj; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i].obj[j]);
            }
            if (ncon!=0)
            {
                for (j=0; j<ncon; j++)
                {
                    fprintf(fpt,"%e\t",pop->ind[i].constr[j]);
                }
            }
            if (nreal!=0)
            {
                for (j=0; j<nreal; j++)
                {
                    fprintf(fpt,"%e\t",pop->ind[i].xreal[j]);
                }
            }
            if (nbin!=0)
            {
                for (j=0; j<nbin; j++)
                {
                    for (k=0; k<nbits[j]; k++)
                    {
                        fprintf(fpt,"%d\t",pop->ind[i].gene[j][k]);
                    }
                }
            }
            fprintf(fpt,"%e\t",pop->ind[i].constr_violation);
            fprintf(fpt,"%d\t",pop->ind[i].rank);
            fprintf(fpt,"%e\n",pop->ind[i].crowd_dist);
        }
    }
    return;
}

/* Randomized quick sort routine to sort a population based on a particular objective chosen */
void quicksort_front_obj(population *pop, int objcount, int obj_array[], int obj_array_size)
{
    q_sort_front_obj (pop, objcount, obj_array, 0, obj_array_size-1);
    return;
}

/* Actual implementation of the randomized quick sort used to sort a population based on a particular objective chosen */
void q_sort_front_obj(population *pop, int objcount, int obj_array[], int left, int right)
{
    int index;
    int temp;
    int i, j;
    double pivot;
    if (left<right)
    {
        index = rnd (left, right);
        temp = obj_array[right];
        obj_array[right] = obj_array[index];
        obj_array[index] = temp;
        pivot = pop->ind[obj_array[right]].obj[objcount];
        i = left-1;
        for (j=left; j<right; j++)
        {
            if (pop->ind[obj_array[j]].obj[objcount] <= pivot)
            {
                i+=1;
                temp = obj_array[j];
                obj_array[j] = obj_array[i];
                obj_array[i] = temp;
            }
        }
        index=i+1;
        temp = obj_array[index];
        obj_array[index] = obj_array[right];
        obj_array[right] = temp;
        q_sort_front_obj (pop, objcount, obj_array, left, index-1);
        q_sort_front_obj (pop, objcount, obj_array, index+1, right);
    }
    return;
}

/* Randomized quick sort routine to sort a population based on crowding distance */
void quicksort_dist(population *pop, int *dist, int front_size)
{
    q_sort_dist (pop, dist, 0, front_size-1);
    return;
}

/* Actual implementation of the randomized quick sort used to sort a population based on crowding distance */
void q_sort_dist(population *pop, int *dist, int left, int right)
{
    int index;
    int temp;
    int i, j;
    double pivot;
    if (left<right)
    {
        index = rnd (left, right);
        temp = dist[right];
        dist[right] = dist[index];
        dist[index] = temp;
        pivot = pop->ind[dist[right]].crowd_dist;
        i = left-1;
        for (j=left; j<right; j++)
        {
            if (pop->ind[dist[j]].crowd_dist <= pivot)
            {
                i+=1;
                temp = dist[j];
                dist[j] = dist[i];
                dist[i] = temp;
            }
        }
        index=i+1;
        temp = dist[index];
        dist[index] = dist[right];
        dist[right] = temp;
        q_sort_dist (pop, dist, left, index-1);
        q_sort_dist (pop, dist, index+1, right);
    }
    return;
}

# define onemax
/*# define trap5*/
/*# define maxcut*/

/*  Test problem ONEMAX
 # of real variables = 0
 # of bin variables = nbin
 # of objectives = 2
 # of constraints = 0
 */

#ifdef onemax
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    int i;
    double f1, f2;
    f1 = 0;
    f2 = 0;
    for (i=0; i < nbin; i++)
    {
        if (gene[i][0] == 0)
            f1+=1;
        else
            f2+=1;
    }
    obj[0]=f1;
    obj[1]=f2;
    return;
}
#endif

/*  Test problem TRAP5
 # of real variables = 0
 # of bin variables = nbin
 # of objectives = 2
 # of constraints = 0
 */
#ifdef trap5
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    int    i, j, m, u0,u1,k,f1,f2;
    
    k = 5;
    m      = nbin / k;
    f1=0;
    f2=0;
    for( i = 0; i < m; i++ )
    {
        u0 = 0;
        u1 = 0;
        for( j = 0; j < k; j++ )
        {
            if (gene[i*k+j][0] == 1)
                u1+=1;
            else
                u0+=1;
        }
        
        if( u1 == k )
            f1 += k;
        else
            f1 += (k-1-u1);
        
        if( u0 == k )
            f2 += k;
        else
            f2 += (k-1-u0);
    }
    
    
    obj[0] = f1;
    obj[1] = f2;
    return;
}
#endif

/*  Test problem SCH1
 # of real variables = 1
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 0
 */

#ifdef sch1
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    obj[0] = pow(xreal[0],2.0);
    obj[1] = pow((xreal[0]-2.0),2.0);
    return;
}
#endif

/*  Test problem SCH2
 # of real variables = 1
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 0
 */

#ifdef sch2
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    if (xreal[0]<=1.0)
    {
        obj[0] = -xreal[0];
        obj[1] = pow((xreal[0]-5.0),2.0);
        return;
    }
    if (xreal[0]<=3.0)
    {
        obj[0] = xreal[0]-2.0;
        obj[1] = pow((xreal[0]-5.0),2.0);
        return;
    }
    if (xreal[0]<=4.0)
    {
        obj[0] = 4.0-xreal[0];
        obj[1] = pow((xreal[0]-5.0),2.0);
        return;
    }
    obj[0] = xreal[0]-4.0;
    obj[1] = pow((xreal[0]-5.0),2.0);
    return;
}
#endif

/*  Test problem FON
 # of real variables = n
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 0
 */

#ifdef fon
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double s1, s2;
    int i;
    s1 = s2 = 0.0;
    for (i=0; i<nreal; i++)
    {
        s1 += pow((xreal[i]-(1.0/sqrt((double)nreal))),2.0);
        s2 += pow((xreal[i]+(1.0/sqrt((double)nreal))),2.0);
    }
    obj[0] = 1.0 - exp(-s1);
    obj[1] = 1.0 - exp(-s2);
    return;
}
#endif

/*  Test problem KUR
 # of real variables = 3
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 0
 */

#ifdef kur
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    int i;
    double res1, res2;
    res1 = -0.2*sqrt((xreal[0]*xreal[0]) + (xreal[1]*xreal[1]));
    res2 = -0.2*sqrt((xreal[1]*xreal[1]) + (xreal[2]*xreal[2]));
    obj[0] = -10.0*( exp(res1) + exp(res2));
    obj[1] = 0.0;
    for (i=0; i<3; i++)
    {
        obj[1] += pow(fabs(xreal[i]),0.8) + 5.0*sin(pow(xreal[i],3.0));
    }
    return;
}
#endif

/*  Test problem POL
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 0
 */

#ifdef pol
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double a1, a2, b1, b2;
    a1 = 0.5*sin(1.0) - 2.0*cos(1.0) + sin(2.0) - 1.5*cos(2.0);
    a2 = 1.5*sin(1.0) - cos(1.0) + 2.0*sin(2.0) - 0.5*cos(2.0);
    b1 = 0.5*sin(xreal[0]) - 2.0*cos(xreal[0]) + sin(xreal[1]) - 1.5*cos(xreal[1]);
    b2 = 1.5*sin(xreal[0]) - cos(xreal[0]) + 2.0*sin(xreal[1]) - 0.5*cos(xreal[1]);
    obj[0] = 1.0 + pow((a1-b1),2.0) + pow((a2-b2),2.0);
    obj[1] = pow((xreal[0]+3.0),2.0) + pow((xreal[1]+1.0),2.0);
    return;
}
#endif

/*  Test problem VNT
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 3
 # of constraints = 0
 */

#ifdef vnt
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    obj[0] = 0.5*(xreal[0]*xreal[0] + xreal[1]*xreal[1]) + sin(xreal[0]*xreal[0] + xreal[1]*xreal[1]);
    obj[1] = (pow((3.0*xreal[0] - 2.0*xreal[1] + 4.0),2.0))/8.0 + (pow((xreal[0]-xreal[1]+1.0),2.0))/27.0 + 15.0;
    obj[2] = 1.0/(xreal[0]*xreal[0] + xreal[1]*xreal[1] + 1.0) - 1.1*exp(-(xreal[0]*xreal[0] + xreal[1]*xreal[1]));
    return;
}
#endif

/*  Test problem ZDT1
 # of real variables = 30
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 0
 */

#ifdef zdt1
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double f1, f2, g, h;
    int i;
    f1 = xreal[0];
    g = 0.0;
    for (i=1; i<30; i++)
    {
        g += xreal[i];
    }
    g = 9.0*g/29.0;
    g += 1.0;
    h = 1.0 - sqrt(f1/g);
    f2 = g*h;
    obj[0] = f1;
    obj[1] = f2;
    return;
}
#endif

/*  Test problem ZDT2
 # of real variables = 30
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 0
 */

#ifdef zdt2
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double f1, f2, g, h;
    int i;
    f1 = xreal[0];
    g = 0.0;
    for (i=1; i<30; i++)
    {
        g += xreal[i];
    }
    g = 9.0*g/29.0;
    g += 1.0;
    h = 1.0 - pow((f1/g),2.0);
    f2 = g*h;
    obj[0] = f1;
    obj[1] = f2;
    return;
}
#endif

/*  Test problem ZDT3
 # of real variables = 30
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 0
 */

#ifdef zdt3
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double f1, f2, g, h;
    int i;
    f1 = xreal[0];
    g = 0.0;
    for (i=1; i<30; i++)
    {
        g += xreal[i];
    }
    g = 9.0*g/29.0;
    g += 1.0;
    h = 1.0 - sqrt(f1/g) - (f1/g)*sin(10.0*PI*f1);
    f2 = g*h;
    obj[0] = f1;
    obj[1] = f2;
    return;
}
#endif

/*  Test problem ZDT4
 # of real variables = 10
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 0
 */

#ifdef zdt4
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double f1, f2, g, h;
    int i;
    f1 = xreal[0];
    g = 0.0;
    for (i=1; i<10; i++)
    {
        g += xreal[i]*xreal[i] - 10.0*cos(4.0*PI*xreal[i]);
    }
    g += 91.0;
    h = 1.0 - sqrt(f1/g);
    f2 = g*h;
    obj[0] = f1;
    obj[1] = f2;
    return;
}
#endif

/*  Test problem ZDT5
 # of real variables = 0
 # of bin variables = 11
 # of bits for binvar1 = 30
 # of bits for binvar2-11 = 5
 # of objectives = 2
 # of constraints = 0
 */

#ifdef zdt5
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    int i, j;
    int u[11];
    int v[11];
    double f1, f2, g, h;
    for (i=0; i<11; i++)
    {
        u[i] = 0;
    }
    for (j=0; j<30; j++)
    {
        if (gene[0][j] == 1)
        {
            u[0]++;
        }
    }
    for (i=1; i<11; i++)
    {
        for (j=0; j<4; j++)
        {
            if (gene[i][j] == 1)
            {
                u[i]++;
            }
        }
    }
    f1 = 1.0 + u[0];
    for (i=1; i<11; i++)
    {
        if (u[i] < 5)
        {
            v[i] = 2 + u[i];
        }
        else
        {
            v[i] = 1;
        }
    }
    g = 0;
    for (i=1; i<11; i++)
    {
        g += v[i];
    }
    h = 1.0/f1;
    f2 = g*h;
    obj[0] = f1;
    obj[1] = f2;
    return;
}
#endif

/*  Test problem ZDT6
 # of real variables = 10
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 0
 */

#ifdef zdt6
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double f1, f2, g, h;
    int i;
    f1 = 1.0 - ( exp(-4.0*xreal[0]) ) * pow( (sin(6.0*PI*xreal[0])),6.0 );
    g = 0.0;
    for (i=1; i<10; i++)
    {
        g += xreal[i];
    }
    g = g/9.0;
    g = pow(g,0.25);
    g = 1.0 + 9.0*g;
    h = 1.0 - pow((f1/g),2.0);
    f2 = g*h;
    obj[0] = f1;
    obj[1] = f2;
    return;
}
#endif

/*  Test problem BNH
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 2
 */

#ifdef bnh
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    obj[0] = 4.0*(xreal[0]*xreal[0] + xreal[1]*xreal[1]);
    obj[1] = pow((xreal[0]-5.0),2.0) + pow((xreal[1]-5.0),2.0);
    constr[0] = 1.0 - (pow((xreal[0]-5.0),2.0) + xreal[1]*xreal[1])/25.0;
    constr[1] = (pow((xreal[0]-8.0),2.0) + pow((xreal[1]+3.0),2.0))/7.7 - 1.0;
    return;
}
#endif

/*  Test problem OSY
 # of real variables = 6
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 6
 */

#ifdef osy
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    obj[0] = -(25.0*pow((xreal[0]-2.0),2.0) + pow((xreal[1]-2.0),2.0) + pow((xreal[2]-1.0),2.0) + pow((xreal[3]-4.0),2.0) + pow((xreal[4]-1.0),2.0));
    obj[1] = xreal[0]*xreal[0] +  xreal[1]*xreal[1] + xreal[2]*xreal[2] + xreal[3]*xreal[3] + xreal[4]*xreal[4] + xreal[5]*xreal[5];
    constr[0] = (xreal[0]+xreal[1])/2.0 - 1.0;
    constr[1] = 1.0 - (xreal[0]+xreal[1])/6.0;
    constr[2] = 1.0 - xreal[1]/2.0 + xreal[0]/2.0;
    constr[3] = 1.0 - xreal[0]/2.0 + 3.0*xreal[1]/2.0;
    constr[4] = 1.0 - (pow((xreal[2]-3.0),2.0))/4.0 - xreal[3]/4.0;
    constr[5] = (pow((xreal[4]-3.0),2.0))/4.0 + xreal[5]/4.0 - 1.0;
    return;
}
#endif

/*  Test problem SRN
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 2
 */

#ifdef srn
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    obj[0] = 2.0 + pow((xreal[0]-2.0),2.0) + pow((xreal[1]-1.0),2.0);
    obj[1] = 9.0*xreal[0] - pow((xreal[1]-1.0),2.0);
    constr[0] = 1.0 - (pow(xreal[0],2.0) + pow(xreal[1],2.0))/225.0;
    constr[1] = 3.0*xreal[1]/10.0 - xreal[0]/10.0 - 1.0;
    return;
}
#endif

/*  Test problem TNK
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 2
 */

#ifdef tnk
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    obj[0] = xreal[0];
    obj[1] = xreal[1];
    if (xreal[1] == 0.0)
    {
        constr[0] = -1.0;
    }
    else
    {
        constr[0] = xreal[0]*xreal[0] + xreal[1]*xreal[1] - 0.1*cos(16.0*atan(xreal[0]/xreal[1])) - 1.0;
    }
    constr[1] = 1.0 - 2.0*pow((xreal[0]-0.5),2.0) + 2.0*pow((xreal[1]-0.5),2.0);
    return;
}
#endif

/*  Test problem CTP1
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 2
 */

#ifdef ctp1
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double g;
    g = 1.0 + xreal[1];
    obj[0] = xreal[0];
    obj[1] = g*exp(-obj[0]/g);
    constr[0] = obj[1]/(0.858*exp(-0.541*obj[0]))-1.0;
    constr[1] = obj[1]/(0.728*exp(-0.295*obj[0]))-1.0;
    return;
}
#endif

/*  Test problem CTP2
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 1
 */

#ifdef ctp2
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double g;
    double theta, a, b, c, d, e;
    double exp1, exp2;
    theta = -0.2*PI;
    a = 0.2;
    b = 10.0;
    c = 1.0;
    d = 6.0;
    e = 1.0;
    g = 1.0 + xreal[1];
    obj[0] = xreal[0];
    obj[1] = g*(1.0  - sqrt(obj[0]/g));
    exp1 = (obj[1]-e)*cos(theta) - obj[0]*sin(theta);
    exp2 = (obj[1]-e)*sin(theta) + obj[0]*cos(theta);
    exp2 = b*PI*pow(exp2,c);
    exp2 = fabs(sin(exp2));
    exp2 = a*pow(exp2,d);
    constr[0] = exp1/exp2 - 1.0;
    return;
}
#endif

/*  Test problem CTP3
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 1
 */

#ifdef ctp3
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double g;
    double theta, a, b, c, d, e;
    double exp1, exp2;
    theta = -0.2*PI;
    a = 0.1;
    b = 10.0;
    c = 1.0;
    d = 0.5;
    e = 1.0;
    g = 1.0 + xreal[1];
    obj[0] = xreal[0];
    obj[1] = g*(1.0  - sqrt(obj[0]/g));
    exp1 = (obj[1]-e)*cos(theta) - obj[0]*sin(theta);
    exp2 = (obj[1]-e)*sin(theta) + obj[0]*cos(theta);
    exp2 = b*PI*pow(exp2,c);
    exp2 = fabs(sin(exp2));
    exp2 = a*pow(exp2,d);
    constr[0] = exp1/exp2 - 1.0;
    return;
}
#endif

/*  Test problem CTP4
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 1
 */

#ifdef ctp4
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double g;
    double theta, a, b, c, d, e;
    double exp1, exp2;
    theta = -0.2*PI;
    a = 0.75;
    b = 10.0;
    c = 1.0;
    d = 0.5;
    e = 1.0;
    g = 1.0 + xreal[1];
    obj[0] = xreal[0];
    obj[1] = g*(1.0  - sqrt(obj[0]/g));
    exp1 = (obj[1]-e)*cos(theta) - obj[0]*sin(theta);
    exp2 = (obj[1]-e)*sin(theta) + obj[0]*cos(theta);
    exp2 = b*PI*pow(exp2,c);
    exp2 = fabs(sin(exp2));
    exp2 = a*pow(exp2,d);
    constr[0] = exp1/exp2 - 1.0;
    return;
}
#endif

/*  Test problem CTP5
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 1
 */

#ifdef ctp5
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double g;
    double theta, a, b, c, d, e;
    double exp1, exp2;
    theta = -0.2*PI;
    a = 0.1;
    b = 10.0;
    c = 2.0;
    d = 0.5;
    e = 1.0;
    g = 1.0 + xreal[1];
    obj[0] = xreal[0];
    obj[1] = g*(1.0  - sqrt(obj[0]/g));
    exp1 = (obj[1]-e)*cos(theta) - obj[0]*sin(theta);
    exp2 = (obj[1]-e)*sin(theta) + obj[0]*cos(theta);
    exp2 = b*PI*pow(exp2,c);
    exp2 = fabs(sin(exp2));
    exp2 = a*pow(exp2,d);
    constr[0] = exp1/exp2 - 1.0;
    return;
}
#endif

/*  Test problem CTP6
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 1
 */

#ifdef ctp6
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double g;
    double theta, a, b, c, d, e;
    double exp1, exp2;
    theta = 0.1*PI;
    a = 40.0;
    b = 0.5;
    c = 1.0;
    d = 2.0;
    e = -2.0;
    g = 1.0 + xreal[1];
    obj[0] = xreal[0];
    obj[1] = g*(1.0  - sqrt(obj[0]/g));
    exp1 = (obj[1]-e)*cos(theta) - obj[0]*sin(theta);
    exp2 = (obj[1]-e)*sin(theta) + obj[0]*cos(theta);
    exp2 = b*PI*pow(exp2,c);
    exp2 = fabs(sin(exp2));
    exp2 = a*pow(exp2,d);
    constr[0] = exp1/exp2 - 1.0;
    return;
}
#endif

/*  Test problem CTP7
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 1
 */

#ifdef ctp7
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double g;
    double theta, a, b, c, d, e;
    double exp1, exp2;
    theta = -0.05*PI;
    a = 40.0;
    b = 5.0;
    c = 1.0;
    d = 6.0;
    e = 0.0;
    g = 1.0 + xreal[1];
    obj[0] = xreal[0];
    obj[1] = g*(1.0  - sqrt(obj[0]/g));
    exp1 = (obj[1]-e)*cos(theta) - obj[0]*sin(theta);
    exp2 = (obj[1]-e)*sin(theta) + obj[0]*cos(theta);
    exp2 = b*PI*pow(exp2,c);
    exp2 = fabs(sin(exp2));
    exp2 = a*pow(exp2,d);
    constr[0] = exp1/exp2 - 1.0;
    return;
}
#endif

/*  Test problem CTP8
 # of real variables = 2
 # of bin variables = 0
 # of objectives = 2
 # of constraints = 2
 */

#ifdef ctp8
void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double g;
    double theta, a, b, c, d, e;
    double exp1, exp2;
    g = 1.0 + xreal[1];
    obj[0] = xreal[0];
    obj[1] = g*(1.0  - sqrt(obj[0]/g));
    theta = 0.1*PI;
    a = 40.0;
    b = 0.5;
    c = 1.0;
    d = 2.0;
    e = -2.0;
    exp1 = (obj[1]-e)*cos(theta) - obj[0]*sin(theta);
    exp2 = (obj[1]-e)*sin(theta) + obj[0]*cos(theta);
    exp2 = b*PI*pow(exp2,c);
    exp2 = fabs(sin(exp2));
    exp2 = a*pow(exp2,d);
    constr[0] = exp1/exp2 - 1.0;
    theta = -0.05*PI;
    a = 40.0;
    b = 2.0;
    c = 1.0;
    d = 6.0;
    e = 0.0;
    exp1 = (obj[1]-e)*cos(theta) - obj[0]*sin(theta);
    exp2 = (obj[1]-e)*sin(theta) + obj[0]*cos(theta);
    exp2 = b*PI*pow(exp2,c);
    exp2 = fabs(sin(exp2));
    exp2 = a*pow(exp2,d);
    constr[1] = exp1/exp2 - 1.0;
    return;
}
#endif


void selection (population *old_pop, population *new_pop)
{
    int *a1;
    int temp;
    int i,j,index;
    int rand;
    individual *parent1, *parent2;
    individual *child2;
    a1 = (int *)malloc(popsize*sizeof(int));
    // Fill array a1 with numbers 0 until popsize-1
    for (i=0; i<popsize; i++)
    {
        a1[i] = i;
    }
    // Permute the order
    for (i=0; i<popsize; i++)
    {
        rand = rnd (i, popsize-1);
        temp = a1[rand];
        a1[rand] = a1[i];
        a1[i] = temp;
    }
    index=0;
    // Loop over the population
    for (i=0; i<popsize; i+=2)
    {
        // If the indices will exceed the size of the population, reshuffle the a1 array.
        if(index+3 >= popsize)
        {
            for (j=0; j<popsize; j++)
            {
                a1[j] = j;
            }
            for (j=0; j<popsize; j++)
            {
                rand = rnd (j, popsize-1);
                temp = a1[rand];
                a1[rand] = a1[j];
                a1[j] = temp;
            }
            index=0;
        }
        if(i+1 < popsize)
        {
            if(debuglog) cout << "\nPerforming tournament (" << a1[index] << ", " << a1[index+1] << ") and (" << a1[index+2] << "," << a1[index+3] << ")" << endl;
            parent1 = tournament (&old_pop->ind[a1[index]], &old_pop->ind[a1[index+1]]);
            parent2 = tournament (&old_pop->ind[a1[index+2]], &old_pop->ind[a1[index+3]]);
            crossover (parent1, parent2, &new_pop->ind[i], &new_pop->ind[i+1]);
            
            if(debuglog){
                printf("p1: ");
                parent1->print();
                printf("p2: ");
                parent2->print();
                printf("c1: ");
                new_pop->ind[i].print();
                printf("c2: ");
                new_pop->ind[i+1].print();
            }
        }
        else
        {
            parent1 = tournament (&old_pop->ind[a1[index]], &old_pop->ind[a1[index+1]]);
            parent2 = tournament (&old_pop->ind[a1[index+2]], &old_pop->ind[a1[index+3]]);
            
            child2 = (individual*)malloc(sizeof(individual));
            allocate_memory_ind(child2);
            crossover (parent1, parent2, &new_pop->ind[i], child2);
            
            deallocate_memory_ind(child2);
            free(child2);
        }
        index+=4;
    }
    free (a1);
    return;
}
/* Routine for binary tournament */
individual* tournament (individual *ind1, individual *ind2)
{
    int flag;
    flag = check_dominance (ind1, ind2);
    if (flag==1)
    {
        return (ind1);
    }
    if (flag==-1)
    {
        return (ind2);
    }
    if (ind1->crowd_dist > ind2->crowd_dist)
    {
        return(ind1);
    }
    if (ind2->crowd_dist > ind1->crowd_dist)
    {
        return(ind2);
    }
    if ((randomperc()) <= 0.5)
    {
        return(ind1);
    }
    else
    {
        return(ind2);
    }
}
