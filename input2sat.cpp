#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
int n = 0;
int m = 0;

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

// matches a box into an unique value depending on m and n
// for differing inner or outer perimeter box.
int z(int casilla) {
  return ((m*n) + m)*2 + casilla;
}

// matches a box into a unique value depending on m and n
// for stablishing reachability for casilla1 to casilla2
int r(int casilla1, int casilla2) {
  return ((m*n) + m)*2 + m*n + (casilla1 - 1)*m*n + casilla2;
}

int main(int argc, char ** argv) {

  FILE * inputfile;
  FILE * outfile;

  inputfile = fopen(argv[1],"r");
  outfile   = fopen("clauses.cnf","w");

  char *  line = NULL;
  size_t  len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, inputfile)) != -1) {
    cout << "c " << line;

    vector<string> informacion = split(line, ' ');

    n = stoi(informacion[0]);
    m = stoi(informacion[1]);
    vector<string> valores(informacion.end() - n, informacion.end());

    int nvariables = (m*n + m) * 2;
    cout << "c N : "<< n << "M : " << m << "\n";
    cout << "p cnf " << nvariables << " 0\n";

    int casilla  = 1;

    // set the type 0 clauses
    // for(int i = 0; i < n; i++){
    //   string fila = valores[i];
    //   for( int j = 0; j < m; j++) {
    //     cout << "c C" << casilla << ": "<< fila[j] << "\n";
    //     int n_casilla = (casilla - 1)*3 + casilla;
    //     int e_casilla = n_casilla + 1;
    //     int s_casilla = n_casilla + 2;
    //     int w_casilla = n_casilla + 3;
    //     if ((j <= m - 2)) {
    //       // cout << "condicion pared para la casilla C" << casilla << " \n";
    //       int casilla_lat   = casilla + 1;
    //       int n_casilla_lat = (casilla_lat - 1)*3 + casilla_lat;
    //       int w_casilla_lat = n_casilla_lat + 3;
    //       // restriccion:
    //       cout << "-" << e_casilla << " " << w_casilla_lat << " 0 ";
    //       cout << e_casilla << " -" << w_casilla_lat << " 0\n" ;
    //     }
    //     if (i <= n - 2) {
    //       int casilla_inf   = casilla + m;
    //       int n_casilla_inf = (casilla_inf - 1)*3 + casilla_inf;
    //
    //       cout << "-" << s_casilla << " " << n_casilla_inf << " 0 ";
    //       cout << s_casilla << " -" << n_casilla_inf << " 0\n" ;
    //     }
    //     casilla++;
    //   }
    // }

    casilla  = 1;

    // set the type 1 clauses
    cout << "c TYPE 1 CLAUSES\n";

    for(int i = 0; i < n; i++){
      string fila = valores[i];
      for( int j = 0; j < m; j++) {
        cout << "c C" << casilla << ": "<< fila[j] << "\n";

        // check for the number of the segments for the cell
        int n_casilla = casilla;
        int w_casilla = (n*m + m) + casilla + i;
        int s_casilla = n_casilla + m;
        int e_casilla = w_casilla + 1;

        // check for type 1 clauses

        // the box has no segments
        if (fila[j] =='0'){
          cout << " -" << n_casilla << " 0\n";
          cout << " -" << s_casilla << " 0\n";
          cout << " -" << e_casilla << " 0\n";
          cout << " -" << w_casilla << " 0\n";
        } // the box has only one segment
        else if (fila[j] =='1'){
          cout << n_casilla << " " << e_casilla << " " << s_casilla << " " << w_casilla << " 0\n";
          cout << "-" << s_casilla << " -" << w_casilla << " 0\n";
          cout << "-" << e_casilla << " -" << s_casilla << " 0\n";
          cout << "-" << n_casilla << " -" << w_casilla << " 0\n";
          cout << "-" << n_casilla << " -" << e_casilla << " 0\n";
          cout << "-" << n_casilla << " -" << s_casilla << " 0\n";
          cout << "-" << e_casilla << " -" << w_casilla << " 0\n";
        } // the box has two segments
        else if (fila[j] =='2'){
          cout << " -" << n_casilla << " -" << e_casilla << " -" << s_casilla << " -" << w_casilla << " 0\n";
          cout << "-" << n_casilla << " -" << s_casilla << " -" << e_casilla << " 0\n";
          cout << "-" << n_casilla << " -" << s_casilla << " -" << w_casilla << " 0\n";
          cout << w_casilla << " " << e_casilla << " " << s_casilla << " 0\n";
          cout << w_casilla << " " << e_casilla << " " << n_casilla << " 0\n";
          cout << "-" << n_casilla << " -" << w_casilla << " -" << e_casilla << " 0\n";
          cout << n_casilla << " " << e_casilla << " " << s_casilla << " 0\n";
          cout << w_casilla << " " << n_casilla << " " << s_casilla << " 0\n";
          cout << "-" << s_casilla << " -" << w_casilla << " -" << e_casilla << " 0\n";
          cout << w_casilla << " " << n_casilla << " " << e_casilla << " 0\n";

        } // the box has three segments
        else if (fila[j] =='3'){
          cout << " -" << n_casilla << " -" << e_casilla << " -" << s_casilla << " -" << w_casilla << " 0\n";
          cout << n_casilla << " " << s_casilla << " 0\n";
          cout << n_casilla << " " << e_casilla << " 0\n";
          cout << n_casilla << " " << w_casilla << " 0\n";
          cout << s_casilla << " " << e_casilla << " 0\n";
          cout << s_casilla << " " << w_casilla << " 0\n";
          cout << w_casilla << " " << e_casilla << " 0\n";

        } // the box has all four segments
        else if (fila[j] =='4'){
          cout << n_casilla << " 0\n";
          cout << s_casilla << " 0\n";
          cout << e_casilla << " 0\n";
          cout << w_casilla << " 0\n";
        }

        // Check for type 2 clauses
        // left border
        if (j == 0) {
          cout << " -" << w_casilla << " "  << z(casilla) << " 0\n";
          cout << " -" << w_casilla << " -" << z(casilla) << " 0\n";
        } // right border
        else if (j == m - 1) {
          cout << " -" << e_casilla << " "  << z(casilla) << " 0\n";
          cout << " -" << e_casilla << " -" << z(casilla) << " 0\n";
        } // upper border
        else if (i == 0) {
          cout << " -" << s_casilla << " "  << z(casilla) << " 0\n";
          cout << " -" << s_casilla << " -" << z(casilla) << " 0\n";
        } // lower border
        else if (i == n -1 ){
          cout << " -" << n_casilla << " "  << z(casilla) << " 0\n";
          cout << " -" << n_casilla << " -" << z(casilla) << " 0\n";
        } // inner
        else {

          // CNF CLAUSE
          // !p v q
          cout << " -" <<  z(casilla) << " -"  << e_casilla << " -" << n_casilla << " -"<< s_casilla << " -"<< w_casilla << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << e_casilla << " -" << n_casilla << " -"<< s_casilla << " "<< z(casilla - 1) << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << e_casilla << " -" << n_casilla << "  "<< z(casilla + m) << " -" << w_casilla << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << e_casilla << " -" << n_casilla << " " << z(casilla + m)  << " " << z(casilla - 1) << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << n_casilla << " -" << s_casilla << " -"<< w_casilla << " "<< z(casilla + 1) << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << n_casilla << " -" << s_casilla << " " << z(casilla - 1 ) << " "<< z(casilla + 1) << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << n_casilla << " -" << w_casilla << " " << z(casilla + m ) << " "<< z(casilla + 1) << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << n_casilla << " " << z(casilla + m) << " " << z(casilla - 1 ) << " "<< z(casilla + 1) << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << e_casilla << " -" << s_casilla << " -" << w_casilla << " " << z(casilla - m) << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << e_casilla << " -" << s_casilla << " "  << z(casilla - m ) << " "<< z(casilla - 1) << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << e_casilla << " -" << w_casilla << " "  << z(casilla - m ) << " "<< z(casilla + m) << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << e_casilla << " "  << z(casilla + m )    << " "<< z(casilla - m) << " "<< z(casilla - 1)<< " 0\n";
          cout << " -" <<  z(casilla) << " -"  << s_casilla << " -" << w_casilla << " " << z(casilla - m) << " "<< z(casilla + 1)<< " 0\n";
          cout << " -" <<  z(casilla) << " -"  << s_casilla <<  " " << z(casilla - m) << " "<< z(casilla + 1) << " " << z(casilla - 1) << " 0\n";
          cout << " -" <<  z(casilla) << " -"  << w_casilla <<  " " << z(casilla - m) << " "<< z(casilla + 1) << " " << z(casilla + m) << " 0\n";
          cout << " -" <<  z(casilla) << " "   << z(casilla - 1) <<  " " << z(casilla - m) << " "<< z(casilla + 1) << " " << z(casilla + m) << " 0\n";

          // p v !q
          cout << " " <<  z(casilla) << " " << n_casilla <<  " -" << z(casilla - m) << " 0\n";
          cout << " " <<  z(casilla) << " " << e_casilla <<  " -" << z(casilla + 1) << " 0\n";
          cout << " " <<  z(casilla) << " " << s_casilla <<  " -" << z(casilla + m) << " 0\n";
          cout << " " <<  z(casilla) << " " << w_casilla <<  " -" << z(casilla - 1) << " 0\n";
        }

        casilla++;
      }
    }

    // // set the type 1 clauses
    // cout << "c TYPE 3 CLAUSES\n";
    //
    // // Check for type 3 clauses
    // // Every box can reach itself
    //
    // for (int casilla1 = 1; casilla1 <= n*m; casilla1++){
    //   for(int casilla2 = 1; casilla2 <= n*m; casilla2++){
    //     cout << r(casilla1, casilla2) << '\n';
    //   }
    // }
  }


  return 0;

}
