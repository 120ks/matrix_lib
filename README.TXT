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
    Matrix<T> matrix{const matrix_base& object<T>};     //creates a matrix from the derived matrix, Slice_view or mask_view, copies values
    Matrix<T> matrix{const matrix_base& object<R>};     //copies values of different type of matrix into matrix<T> values will be T
                                                        

//indexing

                        //indexing is done using operator() instead of the normal [] (works with c++ version 20)
    matrix(row, col)    //returns the object inside the matrix at indexes row and col. (row and col are type size_t);
    matrix(row, col)    //returns reference when matrix is not const, and value when matrix is const
    matrix(-1, -3)      //returns reference to matrix val at last row and third last index

    //negative indexing is rows + index, cols + index -> rows - abs(index)...

//slicing

                                    //indexing is done using operator()
    matrix(Slice rows, Slice cols)  //returns a view to matrix object that behaves like a normal matrix, but changes the underlying matrix object
                                    // the shorter version for slice is sl
                                    //the last number given to slice is exclusive
    matrix(sl{1, 3}, sl{3, 6})      //returns a Slice_view object that behaves like a normal matrix with 2 rows and three cols, 
                                        but under the hood changes made to the Slice_view are made to rows 1-2 and 3-5 in the matrix
    matrix(sl{30, 0}, sl{20, 0})    //making the last element 0 in a slice sets the end to the rows/cols of the sliced object, use this if you are not sure
                                        how many rows / cols there are

    matrix(sl{-2, 0}, sl{-4, -1})   //slices using the two last rows with cols from fourth last to last (last exclusive)

    //single row / col
    matrix(sl)      //row slicing () returns matrix(sl{i, i2}, sl{})
    matrix[sl]      //col slicing () returns matrix(sl{}, sl{i, i2})

    matrix.rows_at(long long, long long)       //works the same as matrix(sl{long long, long long})
    matrix.rows_at(long long)                  //single row
    //same for cols

//masking
//use [mask] operator to apply operation on the true elements of the mask
//comparison operators return a Matrix<bool> object
//flag matrix.M() returns a reference matrix to all values that met criteria

    Matrix<bool> mask = matrix < 2:         //makes a mask for matrix, values under 2 -> true, over -> false
    Matrix<bool> mask = matrix > matrix2;   //similar
    matrix[mask]= 1;                            //sets all values where mask(index) == true to 1         
    matrix[matrix<matrix2] = matrix2>matrix1;   //making mask inside masking operator[] "sets all vals under matrix2 to vals of matrix2 over matrix1 (must be same dimmed) 
    matrix[mask1 && mask2] = 0                  //all values that are true in both masks get assigned 0
    matrix[mask1 || mask2] -= 5     //all values that are true in either matrix get substracted 5
    matrix[!mask1] /= 2             //all false values get divided by 2
    matrix.M() < 0 = 0              //directly assign all values under 0 to be 0, no mask needed.

    auto mv = matrix[mask]               //returns a "Mask_view<T>" which is a reference to all true vals, in this sized 1 x true_vals
    auto mv = matrix[row_mask]           //returns a Mask_view to all elements of every row that was true, sized true_rows x cols
    --//-- for cols

//a row/col mask is a mask of dims either (rows x 1) for rows or (1 x cols) for cols, rows/cols must match matrix -> if not, exception




    
//Operators

//arithemtic operators +,',*,/
//comparison operators <, >, <=, >=, ==, !=
//logical (only work on Matrix<bool>) !, &&, ||

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

    //returns a temp object that direcly creates a mask_view object
    matrix.M() < 0 = 2
    matrix.M() < matrix2 *= matrix2.M()>matrix1;




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



//for Matrix_view only member functions
reset_view()                    //resets the slice so becomes a plain view of matrix
force_update()                  //NOT RECOMMENDED, will update version to match current version of matrix, keeps slicing data.



//lifetime

//for both matrix and matrix_view exists override virtual function, allowing the checking of the data with if(mx, mxv, mxb)
//returning true if the data is valid and wont throw no data exceptions.
//for compatability the mask_view also has one, returning true always.


