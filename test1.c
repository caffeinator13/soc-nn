#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
/******************************************************************
   Network Configuration - customized per network
 ******************************************************************/

const int PatternCount = 40192;
const int InputNodes = 3;
const int HiddenNodes = 20;
const int OutputNodes = 1;
const float LearningRate = 0.3;
const float Momentum = 0.9;
const float InitialWeightMax = 0.5;
const float Success = 0.0001;

float Input[PatternCount][InputNodes];
float Target[PatternCount][OutputNodes];

/******************************************************************
   End Network Configuration
 ******************************************************************/

int i, j, p, q, r;
int ReportEvery1000;
int RandomizedIndex[PatternCount];
long  TrainingCycle;
float Rando;
float Error;
float Accum;
float del;

float Hidden[HiddenNodes];
float Output[OutputNodes];
float HiddenWeights[InputNodes + 1][HiddenNodes];
float OutputWeights[HiddenNodes + 1][OutputNodes];
float HiddenDelta[HiddenNodes];
float OutputDelta[OutputNodes];
float ChangeHiddenWeights[InputNodes + 1][HiddenNodes];
float ChangeOutputWeights[HiddenNodes + 1][OutputNodes];


// Function Prototypes.
void initWeights();
void initData();
void check();
double getRand();
void toTerminal();

int main()
{
    initData();
	srand((unsigned)time(0)); // Seed the generator with system time.
	ReportEvery1000 = 1;
  for ( p = 0 ; p < PatternCount ; p++ ) {
    RandomizedIndex[p] = p ;
  }
	initWeights();
	check();
    return 0;
}

