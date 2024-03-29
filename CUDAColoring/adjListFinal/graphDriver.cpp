// Graph coloring 

#include <ctime> 
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>  
#include <iostream> 
#include <math.h>  
#include <set> 
#include "graphColoring.h" 
#include <assert.h>

#include <fstream>
#include <string>
using namespace std;  

//----------------------- Read Sparse Graph from Matrix Market format --------------//
// Author: Shusen
void inline swap(int &a, int &b) {int temp = a; a=b; b=temp; }
int inline findmax(int *a, int n){
	int max=0;
	for(int i=0; i<n; i++)
		if(a[i]>max)
			max = a[i];
	return max;	}
int getAdjacentListFromSparseMartix_mtx(int *&adjacencyList, long &graphsize, const char* filename) //return the maxDegree
{
	int maxDegree=0;
	long row=0, col=0, entries=0;

	ifstream mtxf;
	mtxf.open(filename);
	cout << string(filename) << endl;

	mtxf >> row >> col >> entries ;
	cout<< row <<" " << col <<" " << entries << endl;
	graphsize = col>row? col:row;

	//calculate maxDegree in the following loop
	int donotcare = 0;
	int nodecol = 0;
	int noderow = 0;

	int *graphsizeArray = new int[graphsize];
	memset(graphsizeArray, 0 , sizeof(int)*graphsize);
	int j=0;
	for(int i=0; i<entries; i++)
	{
		j++;
		mtxf >> noderow >> nodecol >> donotcare;
		//cout << noderow << " " << nodecol << " " << donotcare << endl;
		//assert(noderow!=nodecol);
		if(noderow == nodecol)
			continue;
		graphsizeArray[noderow-1]++;
		graphsizeArray[nodecol-1]++;
	}
	maxDegree = findmax(graphsizeArray,graphsize);



	cout << j <<" maxDegree: "<< maxDegree << endl;
	mtxf.close();

	//create the adjacency list matrix
	
	adjacencyList = new int[maxDegree*graphsize];
	int *adjacencyListLength = new int[graphsize];
	memset(adjacencyList, -1, sizeof(int)*maxDegree*graphsize);
	memset(adjacencyListLength, 0, sizeof(int)*graphsize); //indicate how many element has been stored in the list of each node
	//for(int i=0; i<maxDegree; i++)
	//	cout<<adjacencyList[i]<<endl;
	


	//update the adjacency list matrix from the file
	mtxf.open(filename);
	int nodeindex=0, connection=0;
	
	mtxf >> donotcare >> donotcare >> donotcare;
	//cout << donotcare << endl;

	for(int i=0; i<entries; i++)
	{
		mtxf >> connection >> nodeindex >> donotcare;
		if(noderow == nodecol)
			continue;
		//because node in mtx file begin with 1 but in the program begin with 0
		adjacencyList[(nodeindex-1)*maxDegree + adjacencyListLength[nodeindex-1] ]=connection-1; 
		adjacencyListLength[nodeindex-1]++;

		swap(connection, nodeindex);
		adjacencyList[(nodeindex-1)*maxDegree + adjacencyListLength[nodeindex-1] ]=connection-1; 
		adjacencyListLength[nodeindex-1]++;		
	}

	delete [] adjacencyListLength;

	return maxDegree;

	for(int i=0; i<maxDegree; i++)
	{
		cout << " " << adjacencyList[(graphsize-1)*maxDegree+i];
	}
}

//----------------------- Graph initializations -----------------------//
// generate a graph in adjacency list representation


// Author: Pascal 
// genetates a graph 
void generateMatrix(int *matrix, int size, int num){  
	int x, y;  
	int count = 0;  
	
	while (count < num){  
		x = rand()%GRAPHSIZE;  
		y = rand()%GRAPHSIZE;  
		
		if (matrix[x*size + y] != 1)            // if not already assigned an edge 
			if (x != y){  
				matrix[x*size + y] = 1;       // non directional graph  
				matrix[y*size + x] = 1;  
				count++;  
			}  
	}  
}  