//few notes:
    1. the flag becomes false in two scenarios:
        - when the matrix is destructed
        - when the matrix is moved.

    2. the matrix can have its id be true when empty matrix is reconstructed, this means for example matrix.A() = func(std::move(matrix));
        - however all previous views lose their ability to connect to this, as moving makes gen -negative and reassign updates it to be = abs(gen) + 1

    3. Every matrix has a generation or "version" where if the matrix dims change (mostly in assign flag), the generation count is upped.
        if some matrix_view had generation = 2, and the current is 3, that view cant be used (triggers exception);
        however using mxv.reset_view() updates to current matrix, however loses all slice data.
        if matrix is totally unusable ("no data") generation is negative, and updating fails.

        validity is checked with if(view);

    4. the assign flag matrix.A() = matrix2, is when the version changes.using normal matrix = matrix2 keeps dims

    5. if you are 100% sure, that the dimensions have not changed between version, you can use .force_update(); that automatically updates 
    the version of view to match the matrix, this will only check that the version is not negative "data exists".



//similarity and connectivity

the matrix type objects (matrix, matrix_view and mask_view) are all treated as they were a full matrix, this means they all have same functions and operations
with slight differences in return type on mask_view slicing

all mx type object inherit a list of different member functions ranging from sums, mins, to normalization. correlations all the way to full on statistical models and charting features

they come in roughly two categories.
1. functions that change the data on the Matrix
2. functions that return a new matrix / scalar

the rule of thumb is that "if the function performs operation that would always return a same dimension matrix, it edits the underlying, otherwise returns"
- "common confusion" shift_rows/cols, while it always returns a same dimmed matrix, the data inside does not share same dims (offsets are nan).
- "more heavy functions like form_distribution and HMM are defined as global functions.

the functions are named to be as interpretable as possible while not being too long
//comments are added to ones that might not give clear idea

