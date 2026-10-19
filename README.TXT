//
//CORE PRINCIPLE
//

Allow for dynamic operating, viewing, slicing and masking and all of these mixed and combined.
the goal is to allows pretty much all that make semantically some sense (not full but enough to be realistically possible)


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


//masking

                                            //comparison operators return a Matrix<boolean> object
    Matrix<boolean> mask = {matrix < 2};    //mask object is a matrix that holds a custom boolean object
    Matrix<boolean> mask = matrix > 0       //returns a mask size matrix that has true or false
    matrix[mask]= 1;                       // makes all of the values that are true equal to one.
    matrix[matrix<0] = 0;                   //all values under 0 in the matrix will be under zero
                                            //mask has to match with rows and cols the matrix or its slice

    
    
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
matrix = matrix(mask)   //copies all true elements to matrix
matrix = matrix(slice)  //makes a new mask of the sliced matrix

!mask                   //returns a opposite mask (mask is not altered) //use mask = !mask to save the opposite
mask && mask            //returns mask where both masks had true in same index n
mask ||                 //returns a mask atleast one mask had true in index n


    //important note on arithmetic operations. To enable statistics (avg, std, normalization, etc) and ml operations like sliding multiplication,
        the broadcast (function behind arit operations) only checks for empty row / cols, this means that if you pass in matrix 4x4 and 2X2
        the 4x4 gets multiplied in four different places by 2x2. So for row 0, cols 0, 1, 2, 3 the 2x2 uses cols 0, 1, 0, 1 and same for rows.
        make sure you do dimension checking before hand if it is important that they match


//
//FLAGS
//

                                    //flags change how a object is treated or return one that has desired attributes .M(), .T()
                                    // .M() tells that this object will directly be converted into Mask_view object
                                    // .T returns a view from the matrix that treat is as transposed

matrix(slice, slice) > 0 = 0        //would assign the mask to 0
matrix(slice, slice).M() > 0 = 0    //sets the slice's mask flag to opposite (by default false so true) and this will directly create the mask_view object
matrix(slice, slice).M(false) > 0   //nothing happens, a mask (Matrix<boolean>) is created.

matrix.T() * matrix2, this          //returns a transposed view of the matrix (slice_view)

syntax tips...

//even though multioperational syntax is allowed matrix(sl{2}, sl{2, 10})((matrix(sl{1, 10}, sl{2, 24})(sl{3, 4}, sl{2,10})>0)) = 1;" will compile and work
    because view_object are treated as matrices the slice and mask applies to the "apparent matrix" and not the actual one. Therefore it is less error
    prone to just do per line operations. for example making a mask out of a mask_view is allowed, however this returns a true_elements x 1 mask, which will 
    not work with something like matrix(mask_from mask_view);


// when doing a "crazy" operation like creating a mask from over 0 then masking a matrix with it, then slicing the mask_view and mulitpying that by 3, works always sequentally
    1. you make a mask from matrix of NxM : auto mask = matrix>0 : (mask is NxM) and has 10 true elements
    2. you use the mask to create a mask view : matrix[mask] : the mask_view is dimensions 10x1
    3. you slice the mask_view say between 0 and 3 : matrix[mask]({0, 3}) : the returned is slice view object of always type size_t
    4. you multiply this slice by three. So you are multiplying the value in in the index_array and not the array that the mask_view was pointing at, this is why i
        highly discourage not slice mask_view's;
    
    


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

//
//matrix_method::
//

the matrix method has few useful functions that work as the backbone for other function.


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

1. overloading it on operator[] seperates it cleanly from other overloads for example matrix[matrix < 0] = 0;
2. potentially making sliced matrix convert straight away into mask_view, makes it easier to read that way and takes out zero functionality, as chain slicing,
    can be already achieved with 1 (and calculations);

3. instead of creating a seperate mask_view, make the function return a Matrix of Nx1 ptr's to these elements, this way it inherits the full functionality of a masked view object
    and only needed is a Matrix(mxb1<T>, Matrix<boolean>) constructor for masks made from slice_view with this, A flag system will be implemented that "flips" functionality in 
    operators .M() makes a direct mask from ptr objects.
    
    also because this ptr matrix only exists as r-value in expression, doing auto matrix = matrix.M()>0 = 2; would only copy the matrix object. will have to look into this, 
    as it would require saving them as ptr, and matrix currently indexes by returning pure ref (matrix[row * cols + c])
    
