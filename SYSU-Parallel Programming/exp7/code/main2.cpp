#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <mpi.h>

using namespace std;

void ccopy(int n, double x[], double y[]);
void cfft2(int n, double x[], double y[], double w[], double sgn);
void cffti(int n, double w[]);
double cpu_time();
double ggl(double *ds);
void step(int n, int mj, double a[], double b[], double c[], double d[], double w[], double sgn);
void timestamp();

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    double ctime, ctime1, ctime2;
    double error, flops, mflops;
    int first;
    int i, icase, it, ln2, mj, n, nits = 10000;
    double sgn;
    double *w, *x, *y, *z, *packed_buffer;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    timestamp();
    if (rank == 0)
    {
        cout << "\nFFT_PARALLEL\n";
        cout << "  C++ version with MPI_Pack and MPI_Unpack\n";
        cout << "  Demonstrate an implementation of the Fast Fourier Transform\n";
        cout << "  of a complex data vector.\n";
        cout << "  Accuracy check:\n";
        cout << "    FFT ( FFT ( X(1:N) ) ) == N * X(1:N)\n";
        cout << "\n";
        cout << "             N      NITS    Error         Time          Time/Call     MFLOPS\n";
        cout << "\n";
    }

    double seed = 331.0;
    n = 1;
    for (ln2 = 1; ln2 <= 20; ln2++)
    {
        n = 2 * n;
        w = new double[n];
        x = new double[2 * n];
        y = new double[2 * n];
        z = new double[2 * n];
        packed_buffer = new double[2 * n];

        first = 1;

        for (icase = 0; icase < 2; icase++)
        {
            if (first)
            {
                if (rank == 0)
                {
                    for (i = 0; i < 2 * n; i = i + 2)
                    {
                        double z0 = ggl(&seed);
                        double z1 = ggl(&seed);
                        x[i] = z0;
                        z[i] = z0;
                        x[i + 1] = z1;
                        z[i + 1] = z1;
                    }
                }
            }
            else
            {
                for (i = 0; i < 2 * n; i = i + 2)
                {
                    double z0 = 0.0;
                    double z1 = 0.0;
                    x[i] = z0;
                    z[i] = z0;
                    x[i + 1] = z1;
                    z[i + 1] = z1;
                }
            }

            cffti(n, w);
            if (first)
            {
                sgn = +1.0;
                cfft2(n, x, y, w, sgn);
                sgn = -1.0;
                cfft2(n, y, x, w, sgn);
                double fnm1 = 1.0 / (double)n;
                error = 0.0;
                for (i = 0; i < 2 * n; i = i + 2)
                {
                    error = error +
                            pow(z[i] - fnm1 * x[i], 2) +
                            pow(z[i + 1] - fnm1 * x[i + 1], 2);
                }
                error = sqrt(fnm1 * error);
                if (rank == 0)
                {
                    cout << "  " << setw(12) << n
                         << "  " << setw(8) << nits
                         << "  " << setw(12) << error;
                }
                first = 0;
            }
            else
            {
                ctime1 = cpu_time();
                for (it = 0; it < nits; it++)
                {
                    if (rank == 0)
                    {
                        // Pack data from x to packed_buffer
                        MPI_Pack(x, 2 * n, MPI_DOUBLE, packed_buffer, 2 * n * sizeof(double), &i, MPI_COMM_WORLD);
                    }

                    // Broadcast packed_buffer to all processes
                    MPI_Bcast(packed_buffer, 2 * n, MPI_PACKED, 0, MPI_COMM_WORLD);

                    if (rank == 0)
                    {
                        // Unpack data from packed_buffer to y
                        MPI_Unpack(packed_buffer, 2 * n * sizeof(double), &i, y, 2 * n, MPI_DOUBLE, MPI_COMM_WORLD);
                    }

                    // Perform local FFT
                    cfft2(n, y, x, w, sgn);

                    // Gather the results
                    MPI_Gather(x, 2 * n / size, MPI_DOUBLE, y, 2 * n / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

                    // If root, perform final FFT on gathered data
                    if (rank == 0)
                    {
                        cfft2(n, y, x, w, sgn);
                    }
                }
                ctime2 = cpu_time();
                ctime = ctime2 - ctime1;

                flops = 2.0 * (double)nits * (5.0 * (double)n * (double)ln2);

                mflops = flops / 1.0E+06 / ctime;

                if (rank == 0)
                {
                    cout << "  " << setw(12) << ctime
                         << "  " << setw(12) << ctime / (double)(2 * nits)
                         << "  " << setw(12) << mflops << "\n";
                }
            }
        }
        if ((ln2 % 4) == 0)
        {
            nits = nits / 10;
        }
        if (nits < 1)
        {
            nits = 1;
        }
        delete[] w;
        delete[] x;
        delete[] y;
        delete[] z;
        delete[] packed_buffer;
    }

    if (rank == 0)
    {
        cout << "\nFFT_PARALLEL:\n";
        cout << "  Normal end of execution.\n";
        timestamp();
    }

    MPI_Finalize();

    return 0;
}