Matrix<T> flatten()const;

    mask_flag<Tmx, T> M();
    const mask_flag<Tmx, T> M()const;

    mask_flag<Tmx, T> RMA();
    mask_flag<Tmx, T> CMA();
    const mask_flag<Tmx, T> RMA()const;
    const mask_flag<Tmx, T> CMA()const;

    mask_flag<Tmx, T> RMO();
    mask_flag<Tmx, T> CMO();
    const mask_flag<Tmx, T> RMO()const;
    const mask_flag<Tmx, T> CMO()const;

    //DATA OPERATIONS DELEGATIONS
    template<typename Rmx, typename R>
    auto stack(const Matrix_base<Rmx, R> &mxb2);

    template<typename Rmx, typename R>
    auto stack_side(const Matrix_base<Rmx, R> &mxb2);

    template<typename R>
    Matrix<T> concat_rows(std::initializer_list<R> indexes)const;
    template<typename R>
    Matrix<T> concat_cols(std::initializer_list<R> indexes)const;
    template<typename R>
    Mask_view<T> ref_rows(std::initializer_list<R> indexes);
    template<typename R>
    Mask_view<T> ref_cols(std::initializer_list<R> indexes);

    Matrix<T> shift_rows(long long)const;
    Matrix<T> shift_cols(long long)const;
    
    template<typename R = T>
    requires std::is_same_v<R, bool>
    Matrix<bool> row_mask(bool = true)const;

    template<typename R = T>
    requires std::is_same_v<R, bool>
    Matrix<bool> col_mask(bool = true)const;
    
    Matrix<bool> mask_nan()const;
    Matrix<T> drop_nan_rows()const;
    Matrix<T> drop_nan_cols()const;

    //SEARCH
    template<typename R>
    T& closest(const R&);

    template<typename R>
    const T& closest(const R&)const;

    //MEAN SUM VAR SD
    T sum()const;
    Matrix<T> row_sum()const;
    Matrix<T> col_sum()const;

    double mean()const;
    Matrix<double> row_mean()const;
    Matrix<double> col_mean()const;

    T median()const;
    Matrix<T> row_median()const;
    Matrix<T> col_median()const;

    auto var(float = 0)const;
    Matrix<double> row_var(float = 0)const;
    Matrix<double> col_var(float = 0)const;

    auto std(float = 0)const;
    Matrix<double> row_std(float = 0)const;
    Matrix<double> col_std(float = 0)const;

    //MIN AND MAX
    T min()const;
    T max()const;
    Matrix<T> row_min()const;
    Matrix<T> row_max()const;
    Matrix<T> col_min()const;
    Matrix<T> col_max()const;

    //CORRELATION, COVARIANCE
    double auto_corr(long long = 1, float = 1);

    template<typename Rmx, typename R>
    double corr(const Matrix_base<Rmx, R>&, float = 1);

    template<typename Rmx, typename R>
    Matrix<double> auto_row_corr(const Matrix_base<Rmx, R>&, float = 1); //NIMPLY

    template<typename Rmx, typename R>
    Matrix<double> auto_col_corr(const Matrix_base<Rmx, R>&, float = 1); //NIMPLY

    template<typename Rmx, typename R>
    Matrix<double> row_corr(const Matrix_base<Rmx, R>&, float = 1);

    template<typename Rmx, typename R>
    Matrix<double> col_corr(const Matrix_base<Rmx, R>&, float = 1);

    template<typename Rmx, typename R>
    double cov(const Matrix_base<Rmx, R>&, float = 1);

    Matrix<double> cov_matrix(float = 1);

    template<typename Rmx, typename R>
    Matrix<double> row_cov(const Matrix_base<Rmx, R>&, float = 1);

    template<typename Rmx, typename R>
    Matrix<double> col_cov(const Matrix_base<Rmx, R>&, float = 1);

    //NORMS & SCALES
    template<typename R, typename U>
    Tmx& min_max(R = 0, U = 1);
    Tmx& z_scores(float = 1);
    Tmx& robust_scale(float = 0.2, float = 0.8);
    Tmx& robust_z(float = 0.2, float = 0.8, float = 1);
    Tmx& batch_norm(float = 1);
    Tmx& layer_norm(float = 1);
    Tmx& group_norm(long long, float = 1);


    //CLIPS AND EXHAUST
    template<typename R, typename U>
    Tmx& clip(R, U);
    Tmx& clip_Q(float = 0.1, float = 0.9);
    template<typename R, typename U>
    Tmx& clip_to_zero(R = -0.1, U = 0.1);

    template<typename R>
    Tmx& exhaust(R = 1, float = 1);
    Tmx& exhaust_Q(float = 0.1, float = 0.9, float = 1);

    //SPECIAL
    Tmx& abs();
    Tmx& log();
    Tmx& exp();

    //TRIGONOMETRY
    Tmx& sin();
    Tmx& cos();
    Tmx& tan();
    Tmx& tanh();


    //NON LINEAR TRANSFORM
    Tmx& rank(bool = true, bool = false);
    Tmx& quantile();
    Tmx& bucket(long long);
    Tmx& fit_gaussian();
    
    template<typename R, typename U>
    Tmx& randomize_uniform(R = 0, U = 1);

    template<typename R, typename U>
    Tmx& randomize_normal(R = 0, U = 1);

    Tmx& init_he_normal(long long = -1);
    Tmx& init_he_uniform(long long = -1);
    Tmx& init_xav_normal(long long = -1, long long = -1);
    Tmx& init_xav_uniform(long long = -1, long long = -1);


    //POWER AND ROOT
    template<typename R>
    requires (std::is_arithmetic_v<R>)
    Tmx& power(const R&);
    template<typename Rmx, typename R>
    Tmx& power(const Matrix_base<Rmx, R>&);

    template<typename R>
    requires (std::is_arithmetic_v<R>)
    Tmx& root(const R&);
    template<typename Rmx, typename R>
    Tmx& root(const Matrix_base<Rmx, R>&);

    //CHARTING FEATURES
    Tmx& map();
    Tmx& chart_xy();
    Tmx& chart_yx();


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


//UPCOMING FEATURES

//auto matrix{csv} //from csv
//other file types
