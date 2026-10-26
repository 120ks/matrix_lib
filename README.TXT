//
//CORE PRINCIPLE
//

Allow for dynamic operating, viewing, slicing and masking and all of these mixed and combined.
the goal is to allows pretty much all that make semantically some sense (not always but enough to be realistically possible)


//
//syntax
//

//create matrix

    Matrix<T> matrix{rows, cols, initial_value};        //new Matrix object //empty initial_value uses the T default
    Matrix<T> matrix{const matrix<T>&&};                //copies the other named matrix 
    Matrix<T> matrix{matrix<T>&&};                      //makes the r-value matrix into l-value matrix
    Matrix<T> matrix{const matrix_base& object<T>};     //creates a matrix from the derived matrix, Slice_view or mask_view
    Matrix<T> matrix{const matrix_base& object<R>};     //copies values of different type of matrix into matrix<T> values will be T
                                                        //use auto matrix... when the other_matrix/base type T is unknown or changes types.

//indexing

                        //indexing is done using operator() instead of the normal [] (works with c++ version under 20)
    matrix(row, col)    //returns the object inside the matrix at indexes row and col. (row and col are type size_t);
    matrix(row, col)    //returns reference when matrix is not const, and value when matrix is const

//slicing

                                    //indexing is done using operator()
    matrix(Slice rows, Slice cols)  //returns a view to matrix object that behaves like a normal matrix, but changes the underlying matrix object
                                    // the shorter version for slice is sl
                                    //the last number given to slice is exclusive
    matrix(sl{1, 3}, sl{3, 6})      //returns a Slice_view object that behaves like a normal matrix with 2 rows and three cols, 
                                        but under the hood changes made to the Slice_view are made to rows 1-2 and 3-5 in the matrix
    matrix(sl{30, 0}, sl{20, 0})    //making the last element 0 in a slice sets the end to the rows/cols of the sliced object, use this if you are not sure
                                        how many rows / cols there are


//masking
                                            //use [mask] operator to apply operation on the true elements of the mask
                                            //comparison operators return a Matrix<boolean> object
    Matrix<boolean> mask = {matrix < 2};    //mask object is a matrix that holds a custom boolean object
    Matrix<boolean> mask = matrix > 0       //returns a mask size matrix that has true or false
    matrix[mask]= 1;                        // makes all of the values that are true equal to one.
    matrix[matrix<0] = 0;                   //all values under 0 in the matrix will be under zero
    matrix[matrix(sl{1; 0}), sl{1, 0}]      //mask has to match with rows and cols the matrix or its slice //error


    
    
//Operators

                        //arithemtic operators +,',*,/
                        //comparison operators <, >, <=, >=, ==, !=
                        //logical (only work on Matrix<boolean>) !, &&, ||

matrix * matrix2        //returns a matrix where matrix(0,0) * matrix2(0,0) and so on.
matrix *= matrix2       //modifies left side "matrix" by multiplying its elements by corresponding ones on matrix2
matrix * 2              //returns a new matrix that has all elements of matrix multiplied by 2
matrix *= 2             //multiplies all matrix elements by 2

matrix = matrix2        //copies / moves matrix2 into matrix (given that they are same type T)
matrix<T> = matrix2<R>  //copies elements, elements will be of type The
matrix = matrix[mask]   //copies all true elements to matrix

!mask                   //returns a opposite mask (mask is not altered) //use mask = !mask to save the opposite
mask && mask2           //returns mask where both masks had true in same index n
mask || mask2           //returns a mask atleast one mask had true in index n

//ASSIGN OPERATOR

matrix = 1                  //makes all values in matrix 1
matrix<T> = matrix2<R>      //matrix becomes like matrix2 however the datatype of matrix is still the same.
matrix<ref> = matrix<T>     //makes the references the same as val T, does not copy dims and keeps the datatype of ref
matrix<ref> = matrix2<ref>  //copies all of the refs from matrix2, note that this means that matrix wont own its data.






//flags

    //flags are a tool to make syntax on complex operations more user friendly, by routing the operation with a proxy that is designed to "overload operator with same parameters"
    //take matrix(sl{1, 5}, sl{})(sl{2}, sl[3, 5]) to accurately mask this you need
    //"matrix(sl{1, 5}, sl{})(sl{2}, sl[3, 5])[matrix(sl{1, 5}, sl{})(sl{2}, sl[3, 5])<0] = 0;"

    //with flag you can tell the matrix that you dont need to do any logical operations with the mask, and would like it directly
    //so the previous becomes
    //"matrix(sl{1, 5}, sl{})(sl{2}, sl[3, 5]).M <0"
    //way easier and in most cases what you would wanted


//mask_flag
matrix.M <0 = 10                //directly creates a ptr array of all values of matrix under 0, and assigns them as 10
matrix(sl, sl).M() < 0 = 0      //makes all values under 0  = 0 inside the slice
matrix.M()<0;
                                //using flag (mask_flag) to mask is under the hood faster than matrix[mask], as this requires 2x less iterating (no mask needed)
                                //with mask_flag, doing mx = smaller mx works, but does not record its dimensions.




