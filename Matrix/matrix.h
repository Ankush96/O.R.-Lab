class Matrix{
public:

    int rows;
    int cols;
    double** mat;

    Matrix(int, int);

    void read_matrix(void);
    void display_matrix(void);
    Matrix copy();
    Matrix transpose();
    int rank(void);
    double determinant();
    Matrix inverse();
    Matrix multiply(Matrix);
    Matrix readjust();
    void scale_A(int *, Matrix);
    void pivot_rearrange(int *, Matrix);
    void update_leading_0s(int *, Matrix);

}; 