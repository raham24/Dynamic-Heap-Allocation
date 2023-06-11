/* Experimenting with Dynamic (heap) Memory Allocation

Heap-Allocated, Non-Contiguous 2D Arrays.

We've look at how to create 2D arrays that are contiguous in memory: they're
just 1D arrays with indices deconstructed into two parts: A[i][k] is really
just *(A+i*columns+k)==A[i][k].  The disadvantage of using these kinds of arrays is 
that if we passed such an array to a function, we would have to do the
coordinate calculation ourselves. 

There's another way to have heap-allocated 2D arrays that does not suffer 
from this "problem", but at the expense of not having the array contiguous
in memory.  You can create a 2D array of 20x30 integers this way:

  int** A = new int*[20];
  for(int i=0;i<20;i++) A[i] = new int[30];

As you should be able to surmise from this code, what's really created
is first a 1D array of 20 pointers (new int*[20]).  Then, with the
for-loop, each pointer is made to point to an array of 30 integers.
C++ allows you to use the notation A[4][8] to access the 5th row,
9th column with this kind of array, but now it's most definitely not
the same as *(A+4*30+8). Rather, it's now equivalent to *(*(A+4)+8):
the inner * dereferences the pointer A+4 that's pointing to the 5th
row, which is an array of 30 integers.  Then the outer * dereferences
that pointer at offset 8 to access the 9th value of the 5th row.
However, you won't have to calculate the memory address yourself.
You can write a function such as the `print2Dh` function below where you
can use A[4][8] directly.  

1.                      STOP THE LEAK!!!

The program belows compiles without errors, but there are MASSIVE MEMORY
LEAKS even though there's a `delete M` at the end of main.  To see that
there's a leak, open the task manager on the linux VM from the 
applications->systems menu.  There's a similar program, also called
task manager on Windows, and an "activity monitor" program on MacOS.
Look at memory usage while running the program and be ready to hit 
CONTROL-C to stop the program before it uses up all available memory.

Your task is to eliminate all memory leaks: memory usage should stay flat
while your program is running.  YOU MAY NOT DELETE ANY CODE FROM THE
EXISTING PROGRAM: YOU MAY ONLY ADD CODE TO STOP THE LEAKING.  

In particular, DO NOT MAKE ANY CHANGES TO THE does_it_leak() FUNCTION:
that's not where the problem is.

Your code must not introduce other memory errors into the program, such
as deleting something twice.

Use the task manager or similar program to confirm that all leaks have stopped.

And don't just "get it to work." You must be able to explain how and why
what you did was enough to eliminate all leaks.  Explain in comments.

2. Consolidate what you've learned by writing a function that can 
   deallocate an entire non-contiguous 2D array.
*/

#include<iostream>
using namespace std;

double** make2D(int rows, int columns) {
  double **A = new double*[rows]; // creates array of pointers
  for(int i=0;i<rows;i++) A[i] = new double[columns]; // .. to arrays of double
  return A;
}

void print2Dh(double** A, int rows, int columns)
{
  for(int i=0;i<rows;i++) {
    for(int k=0;k<columns;k++) {
        cout << A[i][k] << " ";
    }//for k
    cout << endl;
  }//for i
}//f

double** does_it_leak() // do NOT change this function
{
  double** M = make2D(10,15);  // note <int> because of template
  for(int i=0;i<10;i++)
    for(int k=0;k<15;k++) M[i][k] = i*k*.1;
  // print2Dh(M,10,15);
  return M;
}//does_it_leak()


int main()
{
  double** M;
  while (1) { M=does_it_leak(); 
    for (int i=0;i<10;i++) {delete M[i]; // after we have used the memory, we delete the contents of array not in use
    }
    delete M; // then we delete the array that contains the list of pointers for the arrays we deleted before
    // infinite loop so it will keep going and create a new 2D array which M points to
  }
  delete M;
  return 0;
}//main