// Author:Peihong
// node index sta58800rt from 1
// gets an adjacency list from an adjacencyMatrix
void getAdjacentList(int *adjacencyMatrix, int *adjacentList, int size, int maxDegree)
{
	for (int i=0; i<size; i++){ 
		int nbCount = 0;
		for (int j=0; j<size; j++){
			if ( adjacencyMatrix[i*size + j] == 1)  
			{
				adjacentList[i*maxDegree + nbCount] = j;
				nbCount++;
			}
		}
	}

	/*
	// Adj list display
	for (int i=0; i<10; i++){
		for (int j=0; j<maxDegree; j++){
			cout << adjacentList[i*maxDegree + j] << " ";
		}
		cout << endl;
	}
	*/
}




// Author: Pascal 
// get the degree information for a graph 
int getMaxDegree(int *adjacencyMatrix, int size){  
	int maxDegree = 0;   
	int degree;  
	
	for (int i=0; i<size; i++){  
		degree = 0;  
		
		for (int j=0; j<size; j++)           
			if (    adjacencyMatrix[i*size + j] == 1)  
				degree++;  
		
		if (degree > maxDegree)  
			maxDegree = degree;  
	}  
	
	return maxDegree;  
}  




// Author: Pascal
// get the degree of each element in the graph and returns the maximum degree
void getDegreeList(int *adjacencyList, int *degreeList, int sizeGraph, int maxDegree){
    for (int i=0; i<sizeGraph; i++){
        int count = 0;
        
        for (int j=0; j<maxDegree; j++){
            if (adjacencyList[i*maxDegree + j] != -1)
                count++;
            else
                break;  
        }

        degreeList[i] = count;
    }
}




int inline min(int n1, int n2) 
{ 
	if (n1>=n2) 
		return n2; 
	else 
		return n1; 
} 

// Author: Peihong
int getBoundaryList(int *adjacencyMatrix, int *boundaryList, int size, int &boundaryCount){  
	int maxDegree = 0;   
	int degree;  
	
	set<int> boundarySet; 
	boundarySet.clear(); 


	
	for (int i=0; i<size; i++){  
		degree = 0;  

		int subIdx = i/(float)SUBSIZE;
		int start = subIdx * SUBSIZE;
		int end = min( (subIdx + 1)*SUBSIZE, size );

		for (int j=0; j<size; j++){           
			if ( adjacencyMatrix[i*size + j] == 1)  
				degree++;  
			if ( adjacencyMatrix[i*size + j] == 1 && (j < start || j >= end))
			{
				boundarySet.insert(i);
			}
			
		}
		
		if (degree > maxDegree)  
			maxDegree = degree;  	
	} 
	
	boundaryCount = boundarySet.size();

	set<int>::iterator it = boundarySet.begin(); 
	for (int i=0; it != boundarySet.end(); it++)  
	{ 
		boundaryList[i] = *it;
		i++; 
	}  
	
	return maxDegree;  
}  


// Author: Peihong & Shusen
// getBoundaryList from adjacency list representation
void getBoundaryList_adjList(int *adjacencyList, int *boundaryList, long size, int maxDegree, int &boundaryCount){  
 
	
	set<int> boundarySet; 
	boundarySet.clear(); 

assert(adjacencyList);
	
	for (int i=0; i<size; i++){  


		int subIdx = i/(float)SUBSIZE;
		int start = subIdx * SUBSIZE;
		int end = min( (subIdx + 1)*SUBSIZE, size );

		for (int j=0; j<maxDegree; j++){   
			assert(i*maxDegree+j <= (size-1)*maxDegree+maxDegree-1);        

			if ( adjacencyList[i*maxDegree + j] < start || adjacencyList[i*maxDegree + j] >= end)
			{
				boundarySet.insert(i);
			}
			
		}

	} 

	
	boundaryCount = boundarySet.size();
	boundaryList = new int[boundaryCount];


 
	set<int>::iterator it = boundarySet.begin(); 
	for (int i=0; it != boundarySet.end(); it++)  
	{ 
		boundaryList[i] = *it;
		i++; 
	}  
//cout << "Debug:" <<endl;  
}  



