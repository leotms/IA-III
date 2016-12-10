#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
// global contants
int N, M, T_SEG;

//*****************************************************************************/
// splits a line to separate its elemens into a vector
void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}
//*****************************************************************************/

// returns the value of the variable that represents a segment for point (i,j)
int q(int i, int j, char side) {
  int box = (i - 1)*M + j;
  if (side == 'n'){
    return box;
  } else if (side == 's') {
    return (i*M) + j;
  } else if (side == 'w') {
    return  N*(M + 1) + box + i -1;
  } else if (side == 'e') {
    return N*(M + 1) + box + i;
  }
}

// matches a box into an unique value depending on i y j
// for differing inner or outer perimeter box.
int z(int i, int j) {
  int box = (i - 1)*M + j;
  return T_SEG + box;
}

// matches a box into a unique value depending on m and n
// for stablishing reachability for box c and box c'
int r(int ci, int cj, int cpi, int cpj) {
  int box_c  = (ci - 1)*M + cj;
  int box_cp = (cpi - 1)*M + cpj;
  int box = (box_c - 1)*M*N + box_cp;
  return (N*M) + T_SEG + box;
}


int main(int argc, char ** argv) {

  FILE * inputfile;
  FILE * resfile;
  ofstream outfile;
  ofstream answerfile;


  inputfile  = fopen(argv[1],"r");
  answerfile.open("output.txt");

  char *  line = NULL;
  size_t  len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, inputfile)) != -1) {
    outfile << "c " << line;
    vector<string> info = split(line, ' ');

    outfile.open("clauses.cnf");

    N = stoi(info[0]);
    M = stoi(info[1]);

    // number of vertical segments + horizontal segments
    int SEG_HOR = (N+1)*M;
    int SEG_VER = (M+1)*N;
    T_SEG = SEG_VER + SEG_HOR;

    // we extract the info from the line
    vector<string> values(info.end() - N, info.end());

    // we calculate the number of variables including T_SEG, Z and R.
    int T_VARIABLES = N*N*M*M + N*M + (T_SEG);

    outfile << "c N : "<< N << "M : " << M << "\n";
    // header for the cnf file
    outfile << "p cnf " << T_VARIABLES << " 0\n";

    for(int i = 1; i <= N; i++){
      string row = values[i-1];
      for( int j = 1; j <= M; j++) {

        // CHECK FOR TYPE 1 CLAUSES
        outfile << "c TYPE 1 CLAUSES: for (i,j) = (" << i << "," << j << "):\n";

        // the box has no segments
        if (row[j-1] =='0'){
          outfile << " -" << q(i,j,'n') << " 0\n";
          outfile << " -" << q(i,j,'s') << " 0\n";
          outfile << " -" << q(i,j,'e') << " 0\n";
          outfile << " -" << q(i,j,'w') << " 0\n";
        } // the box has only one segment
        else if (row[j-1] =='1'){
          outfile << q(i,j,'n') << " " << q(i,j,'e') << " " << q(i,j,'s') << " " << q(i,j,'w') << " 0\n";
          outfile << "-" << q(i,j,'s') << " -" << q(i,j,'w') << " 0\n";
          outfile << "-" << q(i,j,'e') << " -" << q(i,j,'s') << " 0\n";
          outfile << "-" << q(i,j,'n') << " -" << q(i,j,'w') << " 0\n";
          outfile << "-" << q(i,j,'n') << " -" << q(i,j,'e') << " 0\n";
          outfile << "-" << q(i,j,'n') << " -" << q(i,j,'s') << " 0\n";
          outfile << "-" << q(i,j,'e') << " -" << q(i,j,'w') << " 0\n";
        } // the box has two segments
        else if (row[j-1] =='2'){
          outfile << " -" << q(i,j,'n') << " -" << q(i,j,'e') << " -" << q(i,j,'s') << " -" << q(i,j,'w') << " 0\n";
          outfile << "-" << q(i,j,'n') << " -" << q(i,j,'s') << " -" << q(i,j,'e') << " 0\n";
          outfile << "-" << q(i,j,'n') << " -" << q(i,j,'s') << " -" << q(i,j,'w') << " 0\n";
          outfile << q(i,j,'w') << " " << q(i,j,'e') << " " << q(i,j,'s') << " 0\n";
          outfile << q(i,j,'w') << " " << q(i,j,'e') << " " << q(i,j,'n') << " 0\n";
          outfile << "-" << q(i,j,'n') << " -" << q(i,j,'w') << " -" << q(i,j,'e') << " 0\n";
          outfile << q(i,j,'n') << " " << q(i,j,'e') << " " << q(i,j,'s') << " 0\n";
          outfile << q(i,j,'w') << " " << q(i,j,'n') << " " << q(i,j,'s') << " 0\n";
          outfile << "-" << q(i,j,'s') << " -" << q(i,j,'w') << " -" << q(i,j,'e') << " 0\n";
          outfile << q(i,j,'w') << " " << q(i,j,'n') << " " << q(i,j,'e') << " 0\n";

        } // the box has three segments
        else if (row[j-1] =='3'){
          outfile << " -" << q(i,j,'n') << " -" << q(i,j,'e') << " -" << q(i,j,'s') << " -" << q(i,j,'w') << " 0\n";
          outfile << q(i,j,'n') << " " << q(i,j,'s') << " 0\n";
          outfile << q(i,j,'n') << " " << q(i,j,'e') << " 0\n";
          outfile << q(i,j,'n') << " " << q(i,j,'w') << " 0\n";
          outfile << q(i,j,'s') << " " << q(i,j,'e') << " 0\n";
          outfile << q(i,j,'s') << " " << q(i,j,'w') << " 0\n";
          outfile << q(i,j,'w') << " " << q(i,j,'e') << " 0\n";

        } // the box has all four segments
        else if (row[j-1] =='4'){
          outfile << q(i,j,'n') << " 0\n";
          outfile << q(i,j,'s') << " 0\n";
          outfile << q(i,j,'e') << " 0\n";
          outfile << q(i,j,'w') << " 0\n";
        }

        // CHECK FOR TYPE 2 CLAUSES
        outfile << "c TYPE 2 CLAUSES: for (i,j) = (" << i << "," << j << "):\n";
        // left border
        if (j == 1) {
          outfile << q(i,1,'w') << " -"  << z(i,1) << " 0\n";
          outfile << " -" << q(i,1,'w') << " " << z(i,1) << " 0\n";
        } // right border
        else if (j == N) {
          outfile << q(i,M,'e') << " -"  << z(i,M) << " 0\n";
          outfile << " -" << q(i,M,'e') << " " << z(i,M) << " 0\n";
        } // upper border
        else if (i == 1) {
          outfile << q(i,j,'n') << " -"  << z(1,j) << " 0\n";
          outfile << " -" << q(i,j,'n') << " " << z(1,j) << " 0\n";
        } // lower border
        else if (i == N){
          outfile << " " << q(N,j,'s') << " -"  << z(N,j) << " 0\n";
          outfile << " -" << q(N,j,'s') << " " << z(N,j) << " 0\n";
        } // inner
        else {

          // CNF CLAUSE
          // !p v q
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'e') << " -" << q(i,j,'n') << " -"<< q(i,j,'s') << " -"<< q(i,j,'w') << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'e') << " -" << q(i,j,'n') << " -"<< q(i,j,'s') << " "<< z(i-1,j) << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'e') << " -" << q(i,j,'n') << "  "<< z(i,j-1) << " -" << q(i,j,'w') << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'e') << " -" << q(i,j,'n') << " " << z(i,j-1)  << " " << z(i-1,j) << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'n') << " -" << q(i,j,'s') << " -"<< q(i,j,'w') << " "<< z(i+1,j) << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'n') << " -" << q(i,j,'s') << " " << z(i-1,j) << " "<< z(i+1,j) << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'n') << " -" << q(i,j,'w') << " " << z(i,j-1) << " "<< z(i+1,j) << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'n') << " " << z(i,j-1) << " " << z(i-1,j) << " "<< z(i+1,j) << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'e') << " -" << q(i,j,'s') << " -" << q(i,j,'w') << " " << z(i,j+1) << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'e') << " -" << q(i,j,'s') << " "  << z(i,j+1) << " "<< z(i-1,j) << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'e') << " -" << q(i,j,'w') << " "  << z(i,j+1) << " "<< z(i,j-1) << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'e') << " "  << z(i,j-1)    << " "<< z(i,j+1) << " "<< z(i-1,j)<< " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'s') << " -" << q(i,j,'w') << " " << z(i,j+1) << " "<< z(i+1,j)<< " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'s') <<  " " << z(i,j+1) << " "<< z(i+1,j) << " " << z(i-1,j) << " 0\n";
          outfile << " -" <<  z(i,j) << " -"  << q(i,j,'w') <<  " " << z(i,j+1) << " "<< z(i+1,j) << " " << z(i,j-1) << " 0\n";
          outfile << " -" <<  z(i,j) << " "   << z(i-1,j) <<  " " << z(i,j+1) << " "<< z(i+1,j) << " " << z(i,j-1) << " 0\n";

          // p v !q
          outfile << " " <<  z(i,j) << " " << q(i,j,'n') <<  " -" << z(i,j+1) << " 0\n"; // CHECK
          outfile << " " <<  z(i,j) << " " << q(i,j,'e') <<  " -" << z(i+1,j) << " 0\n";
          outfile << " " <<  z(i,j) << " " << q(i,j,'s') <<  " -" << z(i,j-1) << " 0\n";
          outfile << " " <<  z(i,j) << " " << q(i,j,'w') <<  " -" << z(i-1,j) << " 0\n";
        }

        // set the type 3 clauses
        outfile << "c TYPE 3 CLAUSES: for (i,j) = (" << i << "," << j << "):\n";

        // Check for type 3 clauses
        // Every box can reach itself
        outfile << r(i,j,i,j) << " 0\n";

        for (int i_p = 1; i_p <= N; i_p++) {
          for (int j_p = 1; j_p <= M; j_p++) {
              // check adjacent boxes for the corners
              // every line except the last one
              if (i_p + 1 < N) {
                //bottom adjacent
                outfile << " -"  << r(i,j,i_p,j_p) << " "  << q(i_p,j_p,'s') << " "  << r(i,j, i_p+1,j_p) << " 0\n";
              } // every line except the first one
              if (i_p - 1 > 1) {
                //top adjacent
                outfile << " -"  << r(i,j, i_p,j_p) << " "  << q(i_p,j_p,'n') << " "  << r(i,j, i_p-1,j_p) << " 0\n";
              } // every column except the first one
              if (j_p - 1 > 1) {
                //left adjacent
                outfile << " -"  << r(i,j, i_p,j_p) << " "  <<  q(i_p,j_p,'w') << " "  << r(i,j, i_p,j_p-1) <<  " 0\n";
              } // every column except the last one
              if (j_p + 1 < M) {
                //right adjacent
                outfile << " -"  << r(i,j, i_p,j_p) << " "  <<  q(i_p,j_p,'e') << " "  << r(i,j, i_p, j_p+1) << " 0\n";
              }
          }
        }

        // Check for type 4 clauses
        // we iterate over every char
        outfile << "c TYPE 4 CLAUSES: for (i,j) = (" << i << "," << j << "):\n";
        for (int i_p = 1; i_p <= N; i_p++) {
          for(int j_p = 1; j_p <=M; j_p++){
            outfile << z(i,j) << " " << z(i_p,j_p) << " " << r(i,j,i_p,j_p) << " 0\n";
          }
        }

      }
    }

    // OTHER CLAUSES

    // makes restrictions for adjacency of segments
    // upper-left corner
    outfile << " -" << q(1,1,'n') << " " << q(1,1,'w') << " 0\n";
    outfile << " -" << q(1,1,'w') << " " << q(1,1,'n') << " 0\n";

    // upper-right corner
    outfile << " -" << q(1,M,'n') << " " << q(1,M,'e') << " 0\n";
    outfile << " -" << q(1,M,'e') << " " << q(1,M,'n') << " 0\n";

    // lower-left corner
    outfile << " -" << q(N,1,'s') << " " << q(N,1,'w') << " 0\n";
    outfile << " -" << q(N,1,'w') << " " << q(N,1,'s') << " 0\n";

    // lower-right corner
    outfile << " -" << q(N,M,'s') << " " << q(N,M,'e') << " 0\n";
    outfile << " -" << q(N,M,'e') << " " << q(N,M,'s') << " 0\n";

    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= M; j++) {
            // upper border without corner
            if (i == 0 && j != 0 && j != M) {
                outfile << " -" << q(1,j,'n') << " " << q(1,j+1,'n') << " " << q(1,j,'e') << " 0\n";
                outfile << q(1,j,'n') << " -" << q(1,j+1,'n') << " " << q(1,j,'e') << " 0\n";
                outfile << q(1,j,'n') << " " << q(1,j+1,'n') << " -" << q(1,j,'e') << " 0\n";
                outfile << " -" << q(1,j,'n') << " -" << q(1,j+1,'n') << " -" << q(1,j,'e') << " 0\n";
            }

            // lower border without corner
            else if (i == N && j != 0 && j != M) {
                outfile << " -" << q(N,j,'s') << " " << q(N,j+1,'s') << " " << q(N,j,'e') << " 0\n";
                outfile << q(N,j,'s') << " -" << q(N,j+1,'s') << " " << q(N,j,'e') << " 0\n";
                outfile << q(N,j,'s') << " " << q(N,j+1,'s') << " -" << q(N,j,'e') << " 0\n";
                outfile << " -" << q(N,j,'s') << " -" << q(N,j+1,'s') << " -" << q(N,j,'e') << " 0\n";
            }

            // left border without corner
            if (j == 0 && i != 0 && i != N ){
                outfile << " -" << q(i,1,'w') << " " << q(i+1,1,'w') << " " << q(i,1,'s') << " 0\n";
                outfile << q(i,1,'w') << " -" << q(i+1,1,'w') << " " << q(i,1,'s') << " 0\n";
                outfile << q(i,1,'w') << " " << q(i+1,1,'w') << " -" << q(i,1,'s') << " 0\n";
                outfile << " -" << q(i,1,'w') << " -" << q(i+1,1,'w') << " -" << q(i,1,'s') << " 0\n";
            }

            // right border without corner
            else if (j == M  && i != 0 && i != N){
                outfile << " -" << q(i,M,'e') << " " << q(i+1,M,'e') << " " << q(i,M,'s') << " 0\n";
                outfile << q(i,M,'e') << " -" << q(i+1,M,'e') << " " << q(i,M,'s') << " 0\n";
                outfile << q(i,M,'e') << " " << q(i+1,M,'e') << " -" << q(i,M,'s') << " 0\n";
                outfile << " -" << q(i,M,'e') << " -" << q(i+1,M,'e') << " -" << q(i,M,'s') << " 0\n";
            }

            // inside cases
            if (i > 0 && i < N && j > 0 && j < M) {

                // when a segment is adjacent to (i,j), there is another segment adjacent to (i,j)
                outfile << " -" << q(i,j,'e') << " " << q(i,j,'s') << " " << q(i+1,j+1,'w') << " " << q(i+1,j+1,'n') << " 0\n";
                outfile << q(i,j,'e') << " -" << q(i,j,'s') << " " << q(i+1,j+1,'w') << " " << q(i+1,j+1,'n') << " 0\n";
                outfile << q(i,j,'e') << " " << q(i,j,'s') << " -" << q(i+1,j+1,'w') << " " << q(i+1,j+1,'n') << " 0\n";
                outfile << q(i,j,'e') << " " << q(i,j,'s') << " " << q(i+1,j+1,'w') << " -" << q(i+1,j+1,'n') << " 0\n";

                // there are only two adjacent segments for (i,j)
                outfile << " -" << q(i,j,'e') << " -" << q(i,j,'s') << " -" << q(i+1,j+1,'w') << " 0\n";
                outfile << " -" << q(i,j,'e') << " -" << q(i,j,'s') << " -" << q(i+1,j+1,'n') << " 0\n";
                outfile << " -" << q(i,j,'e') << " -" << q(i+1,j+1,'w') << " -" << q(i,j,'s')  << " 0\n";
                outfile << " -" << q(i,j,'e') << " -" << q(i+1,j+1,'w') << " -" << q(i+1,j+1,'n') << " 0\n";
                outfile << " -" << q(i,j,'e') << " -" << q(i+1,j+1,'n') << " -" << q(i,j,'s') << " 0\n";
                outfile << " -" << q(i,j,'e') << " -" << q(i+1,j+1,'n') << " -" << q(i+1,j+1,'w')  << " 0\n";
                outfile << " -" << q(i,j,'s') << " -" << q(i+1,j+1,'w') << " -" << q(i,j,'e') << " 0\n";
                outfile << " -" << q(i,j,'s') << " -" << q(i+1,j+1,'w') << " -" << q(i+1,j+1,'n') << " 0\n";
                outfile << " -" << q(i,j,'s') << " -" << q(i+1,j+1,'n') << " -" << q(i,j,'e') << " 0\n";
                outfile << " -" << q(i,j,'s') << " -" << q(i+1,j+1,'n') << " -" << q(i+1,j+1,'w') << " 0\n";
                outfile << " -" << q(i+1,j+1,'w') << " -" << q(i+1,j+1,'n') << " -" << q(i,j,'e') << " 0\n";
                outfile << " -" << q(i+1,j+1,'w') << " -" << q(i+1,j+1,'n') << " -" << q(i,j,'s') << " 0\n";
            }
        }
    }

    outfile.close();
    // we excecute minisat and redirect the output to the file sat.log
    int status = system("./minisat/core/minisat clauses.cnf res.out >> sat.log");
    resfile = fopen("res.out","r");

    char *  line_res = NULL;
    size_t  len_res = 0;
    ssize_t read_res;

    read_res = getline(&line_res, &len_res, resfile);
    // means the problem is satisfiable
    if (line_res[0] == 'S') {
      answerfile << line;
      answerfile << N << " " << M << " ";
      // we read the line containing the answer
      read_res = getline(&line_res, &len_res, resfile);
      vector<string> answer = split(line_res, ' ');

      string binary_horizontal = "";
      string binary_vertical   = "";
      for(int i = 0; i < T_SEG; i++) {
        string var = answer[i];
        //check if the variable matches an horizontal segment
        if (i < SEG_HOR) {
          if(var[0] == '-') {
            binary_horizontal += '0';
          } else {
            binary_horizontal += '1';
          }
        } else {
          if(var[0] == '-') {
            binary_vertical += '0';
          } else {
            binary_vertical += '1';
          }
        }
      }

      vector<string> vector_horizontal;
      vector<string> vector_vertical;

      int h_i = 0;
      int v_i = 0;
      string aux = "";
      for (int i = 0; i < SEG_HOR; i++){
        aux += binary_horizontal[i];
        h_i++;
        if (h_i == M) {
          vector_horizontal.push_back(aux);
          aux = "";
          h_i = 0;
        }
      }

      for (int i = 0; i < SEG_VER; i++){
        aux += binary_vertical[i];
        v_i++;
        if (v_i == M + 1) {
          vector_vertical.push_back(aux);
          aux = "";
          v_i = 0;
        }
      }

      string final_binary = "";
      for (int i = 0; i < N + 1; i++) {
        final_binary += vector_horizontal[i];
        final_binary += " ";
        if (i < N) {
          final_binary += vector_vertical[i];
          final_binary += " ";
        }
      }

      answerfile << final_binary << '\n';

    }
  }

  answerfile.close();
  int sis = system("rm *.out *.cnf");

  return 0;

}
