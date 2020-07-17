#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
using namespace std;
const int MOVIES = 6;
const int HALLS = 30;
const int LIST = 30;
const double DURATION[LIST] = {115,75,113,132,101,102,110,91,120,95,112,96,102,216,130,143,139,150,142,142,124,139,99,115,102,95,94,88,112,102};
const double RATES[LIST] = {6.8,6.6,5.7,7.6,5.9,7.6,4.5,3.2,5.4,6.5,4.2,5.3,5.7,7.5,6.2,6.2,7.5,6.7,7,7.1,6.6,6,6.6,6.1,7,5.8,3.3,5.4,5.6,5.7};
const int PARENTS = 2;
int chromosome[HALLS][MOVIES];
double total_time=MOVIES*120;
double total_r=MOVIES*10;

double fitness[HALLS];
double xfitness[HALLS*2];
int parent[PARENTS][MOVIES];
const int MAX_GENERATION = 100;
const int CHILDREN = 2;
const double CROSSOVER_PROBABILITY = 0.2 ;
int parents[2][MOVIES];

int children[CHILDREN][MOVIES];
//declare mutation as a constant
const double MUTATION = 0.9;
int newChromosome[HALLS][MOVIES];
int newChromoCounter=0;
double bestFitness = 0.0;
double avgFitness = 0.0;
int bestChromosome[MOVIES];

ofstream bestFitnessFile, avgFitnessFile, bestChromosomeFile;

 bool check(int id){
 bool checker = false;

                  for (int k =0; k<MOVIES; k++){
                      for (int l=k; l<MOVIES; l++){
                         if (l!=k){
                                 if(chromosome[id][k]==chromosome[id][l]){
                                      checker = true;
                                 }
                         }
                     }
                     }
                  return checker;

 }

void initializeulation(){
	int randNum;
	srand (time(NULL));

	for (int c=0; c<HALLS; c++){
             do{
                for (int i=0; i<MOVIES; i++){

                    randNum = rand() % 30;
                    chromosome[c][i] = randNum;

            }
             }while(check(c));
	}


}


void printChromosome() {
	for (int c=0; c<HALLS; c++) {
		cout << "\tH" << c << "\t";
		for (int i=0; i<MOVIES; i++) {
			cout << chromosome[c][i] << " ";
		}
		cout << endl;
	}
}

void evaluateChromosome() {
    double sum_duration =0;
    double sum_rate = 0;
	double P1 = 0;
	double P2 = 0;

	for (int c=0; c<HALLS; c++) {
        sum_duration =0;
        sum_rate = 0;
		P1 = 0;
		P2 = 0;
		for (int i=0; i<MOVIES; i++) {
                sum_duration=sum_duration+DURATION[chromosome[c][i]];
                sum_rate=sum_rate+RATES[chromosome[c][i]];
		}

        P1= 1-abs((total_time-sum_duration)/total_time);
		P2= 1-abs((total_r-sum_rate)/total_r);
        fitness[c]=((P1 + P2)/2);
        cout<<  "\tP1 = " <<P1 << "\tP2 = " << P2 << endl;
	}
	cout << endl;

    int k;
  	for (int c=0; c<HALLS; c++) {
     	for (k =0; k<MOVIES; k++){
         	for (int l=k; l<MOVIES; l++){
            if (l!=k){
                    if(chromosome[c][k]==chromosome[c][l]){
                             fitness[c]=0;
                    }
            }
 		}
     }
 	}


}


void parentSelection(){
//declare necessary variables
int parent1,parent2;
int indexParents[2];

 do{
        for (int p=0; p<2; p++){//tournament selection
            parent1 = rand()  % HALLS;
            do {
                    parent2 = rand()  % HALLS;
            } while (parent1==parent2);

            if (fitness[parent1] <= fitness[parent2]){
                indexParents[p]=parent2;
            }
            else{
                indexParents[p]=parent1;
            }
            cout << "\t Players: " << parent1 << " vs " << parent2;
            cout << "\n\t Fitness: " << fitness[parent1] << " vs " << fitness[parent2];
            cout << "\n\t Winner: " << indexParents[p];
        }//end of tournament
    } while(indexParents[0]==indexParents[1]);

    for (int p=0;p<2;p++){
            cout << "\n\t Parents " << p+1 <<" : ";
        for (int g=0; g<MOVIES; g++){
            parents[p][g]=chromosome[indexParents[p]][g];
            cout << parents[p][g] << " ";
        }
    }
    cout << endl;
}