//----------------------- Fast Fit Graph Coloring -----------------------//

// Author: Pascal & Shusen
// GraphColor Adjacency list
int colorGraph_FF(int *list, int *colors, int size, int maxDegree){  
	int numColors = 0;  
	int i, j;  
	
	int * degreeArray;  
	degreeArray = new int[maxDegree+1];  
	
	
	for (i=0; i<size; i++)  
	{                 
		// initialize degree array  
		for (j=0; j<=maxDegree; j++)  
			degreeArray[j] = j+1;  
		
		
		// check the colors  
		for (j=0; j<maxDegree; j++){  
			if (i == j)  
				continue;  
			
			// check connected  
			if (    list[i*maxDegree + j] != -1)  
				if (colors[list[i*maxDegree + j]] != 0)  
					degreeArray[colors[list[i*maxDegree + j]]-1] = 0;   // set connected spots to 0  
		}  
		
		for (j=0; j<=maxDegree; j++)  
			if (degreeArray[j] != 0){  
				colors[i] = degreeArray[j];  
				break;  
			}  
		
		if (colors[i] > numColors)  
			numColors=colors[i];  
	}  

	delete[] degreeArray; 
	
	return numColors;  
}  




//----------------------- SDO Improved Graph Coloring -----------------------//

// Author: Pascal
// returns the degree of that node
int degree(int vertex, int *degreeList){
        return degreeList[vertex];
}



// Author: Pascal
// return the saturation of that node
int saturation(int vertex, int *adjacencyList, int *graphColors, int maxDegree){
    int saturation = 0;
    int *colors = new int[maxDegree+1];

    memset(colors, 0, (maxDegree+1)*sizeof(int));           // initialize array


    for (int i=0; i<maxDegree; i++){
        if (adjacencyList[vertex*maxDegree + i] != -1)
            colors[ graphColors[vertex] ] = 1;                      // at each colored set the array to 1
        else
            break;
    }


    for (int i=1; i<maxDegree+1; i++)                                       // count the number of 1's but skip uncolored
        if (colors[i] == 1)
            saturation++;

	delete[] colors; 	

    return saturation;
}




// Author: Pascal
// colors the vertex with the min possible color
int color(int vertex, int *adjacencyList, int *graphColors, int maxDegree, int numColored){
    int *colors = new int[maxDegree + 1];
    memset(colors, 0, (maxDegree+1)*sizeof(int));   
    
    if (graphColors[vertex] == 0)
            numColored++;
    
    for (int i=0; i<maxDegree; i++)                                         // set the index of the color to 1
        if (adjacencyList[vertex*maxDegree + i] != -1)
            colors[  graphColors[  adjacencyList[vertex*maxDegree + i]  ]  ] = 1;
        else {
            break;
        }

    

    for (int i=1; i<maxDegree+1; i++)                                       // nodes still equal to 0 are unassigned
        if (colors[i] != 1){
            graphColors[vertex] = i;
            break;
        }
    
	delete[] colors; 

    return numColored;
}




// Author: Pascal
int sdoIm(int *adjacencyList, int *graphColors, int *degreeList, int sizeGraph, int maxDegree){
    int satDegree, numColored, max, index;
    numColored = 0;
    int iterations = 0;
    

    while (numColored < sizeGraph){
        max = -1;
        
        for (int i=0; i<sizeGraph; i++){
            if (graphColors[i] == 0)                        // not colored
            {
                satDegree = saturation(i,adjacencyList,graphColors, maxDegree);

                if (satDegree > max){
                    max = satDegree;
                    index = i;                              
                }

                if (satDegree == max){
                    if (degree(i,degreeList) > degree(index,degreeList))
                        index = i;
                }
            }

            numColored = color(index,adjacencyList,graphColors, maxDegree, numColored);
            iterations++;
        }
    }
    
    return iterations;
}






//----------------------- Conflict Solve -----------------------//

