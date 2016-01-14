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
    void rank(void);
    double determinant();
    Matrix inverse();
    Matrix multiply(Matrix);
    Matrix readjust();

}; 