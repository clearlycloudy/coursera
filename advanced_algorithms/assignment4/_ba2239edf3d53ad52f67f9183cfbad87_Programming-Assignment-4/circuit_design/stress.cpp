#include <random>
#include <iostream>
#include <chrono>
#include <fstream>

using namespace std;

// unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
// std::default_random_engine generator(seed1);

int main( int argc, char** argv ){

    while(true){
	{
	    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed1);

	    std::uniform_int_distribution<int> distribution(2, 10000);
		
	    int vertex_count = distribution(generator);

	    std::uniform_int_distribution<int> distribution2(1,10000);
	    // std::uniform_int_distribution<int> distribution2(2,2);

	    int clauses = distribution2(generator);

	    std::uniform_int_distribution<int> distribution3(1,vertex_count);

	    std::uniform_int_distribution<int> distribution4(0,1);
	    std::uniform_int_distribution<int> distribution5(0,1);

	    ofstream f("stress_file.txt", std::ofstream::out | std::ofstream::trunc);
	    ofstream f2("stress_file_2.txt", std::ofstream::out | std::ofstream::trunc);

	    f << "p cnf " << vertex_count << " " << clauses << endl;
	    f2 << vertex_count << " " << clauses << endl;
    
	    for (int i = 0; i < clauses; ++i) {
	
		int term0 = distribution3(generator);

		int term1 = distribution3(generator);
		// int term1 = term0;

		// while( term0 == term1 ){
		//     term1 = distribution3(generator);
		// }

		int b = distribution4(generator);
		if( 1 == b ){
		    term0 = term0 * -1;
		    // term1 = term1 * -1;
		}
		b = distribution5(generator);
		{
		    term1 = term1 * -1;
		}
		f << term0 << " " << term1 << " 0" << endl;
		f2 << term0 << " " << term1 << endl;
		f.flush();
		f2.flush();
	    }
	}
	FILE * in;
	char buff[512];
	string out_minisat;
	if(!(in = popen("minisat stress_file.txt", "r"))){
	// if(!(in = popen("./orig < stress_file_2.txt", "r"))){
	    cerr << "error calling reference solver" << endl;
	    return 1;
	}
	while(fgets(buff, sizeof(buff), in)!=NULL){
	    out_minisat += buff;
	}
	pclose(in);

	FILE * in2;
	char buff2[512];
	string out_solver;
	if(!(in2 = popen("./out < stress_file_2.txt", "r"))){
	// if(!(in2 = popen("./out < test2.txt", "r"))){
	    cerr << "error calling our solver" << endl;
	    return 1;
	}
	while(fgets(buff2, sizeof(buff2), in2)!=NULL){
	    out_solver += buff2;
	}
	pclose(in2);

	cout << "buf minisat: " << out_minisat << endl;
	cout << "buf our solver: " << out_solver << endl;

	bool flag_our = 1;
	bool flag_minisat = 1;
	if( std::string::npos != out_solver.find( "UNSATISFIABLE" ) ){
	    flag_our = 0;
	}

	if( std::string::npos != out_minisat.find( "UNSATISFIABLE" ) ){
	    flag_minisat = 0;
	}
	if( flag_our != flag_minisat ){
	    cout << "mismatch found." << endl;
	    getchar();
	}
	// if( 0 == flag_our ){
	//     getchar();
	// }
    }

    
    
    return 0;
}
