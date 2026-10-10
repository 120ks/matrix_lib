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

slicing is done using operator() with four params (row_index first, row_index_exclusive, col_index firs, col_index_exclusive);
this can be chainer where the latters slice the already sliced sub matrix.

this submatrix can be treated with the same arithmetic operators as regular matrix
for example

mx(3,4 , 3,5) *= mx2(3,4 , 5,7);


the assign operator works differently between Matrix and Matrix_view
with Matrix the object "becomes" the assigned object (matrix or view) 
with Matrix_view the slice's values become the assigned objects value.

so if Matrix mx is 3x5
and mx = mx2(2,4 , 3,4);
mx becomes a 2x1 Matrix.

with Matrix_view the slice only changes value.


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

many functions in Matrix_method have Operation specifier (op), these are add, divide, multiply, substract, root, power and equals, these 
are the last argument of a function and are told as Operation::multiplu for example.

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


//
//design way
//

The core design element (after some while), was to treat Matrix_view as plain r-value, constructors accessible only by friend classes.
this means that for example multislicing and assigning it into object cant be done. Matrix_view mxv( mx(1,5, 2,4)(1,2,1,2)) would create a dangling
ptr if mx goes out of scope before mxv. Disallowing public creation removes this problem.

the Matrix_base is a interface class of only pure virtuals (excluding constructor), this is used the "usual" way to allow
dynamic operating in Matrix method.