void crossover(){

int crossover_point=0;
//1. Copy both parent’s chromosome to children chromosomes
//2. Generate a random number from 0-1. Make sure it is real value data type
double e=((rand() % 10)+1)/10.0;
//3. If (2) less than crossover probability
if(e<CROSSOVER_PROBABILITY){
    //2.1 generate a random crossover point
     crossover_point= rand() %MOVIES;
     cout <<"Crossover position:   "<<crossover_point+1<< endl;

         for(int i=0; i<MOVIES;i++){
                 if(i <crossover_point){
                     children[0][i]=parents[0][i];
                     children[1][i]=parents[1][i];
                 }else{
                     children[0][i]=parents[1][i];
                     children[1][i]=parents[0][i];
                 }
         }
}
//2.2 Crossover parent 1 and parent 2 to produce the children
//4. Print children 1 & 2
	for (int c=0; c<2; c++){
    		cout << "\tC" << c << "\t";
    		for (int i=0; i<MOVIES; i++){
    			cout << children[c][i] << " ";
    		}
    		cout << endl;
    	}
}

void mutation(){
 float prob= 0;
 int mut_point=0;
  prob=(rand() % 11)/10.0;
  if(prob<MUTATION){
      mut_point=rand()%MOVIES;
      children[0][mut_point]=rand()%30;
      children[1][mut_point]=rand()%30;
  }
  cout <<"Mutation position:   "<<mut_point+1<< endl;
  	for (int c=0; c<2; c++){
    		cout << "\tC" << c << "\t";
    		for (int i=0; i<MOVIES; i++){
    			cout << children[c][i] << " ";
    		}
    		cout << endl;
    	}

}

void survivalSelection() {
	for (int c = 0; c < 2; c++) {//copy children to newChromosome
		for (int g = 0; g < MOVIES; g++) {
			newChromosome[newChromoCounter][g] = children[c][g];
		}
		newChromoCounter++;
	}
	//print the new chromosome

	for (int c = 0; c < newChromoCounter; c++) {
		cout << "\t New chromosome " << c << "  : ";
		for (int g = 0; g < MOVIES; g++) {
			cout << newChromosome[c][g] << " ";
		}
		cout << endl;
	}
	cout << "\n-----------------------------------------------------------------------------" << endl;
}

void copyChromosome() {
        int temp[1][MOVIES];
        double tempf;
        int cou = HALLS*2;
		int combinechromosome[cou][MOVIES];
        double Gfitness[HALLS];

		for(int asd=0;asd<HALLS;asd++){
            for(int i= 0 ; i<MOVIES; i++){
                     combinechromosome[asd][i]=chromosome[asd][i];
                 }
		}
 		for(int hu=HALLS;hu<cou;hu++){
                 for(int i= 0 ; i<MOVIES; i++){
                     combinechromosome[hu][i]=newChromosome[hu-HALLS][i];
                 }
 		}
 		cout<<"But combined unsorted"<<endl;
        for (int c=0; c<cou; c++) {
		cout << "\tH" << c << "\t";
		for (int i=0; i<MOVIES; i++) {
			cout << combinechromosome[c][i] << " ";
		}
		cout<<endl;
    }
    cout<<endl;

    double sum_duration =0;
    double sum_rate = 0;
	double P1 = 0;
	double P2 = 0;

	for (int c=0; c<HALLS; c++) {
        sum_duration =0;
        sum_rate = 0;
		P1 = 0;
		P2 = 0;
		for (int i=0; i<MOVIES; i++) {
                sum_duration=sum_duration+DURATION[newChromosome[c][i]];
                sum_rate=sum_rate+RATES[newChromosome[c][i]];
		}
        P1= 1-abs((total_time-sum_duration)/total_time);
        P2= 1-abs((total_r-sum_rate)/total_r);
        xfitness[c+HALLS]=((P1 + P2)/2);
        cout<<  "\tP1 = " <<P1 << "\tP2 = " << P2;
        cout<<endl;
	}
	cout<<endl;

    int k;
    for (int c=0; c<cou; c++) {
     for (k =0; k<MOVIES; k++){
         for (int l=k; l<MOVIES; l++){
            if (l!=k){
                    if(chromosome[c][k]==chromosome[c][l]){
                             xfitness[c]=0;
                    }
            }
 		}
     }
}

 	for(int h=0;h<HALLS;h++){
            xfitness[h]=fitness[h];
    }


 		for(int f=0;f<HALLS;f++){
        cout << "\txFitness [" << f << "] = " << xfitness[f];
        cout<<endl;
 		}
 		for(int kk=HALLS;kk<cou;kk++){

            cout<< "\txFitness [" << kk << "] = " <<xfitness[kk];
            cout<<endl;
    }
                    for(int i=0;i<cou;i++)
                    {

                        for(int j=i+1;j<cou;j++)
                        {
                            for(int ds= 0 ; ds<MOVIES; ds++){
								temp[0][ds]=0;
                            }
                            tempf=0;
                            if(xfitness[i]<xfitness[j])
                            {

                                tempf =xfitness[i];
                                xfitness[i]=xfitness[j];
                                xfitness[j]=tempf;

                                for(int zz= 0 ; zz<MOVIES; zz++){
                                temp[0][zz]=combinechromosome[i][zz];
                                }
                                for(int is= 0 ; is<MOVIES; is++){
                                combinechromosome[i][is]=combinechromosome[j][is];
                                }
                                for(int id= 0 ; id<MOVIES; id++){
                                combinechromosome [j][id]=temp[0][id];
                                }
                            }
                        }
                    }
                    cout << "\n-----------------------------------------------------------------------------" << endl;
                    cout<<endl;

    for (int c=0; c<cou; c++) {
		cout << "\tH" << c << "\t";
		for (int i=0; i<MOVIES; i++) {
			cout << combinechromosome[c][i] << " ";
		}
		cout<<endl;
    }
    cout<<endl;

    for(int f=0;f<HALLS;f++){
        cout << "\txFitness [" << f << "] = " << xfitness[f];
        cout<<endl;
 		}
 		for(int kk=HALLS;kk<cou;kk++){

            cout<< "\txFitness [" << kk << "] = " <<xfitness[kk];
        cout<<endl;
    }

	for (int c = 0; c < HALLS; c++) {//copy newChromosome to chromosome
		for (int g = 0; g < MOVIES; g++) {
			chromosome[c][g] = combinechromosome[c][g];
		}
	}
}

