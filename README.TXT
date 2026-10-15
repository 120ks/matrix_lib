//
//CORE PRINCIPLE
//

The mrx library has two main parts the Matrix class object and a Matrix_view class object.
These two combined allow smooth indexing and slicing with automatic broadcasting trough operators


The broadcasting function found in Matrix_method::broadcast() does inplace broadcasting to the first object given. The broadcast
only checks for invalid row/col number (0) and invalid divider / root (0), otherwise it allows broadcasting dynamically, for example
you can broadcast addison from 5X5 matrix to 2X2 matrix.
important note is that the calculation is done using the larger rows and cols between the two. This is used for example to get total sum
which can be achieved by initializing 1X1 matrix with value 0, and broadcasting with addison the full matrix into it. 
broadcast(1x1, NxM, Operation::add)

other functions include (for now) dot product, sum(total and row/col wise), avg(total and row/col) std(total and row/col), 
finding if matrix has 0, getting transposed matrix. (more will be added)



//
//syntax
//

//create matrix
    to create the first Matrix object, you do Matrix mx{} //which creates an empty matrix object.
    The matrix has a strict set of rows and cols, so reforming it later is not possible(this is due to optimization reasons).
    to initialize a matrix of set size you give braces the rows and cols (and potentially the initilazion value)
    for example Matrix{10, 5, 0} creates a matrix of 10 x 5 all 0 values.

    you can also assign a matrix for example say you have a matrix object mx
    doing mx = mx2 makes mx the same as mx2 (but not a reference);
    doing mx = {10, 5, 0} also works and the new temp matrix is moved into mx's place.

//slicing 
    to slice a matrix you use operator() with two slice objects Slice, Slice. The first is used for rows, second for cols. 
    
    there are 3 ways to create a slice, first is to give the begin and end for the row/col slice.
    for example mx({1, 5}, {3, 5}) takes rows at index 1, 2, 3, 4 (the end is exclusive). and cols 3, 4
    doing chaining takes uses the previous slice, so slice after mx({1, 5}, {3, 5}) uses the index from the previous slice and not from the full
    matrix.

    second is to use single value, for example mx({1}, {3, 5}) uses only row at index 1

    third is to use empty brace, now here is the caution, if the one brace is empty and the other is empty / sigle value, the 
    program uses indexing size_t size_t its place so specifying atleast one is recommended for example mx(Slice{}, {2}) correctly uses the 
    all rows with col 2 //NOTE: a index_object with no default constructor will be created to be able to omit the Slice from the above mentioned mixup
    problem.

    these three can be combined as wished.

//Operators
    operators include 

        all arithmetics * + / -  (modulo will be added when this is made into a template) 
        all arithemtic assigns *=, += etc.
        assign operator =
        comparison operators > < >= == != etc.
        and slicing and indexing operators ()


        also for mask object the logicals !, && , ||

        indexing uses (row, col) which returns the value in either ref or value depending on constness of the matrix.


//
//broadcast info
//

like said the broadcast only checks that the matrix has some rows and cols, not that they match in "logical way"
this is used dynamically by helper functions but there is important caveat worth noticing.

in the iteration the two matrices loop around their own cols and rows

for example if the highest rows is 8 and highest cols is also 8, with mx1(8x4) and mx2(8x8);

this means iterating 8 rows and 8 cols

where the second array has both row and col match the max of 8rows and 8cols. The first has only rows match
the cols of the first get indexed twice in this example so the first col (0) get values from mx(n, 0) and (n, 4)

this is used to calculate sums, statistics, section multiplication etc.

//
//Matrix_method::
//

the matrix method which handles only Matrix_base (mxb used as short) reference objects.
these allow the calling of virtual functions from Matrix_base allowing for a dynamic way of treating Matrix_view objects as matrix objects
without the need for multiple overloaded counterparts.

many functions in Matrix_method have Operation specifier (op), these are add, divide, multiply, substract, root, power and equals.

there is also the row_wise boolean, that overloads many "statistical" functions like std, avg, sum etc. if given, it performs row_wise operation
if true or col_wise if false.

for example Matrix_method::sum(mx1, true) returns a ROW_COUNT X 1 matrix of rows sums, if false 1 X COL_COUNT of row sums, if no is given
then it returns the whole sum of the matrix mx1.



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

//done with modulos operation


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