void ccopy(int n, double x[], double y[])
{
    int i;
    for (i = 0; i < n; i++)
    {
        y[i * 2 + 0] = x[i * 2 + 0];
        y[i * 2 + 1] = x[i * 2 + 1];
    }
    return;
}

void cfft2(int n, double x[], double y[], double w[], double sgn)
{
    int j, m, mj, tgle;

    m = (int)(log((double)n) / log(1.99));
    mj = 1;
    tgle = 1;
    step(n, mj, &x[0 * 2 + 0], &x[(n / 2) * 2 + 0], &y[0 * 2 + 0], &y[mj * 2 + 0], w, sgn);

    if (n == 2)
    {
        return;
    }

    for (j = 0; j < m - 2; j++)
    {
        mj = mj * 2;
        if (tgle)
        {
            step(n, mj, &y[0 * 2 + 0], &y[(n / 2) * 2 + 0], &x[0 * 2 + 0], &x[mj * 2 + 0], w, sgn);
            tgle = 0;
        }
        else
        {
            step(n, mj, &x[0 * 2 + 0], &x[(n / 2) * 2 + 0], &y[0 * 2 + 0], &y[mj * 2 + 0], w, sgn);
            tgle = 1;
        }
    }
    if (tgle)
    {
        ccopy(n, y, x);
    }

    mj = n / 2;
    step(n, mj, &x[0 * 2 + 0], &x[(n / 2) * 2 + 0], &y[0 * 2 + 0], &y[mj * 2 + 0], w, sgn);
    return;
}

void cffti(int n, double w[])
{
    double arg, aw;
    int i, n2;
    const double pi = 3.141592653589793;

    n2 = n / 2;
    aw = 2.0 * pi / ((double)n);

    for (i = 0; i < n2; i++)
    {
        arg = aw * ((double)i);
        w[i * 2 + 0] = cos(arg);
        w[i * 2 + 1] = sin(arg);
    }
    return;
}

double cpu_time()
{
    double value;
    value = (double)clock() / (double)CLOCKS_PER_SEC;
    return value;
}

double ggl(double *seed)
{
    double d2 = 0.2147483647e10;
    double t, value;

    t = *seed;
    t = fmod(16807.0 * t, d2);
    *seed = t;
    value = (t - 1.0) / (d2 - 1.0);

    return value;
}

void step(int n, int mj, double a[], double b[], double c[], double d[], double w[], double sgn)
{
    double ambr, ambu;
    int j, ja, jb, jc, jd, jw, k, lj, mj2;
    double wjw[2];

    mj2 = 2 * mj;
    lj = n / mj2;

    for (j = 0; j < lj; j++)
    {
        jw = j * mj;
        ja = jw;
        jb = ja;
        jc = j * mj2;
        jd = jc;

        wjw[0] = w[jw * 2 + 0];
        wjw[1] = w[jw * 2 + 1];

        if (sgn < 0.0)
        {
            wjw[1] = -wjw[1];
        }

        for (k = 0; k < mj; k++)
        {
            c[(jc + k) * 2 + 0] = a[(ja + k) * 2 + 0] + b[(jb + k) * 2 + 0];
            c[(jc + k) * 2 + 1] = a[(ja + k) * 2 + 1] + b[(jb + k) * 2 + 1];

            ambr = a[(ja + k) * 2 + 0] - b[(jb + k) * 2 + 0];
            ambu = a[(ja + k) * 2 + 1] - b[(jb + k) * 2 + 1];

            d[(jd + k) * 2 + 0] = wjw[0] * ambr - wjw[1] * ambu;
            d[(jd + k) * 2 + 1] = wjw[1] * ambr + wjw[0] * ambu;
        }
    }
}

void timestamp()
{
    const int TIME_SIZE = 40;
    char time_buffer[TIME_SIZE];
    const struct tm *tm;
    time_t now;

    now = time(NULL);
    tm = localtime(&now);

    strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);

    cout << time_buffer << "\n";

    return;
}
