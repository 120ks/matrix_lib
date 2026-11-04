//
//CORE PRINCIPLE
//

Allow for dynamic operating, viewing, slicing and masking and all of these mixed and combined.
User friendly and fast to produce syntax for quick and idiomatic developing
view safety with version tracking system



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

matrix = matrix[mask]   //copies all true elements to matrix "makes a matrix from true elements"

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

    //used to "skip" the creation of mask object, when you have a simple condition of single comparison operator, use .M()

matrix.M <0 = 10                //directly creates a ptr array of all values of matrix under 0, and assigns them as 10
matrix(sl, sl).M() < 0 = 0      //makes all values under 0  = 0 inside the slice
matrix_view.M()<matrix_view2 *=2 ;              //makes all the values covered by the slice that are under the second matrix_view under times 2.


//assign_flag

    //allows user to change dimensions of matrix or change the view to other view or, make it point at other matrix (must be non const)
    //usage for saving different data into same matrix that has different dimensions
    //or changing the view to other view or other matrix.

    //the assign flag will change the version status of the matrix when it is used, this will make all views unable to access its data unless reset, or force updated

matrix.A() = matrix2  //changes matrix to match matrix2 exactly
matrix.A() = matrix_view //makes matrix same as the view and its slice
matrix.A() = matrix_base(derived)  //makes matrix match matrix_base interface

matrix_view.A() = matrix_view2 //makes matrix_view a copy or moves matrix_view2 to it
matrix_view.A() = matrix   // matrix_view is now pointing at matrix


//view_flag

    //returns a view of matrix (this is matrix specific)
    //used when want to move information of matrix to multiple functions or objects at same time
    //also sometimes cleaner to do auto mxv = matrix.V();
    
func(matrix.V()) //gives the view as parameters, as most functions take either view or matrix, this allows keeping matrix actual data elsewhere
//in multithread functions programs the dot(mxb, mxb) function it is recommended to given
//dot(matrix.V(), matrix2.V()) as this will give the function two copies of the matrices, while slower, than normal
//it frees the matrices for other places after this.






//other member functions for both view and matrix

copy()                          //returns a copy of the slice / matrix
transpose()                     //returns a transposed version of the matrix / slice
matrix(sl{})                    //slices only rows
matrix[sl{}]                    //slices only cols //note [] for cols and () for rows

concat(matrix1, matrix2, axis)  //stacks rows by cols or rows, if axis = axis::Y stacks rows, if axis::Y, cols
if(matrix/mxv/mxb)              //if the matrix / view / derived base class obj, has a valid set of data, meaning it wont throw an exception


//for slice only
reset_view()                    //resets the slice so becomes a plain view of matrix
force_update()                  //NOT RECOMMENDED, will update version to match current version of matrix, keeps slicing data.



//about lifetime

//for both matrix and matrix_view exists override virtual function, allowing the checking of the data with if(mx, mxv, mxb)
//returning true if the data is valid and wont throw no data exceptions. 


//few notes:
    1. the flag becomes false in two scenarios:
        - when the matrix is destructed
        - when the matrix is moved.

    2. the matrix can have its id be true when empty matrix is reconstructed, this means for example matrix = func(std::move(matrix));
        - however all previous views lose their ability to connect to this, as the dims may have changed.

    3. Every matrix has a generation or "version" where if the matrix dims change (mostly in assign flag), the generation count is upped.
        if some matrix_view had generation = 2, and the current is 3, that view cant be used (almost always triggers exception)
        however using mxv.reset_view() updates to current matrix, however loses all slice data.
        if matrix is totally unusable ("no data") generation is negative

    4. the assign flag matrix.A() = matrix2, is when the version changes. Otherwise the version stays the same.

    5. if you are 100% sure, that the dimensions have not changed between version, you can use .force_update(); that automatically updates 
    the version of view to match the matrix, this will only check that the version is not negative.



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


