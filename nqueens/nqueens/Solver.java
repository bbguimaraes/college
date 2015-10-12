package nqueens;

import java.util.List;

public class Solver {
    int l, c;
    char[][] mat;

    public Solver(int n) {
        this.l = 0;
        this.c = 0;
        this.mat = this.create_matrix(n);
    }

    public int nqueen(int l, int c, List<char[][]> r) {
        int n = this.mat.length;
        if(this.possible(this.mat, l, c)) {
            this.mat[l][c] = 'q';
            if(l == n - 1) {
                r.add(this.dup_mat(this.mat));
                this.mat[l][c] = 0;
                return 1;
            } else {
                int np = 0;
                for(int i = 0; i < n; i++) {
                    np += nqueen(l + 1, i, r);
                }
                this.mat[l][c] = 0;
                return np;
            }
        }
        return 0;
    }

    public static char[][] create_matrix(int n) {
        char[][] m = new char[n][n];
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                m[i][j] = 0;
        return m;
    }

    private static char[][] dup_mat(char[][] m) {
        int n = m.length;
        char[][] mat_dup = new char[n][n];
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
                mat_dup[i][j] = m[i][j];
        return mat_dup;
    }

    private static boolean possible(char[][] m, int l, int c) {
        if(l == 0)
            return true;
        int i, j;
        for(i = 0; i < l; i++)
            if(m[i][c] != 0)
                return false;
        i = l - 1;
        j = c - 1;
        while(i >= 0 && j >= 0) {
            if(m[i][j] != 0)
                return false;
            i--;
            j--;
        }
        i = l - 1;
        j = c + 1;
        while(i >= 0 && j < m.length) {
            if(m[i][j] != 0)
                return false;
            i--;
            j++;
        }
        return true;
    }
}
