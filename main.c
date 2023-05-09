// Liabaries used 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "leak_detector_c.h"

// struct cordinate to determine the position of x and y.
typedef struct cordinates{
  
  int x;
  int y;

}cordinates;

// global variable 
cordinates myCord;

// Function Declaration 
void sort(cordinates*arr, int len, int t);
void merge(cordinates *arr, int left, int mid, int right);
void mergeSort(cordinates *arr, int left, int right, int t);
void insertionSort(cordinates *arr,int left, int right);
int binSearch(cordinates arr[], cordinates q, int len);// used when answering quires 
int compareTo(cordinates* ptrPt1, cordinates* ptrPt2); // compares cordinates 
cordinates readData(cordinates cord); // does not read the search queries 

// array of structs x and y cordinates 
// sort by distance -> if has monster or not -> monster ranking 

int main(void) {
  atexit(report_mem_leak); // checks for memory leak 
  
  FILE *file = fopen("out.txt","w");// opens files  

  // initializes variable and makes them equal to what they scan. 
  int numMonsters, searchPoints,t; 
  scanf("%d %d %d %d %d", &numMonsters, &searchPoints, &myCord.x, &myCord.y, &t);
  
  cordinates* monArr = (cordinates*)malloc(sizeof(cordinates)*numMonsters); // Dynamically allocates memory cordinates of monsters.

  // copies data for monster array
  for(int i = 0; i < numMonsters; i++){
    monArr[i] = readData(monArr[i]); 
  }

  sort(monArr, t, numMonsters); // sorts the given cordinates 

  // prints array from sorted 
  for(int i = 0; i < numMonsters; i++){
    printf("%d %d\n", monArr[i].x, monArr[i].y);
    fprintf(file,"%d %d\n", monArr[i].x, monArr[i].y);
  }
  
  cordinates q;// a cordinate for queries and seatrch point 
  // reads data and runs the appropiate functuin and determines if the monster is found or not
  for(int i = 0; i < searchPoints; i++){
    q = readData(q); // reads data
    // depending on the result of the binary search it will display of the mosnter is found at the given cordinate or  not.
    if(binSearch(monArr,q,numMonsters) == -1){ 
      printf("%d %d not found\n", q.x, q.y);
      fprintf(file,"%d %d not found\n", q.x, q.y);
    }
    else{
      int position =(binSearch(monArr,q,numMonsters))+1;
      printf("%d %d found at position %d\n", q.x, q.y, position);
      fprintf(file,"%d %d found at position %d\n", q.x, q.y, position);
    }
  }
  free(monArr); // free monster array 
  fclose(file); // closes the file 
  return 0;
}

//Your sort function should take in the array to be  sorted, the length of the  array as well as the threshold value, t,
// the 1st step to decides which type of sort to use based off of the conditions.
void sort(cordinates arr[], int t, int len){
  // if the length of the array is less than 
  if(len <= t){
    insertionSort(arr, 0, len-1);
  }
  else{
    mergeSort(arr, 0, len-1, t);
  }
}

// called depending on the threshold 
void insertionSort(cordinates *arr,int left, int right){
  // initialize variables 
  int j;
  cordinates hand; // placeholder variable
  
  // sorts accordingly using insertion sort by comparing the left and right side of the array. 
  for(int i = 1; i <=right; i++){
    
    // redetermines variables 
    hand = arr[i];
    j = i - 1;

    while(j>=left && compareTo(&hand,&arr[j]) == -1){
      arr[j+1] = arr[j];
      j--;
    }
    arr[j+1] = hand;
  }
}



// organizes the sort 
void merge(cordinates *arr, int left, int mid, int right){

  // initializes variables based of off the parameters
  int leftSize = mid - left +1;
  int rightSize = right - mid;

  // Dynamically Allocates Memory for Leftside and Right side of the Array
  cordinates *L = (cordinates*)malloc(leftSize*sizeof(cordinates));
  cordinates *R = (cordinates*)malloc(rightSize*sizeof(cordinates));

  // Fills the left side Array 
  for(int i = 0; i < leftSize;i++){
    L[i]=arr[left+i];
  } 
  // Fills the rightside Array 
  for(int j = 0; j < rightSize;j++){  
    R[j]=arr[mid+1+j];
  }

  // initializes the variables 
  int i = 0, j = 0, k = left;
  // compares the array and merges and sorts the values accordingly into the main array (arr). 
  while(i<leftSize && j < rightSize){
    
    if(compareTo(&L[i], &R[j]) < 1){
      arr[k++]=L[i++];
    }
    else{
      arr[k++] = R[j++]; 
    } 
  }
  // fills remaining values for left side 
  while(i<leftSize){
    arr[k++]=L[i++];
  }
  // fills remained values for right side 
  while(j<rightSize){
    arr[k++]=R[j++];
  }

  free(L); // free the left side Array 
  free(R); // frees the right side Array 
  
}

//
void mergeSort(cordinates *arr, int left, int right, int t){
  
  //If the subsection of the array to sort has t or fewer values to sort, Insertion Sort should be used 

  int threshold = right - left + 1;
  // if the thershold is met sertion sort will take place, if not go past it. 
  if(threshold <=t){
    insertionSort(arr,left,right);
    }
  else{
    // recursively calls merge sort till it calls merge 
    int mid = (left+right)/2; // redetermines the value for merge 
    mergeSort(arr, left, mid,t);
    mergeSort(arr, mid+1, right,t);
    merge(arr, left, mid, right);
    
    }
}
// The recursive sort function (you can call this mergeSort) should take in the array, a starting index into the 
// array, an ending index into the array and the threshold value t. In this function, either recursive calls should 
// be made OR a call to an insertion sort function should be made. 

// reads data 
cordinates readData(cordinates cord){
  
  scanf("%d %d", &cord.x, &cord.y); // reads and stores the cordinate 

  return cord; // returns the cordinate 
}


// compares distances 
int compareTo(cordinates* ptrPt1, cordinates* ptrPt2){
   
  // compares the distance of of points from the given cordinate 
  double distanceP1 = sqrt((pow(ptrPt1->x - myCord.x,2)) + (pow(ptrPt1->y - myCord.y, 2))); // try to use pow function 
  double distanceP2 = sqrt((pow(ptrPt2->x - myCord.x,2)) + (pow(ptrPt2->y - myCord.y, 2))); // try to use pow function 

  //determines which cordinate is further from the other, if distance is not enough than x or by y cordinates are determined as layers. 
  if(distanceP1 < distanceP2){
    return -1;
  }
  else if(distanceP1 > distanceP2){
    return 1;
  }
  else{ 
    if(ptrPt1->x < ptrPt2->x){
      return -1;
    }
    else if(ptrPt1->x > ptrPt2->x){
      return 1;
    }
    else{
      if(ptrPt1->y < ptrPt2->y){
        return -1;
      }
      else if(ptrPt1->y > ptrPt2->y){
        return 1;
      }
      else{
        return 0;
      }
    }
  }
}

// conducts a binary search 
int binSearch(cordinates arr[], cordinates q, int len){
  // initializes variables   
  int left = 0;
  int right = len -1;
  
  while(left<=right){
    
    int mid = (left +right)/2; // redetermines what mid is trough out the loop 

    if(compareTo(&q, &arr[mid]) == 0) return mid; // if the mid is found 
    if(compareTo(&q, &arr[mid]) == 1){ // if the value is on the right side 
      left = mid +1;
    }
    else{
      right = mid -1; // redetermies value if the value is on the left side. 
    }
  }
  return -1;
}