void conflictSolveSDO(int *adjacencyList, int *conflict, int conflictSize, int *graphColors, int *degreeList, int sizeGraph, int maxDegree){
    int satDegree, numColored, max, index;
    numColored = 0;
   
	// Set their color to 0
	for (int i=0; i<conflictSize; i++)
		graphColors[conflict[i]-1] = 0;
    

    while (numColored < conflictSize){
        max = -1;
        
        for (int i=0; i<conflictSize; i++){
			int vertex = conflict[i]-1;
            if (graphColors[vertex] == 0)                        // not colored
            {
                satDegree = saturation(vertex, adjacencyList, graphColors, maxDegree);

                if (satDegree > max){
                    max = satDegree;
                    index = vertex;                              
                }

                if (satDegree == max){
                    if (degree(vertex,degreeList) > degree(index,degreeList))
                        index = vertex;
                }
            }

            numColored = color(index,adjacencyList,graphColors, maxDegree, numColored);
        }
    }
}



// Author: Pascal & Shusen
// Solves conflicts using Fast Fit
void conflictSolveFF(int *Adjlist, int size, int *conflict, int conflictSize, int *graphColors, int maxDegree){
	int i, j, vertex, *colorList, *setColors;
	colorList = new int[maxDegree];
	setColors = new int[maxDegree];


	// assign colors up to maxDegree in setColors
	for (i=0; i<maxDegree; i++){
	        setColors[i] = i+1;
	}


	for (i=0; i<conflictSize; i++){
        memcpy(colorList, setColors, maxDegree*sizeof(int));                    // set the colors in colorList to be same as setColors

        vertex = conflict[i]-1;
        

        for (j=0; j<maxDegree; j++){                                            // cycle through the graph
        		if ( Adjlist[vertex*maxDegree + j] != -1 )                      		//      check if node is connected
				colorList[ graphColors[Adjlist[vertex*maxDegree + j]]-1 ] = 0;
			else 
                break;    
        }


        for (j=0; j<maxDegree; j++){                                       	// check the colorList array
        		if (colorList[j] != 0){                                         //    at the first spot where we have a color not assigned
            		graphColors[vertex] = colorList[j];                         //       we assign that color to the node and
                break;                                                      //   	 exit to the next
            }
        }

	}
}



//----------------------- Checking for error -----------------------//

// Author: Pascal 
// Checks if a graph has conflicts or not 
void checkCorrectColoring(int *adjacencyMatrix, int *graphColors){ 
	int numErrors = 0; 
	
	cout << endl << "==================" << endl << "Error checking for Graph" << endl; 
	
	for (int i=0; i<GRAPHSIZE; i++)                 // we check each row 
	{ 
		int nodeColor = graphColors[i]; 
		int numErrorsOnRow = 0; 
		
		for (int j=0; j<GRAPHSIZE;j++){ // check each column in the matrix 
			
			// skip itself 
			if (i == j) 
				continue; 
			
			if (adjacencyMatrix[i*GRAPHSIZE + j] == 1)      // there is a connection to that node 
				if (graphColors[j] == nodeColor) 
				{ 
					cout << "Color collision from: " << i << " @ " << nodeColor << "  to: " << j << " @ " << graphColors[j] << endl; 
					numErrors++; 
					numErrorsOnRow++; 
				} 
		} 
		
		if (numErrorsOnRow != 0) 
			cout << "Errors for node " << i << " : " << numErrorsOnRow << endl; 
	} 
	
	cout << "Color errors for graph : " << numErrors << endl << "==================== " << endl ;    
} 




//----------------------- The meat -----------------------//