//other member functions for both view and matrix

copy()                          //returns a copy of the slice / matrix
transpose()                     //returns a transposed version of the matrix / slice
matrix(sl{})                    //slices only rows
concat(matrix1, matrix2, axis)  //stacks rows by cols or rows, if axis = axis::Y stacks rows, if axis::Y, cols



//matrix_math library
//exists in the matrix_math namespace
//includes statistics, machine learning, and linear algebra for matrices






//
//broadcast info
//

like said the broadcast only checks that the matrix HAS some rows and cols, not that they match in "logical way"
this is used dynamically by helper functions but there is important caveat worth noticing.

in the iteration the two matrices loop around their own cols and rows

for example if the highest rows is 8 and highest cols is also 8, with mx1(8x4) and mx2(8x8);

this means iterating 8 rows and 8 cols

where the second array has both row and col match the max of 8rows and 8cols. The first has only rows match
the cols of the first get indexed twice in this example so the first col (0) get values from mx(n, 0) and (n, 4)

this is used to calculate sums, statistics, section multiplication etc.
the rows / cols in operation with  matrix =/*+-= matrix2 must be divisible, so doing (3x3) * (4x6) is not allowed 3%4 or 4%3 != 0,
but (3x3) * (6*18) is allowed, this design is meant as a a way to better debug semantic errors, with the cost of slight loss of functionality.
allowing the user to catch errors that would not trigger an exception normally.







//
//ideas
//

//
//1
//
to allow using a (bool loop)

if true (by default false) it would instead of using the maximum possible row count, loop trough continuously its own index (if it is smaller than the maximum)

for example


2x2 + 4x4 assuming first has all 0 and second all 1

would produce [1, 3][3, 9] with loop true it would become [4, 4][4, 4] as when c = 2(over bounds) it would again become 0, then 1, same for r
this would need to dynamically fit into the broadcast as creating overload for this makes a messy code (~100 lines) that only changes a little.

//done with modulos operation // changed to work using checks and iteration++


//
//2
//
to allow better use in ml, introducing something like matrix_learning (mxl) that could be used to perform forward and backward dynamic calculations (chain rule)´
also functions like softmax, layer_norm, activation functions (of varying type) propability calculation functions etc.
core flow would be that it would only work with interfaces from classes like Method. Allowing it to exist as a different header that can be imported if wanted.

//done implementing matrix_learn header

//
//3
//
allow masking of matrices that would allow changing certain elements based on conditions like over, under, is, not etc.
this could be done with a mask_object that would return the actual value ref or a fake value like 0 if the object meets the criteria as all operations use
the operator() for indexing, prechecking the value could allow for a smoother way do conditional math and would introduce logical operators.

//done using mask object and mask_view object, however changed the logic with index array that hold the index of all true elements. (no extra iterations)



//
// 4
//

ideas on how to make masking more dynamic,

1. overloading it on operator[] seperates it cleanly from other overloads for example matrix[matrix < 0] = 0; //done


2. potentially making sliced matrix convert straight away into mask_view, makes it easier to read that way and takes out zero functionality, as chain slicing,
    can be already achieved with 1 (and calculations);  //looking into this with flag system.

3. instead of creating a seperate mask_view, make the function return a Matrix of Nx1 ptr's to these elements, this way it inherits the full functionality of a masked view object
    and only needed is a Matrix(mxb1<T>, Matrix<boolean>) constructor for masks made from slice_view with this, A flag system will be implemented that "flips" functionality in 
    operators .M() makes a direct mask from ptr objects.
    
    also because this ptr matrix only exists as r-value in expression, doing auto matrix = matrix.M()>0 = 2; would only copy the matrix object. will have to look into this, 
    as it would require saving them as ptr, and matrix currently indexes by returning pure ref (matrix[row * cols + c])
    
    potential solutions are 
        1. make it use std::reference wrapper, or other ptr wrapper system.
        2. make a second array that works using a flag
        3. make matrix typename T, typename R = T, this way we can set T to be the actual type, but array the second, making it so that the return wont suffer.
        4. wrapper class, kind of defeats the purpose, but this would be very lightweight that would just use the * in operator() indexing.
        //all solutions must have this, must be able to return T& when indexed, must be sliceable, and when matrix = mask_view, the mask_view is copied into matrix.

        //std::reference or second array is likely cleanest solution, best case scenario is that new members would only be added to base (flags)


        //// DONE by converting matrix to handle mask_view using mask_ref<T> objects, and using .M() to route the mask directly into same matrix / slice

        matrix_slice.M() > 0 = 1 //works


4. to fully utilize slice_view, make it be a construcatable object, a lightweight and movable that has access to data.
    - allow creating it from other slice_view.
    - monitoring if the undelying matrix it points at exists.
        - matrix -> true table, the table will destruct the object