void initWeights(){
	/******************************************************************
    Initialize HiddenWeights and ChangeHiddenWeights
  ******************************************************************/
    for(i=0;i<HiddenNodes;i++) {
    	for(j=0;j<=InputNodes;j++){
    		ChangeHiddenWeights[j][i] = 0.0 ;
    		Rando = getRand();

    		HiddenWeights[j][i] = 2.0 * (Rando - 0.5) * InitialWeightMax;
    		printf("%f\n",HiddenWeights[j][i]);    		
    	}
    }

    /******************************************************************
    Initialize OutputWeights and ChangeOutputWeights
  ******************************************************************/
    for(i=0;i<OutputNodes;i++){
    	for(j=0;j<=HiddenNodes;j++){
    		ChangeOutputWeights[j][i] = 0.0;
    		Rando= getRand();

    		OutputWeights[j][i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
    		printf("%f\n",OutputWeights[j][i] );
    	}
    }
    printf("Initial/Untrained Outputs : ");
    toTerminal();
	
    /******************************************************************
    	Begin training
  	******************************************************************/
    	for ( TrainingCycle = 1 ; TrainingCycle < 100000 ; TrainingCycle++) {
    		/******************************************************************
      			Randomize order of training patterns
    		******************************************************************/	
      			//printf("training cycle = %d \n", TrainingCycle );
    		for(p=0;p<PatternCount;p++){

    			q = rand() % PatternCount;
    			r = RandomizedIndex[p];
    			RandomizedIndex[p] = RandomizedIndex[q];
    			RandomizedIndex[q] = r;
    		}
    		Error = 0.0;
    		/******************************************************************
      			Cycle through each training pattern in the randomized order
    		******************************************************************/
    		for ( q = 0 ; q < PatternCount ; q++ ) {
    		      p = RandomizedIndex[q];
    		     /******************************************************************
    		          Compute hidden layer activations
    		      ******************************************************************/

                      for ( i = 0 ; i < HiddenNodes ; i++ ) {
    		             Accum = HiddenWeights[InputNodes][i] ;
    		             for ( j = 0 ; j < InputNodes ; j++ ) {
    		               Accum += Input[p][j] * HiddenWeights[j][i] ;
    		             }
    		             Hidden[i] = 1.0 / (1.0 + exp(-Accum)) ;
    		           }

    		           /******************************************************************
    		             Compute output layer activations and calculate errors
    		           ******************************************************************/

    		           for ( i = 0 ; i < OutputNodes ; i++ ) {
    		             Accum = OutputWeights[HiddenNodes][i] ;
    		             for ( j = 0 ; j < HiddenNodes ; j++ ) {
    		               Accum += Hidden[j] * OutputWeights[j][i] ;
    		             }
    		             Output[i] = 1.0 / (1.0 + exp(-Accum)) ;
    		             OutputDelta[i] = (Target[p][i] - Output[i]) * Output[i] * (1.0 - Output[i]) ;
    		             Error += (Target[p][i] - Output[i]) * (Target[p][i] - Output[i])/PatternCount ;
    		           }

    		           /******************************************************************
    		             Backpropagate errors to hidden layer
    		           ******************************************************************/

    		           for ( i = 0 ; i < HiddenNodes ; i++ ) {
    		             Accum = 0.0 ;
    		             for ( j = 0 ; j < OutputNodes ; j++ ) {
    		               Accum += OutputWeights[i][j] * OutputDelta[j] ;
    		             }
    		             HiddenDelta[i] = Accum * Hidden[i] * (1.0 - Hidden[i]) ;
    		           }


    		           /******************************************************************
    		             Update Inner-->Hidden Weights
    		           ******************************************************************/


    		           for ( i = 0 ; i < HiddenNodes ; i++ ) {
    		             ChangeHiddenWeights[InputNodes][i] = LearningRate * HiddenDelta[i] + Momentum * ChangeHiddenWeights[InputNodes][i] ;
    		             HiddenWeights[InputNodes][i] += ChangeHiddenWeights[InputNodes][i] ;
    		             for ( j = 0 ; j < InputNodes ; j++ ) {
    		               ChangeHiddenWeights[j][i] = LearningRate * Input[p][j] * HiddenDelta[i] + Momentum * ChangeHiddenWeights[j][i];
    		               HiddenWeights[j][i] += ChangeHiddenWeights[j][i] ;
    		             }
    		           }

    		           /******************************************************************
    		             Update Hidden-->Output Weights
    		           ******************************************************************/

    		           for ( i = 0 ; i < OutputNodes ; i ++ ) {
    		             ChangeOutputWeights[HiddenNodes][i] = LearningRate * OutputDelta[i] + Momentum * ChangeOutputWeights[HiddenNodes][i] ;
    		             OutputWeights[HiddenNodes][i] += ChangeOutputWeights[HiddenNodes][i] ;
    		             for ( j = 0 ; j < HiddenNodes ; j++ ) {
    		               ChangeOutputWeights[j][i] = LearningRate * Hidden[j] * OutputDelta[i] + Momentum * ChangeOutputWeights[j][i] ;
    		               OutputWeights[j][i] += ChangeOutputWeights[j][i] ;
    		             }
    		           }
    		         }
    		         /******************************************************************
    		              Every 1000 cycles send data to terminal for display
    		            ******************************************************************/
    		              ReportEvery1000 = ReportEvery1000 -1;
    		              if(ReportEvery1000 == 0){
    		              	printf("TrainingCycle: %ld", TrainingCycle);
    		              	printf("\n Error = %lf",Error);
    		              	toTerminal();

    		              	if(TrainingCycle == 1){
    		              		ReportEvery1000 = 999;
    		              	}
    		              	else{
    		              		ReportEvery1000 = 1000;
    		              	}

    		              }
    		              /******************************************************************
    		                    If error rate is less than pre-determined threshold then end
    		               ******************************************************************/
    		                    if(Error < Success)       break;
    		                    
                                    
                            }
                            printf("\n");
                            printf("TrainingCycle: %ld", TrainingCycle);
    		              	printf("\n Error = %lf",Error);


                            toTerminal();

                            printf("Training Set Solved!");

                            printf("HiddenWeights:");
                            for ( i = 0 ; i < HiddenNodes ; i++ ) {
                                for ( j = 0 ; j <= InputNodes ; j++ ) {
                                  printf("%lf\n",HiddenWeights[j][i]);
                                }
                              }
                              printf("OutputWeights");
                              for ( i = 0 ; i < OutputNodes ; i ++ ) {
                                for ( j = 0 ; j <= HiddenNodes ; j++ ) {

                                  printf("%lf\n",OutputWeights[j][i]);
                                }
                              }
                              
                              ReportEvery1000 =1;


}	


void toTerminal(){
	for (p=0; p<PatternCount;p++){
		printf("********\n");
		printf("Training Pattern: %d",p);
		printf("\nInput = ");
		for(i=0;i<InputNodes;i++){
			printf("%lf\n",Input[p][i]);
		}
		printf("Target = ");
		for(i=0;i<OutputNodes;i++){
			printf("%lf\n",Target[p][i]);
		}
		/******************************************************************
      	Compute hidden layer activations
    	******************************************************************/
      	for(i=0;i<HiddenNodes;i++){
      		Accum = HiddenWeights[InputNodes][i];
      		for(j=0;j<InputNodes;j++){
      			Accum += Input[p][j] * HiddenWeights[j][i];
      		}
      		Hidden[i] = 1.0 / (1.0 + exp(-Accum));
      	}
      	/******************************************************************
      	Compute output layer activations and calculate errors
    	******************************************************************/
      	for ( i = 0 ; i < OutputNodes ; i++ ) {
      	    Accum = OutputWeights[HiddenNodes][i] ;
      	    for ( j = 0 ; j < HiddenNodes ; j++ ) {
      	      	Accum += Hidden[j] * OutputWeights[j][i] ;
      	    }
      	    Output[i] = 1.0 / (1.0 + exp(-Accum)) ;
      	}
      	printf("Output = ");
      	for(i=0;i<OutputNodes;i++){
      		printf("%lf\n",Output[i]);

      	}
	}printf("over terminal\n");
	printf("Error = %0.5f \n", Error);
	printf("training cycle = %d \n", TrainingCycle );
}

double getRand(){
    return double(rand() / double(RAND_MAX));
}

void initData(){
        FILE *inpfile,*oupfile ;
        int ii;
        int jj;
        inpfile=fopen("inp.txt", "r");

        for(ii = 0; ii < PatternCount; ii++)
        {
          for (jj = 0 ; jj < InputNodes; jj++)
          {
            fscanf(inpfile,"%f",&Input[ii][jj]);
            //printf("%lf ",Input[ii][jj], "\n");
          }
          printf("\n");
        }

        fclose(inpfile);
        oupfile=fopen("soc.txt", "r");

        for(ii = 0; ii < PatternCount; ii++)
        {
          for (jj = 0 ; jj < OutputNodes; jj++)
          {
            fscanf(oupfile,"%f",&Target[ii][jj]);
            //printf("%lf ",Target[ii][jj], "\n");
          }
          printf("\n");
        }

        fclose(oupfile);

}

void check() {
	printf("Start Check");
	printf("HiddenWeights :");
  for ( i = 0 ; i < HiddenNodes ; i++ ) {
    for ( j = 0 ; j <= InputNodes ; j++ ) {
      printf("%lf\n",HiddenWeights[j][i]);
    }
  }
  printf("OutputWeights: ");
  for ( i = 0 ; i < OutputNodes ; i ++ ) {
    for ( j = 0 ; j <= HiddenNodes ; j++ ) {
      printf("%lf\n",OutputWeights[j][i]);

    }
  }
  int testnumber = 405;
  float newInput[testnumber][InputNodes];
  float newTarget[testnumber][OutputNodes];
  FILE *testfile,*testtarget;
        int ii;
        int jj;
        testfile=fopen("test.txt", "r");

        for(ii = 0; ii < testnumber; ii++)
        {
          for (jj = 0 ; jj < InputNodes; jj++)
          {
            fscanf(testfile,"%f",&newInput[ii][jj]);
            //printf("%lf ",Input[ii][jj], "\n");
          }
          printf("\n");
        }

        fclose(testfile);
        testtarget=fopen("testtarget.txt", "r");

        for(ii = 0; ii < testnumber; ii++)
        {
          for (jj = 0 ; jj < OutputNodes; jj++)
          {
            fscanf(testtarget,"%f",&newTarget[ii][jj]);
            //printf("%lf ",Target[ii][jj], "\n");
          }
          printf("\n");
        }

        fclose(testtarget);
        
        for ( i = 0 ; i < HiddenNodes ; i++ ) {
    del = HiddenWeights[InputNodes][i] ;
    for ( j = 0 ; j < InputNodes ; j++ ) {
      del += newInput[i][j] * HiddenWeights[j][i] ;
    }
    Hidden[i] = 1.0 / (1.0 + exp(-del)) ;
  }
  for ( i = 0 ; i < testnumber ; i++ ) {
    del = OutputWeights[HiddenNodes][i] ;
    for ( j = 0 ; j < HiddenNodes ; j++ ) {
      del += Hidden[j] * OutputWeights[j][i] ;
    }
    Output[i] = 1.0 / (1.0 + exp(-del)) ;
    OutputDelta[i] = (Target[p][i] - Output[i]) * Output[i] * (1.0 - Output[i]) ;
    Error += 0.5 * (Target[p][i] - Output[i]) * (Target[p][i] - Output[i]) ;
  }

  for ( i = 0 ; i < testnumber ; i++ ) {
    printf("target = %lf\n", newTarget[i][0] );
    printf("Result = %lf\n",Output[i]);
  }
}