int main(){  
	int maxDegree, numColorsSeq, numColorsParallel, boundaryCount, conflictCount;

	int *graphColors = new int[graphsize*sizeof(int)];          
	int *boundaryList = new int[graphsize*sizeof(int)]; 

	memset(graphColors, 0, graphsize*sizeof(int)); 
	memset(boundaryList, 0, graphsize*sizeof(int)); 
	
	conflictCount = boundaryCount = numColorsSeq = numColorsParallel = 0; 
	
	
	long randSeed = time(NULL);
	srand ( randSeed );  // initialize random numbers  
	//srand ( 1271876520 );  // initialize random numbers   
	
	
	cudaEvent_t start, stop, stop_1, stop_4;         
	float elapsedTimeCPU, elapsedTimeGPU, elapsedTimeGPU_1, elapsedTimeGPU_4; 
	
	
	
//--------------------- Graph Creation ---------------------!
	// initialize graph  
/*	generateMatrix(adjacencyMatrix, GRAPHSIZE, NUMEDGES);  
	

	// Display graph: Adjacency Matrix
	
	cout << "Adjacency Matrix:" << endl; 
	for (int i=0; i<GRAPHSIZE; i++){  
		for (int j=0; j<GRAPHSIZE; j++)  
	 		cout << adjacencyMatrix[i*GRAPHSIZE + j] << "  ";  
	 	cout << endl;  
	}  
	     
	
	// determining the maximum degree  
	//maxDegree = getMaxDegree(adjacencyMatrix, GRAPHSIZE);  
	maxDegree = getBoundaryList(adjacencyMatrix, boundaryList, GRAPHSIZE, boundaryCount);	// return maxDegree + boundaryCount (as ref param)
	

*/


/////////////////////////// The following code use the data from sparse matrix file /////////////////////////////
	int *adjacentList = NULL;
	
	
	//reading the matrix market format sparse matrix
	maxDegree = getAdjacentListFromSparseMartix_mtx(adjacentList, graphsize, "ch7-8-b4.mtx"); //return the maxDegree
	assert(adjacentList);
	getBoundaryList_adjList(adjacentList, boundaryList, graphsize, maxDegree, boundaryCount);
	assert(boundaryList);
cout << boundaryCount << endl;
//exit(0);

	// Get degree List
	int *degreeList = new int[graphsize*sizeof(int)];
	memset(degreeList, 0, graphsize*sizeof(int)); 
	getDegreeList(adjacentList, degreeList, graphsize, maxDegree);



	
//--------------------- Sequential Graph Coloring ---------------------!
	cudaEventCreate(&start); 
	cudaEventCreate(&stop); 
	cudaEventRecord(start, 0);  
	
//cout << "Debug:" <<endl; 
	numColorsSeq = colorGraph_FF(adjacentList, graphColors, graphsize, maxDegree);  
	//sdoIm(adjacentList, graphColors, degreeList, GRAPHSIZE, maxDegree);
//cout << "Debug:" <<endl;  	

	cudaEventRecord(stop, 0); 
	cudaEventSynchronize(stop); 
	cudaEventElapsedTime(&elapsedTimeCPU, start, stop); 
	

// Get colors
	numColorsSeq = 0;
	for (int i=0; i<graphsize; i++){
		if ( numColorsSeq < graphColors[i] )
			numColorsSeq = graphColors[i];
	}



	
//--------------------- Checking for color conflict ---------------------!

	cout << "Sequential Conflict check:";
	//checkCorrectColoring(adjacencyMatrix, graphColors); 
	cout << endl;  
	
	
	
	
//--------------------- Parallel Graph Coloring ---------------------!	
	
	int *conflict = new int[boundaryCount*sizeof(int)];                    // conflict array 
	memset(conflict, 0, boundaryCount*sizeof(int));                        // conflict array initialized to 0  
	
	memset(graphColors, 0, graphsize*sizeof(int));                         // reset colors to 0 
	

//exit(0);

//--------------- Steps 1, 2 & 3: Parallel Partitioning + Graph coloring + Conflict Detection
	
	cudaEventCreate(&start); 
	cudaEventCreate(&stop); 
	cudaEventCreate(&stop_1); 
	cudaEventCreate(&stop_4); 
	cudaEventRecord(start, 0); 
	
		
	int *conflictTmp = new int[boundaryCount*sizeof(int)];
	memset(conflictTmp, 0, boundaryCount*sizeof(int));  
      

	//cudaGraphColoring(adjacentList, boundaryList, graphColors, conflictTmp, boundaryCount, maxDegree);
	cudaGraphColoring(adjacentList, boundaryList, graphColors, degreeList, conflictTmp, boundaryCount, maxDegree);
	
	
	
	cudaEventRecord(stop_1, 0); 
	cudaEventSynchronize(stop_1); 


	int interColorsParallel = 0;
	for (int i=0; i<graphsize; i++){
		if ( interColorsParallel < graphColors[i] )
			interColorsParallel = graphColors[i];
	}




//----- Conflict Count
	for(int i=0; i< boundaryCount; i++)
	{
		int node = conflictTmp[i];
		
		if(node >= 1)
		{
			conflict[conflictCount] = node;
			conflictCount++;
		}
	}
  	delete[] conflictTmp;

   
	cudaEventRecord(stop_4, 0); 
    	cudaEventSynchronize(stop_4); 
	



//--------------- Step 4: solve conflicts 
	cout <<"Checkpoint " << endl;
	conflictSolveFF(adjacentList,  graphsize, conflict, conflictCount, graphColors, maxDegree); 
	//conflictSolveSDO(adjacentList, conflict, conflictCount, graphColors,degreeList, graphsize, maxDegree);

	
	
	cudaEventRecord(stop, 0); 
	cudaEventSynchronize(stop); 

	cudaEventElapsedTime(&elapsedTimeGPU, start, stop); 
	cudaEventElapsedTime(&elapsedTimeGPU_1, start, stop_1); 
	cudaEventElapsedTime(&elapsedTimeGPU_4, start, stop_4); 


	numColorsParallel = 0;
	for (int i=0; i<graphsize; i++){
		if ( numColorsParallel < graphColors[i] )
			numColorsParallel = graphColors[i];
	}


	//conflicts
	/*
	cout << "Conclicts: ";
	for (int i=0; i<conflictCount; i++)
		cout << conflict[i] << " colored " <<  graphColors[conflict[i]] << "    ";
	cout << endl;
	*/


	
	// Display information 
	/*cout << "List of conflicting nodes:"<<endl; 
	 for (int k=0; k<conflictCount; k++)  
	 cout << conflict[k] << "  ";  
	 cout << endl << endl;  */
	


//--------------------- Checking for color conflict ---------------------!

	cout << endl <<  "Parallel Conflict check:";	
	//checkCorrectColoring(adjacencyMatrix, graphColors); 	





//--------------------- Parallel Graph Coloring ---------------------!	
	
	
	cout << endl << endl << "Graph Summary" << endl;
	cout << "Vertices: " << GRAPHSIZE << "   Edges: " << NUMEDGES << "   Density: " << (2*NUMEDGES)/((float)GRAPHSIZE*(GRAPHSIZE-1)) << "   Degree: " << maxDegree << endl;
	cout << "Random sed used: " << randSeed << endl;

	cout << endl;
	cout << "Grid Size: " << graphsize << "    Block Size: " << BLOCKSIZE << "     Total number of threads: " << GRIDSIZE*BLOCKSIZE << endl;
	cout << "Graph Subsize: " << SUBSIZE << endl;

	cout << endl;
	cout << "CPU time (Fast Fit): " << elapsedTimeCPU << " ms    -  GPU Time: " << elapsedTimeGPU << " ms" << endl; 
	cout << "ALGO step 1, 2 & 3: " 	<< elapsedTimeGPU_1 << " ms" << endl;  
	cout << "Boundary count: " 		<< elapsedTimeGPU_4 - elapsedTimeGPU_1 << " ms" << endl; 
	cout << "ALGO step 4: " 			<< elapsedTimeGPU - elapsedTimeGPU_4 << " ms" << endl; 

	cout << endl;
	cout << "Boundary Count: " << boundaryCount << endl;
	cout << "Conflict count: " << conflictCount << endl;

	cout << endl;
	cout << "Colors before solving conflict: " << interColorsParallel << endl;
	cout << "Sequential Colors: " << numColorsSeq << "      -       Parallel Colors: " << numColorsParallel << endl;     


	
	
//--------------------- Cleanup ---------------------!		

	//delete[] adjacencyMatrix; 
	//delete[] graphColors; 
	delete[] conflict; 
	//delete[] boundaryList;
	delete[] adjacentList;

	return 0;  
}  

