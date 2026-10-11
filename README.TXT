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
//slicing
//

slicing (now updated) is done witht two slicing objects class Slice that hold only begin and total. When slicing an matrix or matrix_view object,
use mx_obj( {}, {} );

inside the brackets goes the begin index (inclusive) and end index (exclusive), first one is for rows, second for cols
this can be done in three ways.

empty{} takes all rows / cols so mx( {} , {1, 3} ) would select all rows and cols at indexes 1 and 2, same works vice versa and if both.
single{1} takes only that specific col / row for example mx( {1} , {} would take row at index 1 and all its cols. again same works vice versa and both.
both{1, 4} takes the all cols/rows between index 1 and 4 (excluding 4) so mx( {1,4} , {2}; would take the rows of indexes 1, 2, 3 and cols at index 2.
the core principle of this method is that


//
//broadcast info
//

like said the broadcast only checks that the matrix has some rows and cols, not that they match in "logical way"
this is used dynamically by helper functions but there is important caveat worth noticing.

the smaller row and col uses its own values only as long as the iterating row/col is smaller or same as it is. Otherwise it uses
its own maxium possible index value.

for example if the highest rows is 5 and highest cols is also 5, with mx1(5x3) and mx2(5x5);

this means iterating 5 rows and 5 cols

where when c < 3 : mx1(r, c) = mx1(r,c) + mx2(r, c)
when c>=3 : mx1(r, 2) = mx1(r,2) + mx2(r, c)

this is calculated for both mx1 and 2 as 

Matrix( MIN(index r, Matrix_rows-1) , MIN(index c, Matrix_cols-1) );

also important is that the broadcast always changes the first matrix given and never second.
so if a for example Operation::multiply is wanted without mx1 changing this can be done by Matrix mx3{mx1} //copy
and broadcast(mx3, mx2 , Operation::multiply); //all of these are automatically done with operation+,-,/,* (root and pow not);

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
//Operators
//

The core flow in the matrix lib is that it is dynamic.
for this reason almost all of the usual iteration for arithmetic operations is done with overloaded operators.

four different cateqories (for arithmetic)

(examples using +)

mxb + mxb
mxb += mxb
mxb + double
mxb += double

where mxb is either matrix or matrix_view


like said almost every possible combination of matrices is allowed. This can be used to create unique ways of calculations. This method is primarily used by the sum, std avg methods,
to calculate row/col wise products.


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

//done with tile = true in broadcast


//
//2
//
to allow better use in ml, introducing something like matrix_learning (mxl) that could be used to perform forward and backward dynamic calculations (chain rule)´
also functions like softmax, layer_norm, activation functions (of varying type) propability calculation functions etc.
core flow would be that it would only work with interfaces from classes like Method. Allowing it to exist as a different header that can be imported if wanted.