void recordBestFitness() {
	for (int c = 0; c < HALLS; c++) {
		if (bestFitness < fitness[c]) {
 			bestFitness = fitness[c];
 			for (int g = 0; g < MOVIES; g++) {
 				bestChromosome[g] = chromosome[c][g];
 			} // close for gene
 		} // close if
 	} // close c

// 	// output to monitor
	cout << "\n Best Fitness = " << bestFitness;
 	cout << "\n Best Chromosome = ";
 	for (int g = 0; g < MOVIES; g++) {
 		cout << bestChromosome[g] << " ";
 	} // close gene
 	 	cout << endl;
// 	// output to files
 	bestFitnessFile << bestFitness << endl;
 	for (int g = 0; g < MOVIES; g++) {
 		bestChromosomeFile << bestChromosome[g] << " ";
 	} // close gene
 	bestChromosomeFile << endl;
 }

void calcAvgFitness() {
	double sum = 0.0;
 	for (int c = 0; c < HALLS; c++) {
 		sum += fitness[c];
 	}
 	avgFitness = sum / HALLS;
 	cout << "\n Average Fitness = " << avgFitness;
 	cout << "\n-----------------------------------------------------------------------------" << endl;
 	avgFitnessFile << avgFitness << endl;
 }


int main () {
	bestFitnessFile.open("bestFitness.txt");
	avgFitnessFile.open("avgFitness.txt");
	bestChromosomeFile.open("bestChromosome.txt");
	cout << "\nGA START! \n";
	cout << "\nINITIALIZATION... \n";
	//LAB 3



	initializeulation();
	for (int g = 0; g < MAX_GENERATION; g++) {
		cout << "GENERATION " << g+1<< " \n\n";
		//LAB 3
		cout << "\nPRINT INITIAL POPULATION \n";
		printChromosome();
		//LAB 4
		cout << "\nEVALUATE CHROMOSOME \n";
		evaluateChromosome();

	for (int c=0; c<HALLS; c++) {
		cout << "\tFitness of chromosome ["<<c<<"] = " << fitness[c]<< "\t";
		cout << endl;
        }
		recordBestFitness();
		calcAvgFitness();
		newChromoCounter = 0;
		for (int kk = 0; kk < HALLS / 2; kk++) {
			// LAB 5A
			cout << "\nPARENT SELECTION \n";
			parentSelection();
			// LAB 5B
			cout << "\nCROSSOVER \n";
			crossover();
			// LAB 6A
			cout << "\nMUTATION \n";
			mutation();
					// LAB 6B
			cout << "\nSURVIVAL SELECTION \n";
			survivalSelection();
		}
		cout << "\nNEW CHROMOSOMES COPIED TO CHROMOSOME\n";
		copyChromosome();
	}

    bestFitnessFile.close();
    avgFitnessFile.close();
	bestChromosomeFile.close();
}
